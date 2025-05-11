[![github](https://img.shields.io/github/license/pyroceper/RL-Dice.svg)](https://choosealicense.com/licenses/mit/)
[![commit](https://img.shields.io/github/last-commit/pyroceper/rl-dice.svg)](https://github.com/pyroceper/RL-Dice/commits/main)

RL-Dice C++
=======

A C++ port of the awesome roguelike dice library [RL-Dice](https://github.com/timothymtorres/RL-Dice) by [timothymtorres](https://github.com/timothymtorres/)

Build
------------
To build the static library
```
mkdir build && cd build
cmake ..
make
```
Once the library has been built, simply include `dice.hpp` in your project and link against the compiled static library.

Usage
------------
Before rolling dice, you must **register a random number generator function**. If no RNG is registered, all dice rolls will return fixed values.

```C++
#include "dice.hpp"

// register random function
rldice::Dice::registerRNG([](int min, int max) {
  return min + (std::rand() % (max - min + 1));
});
auto d6 = rldice::Dice(6); //1d6
auto d6_rolls = d6.roll(); // returns a vector of dice rolls

auto die = rldice::Dice("3d6+5");
auto die_rolls = die.roll();
```

The dice class provides the following:

* Dice notation strings - used to create or roll dice instances
* Dice number/faces - can be increased or decreased
* Dice bonus - can be increased or decreased
* Dice rerolls - can reroll and filter out via highest or lowest results
* Dice sets - can return multiple results
* Dice pluralism - enable bonus or rerolls to individual dice or to all dice

Credits
------------
[RL-Dice](https://github.com/timothymtorres/RL-Dice) by [timothymtorres](https://github.com/timothymtorres/)
