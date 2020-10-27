<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer

## build & run

Currently I use vcpkg for handling the libraries. Note: download the x64-windows versions as AncientArcher is set to x64 build

Dependencies: glm, assimp, glfw3, opengl

`vcpkg install glm:x64-windows assimp:x64-windows glfw3:x64-windows glad:x64-windows`

# Dev Notes

Notes:

Because Visual Studio has .obj intermediate files that are in the gitignore, to add obj models files and stuff in the models/obj folder to version control, you must run `git add -f *.obj *.png *.mtl` in command line.
