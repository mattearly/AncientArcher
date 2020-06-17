<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer Source

## To Run / Work on Development

### Clone AA and OpenSource Dependencies (include/libs) from Github

folder structure:

```
reposrootfolder
|--mattearly  
|  |--AncientArcher      # Build Last
|--g-truc
|  |--glm                # Don't build, header only library.
|--nothings
|  |--stb                # Don't build, obj is created with AA Build.
|--assimp
|  |--assimp             # Build
|--SDL-mirror
|  |--SDL                # Build
|  |--SDL_mixer          # Don't build, binaries included in AA
|--glfw
|  |--glfw               # Build
```

Yes I have a folder for each github user name. I don't think most people do it this way, however I prefer it thus my .vcproj file(s) are set up to find them that way.

Dependancies are kept up to date by pulling and rebuilding the latest Release and Debug libs (cmake to a build/ folder in repo, build using Visual Studio). Built with master branches of above repos. Note that the ones that aren't being built are only required for their headers.

Builds in x64 only, Release or Debug. There are no Debug symbols for SDL_mixer.
