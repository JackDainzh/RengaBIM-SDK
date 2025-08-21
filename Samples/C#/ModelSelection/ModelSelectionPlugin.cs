/* Sample: Model Selection
 * 
 * This sample illustrates how work with the selection mechanism of Renga API.
 * 
 * Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System;
using System.Collections.Generic;
using System.Text;

namespace ModelSelection
{
  public class ModelSelectionPlugin : Renga.IPlugin
  {
    private Renga.IApplication m_application;

    private Renga.IAction m_showSelectedObjectsAction;
    private Renga.ActionEventSource m_showSelectedObjectsActionEventSource;
    private Renga.ActionEventSource m_selectWallsActionEventSource;

    private Renga.SelectionEventSource m_selectionEventSource;

    public bool Initialize(string pluginFolder)
    {
      m_application = new Renga.Application();

      var selection = m_application.Selection;
      m_selectionEventSource = new Renga.SelectionEventSource(selection);
      m_selectionEventSource.ModelSelectionChanged += OnModelSelectionChanged;

      var ui = m_application.UI;

      var uiPanelExtension = ui.CreateUIPanelExtension();

      m_showSelectedObjectsAction = ui.CreateAction();
      m_showSelectedObjectsAction.ToolTip = "Show selected objects";
      m_showSelectedObjectsAction.Enabled = selection.GetSelectedObjects().Length > 0;
      m_showSelectedObjectsActionEventSource = new Renga.ActionEventSource(m_showSelectedObjectsAction);
      m_showSelectedObjectsActionEventSource.Triggered += OnShowSelectedObjectsButtonClicked;
      uiPanelExtension.AddToolButton(m_showSelectedObjectsAction);

      var selectWallsAction = ui.CreateAction();
      selectWallsAction.ToolTip = "Select walls";
      m_selectWallsActionEventSource = new Renga.ActionEventSource(selectWallsAction);
      m_selectWallsActionEventSource.Triggered += OnSelectWallsButtonClicked;
      uiPanelExtension.AddToolButton(selectWallsAction);

      ui.AddExtensionToPrimaryPanel(uiPanelExtension);

      return true;
    }

    public void Stop()
    {
      m_selectionEventSource.Dispose();
      m_showSelectedObjectsActionEventSource.Dispose();
      m_selectWallsActionEventSource.Dispose();
    }

    private void OnModelSelectionChanged(object sender, EventArgs args)
    {
      var selection = m_application.Selection;
      m_showSelectedObjectsAction.Enabled = selection.GetSelectedObjects().Length > 0;
    }

    private void OnShowSelectedObjectsButtonClicked(object sender, EventArgs args)
    {
      var project = m_application.Project;

      if (project == null)
        return;

      var model = project.Model;
      var modelObjectCollection = model.GetObjects();

      var selection = m_application.Selection;

      int[] selectedObjectIds = (int[])selection.GetSelectedObjects();

      var stringBuilder = new StringBuilder();
      stringBuilder.AppendLine($"{selectedObjectIds.Length} selected objects:");

      foreach (int objectId in selectedObjectIds)
      {
        stringBuilder.AppendLine($"Object id: {objectId}");
      }

      var ui = m_application.UI;
      ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info, "ModelSelection", stringBuilder.ToString());
    }

    private void OnSelectWallsButtonClicked(object sender, EventArgs args)
    {
      var project = m_application.Project;

      if (project == null)
        return;

      var model = project.Model;
      var modelObjects = model.GetObjects();

      int[] modelObjectIds = (int[])modelObjects.GetIds();

      var wallObjectIds = new List<int>();

      foreach (int objectId in modelObjectIds)
      {
        var modelObject = modelObjects.GetById(objectId);

        if (modelObject == null)
          continue;

        if (modelObject.ObjectType != Renga.ObjectTypes.Wall)
          continue;

        wallObjectIds.Add(objectId);
      }

      var selection = m_application.Selection;
      selection.SetSelectedObjects(wallObjectIds.ToArray());
    }
  }
}
