/*  Sample: ControlStateManagement
 *
 *  This sample illustrates a way to implement custom user control state management.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <list>
#include <string>


class ButtonBase;

class ButtonBaseHandler : public Renga::ActionEventHandler
{
public:
  ButtonBaseHandler(Renga::IActionPtr pAction, ButtonBase* pButtonBase);

  virtual void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  ButtonBase* m_pButtonBase;
};

class ButtonBase
{
public:
  ButtonBase(Renga::IUIPtr pUI, const std::wstring& toolTip);

  Renga::IActionPtr getAction();

  virtual void OnTriggered() = 0;

protected:
  std::unique_ptr<ButtonBaseHandler> m_pHandler;
  Renga::IActionPtr m_pAction;
};

class ControllingButton : public ButtonBase
{
public:
  ControllingButton(Renga::IUIPtr pUI, const std::wstring& toolTip);

  void OnTriggered() override;

  void setLinkedAction(Renga::IActionPtr pLinkAction);

private:
  Renga::IActionPtr m_pLinkedAction;
};

class ControlledButton : public ButtonBase
{
public:
  ControlledButton(Renga::IUIPtr pUI, const std::wstring& toolTip);

  void OnTriggered() override;
};


class ControlStateManagementPlugin: public plugins::IPlugin
{
public:
  ControlStateManagementPlugin();
  ~ControlStateManagementPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  typedef std::unique_ptr<ButtonBase> ButtonPtr;

private:
  std::list<ButtonPtr> m_buttonContainer;
};

EXPORT_PLUGIN(ControlStateManagementPlugin);
