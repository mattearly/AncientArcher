#pragma once
#include <string>
#include <windows.h>

static std::string getexepath()
{
  char result[MAX_PATH];
  return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}
