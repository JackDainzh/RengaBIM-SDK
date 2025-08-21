/*  Sample: System Events
 *
 *  This sample illustrates how to react on system events,
 *  particularly how to handle the project closure notification.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

using System;
using System.Runtime.InteropServices;

namespace SystemEvents
{
  public class SystemEventsPlugin : Renga.IPlugin
  {
    Renga.ApplicationEventSource m_appEvents;

    public bool Initialize(string pluginFolder)
    {
      var app = new Renga.Application();
      m_appEvents = new Renga.ApplicationEventSource(app);

      m_appEvents.BeforeApplicationClose += OnBeforeApplicationClose;
      m_appEvents.BeforeProjectClose += OnBeforeProjectClose;
      m_appEvents.ProjectClosed += OnProjectClosed;
      m_appEvents.ProjectCreated += OnProjectCreated;
      m_appEvents.ProjectOpened += OnProjectOpened;
      m_appEvents.ProjectSaved += OnProjectSaved;

      return true;
    }

    public void Stop()
    {
      m_appEvents.Dispose();
    }


    // Using the Win32 MessageBox function here to avoid
    // the otherwise unnecessary dependency on WinForms or WPF:

    [DllImport("user32.dll", CharSet = CharSet.Unicode)]
    static extern int MessageBox(IntPtr hWnd, string text, string caption, uint type);

    // A few MessageBox-related constants:

    const uint MB_ICONQUESTION = 0x00000020;
    const uint MB_ICONINFORMATION = 0x00000040;
    const uint MB_OK = 0x00000000;
    const uint MB_YESNO = 0x00000004;
    const uint MB_DEFBUTTON1 = 0x00000000;
    const int IDNO = 7;
    const string MessageHeader = "Example of Renga event processing";

    private void OnBeforeApplicationClose(Renga.ApplicationCloseEventArgs args)
    {
      int msgboxID = MessageBox(IntPtr.Zero,
        "Does your plug-in allow the application to be closed?",
        MessageHeader,
        MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);

      if (msgboxID == IDNO)
        args.Prevent();
    }

    private void OnBeforeProjectClose(Renga.ProjectCloseEventArgs args)
    {
      int msgboxID = MessageBox(
          IntPtr.Zero,
          "Does your plug-in allow the project to be closed?",
          MessageHeader,
          MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);

      if (msgboxID == IDNO)
        args.Prevent();
    }

    private void OnProjectClosed()
    {
      int msgboxID = MessageBox(
          IntPtr.Zero,
          "Project closed!",
          MessageHeader,
          MB_ICONINFORMATION | MB_OK);
    }

    private void OnProjectCreated()
    {
      MessageBox(IntPtr.Zero, "Project created", MessageHeader, MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);
    }

    private void OnProjectOpened(string filePath)
    {
      MessageBox(IntPtr.Zero, "Project opened from: " + filePath, MessageHeader,
        MB_ICONQUESTION | MB_OK | MB_DEFBUTTON1);
    }

    private void OnProjectSaved(string filePath)
    {
      MessageBox(IntPtr.Zero, "Project saved to: " + filePath, MessageHeader, MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);
    }
  }
}
