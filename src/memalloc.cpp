#include "memalloc.h"

#include <mutex>

extern "C" {
#include <string.h>
#include <unistd.h>
}

std::mutex global_mem_lock;

struct alignas(16) header_t {
    size_t    size    = 0;
    bool      is_free = true;
    header_t* next    = nullptr;
};

header_t* mem_header = nullptr;

header_t* get_free_block(size_t size) {
    auto curr = mem_header;
    while(curr) {
        if(curr->is_free && curr->size >= size) {
            return curr;
        }

        curr = curr->next;
    }

    return nullptr;
}

void* mem_alloc(size_t size) {
    if(!size) return nullptr;

    std::unique_lock<std::mutex> lock(global_mem_lock);

    auto header = get_free_block(size);
    if(header) {
        header->is_free = false;
        return (void*)(header + 1);
    }

    header = (header_t*)sbrk(sizeof(header_t) + size);
    if(header == (void*)-1) {
        return nullptr;
    }

    header->size    = size;
    header->is_free = false;
    header->next    = nullptr;
    if(!mem_header) {
        mem_header = header;
    }

    return (void*)(header + 1);
}

void mem_free(void* block) {
    if(!block) return;
    std::unique_lock<std::mutex> lock(global_mem_lock);

    auto  header  = (header_t*)block - 1;
    void* brk_pos = sbrk(0);
    if((char*)block + header->size < brk_pos) {
        header->is_free = 1;
        return;
    }
    // 释放的位置位于 brk 位置，进行实际释放动作
    if(!mem_header->next) {    // 释放的是最后一个节点
        mem_header = nullptr;
    } else {    // 释放的只是尾节点
        auto parent = mem_header;
        while(parent && (parent->next != header)) {
            parent = parent->next;
        }

        parent->next = nullptr;
    }

    sbrk(-sizeof(header_t) - header->size);
}

void* mem_calloc(size_t num, size_t nsize) {
    if(!num || !nsize) return nullptr;

    auto block = mem_alloc(num * nsize);
    bzero(block, num * nsize);
    return block;
}

void* mem_realloc(void* block, size_t size) {
    if(!block) {
        return mem_alloc(size);
    }
    if(!size) {
        mem_free(block);
        return nullptr;
    }

    auto header = (header_t*)block - 1;
    if(header->size >= size) return block;

    auto new_block = mem_alloc(size);
    if(new_block) {
        memcpy(new_block, block, size);
    }

    mem_free(block);
    return new_block;
}
