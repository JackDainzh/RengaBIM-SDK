/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#include "AssemblyInstanceCreationPlugin.h"

#include "AssemblyCreation.h"

#include <Renga/CreateApplication.hpp>


class AssemblyCreator : public Renga::ActionEventHandler
{
public:
  AssemblyCreator(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication)
    : Renga::ActionEventHandler(pAction),
      m_pApplication(pApplication)
  {
  }

  void OnTriggered() override { create(m_pApplication->GetProject()); }

  void OnToggled(bool checked) override {}

private:
  Renga::IApplicationPtr m_pApplication;
};

AssemblyInstanceCreationPlugin::AssemblyInstanceCreationPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

AssemblyInstanceCreationPlugin::~AssemblyInstanceCreationPlugin()
{
  ::CoUninitialize();
}

Renga::IActionPtr AssemblyInstanceCreationPlugin::CreateAction(Renga::IUIPtr pUI, Renga::IApplicationPtr pApplication)
{
  auto pAction = pUI->CreateAction();

  addHandler(new AssemblyCreator(pAction, pApplication));

  return pAction;
}

void AssemblyInstanceCreationPlugin::addHandler(Renga::ActionEventHandler* pHandler)
{
  m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}

bool AssemblyInstanceCreationPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      pUIPanelExtension->AddToolButton(CreateAction(pUI, pApplication));
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
      pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
    }
  }
  return true;
}

void AssemblyInstanceCreationPlugin::stop()
{
  m_handlerContainer.clear();
}
