/*  Sample: Object Creation
 *
 *  This sample illustrates how to create objects via the Renga API.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */

using Renga;
using System.Reflection;

namespace ObjectCreationPlugin
{
  public class ObjectCreation : Renga.IPlugin
  {
    private Renga.IApplication m_app;
    private readonly List<Renga.ActionEventSource> m_eventSources = new List<Renga.ActionEventSource>();

    private static readonly List<Guid> ObjectsNeedHost = [ObjectTypes.Door, ObjectTypes.Window, ObjectTypes.Opening, ObjectTypes.Hole];

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();

      var dropDownButton = ui.CreateDropDownButton();
      dropDownButton.ToolTip = "Create Object";

      // Create action for every member of ObjectTypes via System.Reflection
      foreach (var field in typeof(Renga.ObjectTypes).GetRuntimeFields())
      {
        dropDownButton.AddAction(CreateObjectCreationAction(ui, field));
      }
      panelExtension.AddDropDownButton(dropDownButton);

      ui.AddExtensionToPrimaryPanel(panelExtension);

      return true;
    }

    public void Stop()
    {
      foreach (var eventSource in m_eventSources)
        eventSource.Dispose();

      m_eventSources.Clear();
    }
    private Renga.IAction CreateObjectCreationAction(Renga.IUI ui, FieldInfo field)
    {
      var action = ui.CreateAction();

      var startIdx = field.Name.IndexOf('<');
      var endIdx = field.Name.IndexOf('>');
      var objectTypeName = field.Name.Substring(startIdx + 1, endIdx - 1);
      action.DisplayName = objectTypeName;

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        // Getting the current model
        IModel model = null;
        if (m_app.ActiveView.Type == ViewType.ViewType_View3D || m_app.ActiveView.Type == ViewType.ViewType_Level)
          model = m_app.Project.Model;
        else if (m_app.ActiveView.Type == ViewType.ViewType_Assembly)
        {
          var representedId = (m_app.ActiveView as IModelView).RepresentedEntityId;
          model = m_app.Project.Assemblies.GetById(representedId) as IModel;
        }
        else return;

        var args = model.CreateNewEntityArgs();
        var typeId = (Guid)field.GetValue(null)!;
        args.TypeId = typeId;

        // Set HostObjectId to dependent objects
        if (ObjectsNeedHost.Contains(typeId))
        {
          int[] selectedObjectsIds = (int[])m_app.Selection.GetSelectedObjects();
          if (selectedObjectsIds.Length != 1)
          {
            ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Warning, "ObjectCreation plugin message", $"You must select one host object to create {objectTypeName} object");
            return;
          }

          int hostObjectId = selectedObjectsIds.First();
          args.HostObjectId = hostObjectId;
        }

        var op = m_app.Project.CreateOperationWithUndo(model.Id);
        op.Start();
        var obj = model.CreateObject(args);
        op.Apply();

        if (obj == null)
          ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Warning, "ObjectCreation plugin message", m_app.LastError);
      };

      m_eventSources.Add(events);

      return action;
    }
  }
}