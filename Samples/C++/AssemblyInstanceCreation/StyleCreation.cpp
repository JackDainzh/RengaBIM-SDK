/*  Sample: AssemblyInstanceCreation
 *
 *  This sample illustrates how to create a complex assembly style and apply it to an assembly instance.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

#include "StyleCreation.h"

#include <Renga/ObjectTypes.h>
#include <Renga/ParameterIds.h>
#include <Renga/ProfileIds.h>
#include <Renga/ProfileParameterIds.h>
#include <Renga/StyleTypeIds.h>


Renga::IEntityPtr createRectanglePlateStyle(
    double width,
    double height,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject)
{
  auto pOperation = pProject->CreateOperation();
  auto pArguments = pProject->Model->CreateNewEntityArgs();

  pOperation->Start();
  pArguments->TypeId = Renga::StyleTypeIds::PlateStyle;
  auto pPlateStyle   = pProject->PlateStyles->Create(pArguments);

  auto pPlateStyleParams = Renga::IParameterContainerPtr{};
  pPlateStyle->QueryInterface(&pPlateStyleParams);

  auto pPlateStyleProfile = pPlateStyleParams->Get(Renga::ParameterIds::ProfileId);
  auto pRectangleProfile  = pProject->Profiles->GetByUniqueId(Renga::ProfileIds::RectangleProfileId);
  pPlateStyleProfile->SetIntValue(pRectangleProfile->Id);

  auto pPlateStyleWidth = pPlateStyleParams->Get(Renga::ProfileParameterIds::rect_params::Width);
  pPlateStyleWidth->SetDoubleValue(width);

  auto pPlateStyleHeight = pPlateStyleParams->Get(Renga::ProfileParameterIds::rect_params::Height);
  pPlateStyleHeight->SetDoubleValue(height);

  auto pPlateStyleName = pPlateStyleParams->Get(Renga::ParameterIds::EntityName);
  pPlateStyleName->SetStringValue(styleName.data());
  pOperation->Apply();

  return pPlateStyle;
}

Renga::IEntityPtr createIShapedColumnStyle(
    double columnProfileDepth,
    double columnProfileFilletRadius,
    double columnProfileFlangeThickness,
    double columnProfileWebThickness,
    double columnProfileWidth,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject)
{
  auto pOperation = pProject->CreateOperation();
  auto pArguments = pProject->Model->CreateNewEntityArgs();

  pOperation->Start();
  pArguments->TypeId = Renga::StyleTypeIds::ColumnStyle;
  auto pColumnStyle  = pProject->ColumnStyles->Create(pArguments);
  pOperation->Apply();

  pOperation->Start();
  auto pColumnStyleParams = Renga::IParameterContainerPtr{};
  pColumnStyle->QueryInterface(&pColumnStyleParams);

  auto pProfileStyle = pColumnStyleParams->Get(Renga::ParameterIds::ProfileId);
  auto iShapeProfile = pProject->Profiles->GetByUniqueId(Renga::ProfileIds::IShapeProfileId);
  pProfileStyle->SetIntValue(iShapeProfile->Id);

  auto pProfileName = pColumnStyleParams->Get(Renga::ParameterIds::EntityName);
  pProfileName->SetStringValue(styleName.data());

  auto pDepth = pColumnStyleParams->Get(Renga::ProfileParameterIds::i_shape_params::Depth);
  pDepth->SetDoubleValue(columnProfileDepth);

  auto pFilletRadius = pColumnStyleParams->Get(Renga::ProfileParameterIds::i_shape_params::FilletRadius);
  pFilletRadius->SetDoubleValue(columnProfileFilletRadius);

  auto pFlangeThickness = pColumnStyleParams->Get(Renga::ProfileParameterIds::i_shape_params::FlangeThickness);
  pFlangeThickness->SetDoubleValue(columnProfileFlangeThickness);

  auto pWebThickness = pColumnStyleParams->Get(Renga::ProfileParameterIds::i_shape_params::WebThickness);
  pWebThickness->SetDoubleValue(columnProfileWebThickness);

  auto pWidth = pColumnStyleParams->Get(Renga::ProfileParameterIds::i_shape_params::Width);
  pWidth->SetDoubleValue(columnProfileWidth);
  pOperation->Apply();

  return pColumnStyle;
}

Renga::IEntityPtr createOneBevelGussetPlateStyle(
    double plateWidth,
    double plateLength,
    double plateBevelLength,
    double plateBevelWidth,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject)
{
  auto pOperation = pProject->CreateOperation();
  auto pArguments = pProject->Model->CreateNewEntityArgs();

  pOperation->Start();
  pArguments->TypeId = Renga::StyleTypeIds::PlateStyle;
  auto pPlateStyle   = pProject->PlateStyles->Create(pArguments);
  pOperation->Apply();

  pOperation->Start();
  auto pPlateStyleParams = Renga::IParameterContainerPtr{};
  pPlateStyle->QueryInterface(&pPlateStyleParams);

  auto pPlateStyleProfile      = pPlateStyleParams->Get(Renga::ParameterIds::ProfileId);
  auto pOneBevelGaussetProfile = pProject->Profiles->GetByUniqueId(Renga::ProfileIds::OneBevelGussetProfileId);
  pPlateStyleProfile->SetIntValue(pOneBevelGaussetProfile->Id);

  auto pPlateStyleWidth = pPlateStyleParams->Get(Renga::ProfileParameterIds::one_bevel_gusset_params::Width);
  pPlateStyleWidth->SetDoubleValue(plateWidth);

  auto pPlateStyleLength = pPlateStyleParams->Get(Renga::ProfileParameterIds::one_bevel_gusset_params::Length);
  pPlateStyleLength->SetDoubleValue(plateLength);

  auto pPlateStyleBevelLength =
      pPlateStyleParams->Get(Renga::ProfileParameterIds::one_bevel_gusset_params::BevelLength);
  pPlateStyleBevelLength->SetDoubleValue(plateBevelLength);

  auto pPlateStyleBevelWidth = pPlateStyleParams->Get(Renga::ProfileParameterIds::one_bevel_gusset_params::BevelWidth);
  pPlateStyleBevelWidth->SetDoubleValue(plateBevelWidth);

  auto pPlateStyleName = pPlateStyleParams->Get(Renga::ParameterIds::EntityName);
  pPlateStyleName->SetStringValue(styleName.data());
  pOperation->Apply();

  return pPlateStyle;
}

Renga::IEntityPtr createTwoBevelGussetPlateStyle(
    double plateWidth,
    double plateLength,
    double plateBevelLength,
    double plateBevelWidth,
    const std::wstring& styleName,
    Renga::IProjectPtr pProject)
{
  auto pOperation = pProject->CreateOperation();
  auto pArguments = pProject->Model->CreateNewEntityArgs();

  pOperation->Start();
  pArguments->TypeId = Renga::StyleTypeIds::PlateStyle;
  auto pPlateStyle   = pProject->PlateStyles->Create(pArguments);
  pOperation->Apply();

  pOperation->Start();
  auto pPlateStyleParams = Renga::IParameterContainerPtr{};
  pPlateStyle->QueryInterface(&pPlateStyleParams);

  auto pPlateStyleProfile      = pPlateStyleParams->Get(Renga::ParameterIds::ProfileId);
  auto pTwoBevelGaussetProfile = pProject->Profiles->GetByUniqueId(Renga::ProfileIds::TwoBevelGussetProfileId);
  pPlateStyleProfile->SetIntValue(pTwoBevelGaussetProfile->Id);

  auto pPlateStyleWidth = pPlateStyleParams->Get(Renga::ProfileParameterIds::two_bevel_gusset_params::Width);
  pPlateStyleWidth->SetDoubleValue(plateWidth);

  auto pPlateStyleLength = pPlateStyleParams->Get(Renga::ProfileParameterIds::two_bevel_gusset_params::Length);
  pPlateStyleLength->SetDoubleValue(plateLength);

  auto pPlateStyleBevelLength =
      pPlateStyleParams->Get(Renga::ProfileParameterIds::two_bevel_gusset_params::BevelLength);
  pPlateStyleBevelLength->SetDoubleValue(plateBevelLength);

  auto pPlateStyleBevelWidth = pPlateStyleParams->Get(Renga::ProfileParameterIds::two_bevel_gusset_params::BevelWidth);
  pPlateStyleBevelWidth->SetDoubleValue(plateBevelWidth);

  auto pPlateStyleName = pPlateStyleParams->Get(Renga::ParameterIds::EntityName);
  pPlateStyleName->SetStringValue(styleName.data());
  pOperation->Apply();

  return pPlateStyle;
}

Renga::IEntityPtr createAssemblyStyle(const std::wstring& styleName, Renga::IProjectPtr pProject)
{
  auto pOperation = pProject->CreateOperation();
  auto pArguments = pProject->Model->CreateNewEntityArgs();
  auto pAssemblies = pProject->GetAssemblies();

  pOperation->Start();
  auto pAssembly            = pAssemblies->Create(pArguments);
  auto pAssemblyParameters = Renga::IParameterContainerPtr{};
  pAssembly->QueryInterface(&pAssemblyParameters);
  auto pAssemblyName = pAssemblyParameters->Get(Renga::ParameterIds::EntityName);
  pAssemblyName->SetStringValue("Demo column");
  pOperation->Apply();

  return pAssembly;
}
