#pragma once
#include <AncientArcher/AncientArcher.h>

bool is_inventory_open = false;
bool is_gui_set = false;
void setgui(int cam) {
  if (is_gui_set)
    return;
  is_gui_set = true;
  // INTERFACE STUFF
  for (float i = .4f; i < .9f; i += .1f)
    for (float j = -.8f; j < -.1f; j += .1f)
      //AA::AddButton(AA::vec2(i, j), AA::vec2(.045, .045), AA::vec3(.5f), .85f);
      AA::AddButton(AA::vec2(i, j), AA::vec2(.045, .045), .85f, "E:\\AssetPack\\tile.bmp");

  //AA::AddButton(AA::vec2(.0, .0), AA::vec2(1, 1), .85f, "E:\\AssetPack\\tile.bmp");

  AA::SetGUIVisibility(is_inventory_open);

  const char* open_inv = "..\\ExampleProject\\res\\open_inv.ogg";
  static int open_sound_id = AA::AddSoundEffect(open_inv);

  const char* close_inv = "..\\ExampleProject\\res\\close_inv.ogg";
  static int close_sound_id = AA::AddSoundEffect(close_inv);

  std::cout << ", tab = open inventory";
  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb)-> bool {
    if (kb.tab) {
      if (!is_inventory_open) {
        // (open inventory) mouse on screen
        AA::SetMouseToNormal();
        AA::SetMouseReadToNormal();
        is_inventory_open = true;
        AA::SetGUIVisibility(is_inventory_open);
        AA::PlaySoundEffect(open_sound_id);
        return true;
      } else {
        // (close inventory) mouse in fpp hidden snapping ot middle mode
        AA::SetMouseToDisabled();
        AA::SetMouseReadToFPP();
        is_inventory_open = false;
        AA::SetGUIVisibility(is_inventory_open);
        AA::PlaySoundEffect(close_sound_id);
        return true;
      }
    }
    return false;
  });
}