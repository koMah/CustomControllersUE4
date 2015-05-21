// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"


/**
 * The public interface to this module.  In most cases, this interface is only public to sibling modules 
 * within this plugin.
 */
class CustomControllersImpl : public IModuleInterface
{

public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
};

