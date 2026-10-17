/**
 * @file termoquimica.c
 * @brief Implementação do módulo de termoquímica.
 */
#include "termoquimica.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

#define TOLERANCIA_ATERMICA 1e-6 /* kJ; abaixo disso o ΔH é considerado zero */

// Entalpias de formação padrão a 25 °C, em kJ/mol
static double buscar_entalpia_bd(const char *nome_substancia, int *encontrou) {
    *encontrou = 1;

    if (strcmp(nome_substancia, "H2O") == 0) return -285.8; // água líquida (vapor seria -241.8)
    if (strcmp(nome_substancia, "CO2") == 0) return -393.5;
    if (strcmp(nome_substancia, "CH4") == 0) return -74.8;
    if (strcmp(nome_substancia, "O2") == 0) return 0.0; // Substância simples

    *encontrou = 0;
    return 0.0;
}

// Classifica a reação pelo sinal de ΔH
static TipoReacao classificar_reacao(double delta_h) {
    if (delta_h < -TOLERANCIA_ATERMICA) {
        return TERMO_REACAO_EXOTERMICA;
    } else if (delta_h > TOLERANCIA_ATERMICA) {
        return TERMO_REACAO_ENDOTERMICA;
    }
    return TERMO_REACAO_ATERMICA;
}

TermoStatus termo_entalpia_reacao_bd(
    int qtd_reagentes, Substancia reagentes[],
    int qtd_produtos, Substancia produtos[],
    double *point_delta_h, TipoReacao *point_tipo_reacao)
{
    int i, encontrou;
    double soma_h_reagentes = 0.0;
    double soma_h_produtos = 0.0;
    double entalpia_temp;

    if (point_delta_h == NULL || point_tipo_reacao == NULL || reagentes == NULL || produtos == NULL) {
        return TERMO_ERRO_PONTEIRO_NULO;
    }

    if (qtd_reagentes <= 0 || qtd_produtos <= 0) {
        return TERMO_ERRO_QUANTIDADE_INVALIDA;
    }

    for (i = 0; i < qtd_reagentes; i++) {
        if (reagentes[i].nome == NULL) return TERMO_ERRO_PONTEIRO_NULO;
        if (!isfinite(reagentes[i].mols) || reagentes[i].mols < 0.0) return TERMO_ERRO_VALOR_INVALIDO;
        entalpia_temp = buscar_entalpia_bd(reagentes[i].nome, &encontrou);
        if (!encontrou) return TERMO_ERRO_SUBSTANCIA_NAO_ENCONTRADA;
        soma_h_reagentes += (reagentes[i].mols * entalpia_temp);
    }

    for (i = 0; i < qtd_produtos; i++) {
        if (produtos[i].nome == NULL) return TERMO_ERRO_PONTEIRO_NULO;
        if (!isfinite(produtos[i].mols) || produtos[i].mols < 0.0) return TERMO_ERRO_VALOR_INVALIDO;
        entalpia_temp = buscar_entalpia_bd(produtos[i].nome, &encontrou);
        if (!encontrou) return TERMO_ERRO_SUBSTANCIA_NAO_ENCONTRADA;
        soma_h_produtos += (produtos[i].mols * entalpia_temp);
    }

    *point_delta_h = soma_h_produtos - soma_h_reagentes;
    *point_tipo_reacao = classificar_reacao(*point_delta_h);

    return TERMO_OK;
}

TermoStatus termo_lei_de_hess(
    int qtd_equacoes, double delta_h_equacoes[], double multiplicadores[],
    double *point_delta_h, TipoReacao *point_tipo_reacao)
{
    int i;
    double delta_h_final = 0.0;

    if (point_delta_h == NULL || point_tipo_reacao == NULL ||
        delta_h_equacoes == NULL || multiplicadores == NULL) {
        return TERMO_ERRO_PONTEIRO_NULO;
    }

    if (qtd_equacoes <= 0) {
        return TERMO_ERRO_QUANTIDADE_INVALIDA;
    }

    for (i = 0; i < qtd_equacoes; i++) {
        if (!isfinite(delta_h_equacoes[i]) || !isfinite(multiplicadores[i])) {
            return TERMO_ERRO_VALOR_INVALIDO;
        }
        delta_h_final += (delta_h_equacoes[i] * multiplicadores[i]);
    }

    *point_delta_h = delta_h_final;
    *point_tipo_reacao = classificar_reacao(*point_delta_h);

    return TERMO_OK;
}
