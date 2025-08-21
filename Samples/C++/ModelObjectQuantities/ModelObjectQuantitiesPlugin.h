/*  Sample: Model Object Quantities
 *
 *  This sample illustrates how to obtain quantities of model objects.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <string>


class ObtainObjectQuantitiesButtonClickHandler;

class ModelObjectQuantitiesPlugin: public plugins::IPlugin
{
public:
  ModelObjectQuantitiesPlugin();
  ~ModelObjectQuantitiesPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::unique_ptr<ObtainObjectQuantitiesButtonClickHandler> m_pObtainObjectQuantitiesButtonClickHandler;
};

class ObtainObjectQuantitiesButtonClickHandler : public Renga::ActionEventHandler
{
public:
  ObtainObjectQuantitiesButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);
  ~ObtainObjectQuantitiesButtonClickHandler();

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  std::wstring obtainObjectQuantities() const;

private:
  Renga::IApplicationPtr m_pApplication;
};

EXPORT_PLUGIN(ModelObjectQuantitiesPlugin);
