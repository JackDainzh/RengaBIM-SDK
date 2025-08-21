/*  Sample 3: Material Export
 *
 *  This sample illustrates how to export 3D object material via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "MaterialExportPlugin.h"

#include <sstream>

#include <Renga/CreateApplication.hpp>


MaterialExportPlugin::MaterialExportPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

MaterialExportPlugin::~MaterialExportPlugin()
{
  ::CoUninitialize();
}

bool MaterialExportPlugin::initialize(const wchar_t* pluginDir)
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
        pAction->PutToolTip(L"Export objects materials");
        m_pExportMaterialsButtonClickHandler.reset(new Export3DObjectsMaterialsButtonClickHandler(pAction, pApplication));
        pUIPanelExtension->AddToolButton(pAction);
      }
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void MaterialExportPlugin::stop()
{
  m_pExportMaterialsButtonClickHandler.reset();
}


Export3DObjectsMaterialsButtonClickHandler::Export3DObjectsMaterialsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}


void Export3DObjectsMaterialsButtonClickHandler::OnTriggered()
{
  std::wstring command_output = export3DObjectsMaterials();

  MessageBox(NULL,
    (LPCWSTR)command_output.c_str(),
    (LPCWSTR)L"Export 3D objects materials result output",
    MB_ICONINFORMATION | MB_OK );
}

std::wstring Export3DObjectsMaterialsButtonClickHandler::export3DObjectsMaterials() const
{
  auto pProject = m_pApplication->GetProject();

  auto pDataExporter = pProject->GetDataExporter();
  auto pObject3DCollection = pDataExporter->GetObjects3D();

  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();

  auto pMaterialManager = pProject->GetMaterialManager();
  auto pLayeredMaterialManager = pProject->GetLayeredMaterialManager();


  std::wstringstream result_output;
  result_output << pObject3DCollection->GetCount() << " objects exported:" << "\n\n";
  for (int i = 0; i < pObject3DCollection->GetCount(); ++i)
  {
    auto pObject3D = pObject3DCollection->Get(i);

    const auto modelObjectId = pObject3D->GetModelObjectId();
    auto pModelObject = pModelObjectCollection->GetById(modelObjectId);

    result_output << "Object: {" << i + 1 << "} " << static_cast<const wchar_t*>(pModelObject->GetName()) << "\n";

    Renga::IObjectWithMaterialPtr pObjectWithMaterial;
    pModelObject->QueryInterface(&pObjectWithMaterial);
    if (pObjectWithMaterial)
    {
      result_output << "  Material type: " << "One-layered" << "\n";
      if (auto pMaterial = pMaterialManager->GetMaterial(pObjectWithMaterial->GetMaterialId()))
        result_output << "  Material name: " << static_cast<const wchar_t*>(pMaterial->GetName()) << "\n";
      else
        result_output << "  No material assigned" << "\n";

      result_output << "\n";
      continue;
    }

    Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
    pModelObject->QueryInterface(&pObjectWithLayeredMaterial);
    if (pObjectWithLayeredMaterial)
    {
      result_output << "  Material type: " << "Multi-layered" << "\n";
      if (auto pLayeredMaterial = pLayeredMaterialManager->GetLayeredMaterialByIdGroupPair(pObjectWithLayeredMaterial->GetLayeredMaterialIdGroupPair()))
      {
        result_output << "  Sandwich name: " << static_cast<const wchar_t*>(pLayeredMaterial->GetName()) << "\n";

        auto pLayers = pLayeredMaterial->GetLayers();

        result_output << "  Sandwich consists of: " << pLayers->GetCount() << " layers" << "\n\n";
        for (int l = 0; l < pLayers->GetCount(); ++l)
        {
          auto pLayer = pLayers->Get(l);

          result_output << "    Layer #" << l << "\n";
          result_output << "    Name: " << static_cast<const wchar_t*>(pLayer->GetMaterial()->GetName()) << "\n";
          result_output << "    Thickness: " << pLayer->GetThickness() << "\n";
        }
      }
      else
      {
        result_output << "  No material assigned" << "\n";
      }
      result_output << "\n";
      continue;
    }

    result_output << "  Material type: " << "None" << "\n";
    result_output << "\n";
  }
  return result_output.str();
}
