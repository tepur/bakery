#ifndef PIE_H
#define PIE_H

#include <type_traits>
#include <cassert>

namespace pie_helper_functions {
    static constexpr double pi() {
        const int CALC_PI_ITERATIONS = 300;

        double res = 3.0;
        for(long long i = 1; i <= CALC_PI_ITERATIONS; i++) {
            if(i % 2 == 1) {
                res += 4.0/(2*i*(2*i + 1)*(2*i + 2));
            }
            else {
                res -= 4.0/(2*i*(2*i + 1)*(2*i + 2));
            }
        }
        return res;
    }
}

template<typename R, R radius, typename... Others>
class Pie {};

template<typename R, R radius>
class Pie<R, radius> {
static_assert(std::is_integral<R>::value, "R has to be an integral type");

protected:
    int stock_;

public:
    explicit Pie() = default;
    
    explicit Pie(int initialStock) : stock_(initialStock) {
        assert(initialStock > 0);
    }
    
    int getStock() const {
        return stock_;
    }
    
    constexpr static double getArea() {
        return radius * radius * pie_helper_functions::pi();
    }
};

template<typename R, R radius, typename P>
class Pie<R, radius, P> : public Pie<R, radius> {
static_assert(std::is_floating_point<P>::value, "P has to be a floating point type");

private:
    P price_;

public:
    explicit Pie() = default;
    
    explicit Pie(int initialStock, P price) : Pie<R, radius>(initialStock), price_(price) {}
    
    void sell() {
        assert((Pie<R, radius>::stock_ > 0));
        Pie<R, radius>::stock_--;
    }
    
    P getPrice() const {
        return price_;
    }
    
    constexpr static double getArea() {
        return Pie<R, radius>::getArea();
    }
    
    void restock(int additionalStock) {
        Pie<R, radius>::stock_ += additionalStock;
    }
};

template<typename R, R radius>
using CherryPie = Pie<R, radius>;

template<typename R, R radius, typename P>
using ApplePie = Pie<R, radius, P>;

#endif /* PIE_H */
