/*  Material Export
 *
 *  This sample illustrates how to export 3D object material via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System;
using System.Text;

namespace MaterialExport
{
  public class MaterialExportPlugin : Renga.IPlugin
  {
    private Renga.Application m_application;
    private Renga.ActionEventSource m_actionEventSource;

    public bool Initialize(string pluginFolder)
    {
      m_application = new Renga.Application();

      var ui = m_application.UI;

      var uiPanelExtension = ui.CreateUIPanelExtension();

      var action = ui.CreateAction();
      action.ToolTip = "Export Objects";

      m_actionEventSource = new Renga.ActionEventSource(action);
      m_actionEventSource.Triggered += OnExport3DObjectsMaterialsButtonClicked;

      uiPanelExtension.AddToolButton(action);

      ui.AddExtensionToPrimaryPanel(uiPanelExtension);

      return true;
    }

    public void Stop()
    {
      m_actionEventSource.Dispose();
    }

    private void OnExport3DObjectsMaterialsButtonClicked(object sender, EventArgs args)
    {
      string commandOutput = Export3DObjectsMaterials();

      var ui = m_application.UI;
      ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info,
        "Export 3D objects materials result output", commandOutput);
    }

    private string Export3DObjectsMaterials()
    {
      var project = m_application.Project;

      var dataExporter = project.DataExporter;
      var object3dCollection = dataExporter.GetObjects3D();

      var model = project.Model;
      var modelObjects = model.GetObjects();

      var materialManager = project.MaterialManager;
      var layeredMaterialManager = project.LayeredMaterialManager;

      var result = new StringBuilder();
      result.AppendLine(object3dCollection.Count.ToString() + " objects exported:");
      result.AppendLine();

      for (int i = 0; i < object3dCollection.Count; i++)
      {
        var object3d = object3dCollection.Get(i);

        var modelObjectId = object3d.ModelObjectId;
        var modelObject = modelObjects.GetById(modelObjectId);

        result.AppendLine("Object: {" + (i + 1) + "}" + modelObject.Name);

        var objectWithMaterial = modelObject as Renga.IObjectWithMaterial;
        if (objectWithMaterial != null)
        {
          result.AppendLine("  Material type: One-layered");

          var material = materialManager.GetMaterial(objectWithMaterial.MaterialId);

          if (material != null)
            result.AppendLine("  Material name: " + material.Name);
          else
            result.AppendLine("  No material assigned");

          result.AppendLine();

          continue;
        }

        var objectWithLayeredMaterial = modelObject as Renga.IObjectWithLayeredMaterial;

        if (objectWithLayeredMaterial != null)
        {
          result.AppendLine("  Material type: Multi-layered");

          var layeredMaterial = layeredMaterialManager.GetLayeredMaterial(objectWithLayeredMaterial.LayeredMaterialId);

          if (layeredMaterial != null)
          {
            result.AppendLine("  Sandwich name: " + layeredMaterial.Name);

            var layers = layeredMaterial.Layers;

            result.AppendLine($"  Sandwich consists of: {layers.Count} layers");
            result.AppendLine();

            for (int l = 0; l < layers.Count; l++)
            {
              var layer = layers.Get(l);

              result.AppendLine("    Layer #" + l);
              result.AppendLine("    Name: " + layer.Material.Name);
              result.AppendLine("    Thickness: " + layer.Thickness);
            }
          }
          else
          {
            result.AppendLine("  No material assigned");
          }

          result.AppendLine();

          continue;
        }

        result.AppendLine("  Material type: None");
        result.AppendLine();
      }

      return result.ToString();
    }
  }
}
