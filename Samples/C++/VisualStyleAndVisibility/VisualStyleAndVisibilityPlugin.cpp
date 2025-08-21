/*  Sample: Views
 *
 *  This sample illustrates how to work with model object views, set visual style of the model view,
 *  hide model objects of specific type
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "VisualStyleAndVisibilityPlugin.h"

#include <sstream>
#include <vector>

#include <Renga/ObjectTypes.h>
#include <Renga/CreateApplication.hpp>

#include <atlsafe.h>


ViewsPlugin::ViewsPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ViewsPlugin::~ViewsPlugin()
{
  ::CoUninitialize();
}

bool ViewsPlugin::initialize(const wchar_t* pluginDir)
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
        pAction->PutToolTip(L"Set new visual style to the view");
        m_buttonClickHandlers.emplace_back(new ViewVisualStyleButtonClickHandler(pAction, pApplication));
        pUIPanelExtension->AddToolButton(pAction);
      }

      if (auto pAction = pUI->CreateAction())
      {
        pAction->PutToolTip(L"Hide all walls");
        m_buttonClickHandlers.emplace_back(new HideAllWallsButtonClickHandler(pAction, pApplication));
        pUIPanelExtension->AddToolButton(pAction);
      }

      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void ViewsPlugin::stop()
{
  m_buttonClickHandlers.clear();
}


ButtonClickHandler::ButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  Renga::ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}


ViewVisualStyleButtonClickHandler::ViewVisualStyleButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  ButtonClickHandler(pAction, pApplication)
{
}

void ViewVisualStyleButtonClickHandler::OnTriggered()
{
  auto pView = m_pApplication->GetActiveView();

  Renga::IModelViewPtr pModelView;
  pView->QueryInterface(&pModelView);

  if (!pModelView)
    return;

  auto visualStyle = pModelView->GetVisualStyle();
  if (visualStyle != Renga::VisualStyle::VisualStyle_Color)
    visualStyle = Renga::VisualStyle::VisualStyle_Color;
  else
    visualStyle = Renga::VisualStyle::VisualStyle_Monochrome;

  pModelView->PutVisualStyle(visualStyle);
}


HideAllWallsButtonClickHandler::HideAllWallsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  ButtonClickHandler(pAction, pApplication)
{
}

void HideAllWallsButtonClickHandler::OnTriggered()
{
  auto pView = m_pApplication->GetActiveView();

  Renga::IModelViewPtr pModelView;
  pView->QueryInterface(&pModelView);

  if (!pModelView)
    return;

  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();

  std::vector<int> wallObjectIds;
  for (int i = 0; i < pModelObjectCollection->GetCount(); ++i)
  {
    auto pModelObject = pModelObjectCollection->GetByIndex(i);
    if (pModelObject->GetObjectType() == Renga::ObjectTypes::Wall)
      wallObjectIds.push_back(pModelObject->GetId());
  }

  CComSafeArray<int> wallObjectIdsSafeArray(static_cast<ULONG>(wallObjectIds.size()));
  for (int i = 0; i < wallObjectIds.size(); i++)
    wallObjectIdsSafeArray.SetAt(i, wallObjectIds[i]);

  pModelView->SetObjectsVisibility(wallObjectIdsSafeArray, FALSE);
}
