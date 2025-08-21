using System;

namespace OpenSaveCloseProject
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

      // Saving project
      renga.Project.Save();

      string saveDir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + "/";
      var project = renga.Project;

      // Saving project as
      project.SaveAs(saveDir + "project.rnp", Renga.ProjectType.ProjectType_Project, false);

      // Closing project
      renga.CloseProject(true);

      // Closing Renga
      renga.Quit();
    }
  }
}
