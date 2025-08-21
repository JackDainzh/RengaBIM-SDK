import sys
import os
import win32com.client


if __name__ == '__main__':

    if len (sys.argv) == 1:
        print("usage: {} sourceProjectFileName".format(os.path.split(__file__)[1]))
        sys.exit(1)

    app = win32com.client.Dispatch("Renga.Application.1")
    app.Visible = True

    projectFilePath = sys.argv[1]
    print("Opening project: " + projectFilePath)

    result = app.OpenProject(projectFilePath)
    if result != 0:
        print("Error opening project")

    print("Getting project")
    project = app.Project
    if project == None:
        print("Error: project equals None")

    if len (sys.argv) == 2:
        print("Saving project")
        result = project.Save();
        if result != 0:
            print("Error saving project")
    else:
        saveAsFilePath = sys.argv[2]
        print("Saving project as {}".format(saveAsFilePath))
        result = project.SaveAs(saveAsFilePath, 0, 1);
        if result != 0:
            print("Error saving project as")

    print("Closing project")
    result = app.CloseProject(1)
    if result != 0:
        print("Error closing project")

    print("Quiting application")
    app.Quit()
