/*  Sample: System Events
 *
 *  This sample illustrates how to react on system events,
 *  particularly how to handle the project closure notification.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ApplicationEventHandler.hpp>

#include <memory>


class EventHandler;

class SystemEventsPlugin: public plugins::IPlugin
{
public:
  SystemEventsPlugin();
  ~SystemEventsPlugin();

  bool initialize(const wchar_t* pluginDir) override;
  void stop() override;

private:
  std::unique_ptr<EventHandler> m_pEventHandler; 
};

class EventHandler : public Renga::ApplicationEventHandler
{
public:
  EventHandler(Renga::IApplicationPtr pApplication);

  // Renga::ApplicationEventHandler
  void OnBeforeApplicationClose(Renga::IApplicationCloseEvent* pEvent) override;
  void OnBeforeProjectClose(Renga::IProjectCloseEvent* pEvent) override;
  void OnProjectClosed() override;
  void OnProjectCreated() override;
  void OnProjectOpened(const std::wstring& filePath) override;
  void OnProjectSaved(const std::wstring& filePath) override;
};

EXPORT_PLUGIN(SystemEventsPlugin);
