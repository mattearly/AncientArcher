#pragma once
#include <string>
#include <windows.h>

std::string getexepath()
{
  char result[MAX_PATH];
  return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}