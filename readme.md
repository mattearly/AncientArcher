<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer

Ancient Archer is a scripting engine for game development, research, or just practice. It can be included into any C++ project as a static or dynamic library.

## Build Ancient Archer

### OS, Compiler

 Compiled on Windows MSVC16, x64. You can use Visual Studio 2019 Community Edition.

### Dependencies

glm, assimp, glfw3, opengl, openal, sndfile

Currently, I use vcpkg for handling the libraries. The line of code below should get you what you need from vcpkg.

`vcpkg install glm:x64-windows assimp:x64-windows glfw3:x64-windows glad:x64-windows openal-soft:x64-windows libsndfile:x64-windows`

Of course there is a little more to know about vcpkg if you haven't used it before, you can see my [vcpkg tutorial video](https://youtu.be/pSirBt4OgXQ) or just look at the documentation for it on the [vcpkg repo](https://github.com/microsoft/vcpkg).

## Use Ancient Archer

Link as you would any other C++ library and you can begin using it in your code.

```cpp
#include <AncientArcher.h>
int main()
{
  AA::Engine->Run();
}
```
