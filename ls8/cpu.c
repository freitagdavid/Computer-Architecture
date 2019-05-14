#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int num_args, char *file_name) {
  if (num_args != 2) {
    printf("usage: ls8 <rom_file>.ls8");
    exit(1);
  }
  FILE *file = fopen(file_name, "r");
  char *c;

  while (1) {
    fgets(c, sizeof(char) * 30, file);
    if (feof(file)) {
      break;
    }
    char *endptr;
    cpu->ram[cpu->PC] = (unsigned char)strtol(c, endptr, 2);
    cpu->PC++;
  }
  cpu->PC = 0;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA,
         unsigned char regB) {
  switch (op) {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;
  }
}

/**
 * Run the CPU
 */
int cpu_ram_read(struct cpu *cpu, int source) { 
  return cpu->ram[source];
  }

void cpu_ram_write(struct cpu *cpu, int destination, int source) { cpu->ram[destination] = source; }

void cpu_run(struct cpu *cpu) {
  int running = 1; // True until we get a HLT instruction
  unsigned char operandA;
  unsigned char operandB;

  while (running) {
    cpu->IR = cpu->ram[cpu->PC];
    int new_pc = ((cpu->IR >> 6) & 0b11) + 1;
    if (cpu->IR >= 64) {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
    } else {
    }
    if (cpu->IR >= 128) {
      operandB = cpu_ram_read(cpu, cpu->PC + 2);
    }
    cpu->PC += new_pc;
    switch (cpu->IR) {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d", cpu->registers[operandA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
      cpu->SP--;
      cpu->ram[cpu->SP] = cpu->registers[operandA];
    case HLT:
      running = 0;
      break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu) {
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->registers, 0, 8 * sizeof(unsigned char));
  cpu->registers[7] = 0xF4;
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->MAR = 0;
  cpu->MDR = 0;
  cpu->FL = 0;
  cpu->SP = 0xF4;
}
