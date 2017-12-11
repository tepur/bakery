#ifndef BAKERY_H
#define BAKERY_H

#include <type_traits>
#include <tuple>
#include "cake.h"
#include "pie.h"

namespace bakery_traits {
    template<typename ...T>
    struct is_one_of {};

    template<typename T1>
    struct is_one_of<T1> {
        static constexpr bool value = false;
    };

    template<typename T1, typename T2, typename... T>
    struct is_one_of<T1, T2, T...> {
        static constexpr bool value = std::is_same<T1, T2>::value 
                                      || is_one_of<T1, T...>::value;
    };


    template<typename ...T>
    struct are_unique {};

    template<>
    struct are_unique<> {
        static constexpr bool value = true;
    };

    template<typename F, typename... T>
    struct are_unique<F, T...> {
        static constexpr bool value = are_unique<T...>::value 
                                      && !is_one_of<F, T...>::value;
    };


    template<typename... T>
    struct sum_area {};

    template<>
    struct sum_area<> {
        static constexpr double value = 0;
    };

    template<typename F, typename... T>
    struct sum_area<F, T...> {
        static constexpr double value = F::getArea() + sum_area<T...>::value;
    };


    template<typename T>
    struct has_price {
        static constexpr bool value = false;
    };

    template<typename T, T length, T width, typename P>
    struct has_price<CreamCake<T, length, width, P>> {
        static constexpr bool value = true;
    };

    template<typename R, R radius, typename P>
    struct has_price<ApplePie<R, radius, P>> {
        static constexpr bool value = true;
    };


    template<typename T>
    struct price_type {
        using type = void;
    };

    template<typename T, T length, T width, typename P>
    struct price_type<CreamCake<T, length, width, P>> {
        using type = P;
    };

    template<typename R, R radius, typename P>
    struct price_type<ApplePie<R, radius, P>> {
        using type = P;
    };


    template<typename C, typename... T>
    struct same_price_type {};

    template<typename C>
    struct same_price_type<C> {
        static constexpr bool value = true;
    };

    template<typename C, typename F, typename... T>
    struct same_price_type<C, F, T...> {
        static constexpr bool value = has_price<F>::value ? 
                                      (std::is_same<C, typename price_type<F>::type>::value 
                                       && same_price_type<C, T...>::value) 
                                      : same_price_type<C, T...>::value;
    };


    template<typename T>
    struct measurement_type {};

    template<typename T, T length, T width, typename... Others>
    struct measurement_type<Cake<T, length, width, Others...>> {
        using type = T;
    };

    template<typename R, R radius, typename... Others>
    struct measurement_type<Pie<R, radius, Others...>> {
        using type = R;
    };


    template<typename A, typename... T>
    struct same_measurement_type {};

    template<typename A>
    struct same_measurement_type<A> {
        static constexpr bool value = true;
    };

    template<typename A, typename F, typename... T>
    struct same_measurement_type<A, F, T...> {
        static constexpr bool value = std::is_same<A, typename measurement_type<F>::type>::value 
                                      && same_measurement_type<A, T...>::value;
    };


    template<typename T>
    struct is_apple_pie {
        static constexpr bool value = false;
    };
    template<typename R, R radius, typename P>
    struct is_apple_pie<ApplePie<R, radius, P>> {
        static constexpr bool value = true;
    };
}

template <typename C, typename A, A shelfArea, typename... P>
class Bakery {
private:
    static_assert(std::is_floating_point<C>::value, "C has to be a floating point type");
    static_assert(std::is_integral<A>::value, "A has to be an integral type");
    static_assert(bakery_traits::are_unique<P...>::value, "types P... are not unique");
    static_assert(bakery_traits::sum_area<P...>::value <= shelfArea, "shelf area is too small");
    static_assert(bakery_traits::same_price_type<C, P...>::value, 
                  "at least 1 price type is diffrent from type C");
    static_assert(bakery_traits::same_measurement_type<A, P...>::value, 
                  "at least 1 measurement type is diffrent from type A");
    static_assert(sizeof...(P) > 0, "bakery has no products");

C profits_ = 0;
std::tuple<P...> bakings_;
   
public:
    explicit Bakery(P... products) {
        bakings_ = std::make_tuple(products...);
    }
    
    C getProfits() {
        return profits_;
    }
    
    template <class Product> 
    void sell() {
        static_assert(bakery_traits::is_one_of<Product, P...>::value, "no product with such type");
        static_assert(bakery_traits::has_price<Product>::value, "product cannot be sold");
    
        if(getProductStock<Product>() > 0) {
            std::get<Product>(bakings_).sell();
            profits_ += std::get<Product>(bakings_).getPrice();
        }
    }
    
    template <class Product> 
    int getProductStock() const {
        static_assert(bakery_traits::is_one_of<Product, P...>::value, "no product with such type");
        
        return std::get<Product>(bakings_).getStock();
    }
    
    template <class Product> 
    void restock(int additionalStock) {
        static_assert(bakery_traits::is_one_of<Product, P...>::value, "no product with such type");
        static_assert(bakery_traits::is_apple_pie<Product>::value, "product is not of type ApplePie");
        
        std::get<Product>(bakings_).restock(additionalStock);
    }
};

#endif /* BAKERY_H */
