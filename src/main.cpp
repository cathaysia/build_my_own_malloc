#include <fmt/format.h>

int main(int argc, char *argv[]) {
    fmt::print("{}: hello, world!", __LINE__);
    return 0;
}
