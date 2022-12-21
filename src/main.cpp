// use c with class
#include <fmt/format.h>

extern "C" {
#include <unistd.h>
}

#include <cassert>

#include "memalloc.h"

int main(int argc, char* argv[]) {
    auto start = brk(0);
    auto a     = mem_alloc(sizeof(int) * 4);
    auto b     = mem_alloc(sizeof(int) * 4);
    auto c     = mem_alloc(sizeof(int) * 4);
    mem_free(a);
    mem_free(b);
    auto p = brk(0);
    mem_free(c);
    auto end = brk(0);
    assert(start != p);
    assert(start == end);
    return 0;
}
