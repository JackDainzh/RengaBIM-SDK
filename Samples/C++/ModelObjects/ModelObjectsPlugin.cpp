/*  Sample: Model Objects
 *
 *  This sample illustrates how to traverse model objects in a project and read their parameters.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "ModelObjectsPlugin.h"

#include <sstream>

#include <Renga/ObjectTypes.h>
#include <Renga/CreateApplication.hpp>


ModelObjectsPlugin::ModelObjectsPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ModelObjectsPlugin::~ModelObjectsPlugin()
{
  ::CoUninitialize();
}

bool ModelObjectsPlugin::initialize(const wchar_t* pluginDir)
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
        pAction->PutToolTip(L"List levels, walls and columns");

        m_pReadObjectsButtonClickHandler.reset(new ReadObjectsButtonClickHandler(pAction, pApplication));

        pUIPanelExtension->AddToolButton(pAction);
      }
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void ModelObjectsPlugin::stop()
{
  m_pReadObjectsButtonClickHandler.reset();
}

ReadObjectsButtonClickHandler::ReadObjectsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  Renga::ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}

void ReadObjectsButtonClickHandler::OnTriggered()
{
  const auto command_output = readModelObjects();

  MessageBox(NULL,
    (LPCWSTR)command_output.c_str(),
    (LPCWSTR)L"Model object list",
    MB_ICONINFORMATION | MB_OK );
}

std::wstring ReadObjectsButtonClickHandler::readModelObjects() const
{
  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();

  std::wstringstream result_output;
  result_output << "Renga levels, walls and columns:" << "\n\n";

  for (int i = 0; i < pModelObjectCollection->GetCount(); ++i)
  {
    const auto pModelObject = pModelObjectCollection->GetByIndex(i);
    const auto type = pModelObject->GetObjectType();

    if (::IsEqualGUID(type, Renga::ObjectTypes::Level))
    {
      Renga::ILevelPtr pLevel;
      pModelObject->QueryInterface(&pLevel);
      
      result_output << "Object type: " << "Level" << "\n";
      result_output << "Level generated name: " << pModelObject->GetName().operator const wchar_t *() << "\n";
      result_output << "Level user defined name: " << pLevel->GetLevelName().operator const wchar_t *() << "\n";
      result_output << "Level elevation: " << pLevel->GetElevation() << " mm." << "\n";
      result_output << "\n\n";
    }
    else if (::IsEqualGUID(type, Renga::ObjectTypes::Column))
    {
      Renga::ILevelObjectPtr pLevelObject;
      pModelObject->QueryInterface(&pLevelObject);

      Renga::IObjectWithMarkPtr pObjectWithMark;
      pModelObject->QueryInterface(&pObjectWithMark);

      Renga::IObjectWithMaterialPtr pObjectWithMaterial;
      pModelObject->QueryInterface(&pObjectWithMaterial);

      result_output << "Object type: " << "Column" << "\n";
      result_output << "Column name: " << pModelObject->GetName().operator const wchar_t *() << "\n";
      result_output << "Column parent level id: " << pLevelObject->GetLevelId() << "\n";
      result_output << "Column material id: " << pObjectWithMaterial->GetMaterialId() << "\n";
      result_output << "Column mark: " << pObjectWithMark->GetMark().operator const wchar_t *() << "\n";
      result_output << "Column offset: " << pLevelObject->GetElevationAboveLevel() << " mm." << "\n";
      result_output << "\n\n";
    }
    else if (::IsEqualGUID(type, Renga::ObjectTypes::Wall))
    {
      Renga::ILevelObjectPtr pLevelObject;
      pModelObject->QueryInterface(&pLevelObject);

      Renga::IObjectWithMarkPtr pObjectWithMark;
      pModelObject->QueryInterface(&pObjectWithMark);

      Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
      pModelObject->QueryInterface(&pObjectWithLayeredMaterial);

      result_output << "Object type: " << "Wall" << "\n";
      result_output << "Wall name: " << pModelObject->GetName().operator const wchar_t *() << "\n";
      result_output << "Wall parent level id: " << pLevelObject->GetLevelId() << "\n";
      result_output << "Wall material id: " << pObjectWithLayeredMaterial->GetLayeredMaterialIdGroupPair().Id << "\n";
      result_output << "Wall mark: " << pObjectWithMark->GetMark().operator const wchar_t *() << "\n";
      result_output << "Wall offset: " << pLevelObject->GetElevationAboveLevel() << " mm." << "\n";
      result_output << "\n\n";
    }
  }
  return result_output.str();
}
