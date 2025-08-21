/*  Sample: ComplexToolButtons
 *
 *  This sample illustrates how to add custom complex tool buttons
 *  to Renga's main toolbar.
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


class ComplexToolButtonsPlugin : public plugins::IPlugin
{
public:
  ComplexToolButtonsPlugin();
  ~ComplexToolButtonsPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  void addHandler(Renga::ActionEventHandler* pHandler);

  Renga::IActionPtr CreateAction(Renga::IUIPtr pUI, const std::wstring& displayName);

private:
  typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;

private:
  std::list<HandlerPtr> m_handlerContainer;
};

EXPORT_PLUGIN(ComplexToolButtonsPlugin);
