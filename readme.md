<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer

Ancient Archer is a C++ scripting engine for game development, research, implementing visual algorithms, or just practice. It can be included into any C++ project as a static or dynamic library.

## Use Ancient Archer

Link as you would any other C++ library, and then you can begin using it in your code.

```cpp
#include <AncientArcher.h>
int main()
{
  AA::InitEngine();
  // optional: your code here -> implement camera, lights, models, sounds, input handling, and their logic at will
  AA::Run();
}
```

## Build Ancient Archer

### OS, Compiler

Compiled on Windows MSVC16, x64. Installing the Visual Studio 2019 Community Edition.

### Dependencies

glm, assimp, glfw3, glad, OpenAL, sndfile

#### Installing Dependencies

For ease of use, use vcpkg to install these libraries. The line of code below should get you what you need from vcpkg.

`vcpkg install glm:x64-windows assimp:x64-windows glfw3:x64-windows glad:x64-windows openal-soft:x64-windows libsndfile:x64-windows`

If you don't know how to set up vcpkg, check their repo here: [vcpkg repo](https://github.com/microsoft/vcpkg). Or if you don't want to read and would rather watch, check my [vcpkg tutorial video](https://youtu.be/pSirBt4OgXQ).
