#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/LuaObject.h>
#include <player.h>
#include <GarrysMod/Lua/LuaInterface.h>
#include "../garrysmod_common/sourcesdk-minimal/tier1/interface.cpp"

using namespace GarrysMod::Lua;

// just define this at all to fix linker errors
CBaseEntityList* g_pEntityList = nullptr;

// different per platform
#ifdef _WIN32
	const char* engineModule = "engine";
#elif defined( __linux__ )
	const char* engineModule = "engine_srv.so";
#endif

// need engine server
IVEngineServer* engineServer = nullptr;

// define some manual offsets
// methods from the SDK seem to not work
#ifdef _WIN32
	ptrdiff_t mAudioOffset = 0x218C;
	ptrdiff_t soundscapeEntOffset = 0x6C;
#elif defined( __linux__ )
	ptrdiff_t mAudioOffset = 0x21A0;
	ptrdiff_t soundscapeEntOffset = 0x6C;
#endif

// creds to raphaelit7 from the GMod Discord server for this function
CBaseEntity* GetEntityFromBaseHandle( CBaseHandle &pHandle ) {
	if ( !pHandle.IsValid() ) {
		// Msg( "pHandle is invalid!\n" );
		return nullptr;
	}

	edict_t* pEdict = engineServer->PEntityOfEntIndex( pHandle.GetEntryIndex() );

	if ( !pEdict ) {
		// Msg( "pEdict is invalid!\n" );
		return nullptr;
	}

	return ( CBaseEntity* )pEdict->GetUnknown();
}

LUA_FUNCTION( GetSoundScape ) {
	LUA->CheckType( 1, GarrysMod::Lua::Type::Entity );

	CBaseHandle* hPly = LUA->GetUserType< CBaseHandle >( 1, GarrysMod::Lua::Type::Entity );

	CBaseEntity* ePly = GetEntityFromBaseHandle( *hPly );
	if ( !ePly ) {
		// Msg( "Could not get the player Entity!\n" );
		LUA->Pop();
		return 0;
	}
	CBasePlayer* ply = ( CBasePlayer* )ePly;

	// do some C++ magic to get what we want
	uintptr_t pPly = ( uintptr_t )ply;
	uintptr_t pAudio = pPly + mAudioOffset;
	uintptr_t pSoundscapeHandle = pAudio + soundscapeEntOffset;
	CNetworkHandleBase< CBaseEntity, audioparams_t::NetworkVar_ent >* pSoundscape = ( CNetworkHandleBase< CBaseEntity, audioparams_t::NetworkVar_ent >* )pSoundscapeHandle;
	CNetworkHandleBase hSoundscape = *pSoundscape;
	CBaseEntity* eSoundscape = GetEntityFromBaseHandle( hSoundscape.m_Value );
	if ( !eSoundscape ) {
		//Msg( "Could not get the soundscape Entity!\n" );
		LUA->Pop();
		CBaseHandle* invalidHandle = new CBaseHandle();
		LUA->PushUserType( new CBaseHandle(), GarrysMod::Lua::Type::Entity );
		return 1;
	}

	LUA->Pop(); // pop the player argument

	eSoundscape->PushEntity();

	return 1;
}

GMOD_MODULE_OPEN() {
	Msg( "[ST - INFO] Began loading GetSoundscapeFn module...\n" );

	CreateInterfaceFn createInterface = Sys_GetFactory( engineModule );

	if ( !createInterface ) {
		Msg( "[ST - ERROR] Could not find CreateInterface!\n" );
		Msg( "[ST - FATAL] GetSoundscapeFn module loading failed!\n" );
		return 0;
	} else {
		Msg( "[ST - INFO] Found CreateInterface!\n" );
	}

	engineServer = ( IVEngineServer* )createInterface( INTERFACEVERSION_VENGINESERVER, nullptr );

	if ( !engineServer ) {
		Msg( "[ST - ERROR] Could not find EngineServer!\n" );
		Msg( "[ST - FATAL] GetSoundscapeFn module loading failed!\n" );
		return 0;
	} else {
		Msg( "[ST - INFO] Found EngineServer!\n" );
	}

	Msg( "[ST - INFO] Loaded GetSoundscapeFn module successfully!\n" );

	/*
	* this creates global GetSoundscape() instead, don't want that
	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
	LUA->PushString( "GetSoundscape" );
	LUA->PushCFunction( GetSoundScape );
	LUA->SetTable( -3 ); // set _G.GetSoundscape = GetSoundScape()
	*/

	LUA->GetField( GarrysMod::Lua::INDEX_REGISTRY, "Player" );
	LUA->PushString( "GetSoundscapeEntity" );
	LUA->PushCFunction( GetSoundScape );
	LUA->SetTable( -3 );

	return 0;
}

GMOD_MODULE_CLOSE() {
	Msg( "Unloaded soundscape fn\n" );
	return 0;

}
