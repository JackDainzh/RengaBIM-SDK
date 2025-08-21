/*  Sample: ApplicationVersion
 *
 *  This sample illustrates how to add custom simple tool button
 *  to Renga's main toolbar and get Renga's version.
 *
 *  Copyright Renga Software LLC, 2023. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <list>
#include <string>


class ApplicationVersionPlugin : public plugins::IPlugin
{
public:
  ApplicationVersionPlugin();
  ~ApplicationVersionPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  void addHandler(Renga::ActionEventHandler* pHandler);

  Renga::IActionPtr CreateAction(Renga::IUIPtr pUI, const Renga::ProductVersion& productVersion);

private:
  typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;

private:
  std::list<HandlerPtr> m_handlerContainer;
};

EXPORT_PLUGIN(ApplicationVersionPlugin);
