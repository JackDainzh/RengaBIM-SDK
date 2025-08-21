/*  Sample: FittingStylesCreator
 *
 *  This sample illustrates how to create fitting styles with particular parameters.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#include "FittingStylesCreatorPlugin.h"

#include <Renga/CreateApplication.hpp>
#include <Renga/StyleTypeIds.h>
#include <Renga/ParameterIds.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <format>
#include <sstream>
#include <string>

#include <atlsafe.h>


class StyleCreator : public Renga::ActionEventHandler
{
public:
  StyleCreator(Renga::IActionPtr pAction, Renga::IApplicationPtr pApp)
    :
    Renga::ActionEventHandler(pAction),
      m_pApp(pApp)
  {
  }

  void createFittingStyle(double elbowAngle, const std::wstring& name)
  {
    auto pProject = m_pApp->Project;

    auto pipeFittingStyles     = pProject->PipeFittingStyles;
    auto pipeFittingCategories = pProject->PipeFittingCategories;

    auto creationArgs        = pipeFittingStyles->CreateNewEntityArgs();
    creationArgs->TypeId     = Renga::StyleTypeIds::PipeFittingStyle;
    creationArgs->CategoryId = pipeFittingCategories->GetByUniqueIdS(m_bendCategoryUuid)->Id;

    auto operation = pProject->CreateOperation();
    operation->Start();
    auto style = pipeFittingStyles->Create(creationArgs);
    operation->Apply();

    auto parameters = Renga::IParameterContainerPtr{};
    style->QueryInterface(&parameters);

    if (parameters)
    {
      operation->Start();

      auto paramIds = parameters->GetIds();
      for (auto i = 0; i < paramIds->Count; ++i)
      {
        auto someParam = parameters->Get(paramIds->Get(i));
        if (wcscmp(someParam->Definition->Name, m_paramName) == 0)
        {
          someParam->SetDoubleValue(elbowAngle);
          break;
        }
      }

      auto nameParam = parameters->Get(Renga::ParameterIds::EntityName);
      nameParam->SetStringValue(SysAllocString(name.data()));

      operation->Apply();
    }
  }

  double calculateAngleBetween(Renga::ICurve3DPtr seg1, Renga::ICurve3DPtr seg2)
  {
    const auto vec1 = createNormalizedVectorFrom(seg1->GetEndPoint(), seg1->GetBeginPoint());
    const auto vec2 = createNormalizedVectorFrom(seg2->GetBeginPoint(), seg2->GetEndPoint());

    return angleBetween(vec1, vec2);
  }

  void OnTriggered() override
  {
    const auto selected = CComSafeArray<int>{m_pApp->Selection->GetSelectedObjects()};

    if (selected.GetCount() == 0)
      return;

    auto pFirstObject = m_pApp->Project->Model->GetObjects()->GetById(selected.GetAt(0));

    auto pRoute = Renga::IRouteParamsPtr{};
    pFirstObject->QueryInterface(&pRoute);
    
    if (!pRoute)
      return;

    auto pContour = pRoute->GetContour();

    auto pPolyCurve = Renga::IPolyCurve3DPtr{};
    pContour->QueryInterface(&pPolyCurve);

    if (!pPolyCurve)
      return;

    const auto segmentCount = pPolyCurve->GetSegmentCount();
    
    if (segmentCount < 2)
      return;

    auto ss = std::wstringstream{};

    for (int segmentIndex = 0; segmentIndex < segmentCount - 1; ++segmentIndex)
    {
      const auto pSegment1 = pPolyCurve->GetSegment(segmentIndex);
      const auto pSegment2 = pPolyCurve->GetSegment(segmentIndex + 1);

      const auto elbowAngle = 180 - calculateAngleBetween(pSegment1, pSegment2);
      const auto nameString = std::format(L"Fitting style {}°", std::round(elbowAngle * 100) / 100.);
      createFittingStyle(elbowAngle, nameString);

      ss << nameString << "\n";
    }

    auto message = ss.str();
    m_pApp->UI->ShowMessageBox(Renga::MessageIcon::MessageIcon_Info, L"List of created styles", message.data());
  }

  private:
  Renga::Vector3D createNormalizedVectorFrom(const Renga::Point3D& begin, const Renga::Point3D& end) const
  {
    auto vec = Renga::Vector3D{};
    vec.X = end.X - begin.X;
    vec.Y = end.Y - begin.Y;
    vec.Z = end.Z - begin.Z;

    const auto length = std::sqrt(std::pow(vec.X, 2) + std::pow(vec.Y, 2) + std::pow(vec.Z, 2));

    vec.X /= length;
    vec.Y /= length;
    vec.Z /= length;

    return vec;
  }

  double angleBetween(const Renga::Vector3D& vec1, const Renga::Vector3D& vec2)
  {
    const auto cosOfAngle = vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z;
    const auto angle      = std::acos(cosOfAngle) / M_PI * 180.0;

    return angle > 180.0 ? (360.0 - angle) : angle;
  }

  void OnToggled(bool checked) override {}

private:
  Renga::IApplicationPtr m_pApp;
  const char* const m_bendCategoryUuid =
      "15D55530-0541-48DF-85E1-AB242E65E21E"; ///< UUID of Renga's template category. You are able to use your own.
  const wchar_t* const m_paramName = L"dimensions.elbow_angle"; ///< Parameter name
};

FittingStylesCreatorPlugin::FittingStylesCreatorPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

FittingStylesCreatorPlugin::~FittingStylesCreatorPlugin()
{
  ::CoUninitialize();
}

Renga::IActionPtr FittingStylesCreatorPlugin::CreateAction(Renga::IUIPtr pUI, Renga::IApplicationPtr app)
{
  auto pAction = pUI->CreateAction();

  m_handler.reset(new StyleCreator(pAction, app));

  return pAction;
}

bool FittingStylesCreatorPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      pUIPanelExtension->AddToolButton(CreateAction(pUI, pApplication));
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
      pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
    }
  }
  return true;
}

void FittingStylesCreatorPlugin::stop()
{
  m_handler.reset();
}
