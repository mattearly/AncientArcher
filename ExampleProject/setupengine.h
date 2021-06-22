#pragma once
#include<AncientArcher/AncientArcher.h>
#include<AncientArcher/Version.h>
#include <iostream>
void setupengine(){
  AA::Init();
#if _DEBUG
  printf("AncientArcherEngine ver: %i.%i.%i\n", AA::ENGINEVERSIONMAJOR, AA::ENGINEVERSIONMINOR, AA::ENGINEVERSIONPATCH);
#endif
}