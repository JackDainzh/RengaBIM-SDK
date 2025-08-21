/*  Sample: Object properties
 *
 *  This sample illustrates how to work with beam params via Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "BeamParamsPlugin.h"

#include <sstream>

#include <Renga/ObjectTypes.h>
#include <Renga/CreateApplication.hpp>


BeamParamsPlugin::BeamParamsPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

BeamParamsPlugin::~BeamParamsPlugin()
{
  ::CoUninitialize();
}

bool BeamParamsPlugin::initialize(const wchar_t* pluginDir)
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
        pAction->PutToolTip(L"Beam Params.");

        m_pButtonClickHandler.reset(new BeamParamsClickHandler(pAction, pApplication));

        pUIPanelExtension->AddToolButton(pAction);
      }
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}


void BeamParamsPlugin::stop()
{
  m_pButtonClickHandler.reset();
}


BeamParamsClickHandler::BeamParamsClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  Renga::ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}


void BeamParamsClickHandler::OnTriggered()
{
  const auto command_output = addPropertyToLevelObjects();

  MessageBox(NULL,
    (LPCWSTR)command_output.c_str(),
    (LPCWSTR)L"Beam Params",
    MB_ICONINFORMATION | MB_OK );
}

Renga::Vector3D BeamParamsClickHandler::GetBaselineOffset(Renga::IBeamParamsPtr pBeamParams, Renga::Point2D offset_)
{
  auto offset = pBeamParams->GetProfilePlacement()->GetTransformFrom()->TransformPoint(&offset_);

  auto pBaseLine = pBeamParams->GetBaseline();

  auto pSectionPlacement = pBeamParams->GetProfilePlacementOnBaseline(pBaseLine->MinParameter);

  auto pSectionPlacementTransform = pSectionPlacement->GetTransformFrom();

  Renga::Point3D point = { offset.X, offset.Y, 0 };
  auto offsetPoint = pSectionPlacementTransform->TransformPoint(&point);
  auto baselineStart = pBaseLine->GetBeginPoint();

  return 
  {
    offsetPoint.X - baselineStart.X,
    offsetPoint.Y - baselineStart.Y,
    offsetPoint.Z - baselineStart.Z,
  };
}

Renga::IPlacement3DPtr BeamParamsClickHandler::getLevelPlacement(Renga::IModelObjectCollectionPtr pModelObjects, Renga::IModelObjectPtr pModelObject)
{
  Renga::ILevelObjectPtr pLevelObject;
  pModelObject->QueryInterface(&pLevelObject);

  auto parentLevelId = pLevelObject->LevelId;
  auto pParentLevel = pModelObjects->GetById(parentLevelId);

  Renga::ILevelPtr pLevel;
  pParentLevel->QueryInterface(&pLevel);

  return pLevel->Placement;
}

std::wstring BeamParamsClickHandler::buildBeamParamsDescription(Renga::IBeamParamsPtr pBeamParams, Renga::IBeamStyleManagerPtr pBeamStyleManager, Renga::IPlacement3DPtr pLevelPlacement)
{
  std::wstring result_output = L"  Beam Param:\n";

  auto pTransformIntoGlobal = pLevelPlacement->GetTransformFrom();
  auto pTransformIntoGlobalWithVerticalOffset = pTransformIntoGlobal->GetCopy();
  Renga::Vector3D vec = { 0, 0, pBeamParams->VerticalOffset };
  pTransformIntoGlobalWithVerticalOffset->AddTranslation(&vec);

  // getting the baseline of the beam
  auto pBaseLineInLevelSpace = pBeamParams->GetBaseline();

  // getting beam profile
  auto pProfile = pBeamStyleManager->GetBeamStyle(pBeamParams->StyleId)->Profile;
  auto pProfilePlacement = pBeamParams->GetProfilePlacement();

  // curves in the global coordinate system

  // getting the baseline of the beam in the global coordinate system
  auto pBaseLine = pBaseLineInLevelSpace->GetTransformed(pTransformIntoGlobal);

  // getting the center line of the beam in the global coordinate system
  auto vecOffset = GetBaselineOffset(pBeamParams, { 0, 0 });
  auto pCenterLine = pBaseLineInLevelSpace->GetOffseted(&vecOffset)->GetTransformed(pTransformIntoGlobalWithVerticalOffset);

  // getting the center line of mass of the beam in the global coordinate system
  vecOffset = GetBaselineOffset(pBeamParams, pProfile->GetCenterOfMass());
  auto pCenterOfMassPath = pBaseLineInLevelSpace->GetOffseted(&vecOffset)->GetTransformed(pTransformIntoGlobalWithVerticalOffset);

  // getting the length of the beam
  result_output += L"  beam length = " + std::to_wstring(pCenterOfMassPath->GetLength()) + L"\n\n";

  // getting the parameter of the corresponding beam start
  double parameter = pBaseLineInLevelSpace->MinParameter;

  auto pProfilePlacementOnBaseline = pBeamParams->GetProfilePlacementOnBaseline(parameter);
  for (int i = 0; i < pProfile->Regions->Count; ++i)
  {
    auto pCurrentRegion = pProfile->Regions->Get(i);
    for (int j = 0; j < pCurrentRegion->GetContourCount(); ++j)
    {
      auto currentContour = pCurrentRegion->GetContour(j)->GetTransformed(pProfilePlacement->GetTransformFrom());

      // obtaining the curve of one of the regions of the beam profile at the beginning of the beam
      auto pSectionCurve = currentContour->CreateCurve3D(pProfilePlacementOnBaseline)->GetTransformed(pTransformIntoGlobalWithVerticalOffset);
    }
  }
  return result_output;
}

std::wstring BeamParamsClickHandler::addPropertyToLevelObjects()
{
  auto pProject = m_pApplication->GetProject();

  if (!pProject)
    return L"The Renga project is null";

  std::wstring result_output = L"Beam Params:\n";

  auto pBeamStyleManager = pProject->BeamStyleManager;

  auto pModelObjects = pProject->Model->GetObjects();
  for (int i = 0; i < pModelObjects->Count; i++)
  {
    auto pModelObject = pModelObjects->GetByIndex(i);

    Renga::IBeamParamsPtr pBeamParams;
    pModelObject->QueryInterface(&pBeamParams);
    if (pBeamParams)
    {
      auto pLevelPlacement = getLevelPlacement(pModelObjects, pModelObject);
      result_output += buildBeamParamsDescription(pBeamParams, pBeamStyleManager, pLevelPlacement);
    }
  }
  return result_output;
}
