/*  Sample: FittingStylesCreator
 *
 *  This sample illustrates how to create fitting styles with particular parameters.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>


class FittingStylesCreatorPlugin : public plugins::IPlugin
{
public:
  FittingStylesCreatorPlugin();
  ~FittingStylesCreatorPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  Renga::IActionPtr CreateAction(Renga::IUIPtr pUI, Renga::IApplicationPtr app);

private:
  typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;

private:
  HandlerPtr m_handler;
};

EXPORT_PLUGIN(FittingStylesCreatorPlugin);
