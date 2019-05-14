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
void cpu_ram_read(struct cpu *cpu) { cpu->MDR = cpu->ram[cpu->MAR]; }

void cpu_ram_write(struct cpu *cpu) { cpu->ram[cpu->MAR] = cpu->MDR; }

void cpu_run(struct cpu *cpu) {
  int running = 1; // True until we get a HLT instruction
  unsigned char operandA;
  unsigned char operandB;
  while (running) {
    cpu->IR = cpu->ram[cpu->PC];
    if (cpu->IR >= 64) {
      cpu->MAR = cpu->PC + 1;
      cpu_ram_read(cpu);
      operandA = cpu->MDR;
      cpu->PC += 1;
    } else {
      cpu->PC++;
    }
    if (cpu->IR >= 128) {
      cpu->MAR = cpu->PC + 1;
      cpu_ram_read(cpu);
      operandB = cpu->MDR;
      cpu->PC += 1;
    }
    switch (cpu->IR) {
    case LDI:
      cpu->registers[operandA] = operandB;
      cpu->PC++;
      break;
    case PRN:
      printf("%d", cpu->registers[operandA]);
      cpu->PC++;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      cpu->PC++;
      break;
    case HLT:
      running = 0;
      break;
    }

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this
    // instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu) {
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xF4;
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->MAR = 0;
  cpu->MDR = 0;
  cpu->FL = 0;
}
