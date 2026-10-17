/* Macros e funções simples usadas pelos testes de cada módulo. */
#ifndef TESTE_H
#define TESTE_H

#include <math.h>
#include <stdio.h>
#include <string.h>

extern int total;
extern int falhas;

#define VERIFICA(cond, descricao)                                          \
    do {                                                                   \
        total++;                                                           \
        if (cond) {                                                        \
            printf("  [OK]    %s\n", descricao);                           \
        } else {                                                           \
            falhas++;                                                      \
            printf("  [FALHA] %s (linha %d)\n", descricao, __LINE__);      \
        }                                                                  \
    } while (0)

#define PERTO(a, b, tol) (fabs((a) - (b)) <= (tol))
#define IGUAIS(a, b)     ((a) != NULL && strcmp((a), (b)) == 0)

/* Mostra quantos testes passaram; devolve 0 se todos passaram e 1 se não. 
 * Implentação em teste.c
 */
int fim_dos_testes(const char *modulo);

#endif
