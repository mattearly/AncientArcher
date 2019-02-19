# Ancient Archer (TM)

###### Author(s): Matthew Early;  Reviewer(s): Matthew Early

#### Overview

Ancient Archer is a 3D RPG Adventure Game.

#### Context

To build Ancient Archer, we need to implement a main player with stats, a efficient dynamic 3D object system, and a world layout that has a progression-based design, and UI system.

The game should tell a unique story while going through the world, and scale from low level to high level as a typical RPG would.  

#### Goals

Ancient Archer should be considered (nearly) complete when the entire world area is playable, the leveling system works, the story makes sense, difficulty is appropriate.

Metrics: playtime, player move distance, levels, kills, items obtained, time in each area, npcs talked to

non-goals: multiplayer, moddability

#### Milestones

* Start Date: Feb-13-2019
* Milestone 1 - 3D object system works: Feb-20-2019
* Milestone 2 - player and items work: Feb-27-2019
* Milestone 3 - world built: March-06-2019
* Milestone 4 - interactables work (monsters, items, drops): March-13-2019
* Milestone 5 - story links are built: March-20-2019
* Milestone 6 - experience and leveling system work: March-27-2019
* End Date: March-27-209

#### Existing Solutions

My partially built engine AncientArcher

* Programming Language: C++ std11
* Additional Libraries: 
    * glad for OpenGL 3.30 graphics pipeline
    * glfw3 for windowing and input
    * SDL2_mixer for playing sound
    * glm for 3d math
    * stb_image to load texture
    * assimp to load meshes and models

#### Proposed Solution

We'll be building our own in house solution, continuing on the current Ancient Archer project.

Program should flow something like this on run:

* run: init systems -> intro -> main menu {new game, continue, options}
    * new game: starts a new game from the beginning
    * continue: continue your playthough
    * options: adjust game settings

In game, 3D areas should load based on distance from the player. Worlds will be predrawn.

#### Alternative Solutions

* Random level gautlet-esque play

#### Testability, Monitoring and Alerting

* A logging system in place that writes to a log file stamped with run date and time

#### Detailed Scoping and Timeline

[TASK TRACKER](https://github.com/mattearly/AncientArcher/projects)
