#include "non_default_constructible.h"

struct S {
    const NonDefaultConstructible<std::string> first;
    const std::string last;
};

int main(int argc, char** argv) {
    S s{.last{"last"}}; // should fail because first isn't initialized
}