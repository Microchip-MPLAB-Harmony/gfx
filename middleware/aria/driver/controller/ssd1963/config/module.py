def loadModule():	
	cntlComponent = Module.CreateComponent("gfx_driver_ssd1963", "SSD1963", "/Graphics/Driver", "config/ssd1963_controller.py")
	cntlComponent.setDisplayType("SSD1963 Display Driver")
	cntlComponent.addCapability("gfx_driver_ssd1963", "Display Driver", False)
	cntlComponent.addDependency("Display Interface", "Display Interface", False, True)
	cntlComponent.addDependency("SYS_TIME", "SYS_TIME", True, True)
