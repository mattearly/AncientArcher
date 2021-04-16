#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Types.h>

bool is_gui_set = false;
void setgui()
{
  if (is_gui_set)
    return;
  AA::AddButton(AA::vec2(.2f), AA::vec2(0.1f, 0.1f));
  is_gui_set = true;
}