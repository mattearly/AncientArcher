#pragma once
#include "SettingsOptions.h"
namespace AA
{
class Settings
{
public:
  static Settings* Get();
  SettingsOptions GetOptions();
  void SetOptions(SettingsOptions opt) noexcept;
  void ResetSettings();
private:
  Settings();

  SettingsOptions mOptions;
};
} // end namespace AA
