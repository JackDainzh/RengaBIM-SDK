/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#include "AssemblyCreation.h"

#include "ObjectCreation.h"
#include "Settings.h"
#include "StyleCreation.h"

#include <Renga/ObjectTypes.h>
#include <Renga/ParameterIds.h>
#include <Renga/ProfileIds.h>
#include <Renga/ProfileParameterIds.h>
#include <Renga/StyleTypeIds.h>

#include <list>
#include <string>


void create(Renga::IProjectPtr pProject)
{
  // create operation
  auto pOperation = pProject->CreateOperation();

  // create assembly style
  auto pAssembly = createAssemblyStyle(L"Demo column", pProject);

  // create bottom plate style
  auto pBottomPlateStyle =
      createRectanglePlateStyle(bottomPlateWidth, bottomPlateHeight, L"Demo bottom plate syle", pProject);

  // create top plate style
  auto pTopPlateStyle = createRectanglePlateStyle(topPlateWidth, topPlateHeight, L"Demo top plate syle", pProject);

  // create placement
  auto origin    = Renga::Point3D{0, 0, 0};
  auto placement = Renga::Placement3D{origin, Renga::Vector3D{1, 0, 0}, Renga::Vector3D{0, 0, 1}};

  // Assembly to assembly model
  auto pAssemblyModel = Renga::IModelPtr{};
  pAssembly->QueryInterface(&pAssemblyModel);

  // create bottom plate
  createPlate(pAssemblyModel, pOperation, placement, pBottomPlateStyle, bottomPlateThickness, 1);

  // move placement for column
  placement.Origin.Z += bottomPlateThickness;

  // create column style
  auto pColumnStyle = createIShapedColumnStyle(
      columnProfileDepth,
      columnProfileFilletRadius,
      columnProfileFlangeThickness,
      columnProfileWebThickness,
      columnProfileWidth,
      L"Demo column style",
      pProject);

  // create bottom column
  createColumn(pAssemblyModel, pOperation, placement, pColumnStyle, columnHeight);

  // move placement for column
  placement.Origin.Z += columnHeight;

  // create top column
  createColumn(pAssemblyModel, pOperation, placement, pColumnStyle, columnHeight);

  // move placement for top plate
  placement.Origin.Z += columnHeight;

  // create top plate
  createPlate(pAssemblyModel, pOperation, placement, pTopPlateStyle, topPlateThickness, 1);

  // create style for support plates
  auto pSupportPlateStyle = createOneBevelGussetPlateStyle(
      supportPlateWidth,
      supportPlateLength,
      supportPlateBevelLength,
      supportPlateBevelWidth,
      L"Demo support plate syle",
      pProject);

  std::list<PlateParameters> supportPlateParametersList;
  supportPlateParametersList.emplace_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = columnProfileWidth / 2,
              .Y = -columnProfileDepth / 2 + columnProfileFlangeThickness,
              .Z = bottomPlateThickness},
      .pStyle = pSupportPlateStyle,
      .phi    = 0,
      .theta  = 90,
      .psi    = 0});
  supportPlateParametersList.push_back(PlateParameters{
      .point  = Renga::Point3D{.X = columnProfileWidth / 2, .Y = columnProfileDepth / 2, .Z = bottomPlateThickness},
      .pStyle = pSupportPlateStyle,
      .phi    = 0,
      .theta  = 90,
      .psi    = 0});
  supportPlateParametersList.emplace_back(PlateParameters{
      .point =
          Renga::Point3D{.X = -columnProfileWebThickness / 2, .Y = columnProfileDepth / 2, .Z = bottomPlateThickness},
      .pStyle = pSupportPlateStyle,
      .phi    = 90,
      .theta  = 90,
      .psi    = 0});
  supportPlateParametersList.emplace_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = -columnProfileWidth / 2,
              .Y = columnProfileDepth / 2 - columnProfileFlangeThickness,
              .Z = bottomPlateThickness},
      .pStyle = pSupportPlateStyle,
      .phi    = 180,
      .theta  = 90,
      .psi    = 0});
  supportPlateParametersList.emplace_back(PlateParameters{
      .point  = Renga::Point3D{.X = -columnProfileWidth / 2, .Y = -columnProfileDepth / 2, .Z = bottomPlateThickness},
      .pStyle = pSupportPlateStyle,
      .phi    = 180,
      .theta  = 90,
      .psi    = 0});
  supportPlateParametersList.emplace_back(PlateParameters{
      .point =
          Renga::Point3D{.X = columnProfileWebThickness / 2, .Y = -columnProfileDepth / 2, .Z = bottomPlateThickness},
      .pStyle = pSupportPlateStyle,
      .phi    = 270,
      .theta  = 90,
      .psi    = 0});

  for (const auto& param : supportPlateParametersList)
    createPlate(param, pAssemblyModel, pOperation, supportPlateBevelThickness, 6);

  // create fastening 1 style
  auto pFastening1PlateStyle =
      createRectanglePlateStyle(widthFastening1Plate, heightFastening, L"Demo fastening plate 1", pProject);

  // create fastening 2 style
  auto pFastening2PlateStyle =
      createRectanglePlateStyle(widthFastening2Plate, heightFastening, L"Demo fastening plate 2", pProject);

  // create fastening 3 style
  auto pFastening3PlateStyle =
      createRectanglePlateStyle(widthFastening3Plate, heightFastening, L"Demo fastening plate 3", pProject); 

  // create fastening plates
  auto zForfastenedPlates = bottomPlateThickness + columnHeight;
  std::list<PlateParameters> fastenedPlateParametersList;
  fastenedPlateParametersList.push_back(PlateParameters{
      .point  = Renga::Point3D{.X = columnProfileWebThickness / 2, .Y = 0, .Z = zForfastenedPlates},
      .pStyle = pFastening1PlateStyle,
      .phi    = 90,
      .theta  = 90,
      .psi    = 0});
  fastenedPlateParametersList.push_back(PlateParameters{
      .point =
          Renga::Point3D{.X = -columnProfileWebThickness / 2 - thicknessFastening, .Y = 0, .Z = zForfastenedPlates},
      .pStyle = pFastening1PlateStyle,
      .phi    = 90,
      .theta  = 90,
      .psi    = 0});
  fastenedPlateParametersList.push_back(PlateParameters{
      .point  = Renga::Point3D{.X = 0, .Y = columnProfileDepth / 2 + thicknessFastening, .Z = zForfastenedPlates},
      .pStyle = pFastening2PlateStyle,
      .phi    = 0,
      .theta  = 90,
      .psi    = 0});
  fastenedPlateParametersList.push_back(PlateParameters{
      .point  = Renga::Point3D{.X = 0, .Y = -columnProfileDepth / 2, .Z = zForfastenedPlates},
      .pStyle = pFastening2PlateStyle,
      .phi    = 0,
      .theta  = 90,
      .psi    = 0});
  fastenedPlateParametersList.push_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = columnProfileWidth / 2 - widthFastening3Plate / 2,
              .Y = columnProfileDepth / 2 - columnProfileFlangeThickness - thicknessFastening,
              .Z = zForfastenedPlates},
      .pStyle = pFastening3PlateStyle,
      .phi    = 180,
      .theta  = 90,
      .psi    = 0});
  fastenedPlateParametersList.push_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = columnProfileWidth / 2 - widthFastening3Plate / 2,
              .Y = -columnProfileDepth / 2 + columnProfileFlangeThickness,
              .Z = zForfastenedPlates},
      .pStyle = pFastening3PlateStyle,
      .phi    = 180,
      .theta  = 90,
      .psi    = 0});
  fastenedPlateParametersList.push_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = -columnProfileWidth / 2 + widthFastening3Plate / 2,
              .Y = columnProfileDepth / 2 - columnProfileFlangeThickness,
              .Z = zForfastenedPlates},
      .pStyle = pFastening3PlateStyle,
      .phi    = 0,
      .theta  = 90,
      .psi    = 0});
  fastenedPlateParametersList.push_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = -columnProfileWidth / 2 + widthFastening3Plate / 2,
              .Y = -columnProfileDepth / 2 + columnProfileFlangeThickness + thicknessFastening,
              .Z = zForfastenedPlates},
      .pStyle = pFastening3PlateStyle,
      .phi    = 0,
      .theta  = 90,
      .psi    = 0});

  for (const auto& param : fastenedPlateParametersList)
    createPlate(param, pAssemblyModel, pOperation, thicknessFastening, 1);

  // create shelf plate style
  auto pShelfPlateStyle = createTwoBevelGussetPlateStyle(
      shelfWidth,
      shelfLength,
      shelfBevelLength,
      shelfBevelWidth,
      L"Demo shelf plate syle",
      pProject);

  //create overlay plate style
  auto pOverlayPlateStyle =
      createRectanglePlateStyle(overlayWidth, overlayHeight, L"Demo overlay plate syle", pProject);

  // create top plates
  auto zShelf   = bottomPlateThickness + 2 * columnHeight - overlayWidth - thicknessCommon;
  auto zOverlay = bottomPlateThickness + 2 * columnHeight;

  std::list<PlateParameters> topPlateParametersList;
  topPlateParametersList.push_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = columnProfileWidth / 2,
              .Y = -columnProfileDepth / 2 + columnProfileFlangeThickness,
              .Z = zShelf},
      .pStyle = pShelfPlateStyle,
      .phi    = 90,
      .theta  = 0,
      .psi    = 0});
  topPlateParametersList.push_back(PlateParameters{
      .point =
          Renga::Point3D{
              .X = -columnProfileWidth / 2,
              .Y = columnProfileDepth / 2 - columnProfileFlangeThickness,
              .Z = zShelf},
      .pStyle = pShelfPlateStyle,
      .phi    = 270,
      .theta  = 0,
      .psi    = 0});
  topPlateParametersList.push_back(PlateParameters{
      .point  = Renga::Point3D{.X = columnProfileWebThickness / 2, .Y = thicknessCommon / 2, .Z = zOverlay},
      .pStyle = pOverlayPlateStyle,
      .phi    = 0,
      .theta  = 90,
      .psi    = 270});
  topPlateParametersList.push_back(PlateParameters{
      .point  = Renga::Point3D{.X = -columnProfileWebThickness / 2, .Y = -thicknessCommon / 2, .Z = zOverlay},
      .pStyle = pOverlayPlateStyle,
      .phi    = 180,
      .theta  = 90,
      .psi    = 270});

  for (const auto& param : topPlateParametersList)
    createPlate(param, pAssemblyModel, pOperation, thicknessCommon, 6);

  // create placement for assembly instance
  auto assemblyInstanceOrigin = Renga::Point3D{0, 0, 0};
  auto assemblyInstancePlacement =
      Renga::Placement3D{assemblyInstanceOrigin, Renga::Vector3D{1, 0, 0}, Renga::Vector3D{0, 0, 1}};

  // create instance
  createAssemblyInstance(pProject->Model, pOperation, assemblyInstancePlacement, pAssembly);
}
