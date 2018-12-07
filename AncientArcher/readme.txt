
improvements over last engine (prog3)

- IDE Visual Studio 2017
- OpenGL 3.30+
- added textures
- collision system improved (can now slide along walls instead of getting stuck)
- glad instead of glew
- glfw instead of glut
- glm mathematics instead of Angel
- player position tracked seperately from camera

by Wednesday

- player textures
- archery firing
- melee swing
- jump
- sound effects
- basic lighting
- badguys (AI) / targets to win

bonus:
- mesh varying level ground
- more player stats
- advanced lighting
- hud


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
	- address any missing dependencies