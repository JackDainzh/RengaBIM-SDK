import sys
import win32com.client

if __name__ == '__main__':
	app = win32com.client.Dispatch("Renga.Application.1")
	app.Visible = True

	if app.CreateProject() != 0:
		print("Error opening project")
		sys.exit(1)

	project = app.Project
	if project.HasActiveOperation():
		print("Unable to edit project. Another operation has already begun.")
		app.CloseProject(True)
		app.Quit()
	
	# Creating and starting an operation
	operation = project.CreateOperation()
	operation.Start()
	
	model = project.Model
	propertyMng = project.PropertyManager
	propertyId = '{A288D248-C715-4796-A911-83B2DCD4BDD9}'# random
	propertyName = 'Test string property'
	levelType = '{C3CE17FF-6F28-411F-B18D-74FE957B2BA8}' # GUID for the 'level' object type, as listed in documentation. See "Object types".
	
	# Property registration
	# NOTE: using the S-method accepting the GUIDs as strings
	propertyMng.RegisterPropertyS(propertyId, propertyName, 2) # 2 means string type, see the docs.

	# Assigning property to the level type
	# NOTE: using the S-method to work with the GUID
	propertyMng.AssignPropertyToTypeS(propertyId, levelType)
	print("Property registered in Renga and assigned to all levels")

	# Setting a property value to all levels
	objectCollection = model.GetObjects()
	for index in range(objectCollection.Count):
		object = objectCollection.GetByIndex(index)
		# NOTE: using the S-property
		if object.ObjectTypeS == levelType:
			propertyContainer = object.GetProperties()
			property = propertyContainer.GetS(propertyId)
			# NOTE: quering an additional interface with the GetInterfaceByName() method
			levelModel = object.GetInterfaceByName("ILevel")
			# setting the property value equal to the level name
			property.SetStringValue(levelModel.LevelName)
	
	# Applying the changes
	operation.Apply()

	# Closing the project without saving, dumping all the hard work
	app.CloseProject(True)
	app.Quit()