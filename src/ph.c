#include "ph.h"
#include <math.h>

PhStatus calc_strong_acid_ph(double concentracao_h, double *point_ph) {
    //  Validação de Erros
    if (concentracao_h <= 0.0) {
        return PH_ERR_NEGATIVE_CONC;
    }

   
    // calculo do pH para ácido forte
    *point_ph = -log10(concentracao_h);

    //  Retorno de Sucesso
    return PH_SUCCESS;
}

PhStatus calc_acid_fraco(double concentracao, double ka, double *point_ph) {
    //verificacao de concnetracao e ka
if (concentracao <= 0.0) {
        return ERRO_CONC_NEGAT;
    }
    if (ka <= 0.0) {
        return ERRO_KA_NEGAT;
    }

    //  Cálculo do pH para ácido fraco

    double delta = (ka * ka) - (4.0 * 1.0 * (-ka * concentracao));
    
    // Calculando a concentração de H+ (raiz positiva) */
    double x = (-ka + sqrt(delta)) / 2.0;

    //  Cálculo do pH e Retorno 
    *point_ph = -log10(x);

    return PH_SUCCESS;

}

PhStatus calc_henderson_hasselbalch(double pka, double concentracao_sal, double concentracao_acido, double *point_ph) {
    //  Validação de Erros 
    // Nenhuma das concentrações pode ser zero ou negativa 
    if (concentracao_sal <= 0.0 || concentracao_acido <= 0.0) {
        return ERRO_CONC_NEGAT;
    }

// calculo
    *point_ph = pka + log10(concentracao_sal / concentracao_acido);

    // Retorno de Sucesso 
    return PH_SUCCESS;
}

PhStatus ph_acid_diluido(double concentracao_acido, double *point_ph) {
    // Validação de Erros
    if (concentracao_acido <= 0.0) {
        return ERRO_CONC_NEGAT;
    }

    // Cálculo do pH para ácido diluído
    double kw = 1.0e-14; // Constante de autoionização da água a 25°C 
    
    // Calculando a concentração total de H+ usando a raiz positiva da equação 
    double h_total = (concentracao_acido + sqrt((concentracao_acido * concentracao_acido) + (4.0 * kw))) / 2.0;

    *point_ph = -log10(h_total);

    // 3. Retorno de Sucesso 
    return PH_SUCCESS;
}