/*  Sample: System Events
*
*  This sample illustrates how to react on system events,
*  particularly how to handle the project closure notification.
*
*  Copyright Renga Software LLC, 2018. All rights reserved.
*/

#include "ObjectReinforcement.h"

#include <sstream>
#include <fstream>

#include <atlsafe.h>

#include <Renga/ObjectTypes.h>
#include <Renga/QuantityIds.h>
#include <Renga/CreateApplication.hpp>



namespace extensions
{
  std::wstring toString(const Renga::Point3D& p)
  {
    std::wstringstream ss;
    ss << L"(" << p.X << L", " << p.Y << L", " << p.Z << L")";
    return ss.str();
  }

  std::wstring toString(const Renga::Vector3D& vec)
  {
    std::wstringstream ss;
    ss << L"(" << vec.X << L", " << vec.Y << L", " << vec.Z << L")";
    return ss.str();
  }

  std::wstring toString(const Renga::Curve3DType type)
  {
    std::wstring result;
    switch (type)
    {
    case Renga::Curve3DType_LineSegment:
      result = L"LineSegment";
      break;
    case Renga::Curve3DType_Arc:
      result = L"Arc";
      break;
    case Renga::Curve3DType_PolyCurve:
      result = L"PolyCurve";
      break;
    default:
      result = L"Undefined";
      break;
    }
    return result;
  }

  std::wstring curve3DToString(Renga::ICurve3DPtr pCurve)
  {
    std::wstringstream ss;
    ss << L"type = " << toString(pCurve->GetCurve3DType()) << ", p1 = " << toString(pCurve->GetBeginPoint()) << L", p2 = " << toString(pCurve->GetEndPoint());
    return ss.str();
  }

  std::wstring toString(Renga::ICurve3DPtr pCurve, int indent = 0)
  {
    std::wstringstream ss;

    if (pCurve->GetCurve3DType() == Renga::Curve3DType::Curve3DType_PolyCurve)
    {
      Renga::IPolyCurve3DPtr pPolyCurve;
      pCurve->QueryInterface(&pPolyCurve);

      ss << std::wstring(indent, ' ') << L"Geometry: type = " << toString(pCurve->GetCurve3DType()) << L"\n";

      for (int i = 0; i < pPolyCurve->GetSegmentCount(); ++i)
        ss << std::wstring(indent + 2, ' ') << L"segment: " << curve3DToString(pPolyCurve->GetSegment(i)) << L"\n";
    }
    else
    {
      ss << std::wstring(indent, ' ') << L"segment: " << curve3DToString(pCurve) << L"\n";
    }

    return ss.str();
  }

  std::wstring toString(Renga::IPlacement3DPtr pPlacement)
  {
    std::wstringstream ss;
    ss <<
      L"origin = " << toString(pPlacement->Origin) << L", " <<
      L"axisX = " << toString(pPlacement->AxisX) << L", " <<
      L"axisY = " << toString(pPlacement->AxisX) << L", " <<
      L"axisZ = " << toString(pPlacement->AxisX) << L"";
    return ss.str();
  }

  std::wstring toString(Renga::IPlacement3DCollectionPtr pPlacements, int indent = 0)
  {
    std::wstringstream ss;

    ss << std::wstring(indent, ' ') << L"Placements:\n";

    for (int i = 0; i < pPlacements->GetCount(); ++i)
      ss << std::wstring(indent + 2, ' ') << L"placemnet: " << toString(pPlacements->Get(i)) << L"\n";

    return ss.str();
  }
}

std::wstring convertText(const std::wstring& text) {

  std::wstring ret;

  std::size_t pos1(0);
  std::size_t pos2 = text.find('\n', pos1);
  while (pos2 != text.npos)
  {
    ret += std::wstring(text, pos1, pos2 - pos1);
    ret += L"\r\n";

    pos1 = pos2 + 1;

    pos2 = text.find('\n', pos1);
  }
  return ret;
}





ObjectReinfrocementPlugin::ObjectReinfrocementPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ObjectReinfrocementPlugin::~ObjectReinfrocementPlugin()
{
  ::CoUninitialize();
}

bool ObjectReinfrocementPlugin::initialize(const wchar_t * pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      if (auto pAction = pUI->CreateAction())
      {
        pAction->PutToolTip("Obtain selected object reinforcement");

        m_pButtonClickHandler.reset(new ButtonClickHandler(pAction, pApplication));

        pUIPanelExtension->AddToolButton(pAction);
      }
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void ObjectReinfrocementPlugin::stop()
{
  m_pButtonClickHandler.reset();
}

ButtonClickHandler::ButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication):
  Renga::ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}

ButtonClickHandler::~ButtonClickHandler()
{
}

void ButtonClickHandler::OnTriggered()
{
  CComSafeArray<int> selectedObjectIds(m_pApplication->GetSelection()->GetSelectedObjects());

  if (selectedObjectIds.GetCount() == 0)
  {
    showMessage(L"Obtain object reinforcement", L"Select at least one object");
    return;
  }

  auto pModel = m_pApplication->GetProject()->GetModel();
  auto pModelObject = pModel->GetObjects()->GetById(selectedObjectIds.GetAt(0));
  
  Renga::IObjectReinforcementModelPtr pReinforcementModel(nullptr);
  pModelObject->QueryInterface(&pReinforcementModel);
  
  if (pReinforcementModel == nullptr)
  {
    showMessage(L"Obtain object reinforcement", L"Select object with reinforcement");
    return;
  }
  
  std::wstring command_output = obtainReinfrocement(pReinforcementModel);

  showMessage(L"Obtain object reinforcement", command_output.c_str());
}

void ButtonClickHandler::showMessage(const std::wstring & title, const std::wstring & text) const
{
  //m_pApplication->GetUI()->ShowMessageBox(Renga::MessageIcon::MessageIcon_Info, title.c_str(), text.c_str());

  const std::string fileName = "reinforcement_info.txt";

  std::ofstream ofs;
  ofs.open(fileName, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
  if (!ofs.is_open())
    return;

  const auto& convertedText = convertText(text);

  ofs.write(reinterpret_cast<const char*>(&convertedText.front()), convertedText.size() * 2);
  ofs.close();

  ::Sleep(300u);

  ::ShellExecuteA(nullptr, "open", fileName.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

std::wstring ButtonClickHandler::obtainObjectRebarUsages(Renga::IRebarUsageCollectionPtr pRebarUsages, Renga::IReinforcementUnitStyleManagerPtr pReinforcementManager, int indent) const
{
  std::wstringstream ss;

  int rebarUsageCount = pRebarUsages->GetCount();
  for (int i = 0; i < rebarUsageCount; ++i)
  {
    auto pRebarUsage = pRebarUsages->Get(i);
    auto pRebarStyle = pReinforcementManager->GetRebarStyle(pRebarUsage->GetStyleId());
    auto pTotalLengthQuantity = pRebarUsage->GetQuantities()->Get(Renga::QuantityIds::TotalRebarLength);
    auto pTotalMassQuantity = pRebarUsage->GetQuantities()->Get(Renga::QuantityIds::TotalRebarMass);
    std::wstring styleName(pRebarStyle->GetName());

    ss << std::wstring(indent, ' ') << "Style name: " << styleName <<
      ", D: " << pRebarStyle->GetDiameter() << " mm" <<
      ", Total length: " << pTotalLengthQuantity->AsLength(Renga::LengthUnit::LengthUnit_Meters) << " m" <<
      ", Total mass: " << pTotalMassQuantity->AsMass(Renga::MassUnit::MassUnit_Kilograms) << " kg\n";

    ss <<
      extensions::toString(pRebarUsage->GetRebarGeometry(), indent + 2) <<
      extensions::toString(pRebarUsage->GetPlacements(), indent + 2);
  }
  return ss.str();
}

std::wstring ButtonClickHandler::obtainRebarUsages(Renga::IObjectReinforcementModelPtr pReinforcementModel, Renga::IReinforcementUnitStyleManagerPtr pUnitManager) const
{
  std::wstringstream result_output;
  result_output << "\nParametric reinforcement rebar usages:\n";
  result_output << obtainObjectRebarUsages(pReinforcementModel->GetRebarUsages(), pUnitManager);
  return result_output.str();
}

std::wstring ButtonClickHandler::obtainReinforcementUnitUsages(Renga::IObjectReinforcementModelPtr pReinforcementModel, Renga::IReinforcementUnitStyleManagerPtr pUnitManager) const
{
  std::wstringstream result_output;
  result_output << "\nReinforcement unit usages:\n";

  auto pUnitUsages = pReinforcementModel->GetReinforcementUnitUsages();
  int unitUsageCount = pUnitUsages->GetCount();
  for (int i = 0; i < unitUsageCount; ++i)
  {
    auto pUnitUsage = pUnitUsages->Get(i);
    auto pUnitStyle = pUnitManager->GetUnitStyle(pUnitUsage->GetStyleId());
    auto unitsCountQuantity = pUnitUsage->GetQuantities()->Get(Renga::QuantityIds::ReinforcementUnitCount);
    // here you can get rebar usages of the reinforcement unit 
    // auto pUnitRebarUsages = pUnitStyle->GetRebarUsages();
    std::wstring styleName(pUnitStyle->GetName());

    std::wstring unitType;
    switch (pUnitStyle->GetUnitType())
    {
    case Renga::ReinforcementUnitType::ReinforcementUnitType_Cage: unitType = L"Cage"; break;
    case Renga::ReinforcementUnitType::ReinforcementUnitType_Mesh: unitType = L"Mesh"; break;
    default: unitType = L"Undefined"; break;
    }

    result_output << "Style name: " << styleName <<
      ", Type: " << unitType <<
      ", Total unit count: " << unitsCountQuantity->AsCount() << "\n";

    result_output <<
      extensions::toString(pUnitUsage->GetPlacements()) <<
      obtainObjectRebarUsages(pUnitStyle->GetRebarUsages(), pUnitManager, 2);
  }
  return result_output.str();
}

std::wstring ButtonClickHandler::obtainReinfrocement(Renga::IObjectReinforcementModelPtr pReinforcementModel) const
{
  auto pReinforcementUnitManager = m_pApplication->GetProject()->GetReinforcementUnitStyleManager();

  std::wstringstream result_output;
  result_output << "Object reinforcement:" << "\n";
  result_output << obtainRebarUsages(pReinforcementModel, pReinforcementUnitManager);
  result_output << obtainReinforcementUnitUsages(pReinforcementModel, pReinforcementUnitManager);

  return result_output.str();
}
