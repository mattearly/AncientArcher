#pragma once
#include <string>
#include <AL/al.h>
namespace AA{
static std::string ErrorCheck(ALenum error)
{
    if (error == AL_INVALID_NAME)
    {
        return "Invalid name";
    }
    else if (error == AL_INVALID_ENUM)
    {
        return " Invalid enum ";
    }
    else if (error == AL_INVALID_VALUE)
    {
        return " Invalid value ";
    }
    else if (error == AL_INVALID_OPERATION)
    {
        return " Invalid operation ";
    }
    else if (error == AL_OUT_OF_MEMORY)
    {
        return " Out of memory like! ";
    }

    return " No Error or don't know which error ";

}
}