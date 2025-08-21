/*  Sample: Geometry Grid Types
 *
 *  This sample illustrates how to work with export object geometry grid types via the Renga API.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System;
using System.Text;

namespace GeometryGridTypes
{
  public class GeometryGridTypesPlugin : Renga.IPlugin
  {
    private Renga.Application m_app;
    private Renga.ActionEventSource m_events;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;

      var action = ui.CreateAction();
      action.ToolTip = "Export geometry and discover mesh and grid types";

      m_events = new Renga.ActionEventSource(action);
      m_events.Triggered += (s, e) => { ReportSceneContents(); };

      var panelExtension = ui.CreateUIPanelExtension();
      panelExtension.AddToolButton(action);

      ui.AddExtensionToPrimaryPanel(panelExtension);
      return true;
    }

    public void Stop()
    {
      m_events.Dispose();
    }


    private void ReportSceneContents()
    {
      var project = m_app.Project;
      var dataExporter = project.DataExporter;

      var stringBuilder = new StringBuilder();
      gatherExportData(stringBuilder, dataExporter.GetObjects3D());

      m_app.UI.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info,
                                   "Result of exporting the 3D objects",
                                   stringBuilder.ToString());
    }

    private void gatherExportData(StringBuilder builder, Renga.IExportedObject3DCollection object3DCollection)
    {
      builder.AppendLine("Exported objects:");
      builder.AppendLine();

      int size = object3DCollection.Count;
      for (int i = 0; i < size; ++i)
        gatherExportData(builder, object3DCollection.Get(i));
    }

    private void gatherExportData(StringBuilder builder, Renga.IExportedObject3D object3D)
    {
      var id = object3D.ModelObjectId;
      var modelObject = m_app.Project.Model.GetObjects().GetById(id);

      builder.Append("Object: ");
      builder.AppendLine(modelObject?.Name ?? "undefined model object");

      var meshCount = object3D.MeshCount;
      for (int i = 0; i < meshCount; ++i)
        gatherExportData(builder, object3D.GetMesh(i), object3D.ModelObjectType);
    }

    private void gatherExportData(StringBuilder builder, Renga.IMesh mesh, Guid objectTypeGuid)
    {
      builder.Append("Mesh: type - ");

      var meshType = mesh.MeshType;
      if (meshType == Renga.MeshTypes.Undefined)
        builder.AppendLine("undefined");
      else if (meshType == Renga.MeshTypes.DoorReveal)
        builder.AppendLine("door reveal");
      else if (meshType == Renga.MeshTypes.DoorPanel)
        builder.AppendLine("door panel");
      else if (meshType == Renga.MeshTypes.DoorTransom)
        builder.AppendLine("door transom");
      else if (meshType == Renga.MeshTypes.DoorLining)
		builder.AppendLine("door lining");
      else if (meshType == Renga.MeshTypes.DoorThreshold)
		builder.AppendLine("door threshold");
      else if (meshType == Renga.MeshTypes.DoorPlatband)
	    builder.AppendLine("door platband");
      else if (meshType == Renga.MeshTypes.WindowReveal)
        builder.AppendLine("window reveal");
      else if (meshType == Renga.MeshTypes.WindowPanel)
        builder.AppendLine("window panel");
      else if (meshType == Renga.MeshTypes.WindowSill)
        builder.AppendLine("window sill");
      else if (meshType == Renga.MeshTypes.WindowOutwardSill)
        builder.AppendLine("window outward sill");

      var gridCount = mesh.GridCount;
      for (int i = 0; i < gridCount; ++i)
        gatherExportData(builder, mesh.GetGrid(i), objectTypeGuid);
    }

    void gatherExportData(StringBuilder builder, Renga.IGrid grid, Guid objectTypeGuid)
    {
      if (objectTypeGuid == Renga.ObjectTypes.Wall)
      {
        builder.AppendLine("  Grid: type - wall ");
        switch ((Renga.GridTypes.Wall)grid.GridType)
        {
        case Renga.GridTypes.Wall.Undefined:
          builder.AppendLine("undefined");
          break;
        case Renga.GridTypes.Wall.FrontSide:
          builder.AppendLine("front side");
          break;
        case Renga.GridTypes.Wall.BackSide:
          builder.AppendLine("back side");
          break;
        case Renga.GridTypes.Wall.Bottom:
          builder.AppendLine("bottom");
          break;
        case Renga.GridTypes.Wall.Top:
          builder.AppendLine("top");
          break;
        case Renga.GridTypes.Wall.FirstButt:
          builder.AppendLine("first butt");
          break;
        case Renga.GridTypes.Wall.SecondButt:
          builder.AppendLine("second butt");
          break;
        case Renga.GridTypes.Wall.Reveal:
          builder.AppendLine("reveal");
          break;
        case Renga.GridTypes.Wall.Cut:
          builder.AppendLine("cut");
          break;
        }
      }
    }
  }
}
