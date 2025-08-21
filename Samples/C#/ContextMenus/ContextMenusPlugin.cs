using System;
using System.Collections.Generic;

namespace ContextMenus
{
  public class ContextMenusPlugin : Renga.IPlugin
  {
    private Renga.IContextMenu m_contextMenu;
    private List<Renga.ActionEventSource> m_actionEventsCollection = new List<Renga.ActionEventSource>();
    private Guid m_menuGuid = Guid.NewGuid();
    private Renga.IUI m_ui;

    public bool Initialize(string pluginFolder)
    {
      var app = new Renga.Application();
      m_ui = app.UI;

      m_contextMenu = m_ui.CreateContextMenu();

      // Add a submenu:
      m_contextMenu.AddSeparator();
      m_contextMenu.AddSeparator();

      var pNodeItem = m_contextMenu.AddNodeItem();
      var pAction20 = m_ui.CreateAction();
      pAction20.DisplayName = "Node 1";

      var pAction21 = m_ui.CreateAction();
      pAction21.DisplayName = "Node 2";

      pNodeItem.DisplayName = "Subtree";
      pNodeItem.AddActionItem(pAction20);
      pNodeItem.AddActionItem(pAction21);

      // Add some actions:
      m_contextMenu.AddSeparator();

      AddMenuItem("Message from Sample", (s, e) =>
      {
        m_ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info, "Message from Sample", "Context menu clicked");
      });

      AddMenuItem("Add item", (s, e) =>
      {
        var nodeItem = m_contextMenu.AddNodeItem();
        nodeItem.DisplayName = "New node";
        UpdateContextMenu();
      });

      UpdateContextMenu();
      
      return true;
    }

    public void Stop()
    {
      foreach (var eventSource in m_actionEventsCollection)
        eventSource.Dispose();

      m_actionEventsCollection.Clear();
    }


    private void AddMenuItem(string displayName, EventHandler handler)
    {
      var action = m_ui.CreateAction();
      action.DisplayName = displayName;

      var showMessageActionHandler = new Renga.ActionEventSource(action);
      showMessageActionHandler.Triggered += handler;
      m_actionEventsCollection.Add(showMessageActionHandler);

      m_contextMenu.AddActionItem(action);
    }

    private void UpdateContextMenu()
    {
      m_ui.AddContextMenu(m_menuGuid, m_contextMenu, Renga.ViewType.ViewType_View3D, Renga.ContextMenuShowCase.ContextMenuShowCase_Scene);
    }
  }
}
