#pragma once
class RPG
{
public:
	int run();
private:
	int init();
	enum class RPGSTATE { quit, run } gamestate;
};

