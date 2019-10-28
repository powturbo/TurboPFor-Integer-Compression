/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#ifndef VTENC_MEM_H_
#define VTENC_MEM_H_

#include <stdint.h>

uint16_t mem_read_le_u16(const void* mem_ptr);
uint32_t mem_read_le_u32(const void* mem_ptr);
uint64_t mem_read_le_u64(const void* mem_ptr);

void mem_write_le_u16(void* mem_ptr, uint16_t value);
void mem_write_le_u32(void* mem_ptr, uint32_t value);
void mem_write_le_u64(void* mem_ptr, uint64_t value);

#endif /* VTENC_MEM_H_ */
