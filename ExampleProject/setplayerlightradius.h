#pragma once
#include <AncientArcher/AncientArcher.h>

bool is_player_radius_set = false;

void setplayerlightradius(int ourcam)
{
  if (is_player_radius_set)
    return;
  is_player_radius_set = true;

  static int local_cam = ourcam;

  /*
  * Distance  Constant  Linear  Quadratic
  * 7         1.0       0.7     1.8
  * 13        1.0       0.35    0.44
  * 20        1.0       0.22    0.20
  * 32        1.0       0.14    0.07
  * 50        1.0       0.09    0.032
  * 65        1.0       0.07    0.017
  * 100       1.0       0.045   0.0075
  * 160       1.0       0.027   0.0028
  * 200       1.0       0.022   0.0019
  * 325       1.0       0.014   0.0007
  * 600       1.0       0.007   0.0002
  * 3250      1.0       0.0014  0.000007
  */
  static int raidus_light_id = AA::AddPointLight(AA::GetCamPosition(local_cam), 1.f, .0014f, .000007f, glm::vec3(1), glm::vec3(1), glm::vec3(1.f));

  AA::AddToUpdate([](){
    AA::MovePointLight(raidus_light_id, AA::GetCamPosition(local_cam));
  });

  



}