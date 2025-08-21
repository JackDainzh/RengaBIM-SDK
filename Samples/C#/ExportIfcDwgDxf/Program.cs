using System;

namespace ExportIfcDwgDxf
{
  class Program
  {
    static void Main(string[] args)
    {
      if (args.Length != 1)
      {
        var executable = System.IO.Path.GetFileName(System.Reflection.Assembly.GetExecutingAssembly().Location);
        Console.WriteLine($"usage: {executable} sourceProjectFileName");
        return;
      }

      // Opening Renga
      var renga = new Renga.Application();
      renga.Visible = true;
      int result = renga.OpenProject(args[0]);
      if (result != 0)
      {
        Console.WriteLine("open project '" + args[0] + "' error");
        renga.Quit();
        return;
      }

      string exportDir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + "/";
      var project = renga.Project;

      // Exporting project to IFC
      var ifcExportSettings = renga.CreateIfcExportSettings();
      ifcExportSettings.Version = Renga.IfcVersion.IfcVersion_4;

      project.ExportToIfc2(exportDir + "project.ifc", true, ifcExportSettings);

      // Exporting drawings
      var drawingCollection = project.Drawings;
      var size = drawingCollection.Count;
      for (int i = 0; i < size; ++i)
      {
        var drawing = drawingCollection.Get(i);
        drawing.ExportToDxf(exportDir + "drawing_" + i.ToString() + ".dxf", Renga.AutocadVersion.AutocadVersion_v2000, true);
        drawing.ExportToDwg(exportDir + "drawing_" + i.ToString() + ".dwg", Renga.AutocadVersion.AutocadVersion_v2000, true);
      }
      
      // Closing project
      renga.CloseProject(true);

      // Closing Renga
      renga.Quit();
    }
  }
}
