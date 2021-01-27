#include "../../include/Settings/Settings.h"
namespace AA {

static Settings* ourSettings = nullptr;


Settings* Settings::Get()
{
	if (!ourSettings)
		ourSettings = new Settings();
	return ourSettings;
}

SettingsOptions Settings::GetOptions()
{
	return mOptions;
}

void Settings::SetOptions(SettingsOptions opt) noexcept
{
	mOptions = opt;
}

void Settings::ResetSettings()
{
	delete ourSettings;
	ourSettings = nullptr;
	if (!ourSettings)
		ourSettings = new Settings();
}

Settings::Settings()
{
	//ourSettings = nullptr;
}

}  // end namespace AA