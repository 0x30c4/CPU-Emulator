#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.c"

#define true  1
#define false 0

void print_cpu_debug_log(CPU *cpu, unsigned long long int clock) {
  printf("Clock: %llu\t", clock);
  printf("PC: 0x%x\t", cpu->PC);
  printf("AC: 0x%x\t", cpu->AC);
  printf("XR: 0x%x\t", cpu->XR);
  printf("YR: 0x%x\t", cpu->YR);
  printf("SR: 0x%x\t", cpu->SR);
  printf("SP: 0x%x\t", cpu->SP);
  printf("\n");
}
//
// memory_size is the size of the full memory
// the block size is the size of each memory block
// and for this case its 8 bit block size
void setup_memeory(uint32_t memory_size, const char *filename, uint8_t *memory) {
  load_memory(filename, memory, memory_size);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("usage: %s memory.bin\n", argv[0]);
    printf("need a memory dump file\n");
    return 1;
  }

  const char *memory_dump_file = argv[1];

  // TODO: fix the clock counter logic
  // at the start 7 empty cycle reset sequence
  unsigned long long int clock = 0;
  struct CPU6502 cpu;
  uint8_t *memory = calloc(MEMORY_SIZE, sizeof(uint8_t));

  if (memory == NULL) {
    printf("Memory allocation failed\n");
    return 1;
  }


  setup_memeory(MEMORY_SIZE, memory_dump_file, memory);

  reset_cpu(&cpu, memory);

  while (true) {
    if (clock == 10) {
      break;
    }

    cpu_step(&cpu, memory);

    print_cpu_debug_log(&cpu, clock);

    clock++;
  }

  free(memory);
  return 0;
}
