#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <memory>


template<typename Base, typename... Args>
struct instanciator_base 
{
    virtual std::unique_ptr<Base> operator()(Args&&... args) = 0;
};

template<typename Base, typename Derived, typename... Args>
struct instanciator : instanciator_base<Base, Args...>
{
    virtual std::unique_ptr<Base> operator()(Args&&... args) {
        static_assert(std::is_base_of_v<Base, Derived>, "Not a derived class");
        return std::unique_ptr<Base>(new Derived(args...)); 
    }
};

#endif //UTILITIES_HPP
