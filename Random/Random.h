#pragma once

// unatural rolls
#define roll1d1000      Random::NTKR(1, 1000)      // 1 to 1,000   Thousand
#define roll1d10000     Random::NTKR(1, 10000)     // 1 to 10,000  Ten Thousand
#define roll1d100000    Random::NTKR(1, 100000)    // 1 to 100,000  Hundred Thousand
#define roll1d1000000   Random::NTKR(1, 100000)    // 1 to 1,000,000  Million
#define roll1d10000000  Random::NTKR(1, 1000000)   // 1 to 10,000,000  Ten Million
#define roll1d100000000 Random::NTKR(1, 10000000)  // 1 to 100,000,000  Hundred Million

#define rolld1000      roll1d1000
#define rolld10000     roll1d10000
#define rolld100000    roll1d100000
#define rolld1000000   roll1d1000000
#define rolld10000000  roll1d10000000
#define rolld100000000 roll1d100000000
