/*  Sample: System Events
 *
 *  This sample illustrates how to react on system events,
 *  particularly how to handle the project closure notification.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "SystemEventsPlugin.h"

#include <Renga/CreateApplication.hpp>


const wchar_t const* c_messageHeader = L"Example of Renga event processing";

SystemEventsPlugin::SystemEventsPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

SystemEventsPlugin::~SystemEventsPlugin()
{
  ::CoUninitialize();
}

bool SystemEventsPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  m_pEventHandler.reset(new EventHandler(pApplication));
  return true;
}

void SystemEventsPlugin::stop()
{
  m_pEventHandler.reset();
}


EventHandler::EventHandler(Renga::IApplicationPtr pApplication) :
  Renga::ApplicationEventHandler(pApplication)
{
}

void EventHandler::OnBeforeApplicationClose(Renga::IApplicationCloseEvent* pEvent)
{
  int msgboxID = MessageBox(
      NULL,
      L"Does your plug-in allow the application to be closed?",
      c_messageHeader,
      MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);

  if (msgboxID == IDNO)
    pEvent->Prevent();
}

void EventHandler::OnBeforeProjectClose(Renga::IProjectCloseEvent* pEvent)
{
  int msgboxID = MessageBox(
      NULL,
      L"Does your plug-in allow the project to be closed?",
      c_messageHeader,
      MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);

  if (msgboxID == IDNO)
    pEvent->Prevent();
}

void EventHandler::OnProjectClosed()
{
  MessageBox(NULL, L"Project closed", c_messageHeader, MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);
}

void EventHandler::OnProjectCreated()
{
  MessageBox(NULL, L"Project created", c_messageHeader, MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);
}

void EventHandler::OnProjectOpened(const std::wstring& filePath)
{
  auto message = std::wstring(L"Project opened from: ") + filePath;
  MessageBox(NULL, message.c_str(), c_messageHeader, MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);
}

void EventHandler::OnProjectSaved(const std::wstring& filePath)
{
  auto message = std::wstring(L"Project saved to: ") + filePath;
  MessageBox(NULL, message.c_str(), c_messageHeader, MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);
}
