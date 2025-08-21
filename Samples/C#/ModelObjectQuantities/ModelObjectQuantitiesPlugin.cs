/*  Sample: Model Object Quantities
 *
 *  This sample illustrates how to obtain quantities of model objects.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System.Text;

namespace ModelObjectQuantities
{
  public class ModelObjectQuantitiesPlugin : Renga.IPlugin
  {
    private Renga.Application m_app;
    private Renga.ActionEventSource m_events;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();
      var action = ui.CreateAction();
      action.ToolTip = "Obtain object quantities";
      m_events = new Renga.ActionEventSource(action);
      m_events.Triggered += (s, e) =>
      {
        ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info,
                          "Model object quantity report",
                          ObtainObjectQuantities());
      };
      panelExtension.AddToolButton(action);
      ui.AddExtensionToPrimaryPanel(panelExtension);
      return true;
    }

    public void Stop()
    {
      m_events.Dispose();
    }

    private string ObtainObjectQuantities()
    {
      var modelCollection = m_app.Project.Model.GetObjects();

      var result = new StringBuilder("Quantities of beams and floors:\n\n");

      int objectCount = modelCollection.Count;
      for (int i = 0; i < objectCount; ++i)
      {
        var modelObject = modelCollection.GetByIndex(i);
        var type = modelObject.ObjectType;
        var pQuantityContainer = modelObject.GetQuantities();

        if (type == Renga.ObjectTypes.Beam)
        {
          result.AppendLine("Object type: Beam");

          var pQuantity = pQuantityContainer.Get(Renga.QuantityIds.NetCrossSectionArea);
          result.AppendLine(pQuantity != null
            ? $"Beam cross section area: {pQuantity.AsArea(Renga.AreaUnit.AreaUnit_Meters2)} m²."
            : "Beam total area of the cross section cannot be calculated.");

          pQuantity = pQuantityContainer.Get(Renga.QuantityIds.NominalLength);
          result.AppendLine(pQuantity != null
            ? $"Beam length: {pQuantity.AsLength(Renga.LengthUnit.LengthUnit_Meters)} m."
            : "Beam length cannot be calculated.");

          pQuantity = pQuantityContainer.Get(Renga.QuantityIds.NetVolume);
          result.AppendLine(pQuantity != null
            ? $"Beam net volume: {pQuantity.AsVolume(Renga.VolumeUnit.VolumeUnit_Meters3)} m³."
            : "Beam net volume cannot be calculated.");

          pQuantity = pQuantityContainer.Get(Renga.QuantityIds.OuterSurfaceArea);
          result.AppendLine(pQuantity != null
            ? $"Beam outer surface area: {pQuantity.AsArea(Renga.AreaUnit.AreaUnit_Meters2)} m²."
            : "Beam outer surface area cannot be calculated.");

          pQuantity = pQuantityContainer.Get(Renga.QuantityIds.Perimeter);
          result.AppendLine(pQuantity != null
            ? $"Beam cross section perimeter: {pQuantity.AsLength(Renga.LengthUnit.LengthUnit_Meters)} m."
            : "Beam cross section perimeter cannot be calculated.");

          result.AppendLine();
        }
        else if (type == Renga.ObjectTypes.Floor)
        {
          result.AppendLine("Object type: Floor");

          var pQuantity = pQuantityContainer.Get(Renga.QuantityIds.NominalThickness);
          result.AppendLine(pQuantity != null
            ? $"Floor nominal thickness: {pQuantity.AsLength(Renga.LengthUnit.LengthUnit_Meters)} m."
            : "Floor nominal thickness cannot be calculated");

          pQuantity = pQuantityContainer.Get(Renga.QuantityIds.NetVolume);
          result.AppendLine(pQuantity != null
            ? $"Floor net volume: {pQuantity.AsVolume(Renga.VolumeUnit.VolumeUnit_Meters3)} m³"
            : "Floor net volume cannot be calculated");

          pQuantity = pQuantityContainer.Get(Renga.QuantityIds.NetArea);
          result.AppendLine(pQuantity != null
            ? $"Floor net area: {pQuantity.AsArea(Renga.AreaUnit.AreaUnit_Meters2)} m²."
            : "Floor net area cannot be calculated");

          pQuantity = pQuantityContainer.Get(Renga.QuantityIds.Perimeter);
          result.AppendLine(pQuantity != null
            ? $"Floor perimeter: {pQuantity.AsLength(Renga.LengthUnit.LengthUnit_Meters)} m."
            : "Floor perimeter cannot be calculated");


          result.AppendLine("Floor layer quantities: ");

          var objectWithLayeredMaterial = (Renga.IObjectWithLayeredMaterial)modelObject;
          var layerCollection = objectWithLayeredMaterial.GetLayers();
          int layerCount = layerCollection.Count;
          for (int l = 0; l < layerCount; ++l)
          {
            var pLayer = layerCollection.Get(l);
            var pLayerQuantityContainer = pLayer.GetQuantities();

            pQuantity = pLayerQuantityContainer.Get(Renga.QuantityIds.NetVolume);
            result.AppendLine(pQuantity != null
              ? $"Floor layer {l} net volume: {pQuantity.AsVolume(Renga.VolumeUnit.VolumeUnit_Meters3)} m³."
              : $"Floor layer {l} net volume cannot be calculated");

            pQuantity = pLayerQuantityContainer.Get(Renga.QuantityIds.NetArea);
            result.AppendLine(pQuantity != null
              ? $"Floor layer {l} net area: {pQuantity.AsArea(Renga.AreaUnit.AreaUnit_Meters2)} m²."
              : $"Floor layer {l} net area cannot be calculated.");

            pQuantity = pLayerQuantityContainer.Get(Renga.QuantityIds.NominalThickness);
            result.AppendLine(pQuantity != null
              ? $"Floor layer {l} nominal thickness: {pQuantity.AsLength(Renga.LengthUnit.LengthUnit_Meters)} m."
              : $"Floor layer {l} nominal thickness cannot be calculated.");
          }
          result.AppendLine();
        }
      }

      return result.ToString();
    }
  }
}
