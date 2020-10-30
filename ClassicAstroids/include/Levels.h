#pragma once


enum class currentlevel { start, one, two, three, end };
static currentlevel level;
static int levelonescore(0), leveltwoscore(0), levelthreescore(0);
int totalscore() { return levelonescore + leveltwoscore + levelthreescore; };