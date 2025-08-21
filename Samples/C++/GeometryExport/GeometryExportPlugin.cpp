/*  Sample: Geometry Export
 *
 *  This sample illustrates how to export object geometry via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "GeometryExportPlugin.h"

#include <sstream>

#include <Renga/CreateApplication.hpp>

#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")


std::string GuidToString(GUID guid)
{
  CHAR* cszUuid(nullptr);
  ::UuidToStringA(&guid, (RPC_CSTR*)&cszUuid);
  std::string ret;
  if (cszUuid)
  {
    ret = cszUuid;
    ::RpcStringFreeA((RPC_CSTR*)&cszUuid);
  }
  return ret;
}


GeometryExportPlugin::GeometryExportPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

GeometryExportPlugin::~GeometryExportPlugin()
{
  ::CoUninitialize();
}

bool GeometryExportPlugin::initialize(const wchar_t* pluginDir)
{
  auto pApplication = Renga::CreateApplication();
  if (!pApplication)
    return false;

  if (auto pUI = pApplication->GetUI())
  {
    if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
    {
      if (auto pAction = pUI->CreateAction())
      {
        pAction->PutToolTip(L"Export first 10 Objects");
        m_pExport3DObjectsButtonClickHandler.reset(new Export3DObjectsButtonClickHandler(pAction, pApplication));
        pUIPanelExtension->AddToolButton(pAction);
      }

      if (auto pAction = pUI->CreateAction())
      {
        pAction->PutToolTip(L"Export first 10 Grids");
        m_pExportGridsButtonClickHandler.reset(new ExportGridsButtonClickHandler(pAction, pApplication));
        pUIPanelExtension->AddToolButton(pAction);
      }

      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void GeometryExportPlugin::stop()
{
  m_pExport3DObjectsButtonClickHandler.reset();
  m_pExportGridsButtonClickHandler.reset();
}


Export3DObjectsButtonClickHandler::Export3DObjectsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}

void Export3DObjectsButtonClickHandler::OnTriggered()
{
  std::wstring command_output = export3DObjects();

  MessageBox(NULL,
    (LPCWSTR)command_output.c_str(),
    (LPCWSTR)L"Export 3D objects result output",
    MB_ICONINFORMATION | MB_OK );
}

std::wstring Export3DObjectsButtonClickHandler::export3DObjects() const
{
  auto pProject = m_pApplication->GetProject();

  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();

  auto pDataExporter = pProject->GetDataExporter();
  auto pObject3DCollection = pDataExporter->GetObjects3D();

  std::wstringstream result_output;
  result_output << pObject3DCollection->GetCount() << " objects exported:" << "\n\n";
  for (int i = 0, size = pObject3DCollection->GetCount(); i < min(size, 10); ++i)
  {
    size_t gridCount = 0;
    size_t vertexCount = 0;
    size_t normalCount = 0;
    size_t triangleCount = 0;

    auto pObject3D = pObject3DCollection->Get(i);
    for (int meshIdx = 0; meshIdx < pObject3D->GetMeshCount(); ++meshIdx)
    {
      auto pMesh = pObject3D->GetMesh(meshIdx);
      gridCount += pMesh->GetGridCount();
      for (int gridIdx = 0; gridIdx < pMesh->GetGridCount(); ++gridIdx)
      {
        auto pGrid = pMesh->GetGrid(gridIdx);
        vertexCount += pGrid->GetVertexCount();
        normalCount += pGrid->GetNormalCount();
        triangleCount += pGrid->GetTriangleCount();
      }
    }

    const auto modelObjectId = pObject3D->GetModelObjectId();
    auto pModelObject = pModelObjectCollection->GetById(modelObjectId);

    result_output << "Object {" << i + 1 << "} " << (wchar_t*)pModelObject->GetName() << "\n";
    result_output << "  Object type: " << GuidToString(pObject3D->GetModelObjectType()).c_str() << "\n";
    result_output << "  Mesh count: " << pObject3D->GetMeshCount() << "\n";
    result_output << "  Grid count: " << gridCount << "\n";
    result_output << "  Vertex count: " << vertexCount << "\n";
    result_output << "  Normal count: " << normalCount << "\n";
    result_output << "  Triangle count: " << triangleCount << "\n";

    Renga::IObjectWithMaterialPtr pObjectWithMaterial;
    pModelObject->QueryInterface(&pObjectWithMaterial);
    if (pObjectWithMaterial)
    {
      result_output << "  Material id: " << pObjectWithMaterial->GetMaterialId() << "\n";
    }

    Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
    pModelObject->QueryInterface(&pObjectWithLayeredMaterial);
    if (pObjectWithLayeredMaterial)
    {
      result_output << "  Layers material id: " << pObjectWithLayeredMaterial->GetLayeredMaterialIdGroupPair().Id << "\n";
      result_output << "  Layers material group: ";
      switch (pObjectWithLayeredMaterial->GetLayeredMaterialIdGroupPair().Group)
      {
      case Renga::LayeredMaterialGroup_Undefined:
        result_output << "Undefined";
        break;
      case Renga::LayeredMaterialGroup_Wall:
        result_output << "Wall";
        break;
      case Renga::LayeredMaterialGroup_Floor:
        result_output << "Floor";
        break;
      case Renga::LayeredMaterialGroup_Roof:
        result_output << "Roof";
        break;
      }
      result_output << "\n";
    }
  }
  return result_output.str();
}


ExportGridsButtonClickHandler::ExportGridsButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}

void ExportGridsButtonClickHandler::OnTriggered()
{
  std::wstring command_output = exportGrids();

  MessageBox(NULL,
    (LPCWSTR)command_output.c_str(),
    (LPCWSTR)L"Export grids result output",
    MB_ICONINFORMATION | MB_OK );
}

std::wstring ExportGridsButtonClickHandler::exportGrids() const
{
  auto pProject = m_pApplication->GetProject();
  auto pDataExporter = pProject->GetDataExporter();
  auto pObject3DCollection = pDataExporter->GetObjects3D();

  std::wstringstream result_output;
  auto pGridWithMaterialCollection = pDataExporter->GetGrids();
  result_output << pGridWithMaterialCollection->GetCount() << " grids exported:" << "\n\n";
  for (int i = 0, size = pGridWithMaterialCollection->GetCount(); i < min(size, 10); ++i)
  {
    auto pGridWithMaterial = pGridWithMaterialCollection->Get(i);
    auto pGrid = pGridWithMaterial->GetGrid();

    result_output << "Grid#" << i + 1 << "\n";
    result_output << "  Vertex count: " << pGrid->GetVertexCount() << "\n";
    result_output << "  Normal count: " << pGrid->GetNormalCount() << "\n";
    result_output << "  Triangle count: " << pGrid->GetTriangleCount() << "\n";
    result_output << "  Material id: " << pGridWithMaterial->GetMaterial()->GetId() << "\n";
  }
  return result_output.str();
}
