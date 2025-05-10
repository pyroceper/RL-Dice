#ifndef DICE_HPP
#define DICE_HPP

#define BOOST_REGEX_STANDALONE
#include <boost/regex.hpp>
#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <numeric>


namespace rldice {

    class Dice 
    {
        public:
            // delete copy constructor and copy assignment operator
            Dice(const Dice&) = delete;
            Dice& operator=(const Dice&) = delete;

            // default move constructor and move assignment operator
            Dice(Dice&&) noexcept = default;
            Dice& operator=(Dice&&) noexcept = default;

            explicit Dice(unsigned int faces, unsigned int minimum = 1);
            Dice(const std::string& dice_notation, unsigned int minimum = 1);

            // callback type for RNG (min inclusive, max inclusive)
            using RandFunc = std::function<int(int min, int max)>;

            // register a custom RNG function
            static void registerRNG(RandFunc func);

            const unsigned int getNum() const;
            const unsigned int getFaces() const;
            const unsigned int getMin() const;
            const unsigned int getSets() const;
            const int getBonus() const;
            const int getRerolls() const;
            const std::string toString();

            void setNum(int num);
            void setMin(int minimum);
            void setFaces(int faces);
            void setSets(int sets);
            void setBonus(int bonus);
            void setRerolls(int rerolls);
            
            Dice operator+(int bonus);
            Dice operator-(int bonus);
            Dice operator^(int reroll);
            Dice operator*(int num);
            Dice operator/(int faces);
            Dice operator%(int sets);
        
            const std::vector<int> roll(int minimum = 1) const;

        private:
            std::string notation_;
            unsigned int minimum_;

            unsigned int num_;
            unsigned int faces_;
            unsigned int sets_;

            // can be negative
            int bonus_; 
            int reroll_;

            bool is_bonus_plural_;
            bool is_reroll_plural_;
            bool is_valid_;

            // private constructor for internal use
            explicit Dice(unsigned int minimum, bool internal);

            // static member to hold RNG function
            static RandFunc rand_func_;

            static int defaultRNG(int min, int max);

            bool parseNotation(const std::string& notation, bool just_verify = false);
    };

}


#endif