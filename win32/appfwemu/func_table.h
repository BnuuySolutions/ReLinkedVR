#pragma once

#include <cinttypes>

#define FUNC_TABLE_CALC_SIZE(count) (sizeof(uint64_t) * 2) + (count * 8)

template <uint64_t Count>
struct FuncTable {
  uint64_t total_size;
  void* elements[Count];
  uint64_t unk_0;
};
