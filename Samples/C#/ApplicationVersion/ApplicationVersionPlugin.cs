using System.Collections.Generic;

namespace ApplicationVersion
{
  public class ApplicationVersionPlugin : Renga.IPlugin
  {
    private List<Renga.ActionEventSource> m_eventSources = new List<Renga.ActionEventSource>();

    public bool Initialize(string pluginFolder)
    {
      var app = new Renga.Application();
      var ui = app.UI;
      var panelExtension = ui.CreateUIPanelExtension();

      // Tool button:
      panelExtension.AddToolButton(
        CreateAction(ui, app.Version));

      ui.AddExtensionToPrimaryPanel(panelExtension);
      ui.AddExtensionToActionsPanel(panelExtension, Renga.ViewType.ViewType_View3D);

      return true;
    }

    public void Stop()
    {
      foreach (var eventSource in m_eventSources)
        eventSource.Dispose();

      m_eventSources.Clear();
    }

    private Renga.IAction CreateAction(Renga.IUI ui, Renga.ProductVersion productVersion)
    {
      var action = ui.CreateAction();

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
          var displayName = System.String.Format("Application version: {0}.{1}.{2}.{3}",
              productVersion.Major,
              productVersion.Minor,
              productVersion.Patch,
              productVersion.BuildNumber);
          ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info, "Plugin message", displayName);
      };

      m_eventSources.Add(events);

      return action;
    }
  }
}
