/*  Sample: System Events
*
*  This sample illustrates how to react on system events,
*  particularly how to handle the project closure notification.
*
*  Copyright Renga Software LLC, 2018. All rights reserved.
*/

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <string>


class ButtonClickHandler;

class ObjectReinfrocementPlugin : public plugins::IPlugin
{
public:
  ObjectReinfrocementPlugin();
  ~ObjectReinfrocementPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::unique_ptr<ButtonClickHandler> m_pButtonClickHandler;
};

class ButtonClickHandler : public Renga::ActionEventHandler
{
public:
  ButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);
  ~ButtonClickHandler();

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  void showMessage(const std::wstring& title, const std::wstring& text) const;
  std::wstring obtainRebarUsages(Renga::IObjectReinforcementModelPtr pReinforcementModel, Renga::IReinforcementUnitStyleManagerPtr unitManager) const;
  std::wstring obtainReinforcementUnitUsages(Renga::IObjectReinforcementModelPtr pReinforcementModel, Renga::IReinforcementUnitStyleManagerPtr unitManager) const;
  std::wstring obtainReinfrocement(Renga::IObjectReinforcementModelPtr pReinforcementModel) const;

  std::wstring obtainObjectRebarUsages(Renga::IRebarUsageCollectionPtr pRebarUsages, Renga::IReinforcementUnitStyleManagerPtr pReinforcementManager, int indent = 0) const;


private:
  Renga::IApplicationPtr m_pApplication;
};

EXPORT_PLUGIN(ObjectReinfrocementPlugin);
