<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer

Ancient Archer is a scripting game engine that can be included as a C++ Library. It generally functions by adding functions to process during the main loop and then pressing run. See the demo projects for how to use it.

## build & run

Dependencies: glm, assimp, glfw3, opengl, openal, sndfile

Currently I use vcpkg for handling the libraries. Note: download the x64-windows versions as AncientArcher is set to x64 build

`vcpkg install glm:x64-windows assimp:x64-windows glfw3:x64-windows glad:x64-windows openal-soft:x64-windows libsndfile:x64-windows`

[vcpkg tutorial](https://youtu.be/pSirBt4OgXQ)

## dev notes

Because Visual Studio has .obj intermediate files that are in the gitignore, to add obj models files and stuff in the models/obj folder to version control, you must run `git add -f *.obj *.png *.mtl` in command line.

Anyone is welcome to work on this and submit pull requests. There are a ton of things that can be improved/added.

## road map

(subject to change)
- 2020 November: Audio Integration Focus
- 2020 December: Physics Integration Focus
- 2021 Janurary: Shader and Render Focus
- 2021 Feburary: ???
