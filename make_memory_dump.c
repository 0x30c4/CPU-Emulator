#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.c"

// memory_size is the size of the full memory
// the block size is the size of each memory block
// and for this case its 8 bit block size
void create_memory_layout(uint32_t memory_size, uint8_t *memory) {

  // Reset vector
  write_mem(0xFFFC, 0x00, memory);
  write_mem(0xFFFD, 0x80, memory);

  // our program
  write_mem(0x8000, JMP, memory);
  write_mem(0x8001, 0x00, memory);
  write_mem(0x8002, 0x81, memory);

  write_mem(0x8100, LDA, memory);
  write_mem(0x8101, 0x42, memory);
  write_mem(0x8102, JMP, memory);
  write_mem(0x8103, 0x00, memory);
  write_mem(0x8104, 0x80, memory);
}

int main(int argc, char *argv[]){

  if (argc < 2) {
    printf("usage: %s path/to/memory.bin \n", argv[0]);
    printf("need a memory dump file\n");
    return 1;
  }

  uint8_t *memory = calloc(MEMORY_SIZE, sizeof(uint8_t));

  if (memory == NULL) {
    printf("Memory allocation failed\n");
    return 1;
  }

  create_memory_layout(MEMORY_SIZE, memory);

  dump_memory(argv[1], memory, MEMORY_SIZE);

  return 0;
}
