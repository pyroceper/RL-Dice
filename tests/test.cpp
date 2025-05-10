#include "dice.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>

const int SAMPLE_N = 100000;

float getRollSample(rldice::Dice &dice)
{
    float x = 0;
    for(int i = 0; i < SAMPLE_N; i++) {
        auto rolls = dice.roll();
        for(int j = 0;j < rolls.size(); j++) {
            x = x + rolls[j];
        }
    }
    x = x / SAMPLE_N;
    return std::floor(x * 10.0f) / 10.0f; // truncate 1 decmial place
}

void testInstance()
{
    auto d6 = rldice::Dice(6);
    assert(d6.getNum() == 1);
    assert(d6.getFaces() == 6);
    assert(d6.getBonus() == 0);
    assert(d6.getSets() == 1);
    assert(d6.getRerolls() == 0);
    assert(d6.getMin() == 1);

    // register random function
    std::srand(time(nullptr));
    rldice::Dice::registerRNG([](int min, int max) {
        return min + (std::rand() % (max - min + 1));
    });
}

void testRoll()
{
    auto d6_1st = rldice::Dice(6);
    auto d6_2nd = rldice::Dice("1d6");
    auto result_1 = getRollSample(d6_1st);
    auto result_2 = getRollSample(d6_2nd);
    assert(result_1 >= 3);
    assert(result_2 >= 3);
}

void testAdd()
{
    auto d6 = rldice::Dice(6);
    d6 = d6 + 1;
    assert(d6.toString() == "1d6+1");

    auto result_1 = getRollSample(d6);
    assert(result_1 >= 4);

    d6 = d6 + -1;
    assert(d6.toString() == "1d6");
    auto result_2 = getRollSample(d6);
    assert(result_2 >= 3);
}

void testSub()
{
    auto d6 = rldice::Dice(6);
    d6 = d6 - 1;
    assert(d6.toString() == "1d6-1");
    auto result_1 = getRollSample(d6);
    assert(result_1 >= 1);

    d6 = d6 - (-1);
    assert(d6.toString() == "1d6");
    auto result_2 = getRollSample(d6);
    assert(result_2 >= 3);
}

void testMul()
{
    auto monopoly_dice = rldice::Dice("1d6");
    monopoly_dice = monopoly_dice * 1;
    assert(monopoly_dice.toString() == "2d6");

    auto result_1 = getRollSample(monopoly_dice);
    assert(result_1 >= 6);

    monopoly_dice = monopoly_dice * -1;
    assert(monopoly_dice.toString() == "1d6");

    auto result_2 = getRollSample(monopoly_dice);
    assert(result_2 >= 3);
}

void testDiv()
{
    auto die = rldice::Dice("1d6");
    die = die / 2;
    assert(die.toString() == "1d8");

    auto result_1 = getRollSample(die);
    assert(result_1 == 4);

    die = die / -2;
    assert(die.toString() == "1d6");

    auto result_2 = getRollSample(die);
    assert(result_2 == 3);
}

void testExp()
{
    auto die = rldice::Dice("1d6");
    die = die ^ 1;
    assert(die.toString() == "1d6^+1");

    auto result_1 = getRollSample(die);
    assert(result_1 >= 4);

    die = die ^ -2;
    assert(die.toString() == "1d6^-1");

    auto result_2 = getRollSample(die);
    assert(result_2 >= 2);

    die = die ^ 2;
    assert(die.toString() == "1d6^+1");

    auto result_3 = getRollSample(die);
    assert(result_3 >= 4);
}

void testMod()
{
    auto die = rldice::Dice("1d6");
    die = die % 1;
    assert(die.toString() == "(1d6)x2");

    auto result_1 = getRollSample(die);
    assert(result_1 >= 5);

    die = die % -1;
    assert(die.toString() == "1d6");
    auto result_2 = getRollSample(die);
    assert(result_2 >= 3);
}

void testToString()
{
    auto die = rldice::Dice(6);
    assert(die.toString() == "1d6");
    die = die + 4;
    assert(die.toString() == "1d6+4");
    die = die - 2;
    assert(die.toString() == "1d6+2");
    die = die * 2;
    assert(die.toString() == "3d6+2");
    die = die / -2;
    assert(die.toString() == "3d4+2");
    die = die ^ 1;
    assert(die.toString() == "3d4+2^+1");
    die = die % 2;
    assert(die.toString() == "(3d4+2^+1)x3");
}

int main(int argc, char *argv[])
{
    testInstance();
    testRoll();
    testAdd();
    testSub();
    testMul();
    testExp();
    testMod();
    testToString();
    std::cout << "SUCCESS\n";
    return 0;
}