/* Sample: Geometry Export
 *
 * This sample illustrates how to export object geometry via the Renga API.
 * 
 * Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System;
using System.Text;

namespace GeometryExport
{
  public class GeometryExportPlugin : Renga.IPlugin
  {
    private Renga.Application m_application;

    private Renga.ActionEventSource m_exportObjectsActionEventSource;
    private Renga.ActionEventSource m_exportGridsActionEventSource;

    public bool Initialize(string pluginFolder)
    {
      m_application = new Renga.Application();

      var ui = m_application.UI;

      var uiPanelExtension = ui.CreateUIPanelExtension();

      var exportObjectsAction = ui.CreateAction();
      exportObjectsAction.ToolTip = "Export Objects";
      m_exportObjectsActionEventSource = new Renga.ActionEventSource(exportObjectsAction);
      m_exportObjectsActionEventSource.Triggered += OnExportObjectsButtonClicked;
      uiPanelExtension.AddToolButton(exportObjectsAction);

      var exportGridsAction = ui.CreateAction();
      exportGridsAction.ToolTip = "Export Grids";
      m_exportGridsActionEventSource = new Renga.ActionEventSource(exportGridsAction);
      m_exportGridsActionEventSource.Triggered += OnExportGridsButtonClicked;
      uiPanelExtension.AddToolButton(exportGridsAction);

      ui.AddExtensionToPrimaryPanel(uiPanelExtension);

      return true;
    }

    public void Stop()
    {
      m_exportObjectsActionEventSource.Dispose();
      m_exportGridsActionEventSource.Dispose();
    }

    private void OnExportObjectsButtonClicked(object sender, EventArgs args)
    {
      string commandOutput = Export3DObjects();

      var ui = m_application.UI;
      ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info, "Export 3D objects result output", commandOutput);
    }

    private void OnExportGridsButtonClicked(object sender, EventArgs args)
    {
      string commandOutput = ExportGrids();

      var ui = m_application.UI;
      ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info, "Export grids result output", commandOutput);
    }

    private string Export3DObjects()
    {
      var project = m_application.Project;

      var model = project.Model;
      var modelObjects = model.GetObjects();

      var dataExporter = project.DataExporter;
      var object3dCollection = dataExporter.GetObjects3D();

      var resultOutput = new StringBuilder();
      resultOutput.AppendLine(object3dCollection.Count.ToString() + " objects exported:");
      resultOutput.AppendLine();

      for (int i = 0; i < object3dCollection.Count; i++)
      {
        int gridCount = 0;
        int vertexCount = 0;
        int normalCount = 0;
        int triangleCount = 0;

        var object3d = object3dCollection.Get(i);

        for (int meshIndex = 0; meshIndex < object3d.MeshCount; meshIndex++)
        {
          var mesh = object3d.GetMesh(meshIndex);
          gridCount += mesh.GridCount;
          for (int gridIndex = 0; gridIndex < mesh.GridCount; gridIndex++)
          {
            var grid = mesh.GetGrid(gridIndex);
            vertexCount += grid.VertexCount;
            normalCount += grid.NormalCount;
            triangleCount += grid.TriangleCount;
          }
        }

        var modelObjectId = object3d.ModelObjectId;
        var modelObject = modelObjects.GetById(modelObjectId);

        resultOutput.AppendLine("Object {" + (i + 1) + "} " + modelObject.Name);
        resultOutput.AppendLine("  Object type: " + object3d.ModelObjectType);
        resultOutput.AppendLine("  Mesh count: " + object3d.MeshCount);
        resultOutput.AppendLine("  Grid count: " + gridCount);
        resultOutput.AppendLine("  Vertex count: " + vertexCount);
        resultOutput.AppendLine("  Normal count: " + normalCount);
        resultOutput.AppendLine("  Triangle count: " + triangleCount);

        var objectWithMaterial = modelObject as Renga.IObjectWithMaterial;

        if (objectWithMaterial != null)
        {
          resultOutput.AppendLine("  Material id: " + objectWithMaterial.MaterialId);
        }

        var objectWithLayeredMaterial = modelObject as Renga.IObjectWithLayeredMaterial;

        if (objectWithLayeredMaterial != null)
        {
          resultOutput.AppendLine("  Layers material id: " + objectWithLayeredMaterial.LayeredMaterialId);
          resultOutput.Append("  Layers material group: ");

          switch (objectWithLayeredMaterial.GetLayeredMaterialIdGroupPair().Group)
          {
            case Renga.LayeredMaterialGroup.LayeredMaterialGroup_Undefined:
              resultOutput.AppendLine("Undefined");
              break;

            case Renga.LayeredMaterialGroup.LayeredMaterialGroup_Wall:
              resultOutput.AppendLine("Wall");
              break;

            case Renga.LayeredMaterialGroup.LayeredMaterialGroup_Floor:
              resultOutput.AppendLine("Floor");
              break;

            case Renga.LayeredMaterialGroup.LayeredMaterialGroup_Roof:
              resultOutput.AppendLine("Roof");
              break;
          }
        }
      }

      return resultOutput.ToString();
    }

    private string ExportGrids()
    {
      var project = m_application.Project;
      var dataExporter = project.DataExporter;

      var resultOutput = new StringBuilder();

      var gridWithMaterialCollection = dataExporter.GetGrids();
      resultOutput.AppendLine(gridWithMaterialCollection.Count + " grids exported:");
      resultOutput.AppendLine();

      for (int i = 0; i < gridWithMaterialCollection.Count; i++)
      {
        var gridWithMaterial = gridWithMaterialCollection.Get(i);
        var grid = gridWithMaterial.Grid;

        resultOutput.AppendLine("Grid #" + (i + 1));
        resultOutput.AppendLine(" Vertex count: " + grid.VertexCount);
        resultOutput.AppendLine("  Normal count: " + grid.NormalCount);
        resultOutput.AppendLine("  Triangle count: " + grid.TriangleCount);
        resultOutput.AppendLine("  Material id: " + gridWithMaterial.Material.Id);
      }

      return resultOutput.ToString();
    }
  }
}
