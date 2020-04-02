#pragma once

// SIMPLIFIED SINGLE DICE ROLLERS
// real rolls - dice replicants  (only 1d)
#define roll1d2   Random::ZOOR() + 1
#define roll1d3   Random::NTKR(1, 3)
#define roll1d4   Random::NTKR(1, 4)
#define roll1d6   Random::NTKR(1, 6)
#define roll1d8   Random::NTKR(1, 8)
#define roll1d10  Random::NTKR(1, 10)
#define roll1d12  Random::NTKR(1, 12)
#define roll1d20  Random::NTKR(1, 20)
#define roll1d30  Random::NTKR(1, 30)
#define roll1d34  Random::NTKR(1, 34)
#define roll1d40  Random::NTKR(1, 40)
#define roll1d60  Random::NTKR(1, 60)
#define roll1d100 Random::NTKR(1, 100)
#define roll1d120 Random::NTKR(1, 120)
#define roll1d400 Random::NTKR(1, 400)

// further aliases
#define rolld2   roll1d2
#define rolld3   roll1d3
#define rolld4   roll1d4
#define rolld6   roll1d6
#define rolld8   roll1d8
#define rolld10  roll1d10
#define rolld12  roll1d12
#define rolld20  roll1d20
#define rolld30  roll1d30
#define rolld34  roll1d34
#define rolld40  roll1d40
#define rolld60  roll1d60
#define rolld100 roll1d100
#define rolld120 roll1d120
#define rolld400 roll1d400

