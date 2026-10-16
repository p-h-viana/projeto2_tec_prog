#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "elementos.h"

bool teste_nome(void) {
    bool iguais = (strcmp(elemento_nome("H"), "Hidrogenio") == 0 && strcmp(elemento_nome("Dy"), "Disprosio") == 0 && strcmp(elemento_nome("Og"), "Oganessonio") == 0);
    bool nulos = (elemento_nome("Xx") == NULL && elemento_nome(NULL) == NULL);
    return iguais && nulos;
}

bool teste_numero_atomico(void) {
    bool iguais = (elemento_numero_atomico("H") == 1 && elemento_numero_atomico("Dy") == 66 && elemento_numero_atomico("Og") == 118);
    bool invalidos = (elemento_numero_atomico("Xx") == -1 && elemento_numero_atomico(NULL) == -1);
    return iguais && invalidos;
}

bool teste_massa_atomica(void) {
    bool iguais = (fabs(elemento_massa_atomica("H") - 1.008) < 1e-6 && fabs(elemento_massa_atomica("O") - 15.999) < 1e-6);
    bool invalidos = (elemento_massa_atomica("Xx") == -1 && elemento_massa_atomica(NULL) == -1);
    return iguais && invalidos;
}

bool teste_grupo(void) {
    bool iguais = (elemento_grupo("H") == 1 && elemento_grupo("He") == 18 && elemento_grupo("Og") == 18);
    bool invalidos = (elemento_grupo("Xx") == -1 && elemento_grupo(NULL) == -1);
    return iguais && invalidos;
}

bool teste_periodo(void) {
    bool iguais = (elemento_periodo("H") == 1 && elemento_periodo("Dy") == 6 && elemento_periodo("Og") == 7);
    bool invalidos = (elemento_periodo("Xx") == -1 && elemento_periodo(NULL) == -1);
    return iguais && invalidos;
}

bool teste_classificacao(void) {
    bool iguais = (strcmp(elemento_classificacao("He"), "Gas nobre") == 0);
    bool nulos = (elemento_classificacao("Xx") == NULL && elemento_classificacao(NULL) == NULL);
    return iguais && nulos;
}

bool teste_simbolo(void) {
    bool iguais = (strcmp(elemento_simbolo("Hidrogenio"), "H") == 0 && strcmp(elemento_simbolo("Disprosio"), "Dy") == 0 && strcmp(elemento_simbolo("Oganessonio"), "Og") == 0);
    bool nulos = (elemento_simbolo("Xx") == NULL && elemento_simbolo(NULL) == NULL);
    return iguais && nulos;
}

int main(void) {
    printf("nome:            %s\n", teste_nome() ? "OK" : "FALHA");
    printf("numero atomico:  %s\n", teste_numero_atomico() ? "OK" : "FALHA");
    printf("massa atomica:   %s\n", teste_massa_atomica() ? "OK" : "FALHA");
    printf("grupo:           %s\n", teste_grupo() ? "OK" : "FALHA");
    printf("periodo:         %s\n", teste_periodo() ? "OK" : "FALHA");
    printf("classificacao:   %s\n", teste_classificacao() ? "OK" : "FALHA");
    printf("simbolo:         %s\n", teste_simbolo() ? "OK" : "FALHA");

    return (teste_nome() && teste_numero_atomico() && teste_massa_atomica()
         && teste_grupo() && teste_periodo() && teste_classificacao()
         && teste_simbolo()) ? 0 : 1;
}