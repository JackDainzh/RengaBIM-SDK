/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <list>


class AssemblyInstanceCreationPlugin : public plugins::IPlugin
{
  typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;

public:
  AssemblyInstanceCreationPlugin();
  ~AssemblyInstanceCreationPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  void addHandler(Renga::ActionEventHandler* pHandler);

  Renga::IActionPtr CreateAction(Renga::IUIPtr pUI, Renga::IApplicationPtr pApplication);

private:
  std::list<HandlerPtr> m_handlerContainer;
};

EXPORT_PLUGIN(AssemblyInstanceCreationPlugin);
