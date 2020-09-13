<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer Source

## To Run / Work on Development

### Clone AA and OpenSource Dependencies into the same root folder(include/libs) from Github

folder structure:

```
yourrepofolder/
	AncientArcher      # Build Last
	glm                # Don't build, header only library.
	stb                # Don't build, obj is created with AA Build.
	assimp             # Build
	SDL                # Build
	SDL_mixer          # Don't build, binaries included in AA
	glfw               # Build with MT lib
```

Dependancies are kept up to date by pulling and rebuilding the latest Release and Debug libs (cmake to a build/ folder in repo, build using Visual Studio). Built with master branches of above repos. Note that the ones that aren't being built are only required for their headers.

Builds in x64 only, Release or Debug. There are no Debug symbols for SDL_mixer.

A note on building glfw from source: be sure to change the runtime library to Multi-threaded (/MT) before building to match with this project, otherwise the linker will complain.
