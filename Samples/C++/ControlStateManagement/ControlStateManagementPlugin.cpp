/*  Sample: ControlStateManagement
 *
 *  This sample illustrates a way to implement custom user control state management.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "ControlStateManagementPlugin.h"

#include <Renga/CreateApplication.hpp>


ButtonBaseHandler::ButtonBaseHandler(Renga::IActionPtr pAction, ButtonBase* pButtonBase) :
  Renga::ActionEventHandler(pAction),
  m_pButtonBase(pButtonBase)
{
}

void ButtonBaseHandler::OnTriggered() {
  m_pButtonBase->OnTriggered();
}

ButtonBase::ButtonBase(Renga::IUIPtr pUI, const std::wstring& toolTip)
{
  auto pAction = pUI->CreateAction();
  m_pAction = pAction;

  m_pHandler = std::make_unique<ButtonBaseHandler>(pAction, this);

  m_pAction->PutToolTip(toolTip.c_str());
}

Renga::IActionPtr ButtonBase::getAction()
{
  return m_pAction;
}

ControllingButton::ControllingButton(Renga::IUIPtr pUI, const std::wstring& toolTip) :
  ButtonBase(pUI, toolTip)
{
}

void ControllingButton::OnTriggered()
{
  if (m_pLinkedAction)
    m_pLinkedAction->PutEnabled(!m_pLinkedAction->GetEnabled());
}

void ControllingButton::setLinkedAction(Renga::IActionPtr pLinkedAction)
{
  m_pLinkedAction = pLinkedAction;
}

ControlledButton::ControlledButton(Renga::IUIPtr pUI, const std::wstring& toolTip) :
  ButtonBase(pUI, toolTip)
{
}

void ControlledButton::OnTriggered()
{
  ::MessageBox(0, L"Button clicked", L"ControlStateManagement plugin", MB_OK);
}

ControlStateManagementPlugin::ControlStateManagementPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ControlStateManagementPlugin::~ControlStateManagementPlugin()
{
  ::CoUninitialize();
}

bool ControlStateManagementPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    auto controllingButton = new ControllingButton(pUI, L"Switch other button state");
    auto controlledButton = new ControlledButton(pUI, L"Show message");
    controllingButton->setLinkedAction(controlledButton->getAction());

    m_buttonContainer.emplace_back(ButtonPtr(controllingButton));
    m_buttonContainer.emplace_back(ButtonPtr(controlledButton));
  }
  
  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      for (const auto& pButton : m_buttonContainer)
        pUIPanelExtension->AddToolButton(pButton->getAction());

      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void ControlStateManagementPlugin::stop()
{
  m_buttonContainer.clear();
}
