/*
  https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
*/

#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using std::string;
using std::vector;

vector<string> get_all_files_names_within_folder(string folder);