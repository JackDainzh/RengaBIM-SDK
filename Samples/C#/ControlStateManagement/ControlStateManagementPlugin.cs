using System;
using System.Collections.Generic;

namespace ControlStateManagement
{
  class SimpleButton
  {
    private Renga.IUI m_ui;
    private Renga.ActionEventSource m_actionEvents;

    internal Renga.IAction Action { get; }

    public SimpleButton(Renga.IUI ui, string tooltip)
    {
      m_ui = ui;
      Action = ui.CreateAction();
      Action.ToolTip = tooltip;
      m_actionEvents = new Renga.ActionEventSource(Action);
      m_actionEvents.Triggered += onActionEventsTriggered;
    }

    protected virtual void onActionEventsTriggered(object sender, EventArgs e)
    {
      m_ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info,
                          "Control State Management Plugin",
                          "Simple button pressed");
    }

    public void AddToPanel(Renga.IUIPanelExtension panel)
    {
      panel.AddToolButton(Action);
    }
  }

  class MasterButton : SimpleButton
  {
    private List<Renga.IAction> m_linkedActions = new List<Renga.IAction>();

    public MasterButton(Renga.IUI ui, string tooltip)
      : base(ui, tooltip)
    {
    }

    public void AddLinkedButton(SimpleButton button)
    {
      m_linkedActions.Add(button.Action);
    }

    protected override void onActionEventsTriggered(object sender, EventArgs e)
    {
      foreach (var action in m_linkedActions)
        action.Enabled = !action.Enabled;
    }
  }

  public class ControlStateManagementPlugin : Renga.IPlugin
  {
    List<SimpleButton> m_buttons = new List<SimpleButton>();

    public bool Initialize(string pluginFolder)
    {
      var app = new Renga.Application();
      var ui = app.UI;

      var btn1 = new SimpleButton(ui, "Jenson Button");
      var btn2 = new MasterButton(ui, "Master Button");
      btn2.AddLinkedButton(btn1);

      m_buttons.Add(btn1);
      m_buttons.Add(btn2);

      var panelExtension = ui.CreateUIPanelExtension();

      foreach (var button in m_buttons)
        button.AddToPanel(panelExtension);

      ui.AddExtensionToPrimaryPanel(panelExtension);

      return true;
    }

    public void Stop()
    {
    }
  }
}
