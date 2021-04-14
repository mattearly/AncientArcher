#pragma once
#include <AncientArcher/AncientArcher.h>
//const char* town_theme_stereo = "..\\ExampleProject\\res\\TownTheme_stereo.wav";
const char* talking_stereo = "..\\ExampleProject\\res\\talking_stereo.wav";

void setmusic() {
  AA::AddMusic(talking_stereo);

  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) -> bool {
    if (kb.p) {
      AA::PlayMusic();
      return true;
    }
    else if (kb.o) {
      AA::StopMusic();
      return true;
    }
    else if (kb.u) {
      AA::RemoveMusic();
      return true;
    }
    else if (kb.r) {
      AA::AddMusic(talking_stereo);
      return true;
    }

    return false;
    });

  //AA::PlayMusic();

}