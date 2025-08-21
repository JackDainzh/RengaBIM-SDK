/*  Sample: Views
 *
 *  This sample illustrates how to work with model object views, set visual style of the model view,
 *  hide model objects of specific type
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <string>
#include <list>


class ButtonClickHandler;

class ViewsPlugin: public plugins::IPlugin
{
public:
  ViewsPlugin();
  ~ViewsPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::list<std::unique_ptr<ButtonClickHandler>> m_buttonClickHandlers;
};


class ButtonClickHandler : public Renga::ActionEventHandler
{
public:
  ButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnToggled(bool checked) override {}

protected:
  Renga::IApplicationPtr m_pApplication;
};

class ViewVisualStyleButtonClickHandler : public ButtonClickHandler
{
public:
  ViewVisualStyleButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered() override;
};

class HideAllWallsButtonClickHandler: public ButtonClickHandler
{
public:
  HideAllWallsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered() override;
};


EXPORT_PLUGIN(ViewsPlugin);
