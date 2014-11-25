If Quake 2 Was 2D
=================

By AntonioR, for educational and non-commercial use only, as is - no warranty.

I share this code in hope it will help someone learn a thing or two, even if it's how not to write C++ code :P
I have made a lot of mistakes working on this project and I have learned a lot. This project is not a representation of how I structure my code today, and code-wise it has nothing to do with my Broken Mug engine project.

This project is based on a modified engine from the book Advanced 2D Game Development (2008) by J.S. Harbour:
http://www.amazon.com/Advanced-Game-Development-Jonathan-Harbour/dp/1598633422

It's written in C++, uses DirectX for input and rendering, and FMOD for sound.

Say what ?
==========

A Quake 2D demo made in 2012 using MUG(My Unnamed Game) engine:
http://antonior-software.blogspot.com/p/if-quake-2-was-2d.html

http://www.youtube.com/watch?v=M_Vbn4ssY-M

How to ?
========

The repository includes the source code and VS 2008 projects for the engine and the game.
The game assets can be downloaded from the link on my blog.

You need to have DirectX SDK installed. The include and library directories need to be set:
VS 2008->tools->options->projects and solutions->vc++ directories:
Include files:
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
Library files:
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86

Open the projects and create solutions for both projects in the same folder as the project file.
Then you need to build the engine as a static library. Be sure you set to build as "release". The engine.lib file should be located in the Release folder.

Then link the game to engine and other libs and build as "release". The game.exe file should be in the Exe folder.
Put the game assets there and run Game.exe file.

Game project:
VS 2008->project->properties->configuration properties->linker->input->aditional dependencies:
"..\Engine\Release\Engine.lib"
"..\Engine\fmodex_vc.lib"
d3d9.lib
d3dx9.lib
dxguid.lib
winmm.lib
dinput8.lib
user32.lib
gdi32.lib

Issues
======

I can't say I'm proud with my code, but it works and the game itself is not bad at all, if I dare to say.
When building the projects there will be hundreds of warnings, because I didn't bother cleaning up the code.

Bugs
----
- Collision code is a disaster, but it works in most cases. On high framerates (>150 FPS) there are collision problems on the edges of the sprites and when player goes from one platform to another(player animations switch rapidly).
- Explosions don't seem to work in co-op when the players are in different maps

Updates v1.01
-------------

- Animations no longer speed up on framerates higher than 1000FPS (GetDeltaFix())
- Biomech rocket does damage to the player
- Added frame lock in options menu (that doesn't really work, turn on vsync instead)

