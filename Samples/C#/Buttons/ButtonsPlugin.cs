using System.Collections.Generic;

namespace Buttons
{
  public class ButtonsPlugin : Renga.IPlugin
  {
    private List<Renga.ActionEventSource> m_eventSources = new List<Renga.ActionEventSource>();

    public bool Initialize(string pluginFolder)
    {
      var app = new Renga.Application();
      var ui = app.UI;
      var panelExtension = ui.CreateUIPanelExtension();

      // Tool button:
      panelExtension.AddToolButton(
        CreateAction(ui, "SimpleButton"));

      // DropDownButton:
      var dropDownButton = ui.CreateDropDownButton();
      dropDownButton.ToolTip = "DropDownButton";

      dropDownButton.AddAction(
        CreateAction(ui, "DropDownButton Action1"));

      dropDownButton.AddAction(
        CreateAction(ui, "DropDownButton Action2"));

      dropDownButton.AddAction(
        CreateAction(ui, "DropDownButton Action3"));

      panelExtension.AddDropDownButton(dropDownButton);

      // SplitButton:
      var splitButton = ui.CreateSplitButton(
        CreateAction(ui, "SplitButton Default Action"));

      splitButton.AddAction(
        CreateAction(ui, "SplitButton Action2"));

      splitButton.AddAction(
        CreateAction(ui, "SplitButton Action 3"));

      panelExtension.AddSplitButton(splitButton);


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

    private Renga.IAction CreateAction(Renga.IUI ui, string displayName)
    {
      var action = ui.CreateAction();
      action.DisplayName = displayName;

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info, "Plugin message", displayName + " Handler");
      };

      m_eventSources.Add(events);

      return action;
    }
  }
}
