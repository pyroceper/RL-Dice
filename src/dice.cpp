#include "dice.hpp"

namespace rldice {

    Dice::RandFunc Dice::rand_func_ = Dice::defaultRNG;

    Dice::Dice(unsigned int faces, unsigned int minimum) : Dice(("1d" + std::to_string(faces)), minimum)
    {
    }   
    
    Dice::Dice(const std::string& dice_notation, unsigned int minimum) : notation_(dice_notation), minimum_(minimum)
    {
        is_valid_ = parseNotation(notation_);
    }

    Dice::Dice(unsigned int minimum, bool internal) : minimum_(minimum), notation_("")
    {
    }
    
    bool Dice::parseNotation(const std::string& notation, bool just_verify)
    {
        // dice patterns
        boost::regex dice_pattern(R"(^\(?(\d*)d(\d+)([+-]{1,2}\d+)?(\^[+-]{1,2}\d+)?\)?(x\d+)?$)");
        boost::smatch matches;

        if(boost::regex_match(notation_, matches, dice_pattern)) {
            if(just_verify)
                return true;

            num_ = std::stoi(matches[1].str());
            faces_ = std::stoi(matches[2].str());

            // parse bonus
            if(matches[3].matched) {
                std::string bonus = matches[3].str();
                is_bonus_plural_ = (bonus[1] == '+' || bonus[1] == '-'); // check for '++' and '--'
                bonus_ = std::stoi(bonus);
            }
            else {
                bonus_ = 0;
            }

            // parse rerolls
            if(matches[4].matched) {
                std::string reroll = matches[4].str().substr(1); // remove '^'
                is_reroll_plural_ = (reroll.size() > 1 && (reroll[1] == '+' || reroll[1] == '-')); // check "+1" or "--2"
                reroll_ = std::stoi(reroll);
            }
            else {
                reroll_ = 0;
            }

            reroll_ = matches[4].matched ? std::stoi(matches[4].str().substr(1)) : 0; // remove '^'
            sets_ = matches[5].matched ? std::stoi(matches[5].str().substr(1)) : 1; // remove 'x'
            return true;
        }
        return false;
    }

    const unsigned int Dice::getNum() const 
    {
        return num_;
    }

    const unsigned int Dice::getFaces() const 
    {
        return faces_;
    }

    const unsigned int Dice::getMin() const
    {
        return minimum_;
    }

    const unsigned int Dice::getSets() const
    {
        return sets_;
    }

    const int Dice::getRerolls() const
    {
        return reroll_;
    }

    const int Dice::getBonus() const
    {
        return bonus_;
    }

    const std::string Dice::toString()
    {
        if(notation_.empty()) {
            // get num
            // get faces
            // get bonus
            // get reroll
            // get sets
            // example (3d4+2^+1)x3
            std::string new_notation = std::to_string(num_) + "d" + std::to_string(faces_);
            if(bonus_ > 0)
                new_notation += "+" + std::to_string(bonus_);
            else if(bonus_ < 0)
                new_notation += std::to_string(bonus_);
            
            if(reroll_ >= 1)
                new_notation += "^+" + std::to_string(reroll_);
            else if(reroll_ < 0)
                new_notation += "^" + std::to_string(reroll_);
            
            if(sets_ > 1)
                new_notation = "(" + new_notation + ")x" + std::to_string(sets_);
            
            notation_ = new_notation;

            is_valid_ = parseNotation(notation_, true);
            if(!is_valid_) {
                std::stringstream err;
                err << "Parser Error: Invalid dice notation: " << notation_;
                throw std::invalid_argument(err.str());
            }
        }
        return notation_;
    }

    void Dice::setMin(int minimum)
    {
        if(minimum < 0)
            return;
        minimum_ = minimum;
    }

    void Dice::setNum(int num)
    {
        if(num < 0)
            return;
        num_ = num;
    }

    void Dice::setFaces(int faces)
    {
        if(faces < 0)
            return;
        faces_ = faces;
    }


    void Dice::setSets(int sets)
    {
        if(sets < 0)
            return;
        sets_ = sets;
    }

    void Dice::setBonus(int bonus)
    {
        bonus_ = bonus;
    }

    void Dice::setRerolls(int reroll)
    {
        reroll_ = reroll;
    }

    void Dice::registerRNG(RandFunc func)
    {
        rand_func_ = func;
    }

    // default RNG
    int Dice::defaultRNG(int min, int max)
    {
        return 1;
    }

    const std::vector<int> Dice::roll(int minimum) const
    {
        if(!is_valid_) {
            std::stringstream err;
            err << "Roll Error: Invalid dice notation: " << notation_;
            throw std::invalid_argument(err.str());
        }

        std::vector<int> set_totals;

        for(int set = 0;set < sets_; set++) {
            std::vector<int> rolls;
            int total_rolls = num_ + std::abs(reroll_);
            for(int i = 0;i < total_rolls; i++) {
                int roll = rand_func_(1, faces_);
                if(is_bonus_plural_) 
                    roll += bonus_; // apply to each die
                rolls.push_back(roll);
            }

            if(reroll_ != 0) {
                if(reroll_ > 0) {
                    std::sort(rolls.begin(), rolls.end(), std::greater<int>());
                } else {
                    std::sort(rolls.begin(), rolls.end());
                }
                rolls.resize(num_); // keep only best/worst rolls
            }
                          
            // apply single bonus (if not plural)
            if(!is_bonus_plural_ && bonus_ != 0)
                rolls.back() += bonus_;

            // calculate total for set
            int total = std::accumulate(rolls.begin(), rolls.end(), 0);
            total = std::max(total, static_cast<int>(minimum_));
            set_totals.push_back(total);
        }
        if(minimum > 1 || minimum_ > 1) {
            for(int i = 0;i < sets_;i++) {
                set_totals[i] = std::max(set_totals[i], minimum);
            }
        }
        return set_totals;
    }

    Dice Dice::operator+(int bonus)
    {
        // set num
        // set faces
        // set bonus
        // set reroll
        // set sets
        Dice dice = Dice(minimum_, true);
        dice.setNum(num_);
        dice.setFaces(faces_);
        dice.setBonus(bonus_ + bonus);
        dice.setRerolls(reroll_);
        dice.setSets(sets_);
        
        return dice;
    }

    Dice Dice::operator-(int bonus)
    {
        // set num
        // set faces
        // set bonus
        // set reroll
        // set sets
        Dice dice = Dice(minimum_, true);
        dice.setNum(num_);
        dice.setFaces(faces_);
        dice.setBonus(bonus_ - bonus);
        dice.setRerolls(reroll_);
        dice.setSets(sets_);
        
        return dice;
    }


    Dice Dice::operator^(int reroll)
    {
        // set num
        // set faces
        // set bonus
        // set reroll
        // set sets
        Dice dice = Dice(minimum_, true);
        dice.setNum(num_);
        dice.setFaces(faces_);
        dice.setBonus(bonus_);
        dice.setRerolls(reroll_ + reroll);
        dice.setSets(sets_);      

        return dice;
    }

    Dice Dice::operator*(int num)
    {
        num_ += num;
        if(num_ <= 0) {
            num_ -= num;
            return std::move(*this);
        }
        // set num
        // set faces
        // set bonus
        // set reroll
        // set sets
        Dice dice = Dice(minimum_, true);
        dice.setNum(num_);
        dice.setFaces(faces_);
        dice.setBonus(bonus_);
        dice.setRerolls(reroll_);
        dice.setSets(sets_);      

        return dice;
    }

    Dice Dice::operator/(int faces)
    {
        if(faces == 0) 
            return std::move(*this);
        // set num
        // set faces
        // set bonus
        // set reroll
        // set sets
        Dice dice = Dice(minimum_, true);
        dice.setNum(num_);
        dice.setFaces(faces_ + faces);
        dice.setBonus(bonus_);
        dice.setRerolls(reroll_);
        dice.setSets(sets_);      

        return dice;
    }

    Dice Dice::operator%(int sets)
    {
        sets_ += sets;
        if(sets_ <= 0) {
            sets_ -= sets;
            return std::move(*this);
        }
        // set num
        // set faces
        // set bonus
        // set reroll
        // set sets
        Dice dice = Dice(minimum_, true);
        dice.setNum(num_);
        dice.setFaces(faces_);
        dice.setBonus(bonus_);
        dice.setRerolls(reroll_);
        dice.setSets(sets_); 

        return dice;
    }
}