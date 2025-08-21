/*  Sample: Model Object Quantities
 *
 *  This sample illustrates how to obtain quantities of model objects.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#include "ModelObjectQuantitiesPlugin.h"

#include <sstream>

#include <Renga/ObjectTypes.h>
#include <Renga/QuantityIds.h>
#include <Renga/CreateApplication.hpp>


ModelObjectQuantitiesPlugin::ModelObjectQuantitiesPlugin()
{
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

ModelObjectQuantitiesPlugin::~ModelObjectQuantitiesPlugin()
{
  ::CoUninitialize();
}

bool ModelObjectQuantitiesPlugin::initialize(const wchar_t* pluginDir)
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
        pAction->PutToolTip(L"Obtain object quantities");

        m_pObtainObjectQuantitiesButtonClickHandler.reset(new ObtainObjectQuantitiesButtonClickHandler(pAction, pApplication));

        pUIPanelExtension->AddToolButton(pAction);
      }
      pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
    }
  }
  return true;
}

void ModelObjectQuantitiesPlugin::stop()
{
  m_pObtainObjectQuantitiesButtonClickHandler.reset();
}

ObtainObjectQuantitiesButtonClickHandler::ObtainObjectQuantitiesButtonClickHandler(Renga::IActionPtr pAction, Renga::IApplicationPtr pApplication) :
  Renga::ActionEventHandler(pAction),
  m_pApplication(pApplication)
{
}

ObtainObjectQuantitiesButtonClickHandler::~ObtainObjectQuantitiesButtonClickHandler()
{
}

void ObtainObjectQuantitiesButtonClickHandler::OnTriggered()
{
  std::wstring command_output = obtainObjectQuantities();

  MessageBox(NULL,
    (LPCWSTR)command_output.c_str(),
    (LPCWSTR)L"Model object quantities result output",
    MB_ICONINFORMATION | MB_OK );
}

std::wstring ObtainObjectQuantitiesButtonClickHandler::obtainObjectQuantities() const
{
  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  auto pModelCollection = pModel->GetObjects();

  std::wstringstream result_output;
  result_output << "Renga beams and floors quantities:" << "\n\n";

  for (int i = 0; i < pModelCollection->GetCount(); ++i)
  {
    const auto pModelObject = pModelCollection->GetByIndex(i);
    const auto type = pModelObject->GetObjectType();

    auto pQuantityContainer = pModelObject->GetQuantities();

    if (::IsEqualGUID(type, Renga::ObjectTypes::Beam))
    {
      result_output << "Object type: " << "Beam" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::NetCrossSectionArea))
        result_output << "Beam cross section area: " << pQuantity->AsArea(Renga::AreaUnit_Meters2) << " m^2." << "\n";
      else
        result_output << "Beam total area of the cross section cannot be calculated" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::NominalLength))
        result_output << "Beam length: " << pQuantity->AsLength(Renga::LengthUnit_Meters) << " m." << "\n";
      else
        result_output << "Beam length cannot be calculated" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::NetVolume))
        result_output << "Beam net volume: " << pQuantity->AsVolume(Renga::VolumeUnit_Meters3) << " m^3." << "\n";
      else
        result_output << "Beam net volume cannot be calculated" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::OuterSurfaceArea))
        result_output << "Beam outer surface area: " << pQuantity->AsArea(Renga::AreaUnit_Meters2) << " m^2." << "\n";
      else
        result_output << "Beam outer surface area cannot be calculated" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::Perimeter))
        result_output << "Beam cross section perimeter: " << pQuantity->AsLength(Renga::LengthUnit_Meters) << " m." << "\n";
      else
        result_output << "Beam cross section perimeter cannot be calculated" << "\n";

      result_output << "\n\n";
    }
    else if (::IsEqualGUID(type, Renga::ObjectTypes::Floor))
    {
      result_output << "Object type: " << "Floor" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::NominalThickness))
        result_output << "Floor nominal thickness: " << pQuantity->AsLength(Renga::LengthUnit_Meters) << " m." << "\n";
      else
        result_output << "Floor nominal thickness cannot be calculated" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::NetVolume))
        result_output << "Floor net volume: " << pQuantity->AsVolume(Renga::VolumeUnit_Meters3) << " m^3." << "\n";
      else
        result_output << "Floor net volume cannot be calculated" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::NetArea))
        result_output << "Floor net area: " << pQuantity->AsArea(Renga::AreaUnit_Meters2) << " m^2." << "\n";
      else
        result_output << "Floor net area cannot be calculated" << "\n";

      if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::Perimeter))
        result_output << "Floor perimeter: " << pQuantity->AsLength(Renga::LengthUnit_Meters) << " m." << "\n";
      else
        result_output << "Floor perimeter cannot be calculated" << "\n";


      Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial(nullptr);
      pModelObject->QueryInterface(&pObjectWithLayeredMaterial);

      result_output << "Floor layer quantities: " << "\n";

      const auto pLayerCollection = pObjectWithLayeredMaterial->GetLayers();
      for (int j = 0; j < pLayerCollection->GetCount(); ++j)
      {
        auto pLayer = pLayerCollection->Get(j);
        auto pLayerQuantityContainer = pLayer->GetQuantities();

        if (auto pQuantity = pLayerQuantityContainer->Get(Renga::QuantityIds::NetVolume))
          result_output << "Floor layer " << j << " net volume: " << pQuantity->AsVolume(Renga::VolumeUnit_Meters3) << " m^3." << "\n";
        else
          result_output << "Floor layer " << j << " net volume cannot be calculated" << "\n";

        if (auto pQuantity = pLayerQuantityContainer->Get(Renga::QuantityIds::NetArea))
          result_output << "Floor layer " << j << " net area: " << pQuantity->AsArea(Renga::AreaUnit_Meters2) << " m^2." << "\n";
        else
          result_output << "Floor layer " << j << " net area cannot be calculated" << "\n";

        if (auto pQuantity = pLayerQuantityContainer->Get(Renga::QuantityIds::NominalThickness))
          result_output << "Floor layer " << j << " nominal thickness: " << pQuantity->AsLength(Renga::LengthUnit_Meters) << " m." << "\n";
        else
          result_output << "Floor layer " << j << " nominal thickness cannot be calculated" << "\n";
      }
      result_output << "\n\n";
    }
  }

  return result_output.str();
}

