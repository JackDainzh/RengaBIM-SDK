/*  Sample: ParagraphNumbering
 *
 *  This sample illustrates how to work with ITextObject.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#include "NumberingHandler.h"

#include <atlsafe.h>
#include <string>
#include <tuple>


namespace
{
  BSTR concat(BSTR strLeft, BSTR strRight)
  {
    CComBSTR dd(strLeft);
    std::ignore = dd.AppendBSTR(strRight);

    return dd.Detach();
  }
}

NumberingHandler::NumberingHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr application)
  : Renga::ActionEventHandler(pAction),
    m_application(application)
{
}

void NumberingHandler::OnTriggered()
{
  CComSafeArray<int> selectedObjects = selection()->GetSelectedObjects();

  auto project = m_application->GetProject();
  auto model   = project->GetModel();
  auto objects = model->GetObjects();

  for (auto objIndex = ULONG{0}; objIndex < selectedObjects.GetCount(); ++objIndex)
  {
    const auto id = selectedObjects.GetAt(objIndex);
    auto obj      = objects->GetById(id);

    Renga::ITextObjectPtr pTextObj;
    obj->QueryInterface(&pTextObj);

    if (!pTextObj)
      continue;

    auto pDocument = pTextObj->GetRichTextDocument();

    auto operation = project->CreateOperation();
    operation->Start();

    for (auto pIndex = 0; pIndex < pDocument->GetParagraphCount(); ++pIndex)
    {
      auto pParagraph = pDocument->GetParagraph(pIndex);

      auto token = pParagraph->GetToken(0);

      auto value  = std::to_wstring(pIndex + 1) + L". ";
      auto number = ::SysAllocString(value.data());

      token.Text = concat(number, token.Text);

      pParagraph->PrependToken(token);
      pParagraph->RemoveToken(1);

      ::SysFreeString(number);
    }
    operation->Apply();
  }
}

void NumberingHandler::OnToggled(bool)
{
}

Renga::ISelectionPtr NumberingHandler::selection()
{
  return m_application->GetSelection();
}
