/**
 * @file termoquimica.c
 * @brief Implementação do módulo de termoquímica.
 */
#include "termoquimica.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

#define TOLERANCIA_ATERMICA 1e-6 /* kJ; abaixo disso o ΔH é considerado zero */



static TipoReacao classificar_reacao(double delta_h) {
    if (delta_h < -TOLERANCIA_ATERMICA) {
        return TERMO_REACAO_EXOTERMICA;
    } else if (delta_h > TOLERANCIA_ATERMICA) {
        return TERMO_REACAO_ENDOTERMICA;
    }
    return TERMO_REACAO_ATERMICA;
}

TermoStatus termo_entalpia_reacao_bd(
    int qtd_reagentes, Substancia reagentes[], double ent_reagentes[],
    int qtd_produtos, Substancia produtos[], double ent_produtos[],
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
        
        soma_h_reagentes += (reagentes[i].mols * ent_reagentes[i]);
    }

    for (i = 0; i < qtd_produtos; i++) {
        if (produtos[i].nome == NULL) return TERMO_ERRO_PONTEIRO_NULO;
        if (!isfinite(produtos[i].mols) || produtos[i].mols < 0.0) return TERMO_ERRO_VALOR_INVALIDO;
        
        soma_h_produtos += (produtos[i].mols * ent_produtos[i]);
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
