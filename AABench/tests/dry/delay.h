#pragma once
#include <chrono>
#include <thread>
#include <iostream>

void delay(int ms);
void holdAtCommandPrompt();

void delay(int ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void holdAtCommandPrompt()
{
  std::cout << "Press Enter To Continue\n";
  char a;
  a = std::cin.get();
}
