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
void cpu_ram_read(struct cpu *cpu) { cpu->MDR = cpu->ram[(int)cpu->MAR]; }

void cpu_ram_write(struct cpu *cpu, int address, unsigned char write_data) {
  cpu->ram[address] = cpu->MDR;
}

void cpu_register_write(struct cpu *cpu, unsigned char *destination,
                        unsigned char *source) {
  cpu->MAR = source;
  cpu_ram_read(cpu);
  cpu->registers[(int)destination] = cpu->MDR;
}

void cpu_run(struct cpu *cpu) {
  int running = 1; // True until we get a HLT instruction
  unsigned char *operandA;
  unsigned char *operandB;
  while (running) {
    cpu->IR = cpu->ram[(int)cpu->PC];
    printf("cpu->PC: %u \n", (unsigned char)cpu->PC);
    printf("cpu->IR: %u \n", (unsigned char)cpu->IR);
    // memcpy(cpu->IR, cpu->PC, 1);

    if ((int)cpu->IR >= 64) {
      operandA = cpu->ram[(unsigned char)cpu->IR + 1];
      cpu->PC += 2;
    } else {
      cpu->PC++;
    }
    printf("cpu->PC: %u \n", (unsigned char)cpu->PC);
    printf("cpu->IR: %u \n", (unsigned char)cpu->IR);
    if ((int)cpu->IR >= 128) {
      operandB = cpu->IR + 2;
      cpu->PC++;
    }
    printf("cpu->PC: %u \n", (unsigned char)cpu->PC);
    printf("cpu->IR: %u \n", (unsigned char)cpu->IR);
    switch ((unsigned char)cpu->IR) {
    case LDI:
      printf("OperandA: %d \n", *operandA);
      printf("OperandB: %u \n", operandB);
      cpu_register_write(cpu, operandA, operandB);
      printf("cpu->registers[0]: %u", cpu->registers[0]);
      exit(0);
      break;
    case HLT:
      exit(0);
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
  cpu = (struct cpu *)malloc(sizeof(struct cpu));
  cpu->PC = malloc(sizeof(unsigned char));
  cpu->IR = malloc(sizeof(unsigned char));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xF4;
  cpu->PC = 0;
  cpu->FL = 0;
  cpu->IR = 0;
  cpu->MAR = 0;
  cpu->MDR = 0;
}
