/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#pragma once

#import <RengaCOMAPI.tlb>


struct PlateParameters
{
  Renga::Point3D point;
  Renga::IEntityPtr pStyle;
  double phi;
  double theta;
  double psi;
};

void createColumn(
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    Renga::Placement3D placement,
    Renga::IEntityPtr pStyle,
    double height);

void createPlate(
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    Renga::Placement3D placement,
    Renga::IEntityPtr pStyle,
    double plateThickness,
    int alignType);

void createPlate(
    const PlateParameters& param,
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    double plateThickness,
    int alignType);

void createAssemblyInstance(
    Renga::IModelPtr pModel,
    Renga::IOperationPtr pOperation,
    Renga::Placement3D placement,
    Renga::IEntityPtr pStyle);
