#ifndef CAKE_H
#define CAKE_H

#include<type_traits>
#include<cassert>

namespace cake_helper_functions {
    static constexpr double ln_2() {
        const int CALC_LN_2_ITERATIONS = 4000;
        
        double res = 0.0;
        for(long long i = 0; i <= CALC_LN_2_ITERATIONS; i++) {
            if(i % 2 == 0){
                res += 1.0/((i + 1)*(i + 2));
            }
            else {
                res -= 1.0/((i + 1)*(i + 2));
            }
        }   
        return (res + 1)/2;
    }  
}

template<typename T, T length, T width, typename... Others>
class Cake {};

template<typename T, T length, T width>
class Cake<T, length, width> {
static_assert(std::is_integral<T>::value, "T has to be an integral type");

protected:
    int stock_;

public:
    explicit Cake() = default;

    explicit Cake(int initialStock) : stock_(initialStock) {
        assert(initialStock > 0);
    }
    
    int getStock() const {
        return stock_;
    }
    
    constexpr static double getArea() {
        return length * width * cake_helper_functions::ln_2();
    }
};


template<typename T, T length, T width, typename P>
class Cake<T, length, width, P> : public Cake<T, length, width> {
static_assert(std::is_floating_point<P>::value, "P has to be a floating point type");

private:
    P price_;

public:
    explicit Cake() = default;
    
    explicit Cake(int initialStock, P price) : Cake<T, length, width>(initialStock), price_(price) {}
    
    void sell() {
        assert((Cake<T, length, width>::stock_ > 0));
        Cake<T, length, width>::stock_--;
    }
    
    P getPrice() const {
        return price_;
    }
    
    constexpr static double getArea() {
        return Cake<T, length, width>::getArea();
    }
};
template<typename T, T length, T width>
using CheeseCake = Cake<T, length, width>;

template<typename T, T length, T width, typename P>
using CreamCake = Cake<T, length, width, P>;

#endif /* CAKE_H */
