/*  Sample: Model Objects
 *
 *  This sample illustrates how to traverse model objects in a project and read their parameters.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "ModelSelectionPlugin.h"

#include <sstream>
#include <vector>

#include <Renga/ObjectTypes.h>
#include <Renga/CreateApplication.hpp>

#include <atlsafe.h>

ModelSelectionPlugin::ModelSelectionPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ModelSelectionPlugin::~ModelSelectionPlugin()
{
  ::CoUninitialize();
}

bool ModelSelectionPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  auto pSelection = pApplication->GetSelection();
  if (!pSelection)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      if (auto pShowSelectedObjectsAction = pUI->CreateAction())
      {
        m_pModelSelectionChangedHandler.reset(new ModelSelectionChangedHandler(pSelection, pShowSelectedObjectsAction));
        
        pShowSelectedObjectsAction->PutToolTip(L"Show selected objects");
        m_pShowSelectedObjectsButtonClickHandler.reset(new ShowSelectedObjectsButtonClickHandler(pShowSelectedObjectsAction, pApplication));
        pUIPanelExtension->AddToolButton(pShowSelectedObjectsAction);
      }

      if (auto pSelectWallsAction = pUI->CreateAction())
      {
        pSelectWallsAction->PutToolTip(L"Select walls");
        m_pSelectWallsButtonClickHandler.reset(new SelectWallsButtonClickHandler(pSelectWallsAction, pApplication));
        pUIPanelExtension->AddToolButton(pSelectWallsAction);
      }

      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }

  return true;
}

void ModelSelectionPlugin::stop()
{
  m_pShowSelectedObjectsButtonClickHandler.reset();
  m_pSelectWallsButtonClickHandler.reset();
}

ModelSelectionChangedHandler::ModelSelectionChangedHandler(Renga::ISelectionPtr pSelection, Renga::IActionPtr showSelectedObjectsAction)
  : Renga::SelectionEventHandler(pSelection),
    m_pSelection(pSelection),
    m_pShowSelectedObjectsAction(showSelectedObjectsAction)
{
  UpdateShowSelectedObjectsButton();
}

void ModelSelectionChangedHandler::OnModelSelectionChanged()
{
  UpdateShowSelectedObjectsButton();
}

void ModelSelectionChangedHandler::UpdateShowSelectedObjectsButton()
{
  CComSafeArray<int> selectedObjectIds(m_pSelection->GetSelectedObjects());
  m_pShowSelectedObjectsAction->PutEnabled(selectedObjectIds.GetCount() > 0);
}

ShowSelectedObjectsButtonClickHandler::ShowSelectedObjectsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication)
  : Renga::ActionEventHandler(pAction),
    m_pApplication(pApplication)
{
}

void ShowSelectedObjectsButtonClickHandler::OnTriggered()
{
  auto pProject = m_pApplication->GetProject();

  if (pProject == nullptr)
    return;

  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();

  auto pSelection = m_pApplication->GetSelection();

  CComSafeArray<int> selectedObjectIds(pSelection->GetSelectedObjects());

  std::wstringstream stringStream;
  stringStream << selectedObjectIds.GetCount() << L" selected objects:" << std::endl;

  for (int i = 0; i < (int)selectedObjectIds.GetCount(); i++)
  {
    int objectId = selectedObjectIds.GetAt(i);
    stringStream << L"Object id: " << objectId << std::endl;
  }

  std::wstring resultOutput = stringStream.str();

  MessageBox(NULL,
    (LPCWSTR)resultOutput.c_str(),
    (LPCWSTR)L"Model Selection",
    MB_ICONINFORMATION | MB_OK);
}

SelectWallsButtonClickHandler::SelectWallsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication)
  : Renga::ActionEventHandler(pAction),
    m_pApplication(pApplication)
{
}

void SelectWallsButtonClickHandler::OnTriggered()
{
  auto pProject = m_pApplication->GetProject();

  if (pProject == nullptr)
    return;

  auto pModel = pProject->GetModel();
  auto pModelObjects = pModel->GetObjects();

  CComSafeArray<int> modelObjectIds(pModelObjects->GetIds());

  std::vector<int> wallObjectIds;

  for (int i = 0; i < (int)modelObjectIds.GetCount(); i++)
  {
    int objectId = modelObjectIds.GetAt(i);

    auto pModelObject = pModelObjects->GetById(objectId);

    if (pModelObject == nullptr)
      continue;

    if (pModelObject->GetObjectType() != Renga::ObjectTypes::Wall)
      continue;

    wallObjectIds.push_back(objectId);
  }

  CComSafeArray<int> wallObjectIdsSafeArray((ULONG)wallObjectIds.size());

  for (int i = 0; i < wallObjectIds.size(); i++)
    wallObjectIdsSafeArray.SetAt(i, wallObjectIds[i]);

  auto pSelection = m_pApplication->GetSelection();
  pSelection->SetSelectedObjects(wallObjectIdsSafeArray);
}
