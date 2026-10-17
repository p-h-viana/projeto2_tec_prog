/**
 * @file ph.c
 * @brief Implementação do módulo de pH.
 */
#include "ph.h"
#include <math.h>
#include <stddef.h>

PhStatus ph_acido_forte(double concentracao_h, double *point_ph) {
    // Validação de Erros
    if (point_ph == NULL) {
        return PH_ERRO_PONTEIRO_NULO;
    }
    if (!isfinite(concentracao_h) || concentracao_h <= 0.0) {
        return PH_ERRO_CONC_INVALIDA;
    }

    // Cálculo do pH para ácido forte
    *point_ph = -log10(concentracao_h);

    // Retorno de Sucesso
    return PH_OK;
}

PhStatus ph_acido_fraco(double concentracao, double ka, double *point_ph) {
    // Verificação de concentração e Ka
    if (point_ph == NULL) {
        return PH_ERRO_PONTEIRO_NULO;
    }
    if (!isfinite(concentracao) || concentracao <= 0.0) {
        return PH_ERRO_CONC_INVALIDA;
    }
    if (!isfinite(ka) || ka <= 0.0) {
        return PH_ERRO_KA_INVALIDO;
    }

    // Cálculo do pH para ácido fraco
    double delta = (ka * ka) + (4.0 * ka * concentracao);

    // Raiz positiva escrita como 2·Ka·C / (Ka + √delta), que é igual a
    // (−Ka + √delta) / 2 mas não perde precisão quando Ka é grande
    double x = (2.0 * ka * concentracao) / (ka + sqrt(delta));

    // Cálculo do pH e Retorno
    *point_ph = -log10(x);

    return PH_OK;
}

PhStatus ph_henderson_hasselbalch(double pka, double concentracao_sal,
                                  double concentracao_acido, double *point_ph) {
    // Validação de Erros
    if (point_ph == NULL) {
        return PH_ERRO_PONTEIRO_NULO;
    }
    if (!isfinite(pka)) {
        return PH_ERRO_KA_INVALIDO;
    }
    // Nenhuma das concentrações pode ser zero ou negativa
    if (!isfinite(concentracao_sal) || !isfinite(concentracao_acido) ||
        concentracao_sal <= 0.0 || concentracao_acido <= 0.0) {
        return PH_ERRO_CONC_INVALIDA;
    }

    // Cálculo
    *point_ph = pka + log10(concentracao_sal / concentracao_acido);

    // Retorno de Sucesso
    return PH_OK;
}

PhStatus ph_acido_diluido(double concentracao_acido, double *point_ph) {
    // Validação de Erros
    if (point_ph == NULL) {
        return PH_ERRO_PONTEIRO_NULO;
    }
    if (!isfinite(concentracao_acido) || concentracao_acido <= 0.0) {
        return PH_ERRO_CONC_INVALIDA;
    }

    // Cálculo do pH para ácido diluído
    double kw = 1.0e-14; // Constante de autoionização da água a 25°C

    // Calculando a concentração total de H+ usando a raiz positiva da equação
    double h_total = (concentracao_acido + sqrt((concentracao_acido * concentracao_acido) + (4.0 * kw))) / 2.0;

    *point_ph = -log10(h_total);

    // Retorno de Sucesso
    return PH_OK;
}
