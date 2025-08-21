/*  Sample: ParagraphNumbering
 *
 *  This sample illustrates how to work with ITextObject.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>
#include <Renga/SelectionEventHandler.hpp>

#include <memory>
#include <string>


class ParagraphNumberingPlugin : public plugins::IPlugin
{
public:
  ParagraphNumberingPlugin();
  ~ParagraphNumberingPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  Renga::IActionPtr CreateAction(Renga::IUIPtr pUI, Renga::IApplicationPtr application);

private:
  std::unique_ptr<Renga::ActionEventHandler> m_numberingParagraphsHandler;
};

EXPORT_PLUGIN(ParagraphNumberingPlugin);
