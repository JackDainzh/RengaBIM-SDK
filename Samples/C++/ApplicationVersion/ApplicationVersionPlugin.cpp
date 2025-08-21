/*  Sample: ApplicationVersion
 *
 *  This sample illustrates how to add custom simple tool button
 *  to Renga's main toolbar and get Renga's version.
 *
 *  Copyright Renga Software LLC, 2023. All rights reserved.
 */

#include "ApplicationVersionPlugin.h"

#include <Renga/CreateApplication.hpp>

#include <sstream>
#include <string>

class TextHandler : public Renga::ActionEventHandler
{
public:
  TextHandler(Renga::IActionPtr pAction, const Renga::ProductVersion& productVersion)
    :
    Renga::ActionEventHandler(pAction),
      m_productVersion(productVersion)
  {
  }

  void OnTriggered() override
  {
    auto ss = std::wstringstream{};
    
        ss << L"Application version:"
        << m_productVersion.Major << L'.'
        << m_productVersion.Minor << L'.'
        << m_productVersion.Patch << L'.'
        << m_productVersion.BuildNumber;
    
    auto message = ss.str();
    ::MessageBox(
        nullptr,
        (LPCWSTR)message.c_str(),
        (LPCWSTR) L"ApplicationVersionPluginHeader",
        MB_ICONINFORMATION | MB_OK);
  }

  void OnToggled(bool checked) override {}

private:
  Renga::ProductVersion m_productVersion;
};

ApplicationVersionPlugin::ApplicationVersionPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ApplicationVersionPlugin::~ApplicationVersionPlugin()
{
  ::CoUninitialize();
}

Renga::IActionPtr ApplicationVersionPlugin::CreateAction(Renga::IUIPtr pUI, const Renga::ProductVersion& productVersion)
{
  auto pAction = pUI->CreateAction();

  addHandler(new TextHandler(pAction, productVersion));

  return pAction;
}

void ApplicationVersionPlugin::addHandler(Renga::ActionEventHandler* pHandler)
{
  m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}

bool ApplicationVersionPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      pUIPanelExtension->AddToolButton(CreateAction(pUI, pApplication->GetVersion()));
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
      pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
    }
  }
  return true;
}

void ApplicationVersionPlugin::stop()
{
  m_handlerContainer.clear();
}
