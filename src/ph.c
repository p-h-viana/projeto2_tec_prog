#include "ph.h"
#include <math.h>

PhStatus calc_strong_acid_ph(double concentracao_h, double *point_ph) {
    /* 1. Validação de Erros */
    if (concentracao_h <= 0.0) {
        return PH_ERR_NEGATIVE_CONC;
    }

    /* 2. O Cálculo Matemático */
    /* A função log10 vem da biblioteca <math.h> */
    *point_ph = -log10(concentracao_h);

    /* 3. Retorno de Sucesso */
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

    /* Cálculo do pH para ácido fraco */

    double delta = (ka * ka) - (4.0 * 1.0 * (-ka * concentracao));
    
    /* Calculando a concentração de H+ (raiz positiva) */
    double x = (-ka + sqrt(delta)) / 2.0;

    /* 3. Cálculo do pH e Retorno */
    *point_ph = -log10(x);

    return PH_SUCCESS;

}