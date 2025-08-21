/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#include "ObjectCreation.h"

#include <Renga/ObjectTypes.h>
#include <Renga/ParameterIds.h>

namespace
{
  void initializeColumn(
      Renga::IModelObjectPtr pColumn,
      Renga::IEntityPtr pStyle,
      Renga::IOperationPtr pOperation,
      double height)
  {
    pOperation->Start();
    auto pColumnParams     = pColumn->GetParameters();
    auto pColumnStyleParam = pColumnParams->Get(Renga::ParameterIds::ColumnStyleId);
    pColumnStyleParam->SetIntValue(pStyle->Id);
    pColumnParams->Get(Renga::ParameterIds::ColumnHeight)->SetDoubleValue(height);
    pOperation->Apply();
  }

  void initializePlate(
      Renga::IModelObjectPtr pPlate,
      Renga::IEntityPtr pStyle,
      Renga::IOperationPtr pOperation,
      int alignType,
      double plateThickness)
  {
    pOperation->Start();
    auto pPlateParameters     = pPlate->GetParameters();
    auto pPlateProfileStyleId = pPlateParameters->Get(Renga::ParameterIds::PlateProfileStyleId);
    pPlateProfileStyleId->SetIntValue(pStyle->Id);
    pPlateParameters->Get(Renga::ParameterIds::PlatePositionRelativeToInsertPoint)->SetIntValue(alignType);
    pPlateParameters->Get(Renga::ParameterIds::PlateThickness)->SetDoubleValue(plateThickness);
    pOperation->Apply();
  }

  void initializeAssemblyInstance(
      Renga::IModelObjectPtr pAssemblyInstance,
      Renga::IEntityPtr pStyle,
      Renga::IOperationPtr pOperation)
  {
    pOperation->Start();
    auto pAssemblyInstanceParameters = pAssemblyInstance->GetParameters();
    pAssemblyInstanceParameters->Get(Renga::ParameterIds::AssemblyId)->SetIntValue(pStyle->Id);
    pOperation->Apply();
  }

  void rotatePlate(Renga::IModelObjectPtr pPlate, Renga::IOperationPtr pOperation, double phi, double theta, double psi)
  {
    pOperation->Start();
    auto pPlateParameters = pPlate->GetParameters();
    pPlateParameters->Get(Renga::ParameterIds::RotationAnglePhi)->SetDoubleValue(phi);
    pPlateParameters->Get(Renga::ParameterIds::RotationAngleTheta)->SetDoubleValue(theta);
    pPlateParameters->Get(Renga::ParameterIds::RotationAnglePsi)->SetDoubleValue(psi);
    pOperation->Apply();
  }

  Renga::IModelObjectPtr createObject(
      Renga::IModelPtr pModel,
      Renga::IOperationPtr pOperation,
      Renga::Placement3D placement,
      const GUID type)
  {
    pOperation->Start();
    auto pArguments          = pModel->CreateNewEntityArgs();
    pArguments->TypeId       = type;
    pArguments->_Placement3D = placement;
    auto pObject             = pModel->CreateObject(pArguments);
    pOperation->Apply();

    return pObject;
  }
}

void createColumn(
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    Renga::Placement3D placement,
    Renga::IEntityPtr pStyle,
    double height)
{
  auto pColumn = createObject(pModel, pOperation, placement, Renga::ObjectTypes::Column);

  initializeColumn(pColumn, pStyle, pOperation, height);
}

void createPlate(
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    Renga::Placement3D placement,
    Renga::IEntityPtr pStyle,
    double plateThickness,
    int alignType)
{
  auto pPlate = createObject(pModel, pOperation, placement, Renga::ObjectTypes::Plate);

  initializePlate(pPlate, pStyle, pOperation, alignType, plateThickness);
}

void createPlate(
    const PlateParameters& param,
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    double plateThickness,
    int alignType)
{
  auto originPlate = param.point;
  auto placementPlate =
      Renga::Placement3D{.Origin = originPlate, .xAxis = Renga::Vector3D(1, 0, 0), .zAxis = Renga::Vector3D(0, 0, 1)};

  auto pPlate = createObject(pModel, pOperation, placementPlate, Renga::ObjectTypes::Plate);

  initializePlate(pPlate, param.pStyle, pOperation, alignType, plateThickness);

  rotatePlate(pPlate, pOperation, param.phi, param.theta, param.psi);
}

void createAssemblyInstance(
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    Renga::Placement3D placement,
    Renga::IEntityPtr pStyle)
{
  auto pAssemblyInstance = createObject(pModel, pOperation, placement, Renga::ObjectTypes::AssemblyInstance);

  initializeAssemblyInstance(pAssemblyInstance, pStyle, pOperation);
}
