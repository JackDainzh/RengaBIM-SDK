/*  Sample: Placement Editing
 *
 *  This sample illustrates how to edit placement of level objects from Renga API.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

using Renga;
using System;
using static System.Net.Mime.MediaTypeNames;

namespace PlacementEditing
{
    public class PlacementEditingPlugin : Renga.IPlugin
    {
        private Renga.IApplication m_app;
        private readonly List<Renga.ActionEventSource> m_eventSources = new List<Renga.ActionEventSource>();

        public bool Initialize(string pluginFolder)
        {
            m_app = new Renga.Application();
            var ui = m_app.UI;
            var panelExtension = ui.CreateUIPanelExtension();

            int moveFactor = 1000;
            double angle = 30.0;

            panelExtension.AddToolButton(
              CreateAction(ui, "X+" + moveFactor, (IPlacement3D placement) =>
              {
                  placement.Move(createVector(moveFactor, 0, 0));
                  return placement;
              }));

            panelExtension.AddToolButton(
              CreateAction(ui, "X-" + moveFactor, (IPlacement3D placement) =>
              {
                  placement.Move(createVector(-moveFactor, 0, 0));
                  return placement;
              }));

            panelExtension.AddToolButton(
              CreateAction(ui, "Y+" + moveFactor, (IPlacement3D placement) =>
              {
                  placement.Move(createVector(0, moveFactor, 0));
                  return placement;
              }));

            panelExtension.AddToolButton(
              CreateAction(ui, "Y-" + moveFactor, (IPlacement3D placement) =>
              {
                  placement.Move(createVector(0, -moveFactor, 0));
                  return placement;
              }));

            panelExtension.AddToolButton(
              CreateAction(ui, "Z+" + moveFactor, (IPlacement3D placement) =>
              {
                  placement.Move(createVector(0, 0, moveFactor));
                  return placement;
              }));

            panelExtension.AddToolButton(
              CreateAction(ui, "Z-" + moveFactor, (IPlacement3D placement) =>
              {
                  placement.Move(createVector(0, 0, -moveFactor));
                  return placement;
              }));

            panelExtension.AddToolButton(
              CreateAction(ui, "Rotate around X by " + angle, (IPlacement3D placement) =>
              {
                  placement.Rotate2(placement.Origin, placement.AxisX, (Math.PI / 180) * angle);
                  return placement;
              }));

            panelExtension.AddToolButton(
             CreateAction(ui, "Rotate around Y by " + angle, (IPlacement3D placement) =>
             {
                 placement.Rotate2(placement.Origin, placement.AxisY, (Math.PI / 180) * angle);
                 return placement;
             }));

            panelExtension.AddToolButton(
             CreateAction(ui, "Rotate around Z by " + angle, (IPlacement3D placement) =>
             {
                 placement.Rotate2(placement.Origin, placement.AxisZ, (Math.PI / 180) * angle);
                 return placement;
             }));

            ui.AddExtensionToActionsPanel(panelExtension, Renga.ViewType.ViewType_View3D);
            return true;
        }

        public void Stop()
        {
        }

        private Renga.IAction CreateAction(Renga.IUI ui, string displayName, Func<IPlacement3D, IPlacement3D> func)
        {
            var action = ui.CreateAction();
            action.DisplayName = displayName;

            var events = new Renga.ActionEventSource(action);
            events.Triggered += (s, e) =>
            {
                int[] selectedObjectsIds = (int[])m_app.Selection.GetSelectedObjects();
                if (selectedObjectsIds.Length > 1)
                    return;

                int id = selectedObjectsIds.ElementAt(0);
                var levelObject = m_app.Project.Model.GetObjects().GetById(id) as Renga.ILevelObject;
                if (levelObject == null)
                    return;

                var op = m_app.Project.CreateOperation();
                op.Start();
                var placement = levelObject.GetPlacement();
                placement = func(placement);
                levelObject.SetPlacement(placement);
                op.Apply();
            };

            m_eventSources.Add(events);

            return action;
        }

        static Vector3D createVector(double x, double y, double z)
        {
            var vector = new Renga.Vector3D();

            vector.X = x;
            vector.Y = y;
            vector.Z = z;

            return vector;
        }
    }
}