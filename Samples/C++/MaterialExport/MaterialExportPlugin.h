/*  Sample 3: Material Export
 *
 *  This sample illustrates how to export 3D object material via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <string>


class Export3DObjectsMaterialsButtonClickHandler;

class MaterialExportPlugin: public plugins::IPlugin
{
public:
  MaterialExportPlugin();
  ~MaterialExportPlugin();

  bool initialize(const wchar_t* pluginDir);
  void stop();

private:
  std::unique_ptr<Export3DObjectsMaterialsButtonClickHandler> m_pExportMaterialsButtonClickHandler; 
};

class Export3DObjectsMaterialsButtonClickHandler : public Renga::ActionEventHandler
{
public:
  Export3DObjectsMaterialsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered() override;
  void OnToggled(bool checked) override {}

private:
  std::wstring export3DObjectsMaterials() const;

private:
  Renga::IApplicationPtr m_pApplication;
};

EXPORT_PLUGIN(MaterialExportPlugin);
