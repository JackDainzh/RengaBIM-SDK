/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#pragma once

// settings
constexpr auto bottomPlateWidth     = 350.;
constexpr auto bottomPlateHeight    = 350.;
constexpr auto bottomPlateThickness = 10.;

constexpr auto topPlateWidth     = 200.;
constexpr auto topPlateHeight    = 200.;
constexpr auto topPlateThickness = 10.;

constexpr auto columnProfileDepth           = 200.;
constexpr auto columnProfileFilletRadius    = 13.;
constexpr auto columnProfileFlangeThickness = 10.;
constexpr auto columnProfileWebThickness    = 6.5;
constexpr auto columnProfileWidth           = 200.;

constexpr auto columnHeight = 3000.;

constexpr auto supportPlateWidth          = 45.;
constexpr auto supportPlateLength         = 100.;
constexpr auto supportPlateBevelLength    = 80.;
constexpr auto supportPlateBevelWidth     = 35.;
constexpr auto supportPlateBevelThickness = 10.;

// calculated settings
constexpr auto widthFastening1Plate =
    columnProfileDepth - 2 * columnProfileFilletRadius - 2 * columnProfileFlangeThickness;
constexpr auto heightFastening      = 300.;
constexpr auto thicknessFastening   = 10.;
constexpr auto widthFastening2Plate = columnProfileWidth;
constexpr auto widthFastening3Plate = (columnProfileWidth - columnProfileWebThickness) / 2 - columnProfileFilletRadius;

constexpr auto shelfWidth       = columnProfileDepth - 2 * columnProfileFlangeThickness;
constexpr auto shelfLength      = (columnProfileWidth - columnProfileWebThickness) / 2;
constexpr auto shelfBevelLength = 1.1 * columnProfileFilletRadius;
constexpr auto shelfBevelWidth  = 1.1 * columnProfileFilletRadius;

constexpr auto overlayWidth    = 120.;
constexpr auto overlayHeight   = (columnProfileWidth - columnProfileWebThickness) / 2;
constexpr auto thicknessCommon = 10.;
