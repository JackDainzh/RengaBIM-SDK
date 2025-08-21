/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/ActionEventHandler.hpp>
#include <Renga/IPlugin.h>

#include <memory>


class MepCircleConstructorPlugin : public plugins::IPlugin
{
public:
  MepCircleConstructorPlugin();
  ~MepCircleConstructorPlugin();

  bool initialize(const wchar_t*) override;
  void stop() override{};

private:
  void addHandler(Renga::ActionEventHandler*);

  Renga::IActionPtr CreateAction(Renga::IUIPtr, Renga::IApplicationPtr);

private:
  std::unique_ptr<Renga::ActionEventHandler> m_pHandler;
};

EXPORT_PLUGIN(MepCircleConstructorPlugin);
