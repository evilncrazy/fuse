#include "fuse/fuse.hpp"

#define STATIC_TEST(name, test) static_assert(test, "Test Failed: " name)

#include "core_test.hpp"

int main() {
  core_test();
}
