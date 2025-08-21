/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#pragma once

#import <RengaCOMAPI.tlb>

#include <string>


Renga::IEntityPtr createRectanglePlateStyle(
    double width,
    double height,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject);

Renga::IEntityPtr createIShapedColumnStyle(
    double columnProfileDepth,
    double columnProfileFilletRadius,
    double columnProfileFlangeThickness,
    double columnProfileWebThickness,
    double columnProfileWidth,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject);

Renga::IEntityPtr createOneBevelGussetPlateStyle(
    double plateWidth,
    double plateLength,
    double plateBevelLength,
    double plateBevelWidth,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject);

Renga::IEntityPtr createTwoBevelGussetPlateStyle(
    double plateWidth,
    double plateLength,
    double plateBevelLength,
    double plateBevelWidth,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject);

Renga::IEntityPtr createAssemblyStyle(const std::wstring& styleName, Renga::IProjectPtr pProject);
