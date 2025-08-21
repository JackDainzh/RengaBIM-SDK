/*  Sample: Object reinforcement sample
 *
 *  This sample illustrates how to obtain reinforcement data from model objects.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System.Text;


namespace ObjectReinforcement
{
  public static class RengaExtensions
  {
    public static string ToStringEx(this Renga.Point3D p)
    {
      var stringBuilder = new StringBuilder();
      stringBuilder.AppendFormat("({0}, {1} {2})",
        p.X.ToString("F2"), p.Y.ToString("F2"), p.Z.ToString("F2"));
      return stringBuilder.ToString();
    }

    public static string ToStringEx(this Renga.Vector3D vec)
    {
      var stringBuilder = new StringBuilder();
      stringBuilder.AppendFormat("({0}, {1} {2})",
        vec.X.ToString("F2"), vec.Y.ToString("F2"), vec.Z.ToString("F2"));
      return stringBuilder.ToString();
    }

    private static string Curve3DToString(Renga.ICurve3D curve)
    {
      var stringBuilder = new StringBuilder();
      stringBuilder.AppendFormat("type = {0}, p1 = {1}, p2 = {2}",
        curve.Curve3DType.ToString(),
        curve.GetBeginPoint().ToStringEx(),
        curve.GetEndPoint().ToStringEx());
      return stringBuilder.ToString();
    }

    public static string ToStringEx(this Renga.ICurve3D curve, int indent = 0)
    {
      var result = new StringBuilder();
     
      if (curve.Curve3DType == Renga.Curve3DType.Curve3DType_PolyCurve)
      {
        var polyCurve = curve as Renga.IPolyCurve3D;
        
        result.Append(' ', indent);
        result.AppendFormat("Geometry: type = {0}\n", curve.Curve3DType.ToString());

        for (int i = 0; i < polyCurve.GetSegmentCount(); ++i)
        {
          result.Append(' ', indent + 2);
          result.AppendFormat("segment: {0}\n", Curve3DToString(polyCurve.GetSegment(i)));
        }
      }
      else
      {
        result.Append(' ', indent);
        result.Append("Geometry: " + Curve3DToString(curve));
      }

      return result.ToString();
    }

    public static string ToStringEx(this Renga.IPlacement3DCollection placements, int indent = 0)
    {
      var result = new StringBuilder();

      result.Append(' ', indent);
      result.Append("Placements:\n");

      for (int i = 0; i < placements.Count; ++i)
      {
        result.Append(' ', indent + 2);
        result.AppendFormat("placemnet: {0}\n", placements.Get(i).ToStringEx());
      }

      return result.ToString();
    }


    public static string ToStringEx(this Renga.IPlacement3D placement)
    {
      var stringBuilder = new StringBuilder();
      stringBuilder.AppendFormat("origin = {0}, axisX = {1}, axisY = {2}, axisZ = {3}",
        placement.Origin.ToStringEx(),
        placement.AxisX.ToStringEx(),
        placement.AxisY.ToStringEx(),
        placement.AxisZ.ToStringEx());
      return stringBuilder.ToString();
    }
  }

  public class ObjectReinforcementPlugin : Renga.IPlugin
  {
    private Renga.Application m_app;
    private Renga.ActionEventSource m_events;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();
      var action = ui.CreateAction();

      action.ToolTip = "Obtain selected object reinforcement";
      m_events = new Renga.ActionEventSource(action);
      m_events.Triggered += (s, e) =>
      {
        string info = ObtainSelectedObjectReinforcement();
        info = info.Replace("\n", "\r\n");

        string textFileName = "reinforcement_info.txt";
        System.IO.File.WriteAllText(textFileName, info, System.Text.Encoding.UTF8);
        System.Diagnostics.Process.Start(textFileName);
      };
      panelExtension.AddToolButton(action);
      ui.AddExtensionToPrimaryPanel(panelExtension);
      return true;
    }

    public void Stop()
    {
      m_events.Dispose();
    }

    private string ObtainObjectRebarUsages(Renga.IRebarUsageCollection rebarUsages, Renga.IReinforcementUnitStyleManager reinforcementManager, int indent = 0)
    {
      var result = new StringBuilder();

      int rebarUsageCount = rebarUsages.Count;
      for (int i = 0; i < rebarUsageCount; ++i)
      {
        var rebarUsage = rebarUsages.Get(i);
        var rebarStyle = reinforcementManager.GetRebarStyle(rebarUsage.StyleId);
        var totalLengthQuantity = rebarUsage.GetQuantities().Get(Renga.QuantityIds.TotalRebarLength);
        var totalMassQuantity = rebarUsage.GetQuantities().Get(Renga.QuantityIds.TotalRebarMass);

        result.Append(new string(' ', indent));
        result.AppendFormat("Style name: {0}, D: {1} mm, Total length: {2} m, Total mass: {3} kg\n",
          rebarStyle.Name,
          rebarStyle.Diameter.ToString("F2"),
          totalLengthQuantity.AsLength(Renga.LengthUnit.LengthUnit_Meters).ToString("F2"),
          totalMassQuantity.AsMass(Renga.MassUnit.MassUnit_Kilograms).ToString("F2"));

        result.Append(rebarUsage.GetRebarGeometry().ToStringEx(indent + 2));
        result.Append(rebarUsage.GetPlacements().ToStringEx(indent + 2));
      }
      return result.ToString();
    }

    private string ObtainObjectRebarUsages(Renga.IObjectReinforcementModel reinforcementModel, Renga.IReinforcementUnitStyleManager reinforcementManager)
    {
      var result = new StringBuilder("\nParametric reinforcement rebar usages:\n");
      result.Append(ObtainObjectRebarUsages(reinforcementModel.GetRebarUsages(), reinforcementManager));
      return result.ToString();
    }

    private string ObtainObjectReinforcementUnitUsages(Renga.IObjectReinforcementModel reinforcementModel, Renga.IReinforcementUnitStyleManager reinforcementManager)
    {
      var result = new StringBuilder("\nReinforcement unit usages:\n");

      var unitUsages = reinforcementModel.GetReinforcementUnitUsages();
      int unitUsageCount = unitUsages.Count;
      for (int i = 0; i < unitUsageCount; ++i)
      {
        var unitUsage = unitUsages.Get(i);
        var reinforcementUnitStyle = reinforcementManager.GetUnitStyle(unitUsage.StyleId);
        var unitsCountQuantity = unitUsage.GetQuantities().Get(Renga.QuantityIds.ReinforcementUnitCount);
        // here you can get rebar usages of the reinforcement unit 
        //var unitRebarUsages = reinforcementUnitStyle.GetRebarUsages();

        result.AppendFormat("Style name: {0}, Type: {1}, Total unit count: {2}\n",
          reinforcementUnitStyle.Name,
          reinforcementUnitStyle.UnitType.ToString(),
          unitsCountQuantity.AsCount());

        result.Append(unitUsage.GetPlacements().ToStringEx(0));
        result.Append(ObtainObjectRebarUsages(reinforcementUnitStyle.GetRebarUsages(), reinforcementManager, 2));
      }
      return result.ToString();
    }

    private string ObtainSelectedObjectReinforcement()
    {
      var currentProject = m_app.Project;
      int[] selectedObjectIds = (int[])m_app.Selection.GetSelectedObjects();

      if (selectedObjectIds.Length == 0)
        return "Select at least one object";

      var objectsCollection = currentProject.Model.GetObjects();
      var selectedModelObject = objectsCollection.GetById(selectedObjectIds[0]);
      var reinforcementModel = selectedModelObject as Renga.IObjectReinforcementModel;

      if (reinforcementModel == null)
        return "Select object with reinforcement";

      var result = new StringBuilder("Object reinforcement:\n\n");

      result.AppendLine(ObtainObjectRebarUsages(reinforcementModel, currentProject.ReinforcementUnitStyleManager));
      result.AppendLine(ObtainObjectReinforcementUnitUsages(reinforcementModel, currentProject.ReinforcementUnitStyleManager));
      result.AppendLine();

      return result.ToString();
    }
  }
}
