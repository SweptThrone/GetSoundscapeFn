# GetSoundscapeFn
Garry's Mod binary module for getting the player's active soundscape entity.  

## Summary  
This Garry's Mod binary module adds the function `Player.GetSoundscapeEntity` to  
your server.  
This is a **server-only** module since client-side binary modules are too much  
of a hassle to distribute.  

## Installation and Running
The easiest way to use this module is to download the DLL file from the Releases  
section on the right.  
There is a Windows release and a Linux release.  
Remember that these are **server-only**.  
Place this DLL in your game/server's `garrysmod/lua/bin` folder.  If you do not  
have a `bin` folder, create it.  
In case you don't know, the DLL file for Linux is really an SO file, renamed,  
so **do not** rename this module for Linux.  
In a Lua file somewhere, add `require( "getsoundscapefn" )` before any time you  
use `player:GetSoundscapeEntity()`.

## Notes
If the player does not have an active soundscape, this function will return  
`Weapon [NULL]`.  This entity passes `ent.IsWeapon` but fails `IsValid( ent )`.  
Like every entity-returning function in this game, you should make sure it's  
valid before doing anything with it.  
I wanted to make this return `[NULL Entity]` instead, but I just kept causing  
crashes.

## Building
The harder way to install it would be to set up a module workspace and build it  
yourself.  There's next to no documentation for this so I only recommend you do  
this if you know what you're doing.  
You can get started [here](https://wiki.facepunch.com/gmod/Creating_Binary_Modules:_Premake).

## Contributing
This is the first binary module I've made for Garry's Mod, so it's very possible  
I've done something horribly wrong.  Make requests, report issues, do whatever  
you want.
