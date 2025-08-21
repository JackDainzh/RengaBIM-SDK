/*  Sample: Geometry Grid Types
 *
 *  This sample illustrates how to work with export object geometry grid types via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>


class GeometryGridTypesPlugin : public plugins::IPlugin
{
public:
  GeometryGridTypesPlugin();
  ~GeometryGridTypesPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;

private:
  HandlerPtr m_handler;
};

EXPORT_PLUGIN(GeometryGridTypesPlugin);
