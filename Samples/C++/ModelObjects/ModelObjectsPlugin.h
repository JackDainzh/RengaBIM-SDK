/*  Sample: Model Objects
 *
 *  This sample illustrates how to traverse model objects in a project and read their parameters.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <string>


class ReadObjectsButtonClickHandler;

class ModelObjectsPlugin: public plugins::IPlugin
{
public:
  ModelObjectsPlugin();
  ~ModelObjectsPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::unique_ptr<ReadObjectsButtonClickHandler> m_pReadObjectsButtonClickHandler; 
};

class ReadObjectsButtonClickHandler : public Renga::ActionEventHandler
{
public:
  ReadObjectsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  std::wstring readModelObjects() const;

private:
  Renga::IApplicationPtr m_pApplication;
};

EXPORT_PLUGIN(ModelObjectsPlugin);
