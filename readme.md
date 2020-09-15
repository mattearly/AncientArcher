<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer

## To Run / Work on Development

Requirements: Windows 10, Visual Studio Community Edition 2019, CMake, Git

Dependencies: glm, stb, assimp, glfw

1. Download dependencies (all open source on github) into the same root folder. i.e. 

```
path/to/yourrepos/AncientArcher
path/to/yourrepos/glm
path/to/yourrepos/stb
path/to/yourrepos/assimp
path/to/yourrepos/glfw
```

2. Build assimp

	1. Use CMake to configure and build into `path\to\yourrepos\assimp\build`
	2. Open assimp.sln with Visual Studio Community 2019 (located in the build folder)
	3. Build both Debug x64 and Release x64 Configurations.
	4. Done

3. Build glfw

	1. Same process as assimp, but before building change the runtime library in properties to `/MT`

4. Open AncientArcher.sln and profit


