/*  Sample: Model Object Quantities
 *
 *  This sample illustrates how to work with model object views, set visual style of the model view,
 *  hide model objects of specific type
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System.Text;
using System.Collections.Generic;

namespace VisualStyleAndVisibility
{
  public class VisualStyleAndVisibilityPlugin : Renga.IPlugin
  {
    private Renga.Application m_app;
    private Renga.ActionEventSource m_setNewVisualStyleToViewActionEventSource;
    private Renga.ActionEventSource m_hideAllWallsActionEventSource;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();

      {
        var action = ui.CreateAction();
        action.ToolTip = "Set new visual style to the view";
        m_setNewVisualStyleToViewActionEventSource = new Renga.ActionEventSource(action);
        m_setNewVisualStyleToViewActionEventSource.Triggered += (s, e) => SetNewVisualStyleToView();
        panelExtension.AddToolButton(action);
      }

      {
        var action = ui.CreateAction();
        action.ToolTip = "Hide all walls";
        m_hideAllWallsActionEventSource = new Renga.ActionEventSource(action);
        m_hideAllWallsActionEventSource.Triggered += (s, e) => HideAllWalls();
        panelExtension.AddToolButton(action);
      }

      ui.AddExtensionToPrimaryPanel(panelExtension);
      return true;
    }

    public void Stop()
    {
      m_setNewVisualStyleToViewActionEventSource.Dispose();
      m_hideAllWallsActionEventSource.Dispose();
    }

    private void SetNewVisualStyleToView()
    {
      var view = m_app.ActiveView;

      var modelView = view as Renga.IModelView;
      if (modelView == null)
        return;

      var visualStyle = modelView.VisualStyle;
      if (visualStyle != Renga.VisualStyle.VisualStyle_Color)
        visualStyle = Renga.VisualStyle.VisualStyle_Color;
      else
        visualStyle = Renga.VisualStyle.VisualStyle_Monochrome;

      modelView.VisualStyle = visualStyle;
    }
    private void HideAllWalls()
    {
      var view = m_app.ActiveView;

      var modelView = view as Renga.IModelView;
      if (modelView == null)
        return;

      var project = m_app.Project;
      var model = project.Model;
      var modelObjectCollection = model.GetObjects();


      var wallObjectIds = new List<int>();
      for (int i = 0; i < modelObjectCollection.Count; ++i)
      {
        var modelObject = modelObjectCollection.GetByIndex(i);
        if (modelObject.ObjectType == Renga.ObjectTypes.Wall)
          wallObjectIds.Add(modelObject.Id);
      }

      modelView.SetObjectsVisibility(wallObjectIds.ToArray(), false);
    }
  }
}
