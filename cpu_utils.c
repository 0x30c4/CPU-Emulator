#include <stdint.h>
#include "memory_utils.c"

const uint8_t NOP = 0xEA;
const uint8_t LDA = 0xA9;
const uint8_t JMP = 0x4C;

typedef struct CPU6502 {
  uint16_t PC;                  // Prgoram Counter
  uint8_t  AC;                  // Accumulator
  uint8_t  XR;                  // X Register
  uint8_t  YR;                  // Y Register
  uint8_t  SR;                  // Status Register
  uint8_t  SP;                  // Stack Pointer
} CPU;

// fetch one byte
// current PC address value
uint8_t fetch_byte(CPU *cpu, uint8_t *memory) {
  uint8_t data = read_mem(cpu->PC, memory);
  cpu->PC++;
  return data;
}

uint16_t fetch_word(CPU *cpu, uint8_t *memory) {
  uint8_t lo = fetch_byte(cpu, memory);
  uint8_t hi = fetch_byte(cpu, memory);
  return (hi << 8) | lo;
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
    case JMP:
    {
      uint16_t addr = fetch_word(cpu, memory);
      cpu->PC = addr;
      break;
    }
  }
}
