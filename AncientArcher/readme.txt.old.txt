
improvements over last engine (prog3)

- IDE Visual Studio 2017
- OpenGL 3.30+
- added textures
- collision system improved (can now slide along walls instead of getting stuck)
- glad instead of glew
- glfw instead of glut
- glm mathematics instead of Angel
- player position tracked seperately from camera

done 

- jump
- sound effects
- texture bank class
- player models
- more player stats
- pickups
- archery firing
- melee swing


by Wednesday

- basic lighting
- badguys (AI) / targets to win

bonus:
- mesh varying level ground
- advanced lighting
- hud

later:
- classes to handle drawing objects ready for lighting
- and lighting class that allows emmiting light

to run 

- video card must support OpenGL 3.30 or later
- open in latest version of Visual Studio
	- dependencies should be included with project via the Libraries and Include folders, you'll just have to point to them with Visual Studio.
	- go to menu: Debug / Properties / VC++ Directories
		- add Libraries to project Library path
		- add Include to project Include path
	- go to menu: Debug / Properties / Linker / Input / Additional Dependencies
		- add opengl32.lib
		- add glfw3.lib
		- add SDL2_mixer.lib
		- add SDL2.lib
		- add SDL2main.lib
	- address any missing dependencies