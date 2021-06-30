<img src="AncientArcher.png" height="96px" align="right">

# Ancient Archer

Ancient Archer is a C++ scripting engine for game development, research, implementing visual algorithms, or just practice. It can be included into any C++ project as a static library.

## Use Ancient Archer

Link as you would any other static C++ library, and then you can begin using it in your code.

```cpp
#include <AncientArcher.h>
int main()
{
  AA::Init();
  // your code here -> implement camera, lights, models, sounds, input handling, and their logic at will
  return AA::Run();
}
```

## Build Ancient Archer

### Dependencies

glm, assimp, glfw3, glad, OpenAL-Soft, sndfile, imgui

vcpkg required library install command. The line of code below should get you what you need from vcpkg.

`vcpkg install glm:x64-windows assimp:x64-windows glfw3:x64-windows glad:x64-windows openal-soft:x64-windows libsndfile:x64-windows imgui:x64-windows`

### OS, Compiler, IDE

Use Windows MSVC16, x64. Visual Studio 2019 Community Edition.

## Run

- AAEngine : the core static library
- BaseRun  : test application that inits and runs
- ExampleProject : test application that is a demo fps playground
![AA v0.0.2 example project screenshot](https://i.imgur.com/OL5mloN.png)