#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6
typedef unsigned char *block;

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu) {
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000, 0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA,
         unsigned char regB) {
  switch (op) {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
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
      // cpu_register_write(cpu, operandA, operandB);
      break;
    case PRN:
      printf("%d", cpu->registers[operandA]);
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
  // cpu = (struct cpu *)malloc(sizeof(struct cpu));
  // cpu = calloc(0, sizeof(struct cpu));
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  cpu->registers[7] = 0xF4;
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->MAR = 0;
  cpu->MDR = 0;
  cpu->FL = 0;
}
