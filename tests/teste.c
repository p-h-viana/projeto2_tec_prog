#include "teste.h"

int total = 0;
int falhas = 0;

int fim_dos_testes(const char *modulo)
{
    printf("%s: %d de %d testes passaram\n", modulo, total - falhas, total);
    return falhas == 0 ? 0 : 1;
}
