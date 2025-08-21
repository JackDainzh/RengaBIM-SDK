/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#include "MepCircleConstructorPlugin.h"
#include "CircleBuilder.h"

#include <Renga/CreateApplication.hpp>
#include <Renga/ObjectTypes.h>


MepCircleConstructorPlugin::MepCircleConstructorPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

MepCircleConstructorPlugin::~MepCircleConstructorPlugin()
{
  ::CoUninitialize();
}

Renga::IActionPtr MepCircleConstructorPlugin::CreateAction(Renga::IUIPtr pUI, Renga::IApplicationPtr app)
{
  auto pAction = pUI->CreateAction();

  addHandler(new CircleBuilder(pAction, app));

  return pAction;
}

void MepCircleConstructorPlugin::addHandler(Renga::ActionEventHandler* pHandler)
{
  m_pHandler.reset(pHandler);
}

bool MepCircleConstructorPlugin::initialize(const wchar_t* pluginDir)
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
    }
  }
  return true;
}
