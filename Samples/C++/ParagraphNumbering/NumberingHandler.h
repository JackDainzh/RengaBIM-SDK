/*  Sample: ParagraphNumbering
 *
 *  This sample illustrates how to work with ITextObject.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/ActionEventHandler.hpp>


class NumberingHandler : public Renga::ActionEventHandler
{
public:
  NumberingHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr application);

  void OnTriggered() override;
  void OnToggled(bool) override;

private:
  Renga::ISelectionPtr selection();

private:
  Renga::IApplicationPtr m_application;
};