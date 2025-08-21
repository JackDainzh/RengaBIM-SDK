/*  Sample: 2D Baseline object Editing
 *
 *  This sample illustrates how edit baseline of 2D baseline objects via the Renga API.
 *
 *  Copyright Renga Software LLC, 2025. All rights reserved.
 */
using Renga;

namespace Baseline2DObjectEditing
{
  public class Baseline2DObjectEditingPlugin : Renga.IPlugin
  {
    private Renga.IApplication m_app;
    private readonly List<Renga.ActionEventSource> m_eventSources = new List<Renga.ActionEventSource>();

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();

      panelExtension.AddToolButton(GetBaselineAction(ui));

      var setBaselineDropDownButton = ui.CreateDropDownButton();
      setBaselineDropDownButton.ToolTip = "Set Baseline";
      setBaselineDropDownButton.AddAction(SetLineSegmentCurveAction(ui));
      setBaselineDropDownButton.AddAction(SetCircleCurveAction(ui));
      setBaselineDropDownButton.AddAction(SetClosedCompositeCurveAction(ui));
      setBaselineDropDownButton.AddAction(SetUnclosedCompositeCurveAction(ui));
      setBaselineDropDownButton.AddAction(SetArcCurveAction(ui));
      panelExtension.AddDropDownButton(setBaselineDropDownButton);

      ui.AddExtensionToPrimaryPanel(panelExtension);

      return true;
    }

    public void Stop()
    {
      foreach (var eventSource in m_eventSources)
        eventSource.Dispose();

      m_eventSources.Clear();
    }

    Placement2D toPlacement2D(Placement3D placement3D)
    {
      var origin = new Point2D { X = placement3D.Origin.X, Y = placement3D.Origin.Y };
      var xAxis = new Vector2D { X = placement3D.xAxis.X, Y = placement3D.xAxis.Y };
      return new Placement2D { Origin = origin, xAxis = xAxis };
    }

    private void checkLastError()
    {
      if (m_app.HasLastError)
      {
        m_app.UI.ShowMessageBox(MessageIcon.MessageIcon_Error, "Baseline2DObjectEditing plugin message", m_app.LastError);
        m_app.LastError = null;
      }
    }

    private IModel getCurrentModel()
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
      return model;
    }

    private IModelObject getSelectedObject()
    {
      int[] selectedObjectIds = (int[])m_app.Selection.GetSelectedObjects();
      if (selectedObjectIds.Length != 1)
      {
        m_app.UI.ShowMessageBox(Renga.MessageIcon.MessageIcon_Warning, "Baseline2DObjectEditing plugin message", "You must select only 1 object");
        return null;
      }

      var selectedObjectId = selectedObjectIds.First();
      var model = getCurrentModel();
      return model.GetObjects().GetById(selectedObjectId);
    }

    private IBaseline2DObject getBaseline2DObject()
    {
      var selectedObject = getSelectedObject();
      if (selectedObject == null)
        return null;

      if (selectedObject is not IBaseline2DObject)
      {
        m_app.UI.ShowMessageBox(Renga.MessageIcon.MessageIcon_Warning, "Baseline2DObjectEditing plugin message", "Selected object is not a baseline 2D based object");
        return null;
      }
      return selectedObject as IBaseline2DObject;
    }

    private Renga.IAction GetBaselineAction(Renga.IUI ui)
    {
      var action = ui.CreateAction();
      action.DisplayName = "Get Baseline";

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var baseline2DObject = getBaseline2DObject();
        if (baseline2DObject == null)
          return;

        ICurve2D baseline = baseline2DObject.GetBaseline();
        string text = $"startPoint : ({baseline.GetBeginPoint().X}, {baseline.GetBeginPoint().Y})\n" +
                      $"endPoint   : ({baseline.GetEndPoint().X}, {baseline.GetEndPoint().Y})\n";

        if (baseline is IArc2D arc)
        {
          text += $"center : ({arc.GetCenter().X}, {arc.GetCenter().Y})\n" +
                  $"radius : {arc.GetRadius()}\n" +
                  $"type : Arc2D";
        }
        else if (baseline is IPolyCurve2D polyCurve)
        {
          for (int i = 0; i < polyCurve.GetSegmentCount(); ++i)
          {
            var segment = polyCurve.GetSegment(i);
            text += $"segment {i}:\n";
            text += $"\tstartPoint : ({segment.GetBeginPoint().X}, {segment.GetBeginPoint().Y})\n" +
                    $"\tendPoint   : ({segment.GetEndPoint().X}, {segment.GetEndPoint().Y})\n";
          }
          text += $"type : Composite";
        }
        else
          text += $"type : LineSegment";

        ui.ShowMessageBox(MessageIcon.MessageIcon_Info, "Baseline2DObjectEditing plugin message", text);
      };

      m_eventSources.Add(events);
      return action;
    }

    private Renga.IAction SetLineSegmentCurveAction(Renga.IUI ui)
    {
      var action = ui.CreateAction();
      action.DisplayName = "Set line segment baseline";

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var baseline2DObject = getBaseline2DObject();
        if (baseline2DObject == null)
          return;

        var beginPoint = new Point2D { X = 1000.0, Y = 1000.0 };
        var endPoint = new Point2D { X = 2000.0, Y = 2000.0 };
        var lineSegmentCurve = m_app.Math.CreateLineSegment2D(beginPoint, endPoint);

        var currentModel = getCurrentModel();
        var op = m_app.Project.CreateOperationWithUndo(currentModel.Id);
        op.Start();
        baseline2DObject.SetBaseline(lineSegmentCurve);
        op.Apply();

        checkLastError();
      };

      m_eventSources.Add(events);
      return action;
    }

    private Renga.IAction SetCircleCurveAction(Renga.IUI ui)
    {
      var action = ui.CreateAction();
      action.DisplayName = "Set circle baseline";

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var baseline2DObject = getBaseline2DObject();
        if (baseline2DObject == null)
          return;

        var centerPoint = new Point2D { X = 1000.0, Y = 1000.0 };
        var radius = 500.0;
        var circleCurve = m_app.Math.CreateCircle2D(centerPoint, radius);

        var currentModel = getCurrentModel();
        var op = m_app.Project.CreateOperationWithUndo(currentModel.Id);
        op.Start();
        baseline2DObject.SetBaseline(circleCurve);
        op.Apply();

        checkLastError();
      };

      m_eventSources.Add(events);
      return action;
    }

    private Renga.IAction SetClosedCompositeCurveAction(Renga.IUI ui)
    {
      var action = ui.CreateAction();
      action.DisplayName = "Set closed composite baseline";

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var baseline2DObject = getBaseline2DObject();
        if (baseline2DObject == null)
          return;

        var p1 = new Point2D { X = 1000.0, Y = 1000.0 };
        var p2 = new Point2D { X = 2000.0, Y = 1000.0 };
        var p3 = new Point2D { X = 2000.0, Y = 2000.0 };
        var p4 = new Point2D { X = 1000.0, Y = 2000.0 };
        var curve1 = m_app.Math.CreateLineSegment2D(p1, p2);
        var curve2 = m_app.Math.CreateLineSegment2D(p2, p3);
        var curve3 = m_app.Math.CreateLineSegment2D(p3, p4);
        var curve4 = m_app.Math.CreateLineSegment2D(p4, p1);
        var curves = new[] { curve1, curve2, curve3, curve4 };
        var compositeCurve = m_app.Math.CreateCompositeCurve2D(curves);

        var currentModel = getCurrentModel();
        var op = m_app.Project.CreateOperationWithUndo(currentModel.Id);
        op.Start();
        baseline2DObject.SetBaseline(compositeCurve);
        op.Apply();

        checkLastError();
      };

      m_eventSources.Add(events);
      return action;
    }

    private Renga.IAction SetUnclosedCompositeCurveAction(Renga.IUI ui)
    {
      var action = ui.CreateAction();
      action.DisplayName = "Set unclosed composite baseline";

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var baseline2DObject = getBaseline2DObject();
        if (baseline2DObject == null)
          return;

        var p1 = new Point2D { X = 1000.0, Y = 1000.0 };
        var p2 = new Point2D { X = 2000.0, Y = 1000.0 };
        var p3 = new Point2D { X = 2000.0, Y = 2000.0 };
        var curve1 = m_app.Math.CreateLineSegment2D(p1, p2);
        var curve2 = m_app.Math.CreateLineSegment2D(p2, p3);
        var curves = new[] { curve1, curve2 };
        var compositeCurve = m_app.Math.CreateCompositeCurve2D(curves);

        var currentModel = getCurrentModel();
        var op = m_app.Project.CreateOperationWithUndo(currentModel.Id);
        op.Start();
        baseline2DObject.SetBaseline(compositeCurve);
        op.Apply();

        checkLastError();
      };

      m_eventSources.Add(events);
      return action;
    }

    private Renga.IAction SetArcCurveAction(Renga.IUI ui)
    {
      var action = ui.CreateAction();
      action.DisplayName = "Set arc baseline";

      var events = new Renga.ActionEventSource(action);
      events.Triggered += (s, e) =>
      {
        var baseline2DObject = getBaseline2DObject();
        if (baseline2DObject == null)
          return;

        var beginPoint = new Point2D { X = 1000.0, Y = 1000.0 };
        var intermediatePoint = new Point2D { X = 2000.0, Y = 2000.0 };
        var endPoint = new Point2D { X = 3000.0, Y = 1000.0 };
        var arcCurve = m_app.Math.CreateArc2DByThreePoints(beginPoint, intermediatePoint, endPoint);

        var currentModel = getCurrentModel();
        var op = m_app.Project.CreateOperationWithUndo(currentModel.Id);
        op.Start();
        baseline2DObject.SetBaseline(arcCurve);
        op.Apply();

        checkLastError();
      };

      m_eventSources.Add(events);
      return action;
    }
  }
}