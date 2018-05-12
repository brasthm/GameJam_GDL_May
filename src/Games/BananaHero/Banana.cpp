#include <random>
#include "Banana.hpp"


Banana::Banana(Banana::type_t type)
{
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    
    
}
