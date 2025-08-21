using System;
using System.Text;

namespace Properties
{
  public class Plugin : Renga.IPlugin
  {
    private Renga.Application m_app;

    public bool Initialize(string pluginFolder)
    {
      m_app = new Renga.Application();
      var ui = m_app.UI;
      var panelExtension = ui.CreateUIPanelExtension();

      Action<string, string, Func<string>> createMessageBoxAction = 
        (tooltip, messageTitle, messageCallback) => 
      {
        var action = ui.CreateAction();
        action.ToolTip = tooltip;
        panelExtension.AddToolButton(action);

        var events = new Renga.ActionEventSource(action);
        events.Triggered += (s, e) =>
        {
          ui.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info,
                            messageTitle,
                            messageCallback());
        };
      };

      // add property to levels action
      createMessageBoxAction(
          "Add property to all levels in the model",
          "Adding property to levels",
          AddPropertyToLevels);

      // show levels properties action
      createMessageBoxAction(
          "Show levels properties",
          "Levels properties",
          ShowLevelsProperties);

      // show levels parameters action
      createMessageBoxAction(
          "Show levels parameters",
          "Levels parameters",
          ShowLevelsParameters);

      // get Equipment styles properties action
      createMessageBoxAction(
          "Show Equipment styles properties",
          "Equipment styles properties",
          ShowEquipmentStylesProperties);

      // get Equipment styles parameters action
      createMessageBoxAction(
          "Show Equipment styles parameters",
          "Equipment styles parameters",
          ShowEquipmentStylesParameters);

      ui.AddExtensionToPrimaryPanel(panelExtension);
      return true;
    }

    public void Stop()
    {
    }

    private object GetPropertyValue(Renga.IProperty property)
    {
      switch(property.Type)
      {
        case Renga.PropertyType.PropertyType_Angle: return property.GetAngleValue(Renga.AngleUnit.AngleUnit_Degrees);
        case Renga.PropertyType.PropertyType_Double: return property.GetDoubleValue();
        case Renga.PropertyType.PropertyType_String: return property.GetStringValue();
        case Renga.PropertyType.PropertyType_Area: return property.GetAreaValue(Renga.AreaUnit.AreaUnit_Meters2);
        case Renga.PropertyType.PropertyType_Boolean: return property.GetBooleanValue();
        case Renga.PropertyType.PropertyType_Enumeration: return property.GetEnumerationValue();
        case Renga.PropertyType.PropertyType_Integer: return property.GetIntegerValue();
        case Renga.PropertyType.PropertyType_Length: return property.GetLengthValue(Renga.LengthUnit.LengthUnit_Meters);
        case Renga.PropertyType.PropertyType_Logical: return property.GetLogicalValue();
        case Renga.PropertyType.PropertyType_Mass: return property.GetMassValue(Renga.MassUnit.MassUnit_Kilograms);
        case Renga.PropertyType.PropertyType_Volume: return property.GetVolumeValue(Renga.VolumeUnit.VolumeUnit_Meters3);
      }
      return null;
    }

    private object GetParameterValue(Renga.IParameter parameter)
    {
      switch (parameter.ValueType)
      {
        case Renga.ParameterValueType.ParameterValueType_Double: return parameter.GetDoubleValue();
        case Renga.ParameterValueType.ParameterValueType_String: return parameter.GetStringValue();
        case Renga.ParameterValueType.ParameterValueType_Int: return parameter.GetIntValue();
        case Renga.ParameterValueType.ParameterValueType_Bool: return parameter.GetBoolValue();
      }
      return null;
    }

    private string OutputProperties(Renga.IPropertyContainer properties)
    {
      var result = "";
      var ids = properties.GetIds();
      for (int i = 0; i < ids.Count; ++i)
      {
        var property = properties.Get(ids.Get(i));
        result += string.Format("{0} : {1}\n", property.Name, GetPropertyValue(property));
      }
      return result;
    }

    private string OutputParameters(Renga.IParameterContainer parameters)
    {
      var result = "";
      var ids = parameters.GetIds();
      for (int i = 0; i < ids.Count; ++i)
      {
        var parameter = parameters.Get(ids.Get(i));
        result += string.Format("{0} : {1}\n", parameter.Definition.Text, GetParameterValue(parameter));
      }
      return result;
    }

    private string ShowEquipmentStylesProperties()
    {
      var result = "Equipment style properties:\n\n";
      var project = m_app.Project;
      var styles = project.EquipmentStyles;
      foreach (int id in styles.GetIds())
      {
        var style = styles.GetById(id);
        var properties = style as Renga.IPropertyContainer;

        if (properties != null)
          result += string.Format("{0}: \n{1} \n\n", styles.GetById(id).Name, OutputProperties(properties));
      }

      return result;
    }

    private string ShowEquipmentStylesParameters()
    {
      var result = "Equipment style parameters:\n\n";
      var project = m_app.Project;
      var styles = project.EquipmentStyles;
      foreach (int id in styles.GetIds())
      {
        var style = styles.GetById(id);
        var parameters = style as Renga.IParameterContainer;

        if (parameters != null)
          result += string.Format("{0}: \n{1} \n\n", styles.GetById(id).Name, OutputParameters(parameters));
      }

      return result;
    }

    private string ShowLevelsProperties()
    {
      var result = "Levels properties:\n\n";
      var project = m_app.Project;
      var objects = project.Model.GetObjects();
      foreach (int id in objects.GetIds())
      {
        var currentObject = objects.GetById(id);
        if (currentObject.ObjectType != Renga.ObjectTypes.Level)
          continue;

        var properties = currentObject.GetProperties();

        if (properties != null)
          result += string.Format("{0}: \n{1} \n\n", currentObject.Name, OutputProperties(properties));
      }

      return result;
    }

    private string ShowLevelsParameters()
    {
      var result = "Levels parameters:\n\n";
      var project = m_app.Project;
      var objects = project.Model.GetObjects();
      foreach (int id in objects.GetIds())
      {
        var currentObject = objects.GetById(id);
        if (currentObject.ObjectType != Renga.ObjectTypes.Level)
          continue;

        var parameters = currentObject.GetParameters();

        if (parameters != null)
          result += string.Format("{0}: \n{1} \n\n", currentObject.Name, OutputParameters(parameters));
      }

      return result;
    }

    private string AddPropertyToLevels()
    {
      var project = m_app.Project;

      if (project.HasActiveOperation())
        return "Unable to edit project. Another operation has already begun.";

      var operation = project.CreateOperation();
      operation.Start();

      var propertyManager = project.PropertyManager;

      var propertyDescription = new Renga.PropertyDescription()
      {
        Name = "Sample property",
        Type = Renga.PropertyType.PropertyType_String
      };

      var attributeUuid = Guid.NewGuid();

      // Adding new property description
      propertyManager.RegisterProperty(attributeUuid, propertyDescription);
      propertyManager.AssignPropertyToType(attributeUuid, Renga.ObjectTypes.Level);

      var model = project.Model;
      var modelObjectCollection = model.GetObjects();

      int objectCount = modelObjectCollection.Count;
      for (int i = 0; i < objectCount; ++i)
      {
        var modelObject = modelObjectCollection.GetByIndex(i);
        if (modelObject.ObjectType == Renga.ObjectTypes.Level)
        {
          var pProperty = modelObject.GetProperties().Get(attributeUuid);
          pProperty.SetStringValue("Test value");
        }
      }

      operation.Apply();

      return "Test property set to all levels";
    }
  }
}
