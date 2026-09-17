#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5

uint8_t mem[256] = {0};
uint8_t reg[4] = {0};
unsigned int pc = 0;
int zf = 0;
int running = 1;
unsigned int ciclo = 0;

void carregar_programa(void) {
    const uint8_t programa[] = {
        0x01, 0x01, 0x10, 
        0x05, 0x00, 0x01, 
        0x05, 0x03, 0x00, 
        0x06, 0x01, 0x03, 
        0x08, 0x7F, 0x00, 
        0x02, 0x00, 0x21, 
        0x05, 0x00, 0x00, 
        0x02, 0x01, 0x22, 
        0x01, 0x02, 0x21, 
        0x03, 0x00, 0x02, 
        0x05, 0x03, 0x01, 
        0x04, 0x01, 0x03, 
        0x05, 0x03, 0x00, 
        0x06, 0x01, 0x03, 
        0x09, 0x58, 0x00, 
        0x01, 0x01, 0x22, 
        0x05, 0x03, 0x01, 
        0x04, 0x01, 0x03, 
        0x05, 0x03, 0x00, 
        0x06, 0x01, 0x03, 
        0x09, 0x4F, 0x00, 
        0x02, 0x00, 0x20, 
        0x0A, 0x00, 0x00  
    };

    mem[0] = 0x07;
    mem[1] = 0x40;
    mem[2] = 0x00;
    mem[0x10] = N;
    for (unsigned int i = 0; i < sizeof(programa); i++) {
        mem[0x40 + i] = programa[i];
    }
}

void fetch(uint8_t *op, uint8_t *a, uint8_t *b) {
    if (pc > 253) {
        fprintf(stderr, "Erro: instrucao fora da memoria (PC=%u).\n", pc);
        exit(EXIT_FAILURE);
    }
    *op = mem[pc];
    *a = mem[pc + 1];
    *b = mem[pc + 2];
    pc += 3;
}

void decode_execute(uint8_t op, uint8_t a, uint8_t b) {
    if (op < 0x01 || op > 0x0A ||
        (op <= 0x06 && a > 3) ||
        ((op == 0x03 || op == 0x04 || op == 0x06) && b > 3)) {
        fprintf(stderr, "Erro: opcode ou registrador invalido.\n");
        exit(EXIT_FAILURE);
    }

    switch (op) {
        case 0x01: reg[a] = mem[b]; break;
        case 0x02: mem[b] = reg[a]; break;
        case 0x03: reg[a] = (uint8_t)(reg[a] + reg[b]); break;
        case 0x04: reg[a] = (uint8_t)(reg[a] - reg[b]); break;
        case 0x05: reg[a] = b; break;
        case 0x06: zf = (reg[a] == reg[b]); break;
        case 0x07: pc = a; break;
        case 0x08: if (zf) pc = a; break;
        case 0x09: if (!zf) pc = a; break;
        case 0x0A: running = 0; break;
    }
}

void trace(uint8_t op, uint8_t a, uint8_t b) {
    char instrucao[32];
    const char *nomes[] = {
        "", "LOAD", "STORE", "ADD", "SUB", "MOV",
        "CMP", "JMP", "JZ", "JNZ", "HALT"
    };

    if (op == 0x01 || op == 0x02) {
        snprintf(instrucao, sizeof(instrucao), "%s R%u, 0x%02X",
                 nomes[op], (unsigned int)a, (unsigned int)b);
    } else if (op == 0x03 || op == 0x04 || op == 0x06) {
        snprintf(instrucao, sizeof(instrucao), "%s R%u, R%u",
                 nomes[op], (unsigned int)a, (unsigned int)b);
    } else if (op == 0x05) {
        snprintf(instrucao, sizeof(instrucao), "MOV R%u, %u",
                 (unsigned int)a, (unsigned int)b);
    } else if (op >= 0x07 && op <= 0x09) {
        snprintf(instrucao, sizeof(instrucao), "%s 0x%02X",
                 nomes[op], (unsigned int)a);
    } else {
        snprintf(instrucao, sizeof(instrucao), "HALT");
    }

    printf("Ciclo %3u: %-15s | R0=%3u R1=%3u R2=%3u R3=%3u"
           " | PC=%3u ZF=%d | MEM[0x20]=%3u\n",
           ciclo, instrucao, (unsigned int)reg[0], (unsigned int)reg[1],
           (unsigned int)reg[2], (unsigned int)reg[3], pc, zf,
           (unsigned int)mem[0x20]);
}

int main(void) {
    uint8_t op, a, b;
    carregar_programa();
    printf("MiniCPU - Grupo 3 - Fatorial de %u\n\n", (unsigned int)mem[0x10]);

    while (running) {
        if (ciclo >= 10000) {
            fprintf(stderr, "Erro: limite de ciclos atingido.\n");
            return EXIT_FAILURE;
        }
        fetch(&op, &a, &b);
        decode_execute(op, a, b);
        ciclo++;
        trace(op, a, b);
    }

    printf("\nResultado: %u! = %u (MEM[0x20])\n",
           (unsigned int)mem[0x10], (unsigned int)mem[0x20]);
    return EXIT_SUCCESS;
}
