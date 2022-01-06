#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 8

typedef enum Boolean { false, true } bool;

enum Op {
  NOP = 0x00,

  STA = 0x10,
  LDA = 0x20,
  ADD = 0x30,
  OR = 0x40,
  AND = 0x50,
  NOT = 0x60,
  SUB = 0x70,
  JMP = 0x80,

  JN = 0x90,
  JP = 0x94,
  JV = 0x98,

  JNV = 0x9C,

  JZ = 0xA0,
  JNZ = 0xA4,

  JC = 0xB0,
  JNC = 0xB4,
  JB = 0xB8,

  JNB = 0xBC,

  SHR = 0xE0,
  SHL = 0xE1,
  ROR = 0xE2,
  ROL = 0xE3,

  HLT = 0xF0
};

char *getOpName(int op) {
  switch (op) {
    case NOP:
      return "NOP";
    case STA:
      return "STA";
    case LDA:
      return "LDA";
    case ADD:
      return "ADD";
    case OR:
      return "OR";
    case AND:
      return "AND";
    case NOT:
      return "NOT";
    case SUB:
      return "SUB";
    case JMP:
      return "JMP";
    case JN:
      return "JN";
    case JP:
      return "JP";
    case JV:
      return "JV";
    case JNV:
      return "JNV";
    case JZ:
      return "JZ";
    case JNZ:
      return "JNZ";
    case JC:
      return "JC";
    case JNC:
      return "JNC";
    case JB:
      return "JB";
    case JNB:
      return "JNB";
    case SHR:
      return "SHR";
    case SHL:
      return "SHL";
    case ROR:
      return "ROR";
    case ROL:
      return "ROL";
    case HLT:
      return "HLT";
  }

  return NULL;
}

int *memory, accumulator = 0, programCounter = 0, steps = 0;
bool halted = false;

// Increments the program counter and returns the value from its memory location
int next() {
  if (++programCounter >= MEMORY_SIZE) {
    printf("fatal: program counter out of bounds\n");
    exit(1);
  }

  return memory[programCounter];
}

// Advances the program counter and returns the value from its memory location
int nextStepAddress(char *opName) {
  int address = next();
  if (address < 0 || address > MEMORY_SIZE) {
    printf("%s: address %d out of bounds\n", opName, address);
    exit(1);
  }
  return address;
}

void step() {
  if (halted) {
    printf("warn: program halted\n");
    return;
  }

  if (programCounter > MEMORY_SIZE) {
    printf("fatal: program counter out of bounds\n");
    exit(1);
  }

  steps++;

  int op = memory[programCounter];

  switch (op) {
    case NOP:
      break;
    case STA:
      memory[nextStepAddress("STA")] = accumulator;
      break;
    case LDA:
      accumulator = memory[nextStepAddress("LDA")];
      break;
    case ADD:
      accumulator += memory[next()];
      break;
    case OR:
      break;
    case AND:
      break;
    case NOT:
      break;
    case SUB:
      accumulator -= memory[next()];
      break;
    case JMP:
      programCounter = nextStepAddress("JMP");
      return;  // don't increment program counter
    case JN:
      break;
    case JP:
      break;
    case JV:
      break;
    case JNV:
      break;
    case JZ:
      break;
    case JNZ:
      break;
    case JC:
      break;
    case JNC:
      break;
    case JB:
      break;
    case JNB:
      break;
    case SHR:
      break;
    case SHL:
      break;
    case ROR:
      break;
    case ROL:
      break;
    case HLT:
      halted = true;
      break;
  }

  programCounter++;
}

bool consumesNext(int op) {
  return op == STA || op == LDA || op == ADD || op == OR || op == AND ||
         op == SUB || op == JMP || op == JN || op == JP || op == JV ||
         op == JNV || op == JZ || op == JNZ || op == JC || op == JNC ||
         op == JB || op == JNB;
}

void printMemory() {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    int op = memory[i];
    char *opName = getOpName(op);

    if (i + 1 < MEMORY_SIZE && consumesNext(op)) {
      int nextOp = memory[++i];

      printf("%02d: %d (%s)\n", i - 1, op, opName);
      printf("%02d: ~ %d\n", i, nextOp);
    } else {
      if (opName) {
        printf("%02d: %d (%s)\n", i, op, opName);
      } else {
        printf("%02d: %d\n", i, op);
      }
    }
  }

  printf("\n");
}

void main() {
  memory = (int *)calloc(MEMORY_SIZE, sizeof(int));

  accumulator = 30;

  memory[0] = STA;
  memory[1] = 5;
  memory[2] = LDA;
  memory[3] = 7;
  memory[4] = HLT;

  memory[7] = 4;

  while (!halted) {
    step();
  }

  printMemory();

  printf("accumulator: %d\n", accumulator);
  printf("program counter: %d\n", programCounter);
  printf("steps: %d\n", steps);
}
