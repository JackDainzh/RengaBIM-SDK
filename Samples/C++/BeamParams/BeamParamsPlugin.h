/*  Sample: Object properties
*
*  This sample illustrates how to work with beam params via Renga API.
*
*  Copyright Renga Software LLC, 2018. All rights reserved.
*/

#pragma once

#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <string>


class BeamParamsClickHandler;

class BeamParamsPlugin : public plugins::IPlugin
{
public:
  BeamParamsPlugin();
  ~BeamParamsPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::unique_ptr<BeamParamsClickHandler> m_pButtonClickHandler;
};

class BeamParamsClickHandler : public Renga::ActionEventHandler
{
public:
  BeamParamsClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  Renga::Vector3D GetBaselineOffset(Renga::IBeamParamsPtr pBeamParams, Renga::Point2D offset);
  Renga::IPlacement3DPtr getLevelPlacement(Renga::IModelObjectCollectionPtr pModelObjects, Renga::IModelObjectPtr pModelObject);
  std::wstring buildBeamParamsDescription(Renga::IBeamParamsPtr pBeamParams, Renga::IBeamStyleManagerPtr pBeamStyleManager, Renga::IPlacement3DPtr pLevelPlacement);
  std::wstring addPropertyToLevelObjects();

private:
  Renga::IApplicationPtr m_pApplication;
};

EXPORT_PLUGIN(BeamParamsPlugin);
