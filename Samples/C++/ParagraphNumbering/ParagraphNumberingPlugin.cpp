/*  Sample: ParagraphNumbering
 *
 *  This sample illustrates how to work with ITextObject.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#include "ParagraphNumberingPlugin.h"

#include "NumberingHandler.h"

#include <Renga/CreateApplication.hpp>

#include <atlsafe.h>
#include <string>
#include <tuple>


ParagraphNumberingPlugin::ParagraphNumberingPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ParagraphNumberingPlugin::~ParagraphNumberingPlugin()
{
  ::CoUninitialize();
}

Renga::IActionPtr ParagraphNumberingPlugin::CreateAction(
    Renga::IUIPtr pUI,
    Renga::IApplicationPtr application)
{
  auto pAction = pUI->CreateAction();

  m_numberingParagraphsHandler = std::make_unique<NumberingHandler>(pAction, application);

  return pAction;
}

bool ParagraphNumberingPlugin::initialize(const wchar_t*)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      pUIPanelExtension->AddToolButton(CreateAction(pUI, pApplication));
      pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
    }
  }
  return true;
}

void ParagraphNumberingPlugin::stop()
{
  m_numberingParagraphsHandler.reset();
}
