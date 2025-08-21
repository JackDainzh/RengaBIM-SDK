/*  Sample: Model Selection
 *
 *  This sample illustrates how to work with the selection mechanism of Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once 
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>
#include <Renga/SelectionEventHandler.hpp>

#include <memory>
#include <string>

class ModelSelectionChangedHandler;
class ShowSelectedObjectsButtonClickHandler;
class SelectWallsButtonClickHandler;

class ModelSelectionPlugin: public plugins::IPlugin
{
public:
  ModelSelectionPlugin();
  ~ModelSelectionPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::unique_ptr<ModelSelectionChangedHandler> m_pModelSelectionChangedHandler;
  std::unique_ptr<ShowSelectedObjectsButtonClickHandler> m_pShowSelectedObjectsButtonClickHandler;
  std::unique_ptr<SelectWallsButtonClickHandler> m_pSelectWallsButtonClickHandler;
};

class ModelSelectionChangedHandler : public Renga::SelectionEventHandler
{
public:
  ModelSelectionChangedHandler(Renga::ISelectionPtr pSelection, Renga::IActionPtr pShowSelectedObjectsAction);

  void OnModelSelectionChanged() override;

private:
  void UpdateShowSelectedObjectsButton();

private:
  Renga::IActionPtr m_pShowSelectedObjectsAction;
  Renga::ISelectionPtr m_pSelection;
};

class ShowSelectedObjectsButtonClickHandler : public Renga::ActionEventHandler
{
public:
  ShowSelectedObjectsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  Renga::IApplicationPtr m_pApplication;
};

class SelectWallsButtonClickHandler : public Renga::ActionEventHandler
{
public:
  SelectWallsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  Renga::IApplicationPtr m_pApplication;
};

EXPORT_PLUGIN(ModelSelectionPlugin);
