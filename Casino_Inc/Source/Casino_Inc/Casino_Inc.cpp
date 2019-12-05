// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Casino_Inc.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Casino_Inc, "Casino_Inc" );  // NOLINT

// ReSharper disable CppInconsistentNaming					
//log category's I want to be UpperCamelCased
DEFINE_LOG_CATEGORY(LogCasino_Inc)
DEFINE_LOG_CATEGORY(TileMap)
DEFINE_LOG_CATEGORY(GUI)
DEFINE_LOG_CATEGORY(PlayerInputLog)
DEFINE_LOG_CATEGORY(PathFinding)
DEFINE_LOG_CATEGORY(AIManagement)
DEFINE_LOG_CATEGORY(GOAP)
DEFINE_LOG_CATEGORY(TimeLog)
DEFINE_LOG_CATEGORY(CasinoStats)
// ReSharper restore CppInconsistentNaming
