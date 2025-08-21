using System;
using System.Text;

namespace BeamParams
{
  public class BeamParamsPlugin : Renga.IPlugin
  {
    private Renga.Application m_app;
    private Renga.ActionEventSource m_events;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();
      var action = ui.CreateAction();
      action.ToolTip = "Beam Params";
      m_events = new Renga.ActionEventSource(action);
      m_events.Triggered += (s, e) =>
      {
        ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info,
                          "Beam Params",
                          buildBeamParamsListDescription());
      };
      panelExtension.AddToolButton(action);
      ui.AddExtensionToPrimaryPanel(panelExtension);
      return true;
    }

    public void Stop()
    {
      m_events.Dispose();
    }

    private Renga.Vector3D GetBaselineOffset(Renga.IBeamParams beamParams, Renga.Point2D offset_)
    {
      var offset = beamParams.GetProfilePlacement().GetTransformFrom().TransformPoint(offset_);

      var baseLine = beamParams.GetBaseline();

      var sectionPlacement = beamParams.GetProfilePlacementOnBaseline(baseLine.MinParameter);

      var sectionPlacementTransform = sectionPlacement.GetTransformFrom();

      var offsetPoint = sectionPlacementTransform.TransformPoint(new Renga.Point3D { X = offset.X, Y = offset.Y, Z = 0 });
      var baselineStart = baseLine.GetBeginPoint();

      return new Renga.Vector3D
      {
        X = offsetPoint.X - baselineStart.X,
        Y = offsetPoint.Y - baselineStart.Y,
        Z = offsetPoint.Z - baselineStart.Z,
      };
    }

    private string buildBeamParamsDescription(Renga.IBeamParams beamParams, Renga.IBeamStyleManager beamStyleManager, Renga.IPlacement3D levelPlacement)
    {
      var result_output = new StringBuilder("  Beam Param:\n");

      var transformIntoGlobal = levelPlacement.GetTransformFrom();
      var transformIntoGlobalWithVerticalOffset = transformIntoGlobal.GetCopy();
      transformIntoGlobalWithVerticalOffset.AddTranslation(new Renga.Vector3D { X = 0, Y = 0, Z = beamParams.VerticalOffset });

      // getting the baseline of the beam
      var baseLineInLevelSpace = beamParams.GetBaseline();

      // getting beam profile
      var profile = beamStyleManager.GetBeamStyle(beamParams.StyleId).Profile;
      var profilePlacement = beamParams.GetProfilePlacement();

      // curves in the global coordinate system

      // getting the baseline of the beam in the global coordinate system
      var baseLine = baseLineInLevelSpace.GetTransformed(transformIntoGlobal);

      // getting the center line of the beam in the global coordinate system
      var centerLine = baseLineInLevelSpace.GetOffseted(GetBaselineOffset(beamParams, new Renga.Point2D { X = 0, Y = 0 })).GetTransformed(transformIntoGlobalWithVerticalOffset);

      // getting the center line of mass of the beam in the global coordinate system
      var centerOfMassPath = baseLineInLevelSpace.GetOffseted(GetBaselineOffset(beamParams, profile.GetCenterOfMass())).GetTransformed(transformIntoGlobalWithVerticalOffset);

      // getting the length of the beam
      result_output.AppendLine("  beam length = " + centerOfMassPath.GetLength());
      result_output.AppendLine("");

      // getting the parameter of the corresponding beam start
      double parameter = baseLineInLevelSpace.MinParameter;

      var profilePlacementOnBaseline = beamParams.GetProfilePlacementOnBaseline(parameter);
      for (int i = 0; i < profile.Regions.Count; ++i)
      {
        var currentRegion = profile.Regions.Get(i);
        for (int j = 0; j < currentRegion.GetContourCount(); ++j)
        {
          var currentContour = currentRegion.GetContour(j).GetTransformed(profilePlacement.GetTransformFrom());

          // obtaining the curve of one of the regions of the beam profile at the beginning of the beam
          var sectionCurve = currentContour.CreateCurve3D(profilePlacementOnBaseline).GetTransformed(transformIntoGlobalWithVerticalOffset);
        }
      }

      return result_output.ToString();
    }

    private Renga.IPlacement3D getLevelPlacement(Renga.IModelObjectCollection modelObjects, Renga.IModelObject modelObject)
    {
      var parentLevelId = (modelObject as Renga.ILevelObject).LevelId;
      var parentLevel = modelObjects.GetById(parentLevelId);
      return (parentLevel as Renga.ILevel).Placement;
    }

    private string buildBeamParamsListDescription()
    {
      var project = m_app.Project;

      if (project == null)
        return "The Renga project is null";

      var result_output = new StringBuilder("Beam Params:\n");

      var beamStyleManager = project.BeamStyleManager;

      var modelObjects = project.Model.GetObjects();
      for (int i = 0; i < modelObjects.Count; i++)
      {
        var modelObject = modelObjects.GetByIndex(i);

        var beamParams = modelObject as Renga.IBeamParams;
        if (beamParams != null)
        {
          var levelPlacement = getLevelPlacement(modelObjects, modelObject);
          result_output.AppendLine(buildBeamParamsDescription(beamParams, beamStyleManager, levelPlacement));
        }
      }

      return result_output.ToString();
    }
  }
}
