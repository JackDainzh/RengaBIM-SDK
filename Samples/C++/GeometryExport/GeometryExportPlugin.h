/*  Sample: Geometry Export
 *
 *  This sample illustrates how to export object geometry via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>

#include <memory>
#include <string>


class Export3DObjectsButtonClickHandler;
class ExportGridsButtonClickHandler;

class GeometryExportPlugin: public plugins::IPlugin
{
public:
  GeometryExportPlugin();
  ~GeometryExportPlugin();

  bool initialize(const wchar_t* pluginDir);
  void stop();

private:
  std::unique_ptr<Export3DObjectsButtonClickHandler> m_pExport3DObjectsButtonClickHandler; 
  std::unique_ptr<ExportGridsButtonClickHandler> m_pExportGridsButtonClickHandler; 
};


class Export3DObjectsButtonClickHandler : public Renga::ActionEventHandler
{
public:
  Export3DObjectsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered();
  void OnToggled(bool checked) override {}

private:
  std::wstring export3DObjects() const;

private:
  Renga::IApplicationPtr m_pApplication;
};


class ExportGridsButtonClickHandler : public Renga::ActionEventHandler
{
public:
  ExportGridsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication);

  void OnTriggered();
  void OnToggled(bool checked) override {}

private:
  std::wstring exportGrids() const;

private:
  Renga::IApplicationPtr m_pApplication;
};

EXPORT_PLUGIN(GeometryExportPlugin);
