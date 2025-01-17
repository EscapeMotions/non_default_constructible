![workflow](https://github.com/EscapeMotions/non_default_constructible/actions/workflows/cmake-multi-platform.yml/badge.svg)

A header-only c++ wrapper class for disabling a default constructor of any wrapped type. It's especially useful for initializing members of a struct via an initializer list when you want to make sure that a user of your struct properly initialized all its members.

# Motivation example

Having a struct with parameters for a function:
```cpp
struct SomeArgs {
    int i;
    std::string s;
    std::function<void()> callback;
};
void f(const SomeArgs& args);
...
f({.i=123, .s="hello"}); // callback is not initialized. Maybe the callback member was added to the struct later.
```

If you want to force the caller of `f()` to always set `callback` and `s` explicitly, just wrap them in `NonDefaultConstructible`:
```cpp
#include "non_default_constructible.h"

struct SomeArgs {
    int i;
    NonDefaultConstructible<std::string> s;
    NonDefaultConstructible<std::function<void()>> callback;
};
void f(const SomeArgs& args) {
    // args values available via args.i, args.s.get(), args.callback.get()
    // callback can be called via args.callback.get()() or simply by args.callback()
}
...
f({.i=123, .s="hello"}); // compile error: callback is not initialized
f({.i=123}); // compile error: s and callback are not initialized
f({.i=123, .s="hello", .callback=[]{std::cout << "heeey";}}); // OK
f({.s="hello", .callback=[]{std::cout << "heeey";}}); // OK
```

To use the same access pattern for all members whether they are wrapped in `NonDefaultConstructible` or not, you can wrap all other members in `DefaultConstructible` which allows default construction and just adds the `get()` member function.

# Building and running tests

```
cmake -S . -B build
cmake --build build
cd build
ctest
```
