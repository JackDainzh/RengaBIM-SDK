/*  Sample: Model Objects
 *
 *  This sample illustrates how to traverse model objects in a project and read their parameters.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System.Text;

namespace ModelObjects
{
    public class ModelObjectsPlugin : Renga.IPlugin
  {
    private Renga.Application m_app;
    private Renga.ActionEventSource m_events;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();
      var action = ui.CreateAction();
      action.ToolTip = "List objects";
      m_events = new Renga.ActionEventSource(action);
      m_events.Triggered += (s, e) =>
      {
        ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info,
                          "Model object list",
                          TraverseModelObjects());
      };
      panelExtension.AddToolButton(action);
      ui.AddExtensionToPrimaryPanel(panelExtension);
      return true;
    }

    public void Stop()
    {
      m_events.Dispose();
    }

    private string TraverseModelObjects()
    {
      var modelCollection = m_app.Project.Model.GetObjects();

      var result = new StringBuilder("Renga levels, walls and columns:\n\n");

      var modelObjectCollection = m_app.Project.Model.GetObjects();

      int objCount = modelObjectCollection.Count;
      for (int i = 0; i < objCount; ++i)
      {
        var modelObject = modelObjectCollection.GetByIndex(i);
        var objectType = modelObject.ObjectType;

        if (objectType == Renga.ObjectTypes.Level)
        {
          var level = (Renga.ILevel)modelObject;

          result.AppendLine("Object type: Level");
          result.AppendLine("Level generated name: " + modelObject.Name);
          result.AppendLine("Level user defined name: " + level.LevelName);
          result.AppendLine($"Level elevation: {level.Elevation} mm.");
        }
        else if (objectType == Renga.ObjectTypes.Column)
        {
          var levelObject = (Renga.ILevelObject)modelObject;
          var objectWithMark = (Renga.IObjectWithMark)modelObject;
          var objectWithMaterial = (Renga.IObjectWithMaterial)modelObject;

          result.AppendLine("Object type: Column");
          result.AppendLine("Column name: " + modelObject.Name);
          result.AppendLine("Column parent level id: " + levelObject.LevelId);
          result.AppendLine("Column material id: " + objectWithMaterial.MaterialId);
          result.AppendLine("Column mark: " + objectWithMark.Mark);
          result.AppendLine($"Column offset: {levelObject.ElevationAboveLevel} mm.");
        }
        else if (objectType == Renga.ObjectTypes.Wall)
        {
          var levelObject = (Renga.ILevelObject)modelObject;
          var objectWithMark = (Renga.IObjectWithMark)modelObject;
          var objectWithLayeredMaterial = (Renga.IObjectWithLayeredMaterial)modelObject;

          result.AppendLine("Object type: Wall");
          result.AppendLine("Wall name: " + modelObject.Name);
          result.AppendLine("Wall parent level id: " + levelObject.LevelId);
          result.AppendLine("Wall material id: " + objectWithLayeredMaterial.LayeredMaterialId);
          result.AppendLine("Wall mark: " + objectWithMark.Mark);
          result.AppendLine($"Wall offset: {levelObject.ElevationAboveLevel} mm.");
        }
        else continue;

        result.AppendLine();
      }

      return result.ToString();
    }
  }
}
