#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cpu_utils.c"

#define true  1
#define false 0


const uint32_t MEMORY_SIZE = 0x10000;

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

void setup_memeory(uint8_t *memory) {

  // Reset vector
  write_mem(0xFFFC, 0x00, memory);
  write_mem(0xFFFD, 0x80, memory);

  // our program
  write_mem(0x8000, LDA, memory);
  write_mem(0x8001, 0x42, memory);
  write_mem(0x8002, JMP, memory);
  write_mem(0x8003, 0x00, memory);
  write_mem(0x8004, 0x80, memory);
}

int main(int argc, char *argv[]) {

  // TODO: fix the clock counter logic
  // at the start 7 empty cycle reset sequence
  unsigned long long int clock = 0;
  struct CPU6502 cpu;
  uint8_t *memory = calloc(MEMORY_SIZE, sizeof(uint8_t));

  if (memory == NULL) {
    printf("Memory allocation failed\n");
    return 1;
  }

  setup_memeory(memory);
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
