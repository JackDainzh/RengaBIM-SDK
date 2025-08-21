/*  Sample: CustomContextMenus
 *
 *  This sample illustrates how to create custom context menu items
 *  and/or submenus and associate actions with them.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */


#include <string>

#include "ContextMenusPlugin.h"

#include <Renga/CreateApplication.hpp>


ContextMenuWrapper::ContextMenuWrapper(Renga::IUIPtr pUI, Renga::IContextMenuPtr pContextMenu) :
  m_pUI(pUI),
  m_pContextMenu(pContextMenu)
{
  ::CoCreateGuid(&m_guid);
}

void ContextMenuWrapper::Update()
{
  m_pUI->AddContextMenu(&m_guid, m_pContextMenu, Renga::ViewType_View3D, Renga::ContextMenuShowCase_Scene);
}

CustomContextMenusPlugin::CustomContextMenusPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

CustomContextMenusPlugin::~CustomContextMenusPlugin()
{
  ::CoUninitialize();
}

bool CustomContextMenusPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  auto pUI = pApplication->GetUI();
  auto pContextMenu = pUI->CreateContextMenu();

  m_pContextWrapper.reset(new ContextMenuWrapper(pUI, pContextMenu));


  pContextMenu->AddSeparator();
  pContextMenu->AddSeparator();

  if (auto pNodeItem = pContextMenu->AddNodeItem())
  {
    auto pAction20 = pUI->CreateAction();
    pAction20->PutDisplayName(L"Node 1");

    auto pAction21 = pUI->CreateAction();
    pAction21->PutDisplayName(L"Node 2");

    pNodeItem->PutDisplayName(L"Subtree");
    pNodeItem->AddActionItem(pAction20);
    pNodeItem->AddActionItem(pAction21);
  }

  pContextMenu->AddSeparator();

  auto pAction1 = pUI->CreateAction();
  pAction1->PutDisplayName(L"Message from Sample");

  auto pAction2 = pUI->CreateAction();
  pAction2->PutDisplayName(L"Add item");
  
  m_pShowMessageActionHandler.reset(new ShowMessageActionHandler(pAction1, pUI));
  m_pAddItemActionHandler.reset(new AddItemActionHandler(pAction2, m_pContextWrapper.get()));

  pContextMenu->AddActionItem(pAction1);
  pContextMenu->AddActionItem(pAction2);

  m_pContextWrapper->Update();

  return true;
}

void CustomContextMenusPlugin::stop()
{
  m_pShowMessageActionHandler.reset();
  m_pAddItemActionHandler.reset();
  m_pContextWrapper.reset();
}





ShowMessageActionHandler::ShowMessageActionHandler(Renga::IActionPtr pAction, Renga::IUIPtr pUI) :
  Renga::ActionEventHandler(pAction),
  m_pAction(pAction),
  m_pUI(pUI)
{
}

void ShowMessageActionHandler::OnTriggered()
{
  m_pUI->ShowMessageBox(Renga::MessageIcon::MessageIcon_Info, L"Message from Sample", L"Context menu clicked");
}


AddItemActionHandler::AddItemActionHandler(Renga::IActionPtr pAction, ContextMenuWrapper* pContextWrapper) :
  Renga::ActionEventHandler(pAction),
  m_pAction(pAction),
  m_pContextWrapper(pContextWrapper)
{
}

void AddItemActionHandler::OnTriggered()
{
  auto pNodeItem = m_pContextWrapper->m_pContextMenu->AddNodeItem();
  pNodeItem->PutDisplayName(L"New node");

  m_pContextWrapper->Update();
}
