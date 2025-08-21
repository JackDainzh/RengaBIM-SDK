/*  Sample: ComplexToolButtons
 *
 *  This sample illustrates how to add custom complex tool buttons
 *  to Renga's main toolbar.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "ButtonsPlugin.h"

#include <Renga/CreateApplication.hpp>


class TextHandler : public Renga::ActionEventHandler
{
public:
  TextHandler(Renga::IActionPtr pAction, const std::wstring& text) :
    Renga::ActionEventHandler(pAction),
    m_text(text)
  {
  }

  void OnTriggered() override
  {
    ::MessageBox(nullptr, (LPCWSTR)m_text.c_str(), (LPCWSTR)L"Plugin message", MB_ICONINFORMATION | MB_OK);
  }

  void OnToggled(bool checked) override {}

private:
  std::wstring m_text;
};

ComplexToolButtonsPlugin::ComplexToolButtonsPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ComplexToolButtonsPlugin::~ComplexToolButtonsPlugin()
{
  ::CoUninitialize();
}

Renga::IActionPtr ComplexToolButtonsPlugin::CreateAction(Renga::IUIPtr pUI, const std::wstring& displayName)
{
  auto pAction = pUI->CreateAction();
  pAction->PutDisplayName(displayName.c_str());

  addHandler(new TextHandler(pAction, displayName + L" Handler"));

  return pAction;
}

void ComplexToolButtonsPlugin::addHandler(Renga::ActionEventHandler* pHandler)
{
  m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}

bool ComplexToolButtonsPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      pUIPanelExtension->AddToolButton(
        CreateAction(pUI, L"SimpleButton"));

      if (auto pDropDownButton = pUI->CreateDropDownButton())
      {
        pDropDownButton->PutToolTip(L"DropDownButton");

        pDropDownButton->AddAction(
          CreateAction(pUI, L"DropDownButton Action1"));

        pDropDownButton->AddAction(
          CreateAction(pUI, L"DropDownButton Action2"));

        pDropDownButton->AddAction(
          CreateAction(pUI, L"DropDownButton Action3"));

        pUIPanelExtension->AddDropDownButton(pDropDownButton);
      }


      if (auto pSplitButton = pUI->CreateSplitButton(
        CreateAction(pUI, L"SplitButton Default Action")))
      {
        pSplitButton->AddAction(
          CreateAction(pUI, L"SplitButton Action2"));

        pSplitButton->AddAction(
          CreateAction(pUI, L"SplitButton Action 3"));

        pUIPanelExtension->AddSplitButton(pSplitButton);
      }

      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
      pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
    }
  }
  return true;
}

void ComplexToolButtonsPlugin::stop()
{
  m_handlerContainer.clear();
}
