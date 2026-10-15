#include "termoquimica.h"
#include <string.h> 

double buscar_entalpia_bd(const char *nome_substancia, int *encontrou) {
    *encontrou = 1; 
    
    if (strcmp(nome_substancia, "H2O") == 0) return -285.8;
    if (strcmp(nome_substancia, "CO2") == 0) return -393.5;
    if (strcmp(nome_substancia, "CH4") == 0) return -74.8;
    if (strcmp(nome_substancia, "O2") == 0) return 0.0; // Substância simples
    
    *encontrou = 0; 
    return 0.0;
}

TermoStatus entalpia_reacao_bd(
    int qtd_reagentes, Substancia reagentes[],
    int qtd_produtos, Substancia produtos[],
    double *point_delta_h, TipoReacao *point_tipo_reacao) 
{
    int i, encontrou;
    double soma_h_reagentes = 0.0;
    double soma_h_produtos = 0.0;
    double entalpia_temp;

    if (point_delta_h == 0 || point_tipo_reacao == 0 || reagentes == 0 || produtos == 0) {
        return ERRO_PONTEIRO_NULO;
    }

    if (qtd_reagentes <= 0 || qtd_produtos <= 0) {
        return ERRO_QUANTIDADE_INVALIDA;
    }

    for (i = 0; i < qtd_reagentes; i++) {
        entalpia_temp = buscar_entalpia_bd(reagentes[i].nome, &encontrou);
        if (!encontrou) return ERRO_SUBSTANCIA_NAO_ENCONTRADA;
        soma_h_reagentes += (reagentes[i].mols * entalpia_temp);
    }

    for (i = 0; i < qtd_produtos; i++) {
        entalpia_temp = buscar_entalpia_bd(produtos[i].nome, &encontrou);
        if (!encontrou) return ERRO_SUBSTANCIA_NAO_ENCONTRADA;
        soma_h_produtos += (produtos[i].mols * entalpia_temp);
    }

    *point_delta_h = soma_h_produtos - soma_h_reagentes;

    if (*point_delta_h < 0.0) {
        *point_tipo_reacao = REACAO_EXOTERMICA;
    } else if (*point_delta_h > 0.0) {
        *point_tipo_reacao = REACAO_ENDOTERMICA;
    } else {
        *point_tipo_reacao = REACAO_ATERMICA;
    }

    return SUCESSO_TERMO;
}

TermoStatus lei_de_hess(
    int qtd_equacoes, double delta_h_equacoes[], double multiplicadores[], 
    double *point_delta_h, TipoReacao *point_tipo_reacao) 
{
    int i;
    double delta_h_final = 0.0;

    if (point_delta_h == 0 || point_tipo_reacao == 0 || 
        delta_h_equacoes == 0 || multiplicadores == 0) {
        return ERRO_PONTEIRO_NULO;
    }

    if (qtd_equacoes <= 0) {
        return ERRO_QUANTIDADE_INVALIDA;
    }

    for (i = 0; i < qtd_equacoes; i++) {
        delta_h_final += (delta_h_equacoes[i] * multiplicadores[i]);
    }

    *point_delta_h = delta_h_final;

    if (*point_delta_h < 0.0) {
        *point_tipo_reacao = REACAO_EXOTERMICA;
    } else if (*point_delta_h > 0.0) {
        *point_tipo_reacao = REACAO_ENDOTERMICA;
    } else {
        *point_tipo_reacao = REACAO_ATERMICA;
    }

    return SUCESSO_TERMO;
}