#include <stdint.h>

uint8_t read_mem(uint16_t addr, uint8_t *memory) {
  return *(memory + addr);
}

uint8_t write_mem(uint16_t addr, uint8_t data, uint8_t *memory) {
  // TODO: return some kind of status
  *(memory + addr) = data;
  return *(memory + addr);
}
