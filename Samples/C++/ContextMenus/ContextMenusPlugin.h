/*  Sample: CustomContextMenus
 *
 *  This sample illustrates how to create custom context menu items
 *  and/or submenus and associate actions with them.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>


class ContextMenuWrapper
{
public:
  ContextMenuWrapper(Renga::IUIPtr pUI, Renga::IContextMenuPtr pContextMenu);

  void Update();

public:
  GUID m_guid;
  Renga::IUIPtr m_pUI;
  Renga::IContextMenuPtr m_pContextMenu;
};



class ShowMessageActionHandler;
class AddItemActionHandler;

class CustomContextMenusPlugin: public plugins::IPlugin
{
public:
  CustomContextMenusPlugin();
  ~CustomContextMenusPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::unique_ptr<ShowMessageActionHandler> m_pShowMessageActionHandler;
  std::unique_ptr<AddItemActionHandler> m_pAddItemActionHandler;
  std::unique_ptr<ContextMenuWrapper> m_pContextWrapper;
};

class ShowMessageActionHandler : public Renga::ActionEventHandler
{
public:
  ShowMessageActionHandler(Renga::IActionPtr pAction, Renga::IUIPtr pUI);

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  Renga::IActionPtr m_pAction;
  Renga::IUIPtr m_pUI;
};

class AddItemActionHandler : public Renga::ActionEventHandler
{
public:
  AddItemActionHandler(Renga::IActionPtr pAction, ContextMenuWrapper* pContextWrapper);

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  Renga::IActionPtr m_pAction;
  ContextMenuWrapper* m_pContextWrapper;
};

EXPORT_PLUGIN(CustomContextMenusPlugin);
