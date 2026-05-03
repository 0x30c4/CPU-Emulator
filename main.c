#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.c"

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

  load_memory(memory_dump_file, memory, MEMORY_SIZE);

  reset_cpu(&cpu, memory);

  while (true) {
    if (clock == 10) {
      break;
    }

    sleep(1);
    cpu_step(&cpu, memory);

    print_cpu_debug_log(&cpu, clock);

    clock++;
  }

  free(memory);
  return 0;
}
