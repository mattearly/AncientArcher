#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Types.h>




bool is_gui_set = false;
void setgui()
{
  if (is_gui_set)
    return;
  AA::AddButton(AA::vec2(-.8,-.8), AA::vec2(.08, .11), .21f);
  is_gui_set = true;
}