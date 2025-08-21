/*  Sample: Geometry Grid Types
 *
 *  This sample illustrates how to work with export object geometry grid types via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */


#include "GeometryGridTypesPlugin.h"

#include <Renga/CreateApplication.hpp>
#include <Renga/MeshTypes.h>
#include <Renga/GridTypes.h>
#include <Renga/ObjectTypes.h>

#include <sstream>


class GridExportHandler : public Renga::ActionEventHandler
{
public:
  GridExportHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
    Renga::ActionEventHandler(pAction),
    m_pApplication(pApplication)
  {
  }

  void OnTriggered() override
  {
    auto pProject = m_pApplication->GetProject();
    auto pDataExporter = pProject->GetDataExporter();
    
    auto command_output = gatherExportData(pDataExporter->GetObjects3D());

    MessageBox(NULL,
      (LPCWSTR)command_output.c_str(),
      (LPCWSTR)L"Export 3D objects result output",
      MB_ICONINFORMATION | MB_OK);
  }

  std::wstring gatherExportData(Renga::IExportedObject3DCollectionPtr pObject3DCollection)
  {
    std::wstring ret;
    ret += L"Objects exported:\n\n";

    auto size = pObject3DCollection->GetCount();
    for (int i = 0; i < size; ++i)
      ret += gatherExportData(pObject3DCollection->Get(i));
    return ret;
  }

  std::wstring gatherExportData(Renga::IExportedObject3DPtr pObject3D)
  {
    std::wstring ret;

    ret += L"Object: ";

    auto id = pObject3D->GetModelObjectId();
    auto pProject = m_pApplication->GetProject();
    auto pModel = pProject->GetModel();
    auto pModelObjectCollection = pModel->GetObjects();
    if (auto pModelObject = pModelObjectCollection->GetById(id))
    {
      ret += pModelObject->GetName();
    }
    else
    {
      ret += L"Undefined as model object";
    }
    ret += L"\n";

    auto meshCount = pObject3D->GetMeshCount();
    for (int i = 0; i < meshCount; ++i)
    {
      ret += gatherExportData(pObject3D->GetMesh(i), pObject3D->GetModelObjectType());
    }

    ret += L"\n";

    return ret;
  }

  std::wstring gatherExportData(Renga::IMeshPtr pMesh, const GUID objectType)
  {
    std::wstring ret;

    ret += L"Mesh: type - ";

    auto meshType = pMesh->GetMeshType();
    if (::IsEqualGUID(meshType, Renga::MeshTypes::Undefined))
      ret += L"undefined";
    else if (::IsEqualGUID(meshType, Renga::MeshTypes::DoorReveal))
      ret += L"door reveal";
    else if (::IsEqualGUID(meshType, Renga::MeshTypes::DoorPanel))
      ret += L"door panel";
    else if (::IsEqualGUID(meshType, Renga::MeshTypes::DoorTransom))
      ret += L"door transom";
    else if (::IsEqualGUID(meshType, Renga::MeshTypes::WindowReveal))
      ret += L"window reveal";
    else if (::IsEqualGUID(meshType, Renga::MeshTypes::WindowPanel))
      ret += L"window panel";
    else if (::IsEqualGUID(meshType, Renga::MeshTypes::WindowSill))
      ret += L"window sill";
    else if (::IsEqualGUID(meshType, Renga::MeshTypes::WindowOutwardSill))
      ret += L"window outward sill";

    ret += L"\n";

    auto gridCount = pMesh->GetGridCount();
    for (int i = 0; i < gridCount; ++i)
      ret += gatherExportData(pMesh->GetGrid(i), objectType);
    return ret;
  }

  std::wstring gatherExportData(Renga::IGridPtr pGrid, const GUID objectType)
  {
    std::wstring ret;
    if (::IsEqualGUID(objectType, Renga::ObjectTypes::Wall))
    {
      ret += L"  Grid: type - wall ";
      switch (pGrid->GetGridType())
      {
      case Renga::GridTypes::Wall::Undefined:
        ret += L"undefined";
        break;
      case Renga::GridTypes::Wall::FrontSide:
        ret += L"front side";
        break;
      case Renga::GridTypes::Wall::BackSide:
        ret += L"back side";
        break;
      case Renga::GridTypes::Wall::Bottom:
        ret += L"bottom";
        break;
      case Renga::GridTypes::Wall::Top:
        ret += L"top";
        break;
      case Renga::GridTypes::Wall::FirstButt:
        ret += L"first butt";
        break;
      case Renga::GridTypes::Wall::SecondButt:
        ret += L"second butt";
        break;
      case Renga::GridTypes::Wall::Reveal:
        ret += L"reveal";
        break;
      case Renga::GridTypes::Wall::Cut:
        ret += L"cut";
        break;
      }
      ret += L"\n";
    }
    return ret;
  }


  void OnToggled(bool checked) override {}

private:
  Renga::IApplicationPtr m_pApplication;
};



GeometryGridTypesPlugin::GeometryGridTypesPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

GeometryGridTypesPlugin::~GeometryGridTypesPlugin()
{
  ::CoUninitialize();
}

bool GeometryGridTypesPlugin::initialize(const wchar_t* pluginDir)
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
        pAction->PutToolTip(L"Export geometry and discover mesh and grid types");
        m_handler = std::make_unique<GridExportHandler>(pAction, pApplication);
        pUIPanelExtension->AddToolButton(pAction);
      }
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void GeometryGridTypesPlugin::stop()
{
  m_handler.reset();
}
