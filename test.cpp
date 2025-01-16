#include <iostream>
#include <string>
#include <catch2/catch_test_macros.hpp>
#include "non_default_constructible.h"

TEST_CASE("non_default_constructible", "[non_default_constructible]") {
    struct S {
        const NonDefaultConstructible<std::string> first;
        const NonDefaultConstructible<std::string> last;
        const DefaultConstructible   <std::string> middle;
        const NonDefaultConstructible<std::function<std::string()>> fn1;
        const NonDefaultConstructible<std::function<std::string(int)>> fn2;
        const DefaultConstructible   <std::function<int()>> fn3 = []{return 123;};
    };
    SECTION("default") {
        S s{
            .first{"first"},
            .last{"last"},
            .fn1{[]{return "fn1";}},
            .fn2{[](int i){return "fn2_"+std::to_string(i);}},
        };
        REQUIRE(s.first.get() == "first");
        REQUIRE(s.last.get() == "last");
        REQUIRE(s.middle.get() == "");
        REQUIRE(s.fn1() == "fn1");
        REQUIRE(s.fn2(123) == "fn2_123");
        REQUIRE(s.fn2.get()(123) == "fn2_123");
        REQUIRE(s.fn3() == 123);
    }
    SECTION("override_all") {
        S s{
            .first{"first"},
            .last{"last"},
            .middle{"middle"},
            .fn1{[]{return "fn1";}},
            .fn2{[](int i){return "fn2_"+std::to_string(i);}},
            .fn3{[](){return 456;}},
        };
        REQUIRE(s.first.get() == "first");
        REQUIRE(s.last.get() == "last");
        REQUIRE(s.middle.get() == "middle");
        REQUIRE(s.fn1() == "fn1");
        REQUIRE(s.fn2(123) == "fn2_123");
        REQUIRE(s.fn3() == 456);
    }
}
