#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define true  1
#define false 0

const uint8_t NOP = 0xEA;
const uint8_t LDA = 0xA9;

const uint32_t MEMORY_SIZE = 0x10000;
const uint16_t PAGE_SIZE   = 0xFF;

typedef struct CPU6502 {
  uint16_t PC;                  // Prgoram Counter
  uint8_t  AC;                  // Accumulator
  uint8_t  XR;                  // X Register
  uint8_t  YR;                  // Y Register
  uint8_t  SR;                  // Status Register
  uint8_t  SP;                  // Stack Pointer
} CPU;

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

uint8_t read_mem(uint16_t addr, uint8_t *memory) {
  return *(memory + addr);
}

uint8_t write_mem(uint16_t addr, uint8_t data, uint8_t *memory) {
  // TODO: return some kind of status
  *(memory + addr) = data;
  return *(memory + addr);
}

void setup_memeory(uint8_t *memory) {

  // Reset vector
  write_mem(0xFFFC, 0x00, memory);
  write_mem(0xFFFD, 0x80, memory);

  // our program
  write_mem(0x8000, LDA, memory);
  write_mem(0x8001, 0x42, memory);
  write_mem(0x8002, LDA, memory);
  write_mem(0x8003, 0x41, memory);
  write_mem(0x8004, NOP, memory);
  write_mem(0x8005, LDA, memory);
  write_mem(0x8006, 0x44, memory);
  write_mem(0x8007, NOP, memory);
}

// fetch one byte
// current PC address value
uint8_t fetch_byte(CPU *cpu, uint8_t *memory) {
  uint8_t data = read_mem(cpu->PC, memory);
  cpu->PC++;
  return data;
}

void reset_cpu(CPU *cpu, uint8_t *memory) {

  // perform the reset sequence
  uint8_t lo = read_mem(0xFFFC, memory);
  uint8_t hi = read_mem(0xFFFD, memory);

  // little endian to big endian
  cpu->PC = (hi << 8) | lo;

  cpu->AC = 0x00;
  cpu->XR = 0x00;
  cpu->YR = 0x00;
  cpu->SR = 0x00;
  cpu->SP = 0x00;
}

void cpu_step(CPU *cpu, uint8_t *memory) {
  // read the next step
  uint8_t opcode = fetch_byte(cpu, memory);
  switch (opcode) {
    case NOP:
    {
      break;
    }
    case LDA:
    {
      uint8_t data = fetch_byte(cpu, memory);
      cpu->AC = data;
      break;
    }
  }
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
