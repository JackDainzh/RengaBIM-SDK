/*  Sample: View Switching
 *
 *  This sample illustrates how switch between views via the Renga API.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */
using Renga;

namespace ViewSwitching
{
  public class ViewSwitchingPlugin : Renga.IPlugin
  {
    private Renga.IApplication m_app;
    private readonly List<Renga.ActionEventSource> m_eventSources = new List<Renga.ActionEventSource>();

    private int m_assemblyViewsCount = 0;
    private int m_levelViewsCount = 0;
    private int m_assemblyViewIdx = 0;
    private int m_levelViewIdx = 0;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();

      panelExtension.AddToolButton(CreateMainViewAction(ui, "Switch to Main View"));
      panelExtension.AddToolButton(CreateAssemblyViewsAction(ui, "Switch between Assembly Views"));
      panelExtension.AddToolButton(CreateLevelViewsAction(ui, "Switch between Level Views"));

      ui.AddExtensionToPrimaryPanel(panelExtension);

      return true;
    }

    public void Stop()
    {
    }

    private Renga.IAction CreateMainViewAction(Renga.IUI ui, string name)
    {
      var action = ui.CreateAction();
      action.DisplayName = name;

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var buildingId = m_app.Project.BuildingInfo.Id;
        m_app.OpenViewByEntity(buildingId);
      };

      m_eventSources.Add(events);

      return action;
    }

    private Renga.IAction CreateAssemblyViewsAction(Renga.IUI ui, string name)
    {
      var action = ui.CreateAction();
      action.DisplayName = name;

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        if (m_assemblyViewsCount != m_app.Project.Assemblies.Count)
        {
          m_assemblyViewsCount = m_app.Project.Assemblies.Count;
          m_assemblyViewIdx = 0;
        }

        if (m_assemblyViewsCount == 0)
          return;

        var assemblyId = m_app.Project.Assemblies.GetByIndex(m_assemblyViewIdx % m_assemblyViewsCount).Id;
        m_app.OpenViewByEntity(assemblyId);
        ++m_assemblyViewIdx;
      };

      m_eventSources.Add(events);

      return action;
    }

    private Renga.IAction CreateLevelViewsAction(Renga.IUI ui, string name)
    {
      var action = ui.CreateAction();
      action.DisplayName = name;

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var levels = GetObjectsByType(m_app.Project.Model.GetObjects(), ObjectTypes.Level);

        if (m_levelViewsCount != levels.Count)
        {
          m_levelViewsCount = levels.Count;
          m_levelViewIdx = 0;
        }

        if (m_levelViewsCount == 0)
          return;

        var levelId = levels.ElementAt(m_levelViewIdx % m_levelViewsCount).Id;
        m_app.OpenViewByEntity(levelId);
        ++m_levelViewIdx;
      };

      m_eventSources.Add(events);

      return action;
    }

    public static List<Renga.IModelObject> GetObjectsByType(Renga.IModelObjectCollection objects, Guid type)
    {
      var result = new List<Renga.IModelObject>();
      for (int i = 0; i < objects.Count; ++i)
        if (objects.GetByIndex(i).ObjectType == type)
          result.Add(objects.GetByIndex(i));
      return result;
    }
  }
}