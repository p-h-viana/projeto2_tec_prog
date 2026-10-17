#include "teste.h"
#include "ph.h"

int main(void)
{
    double ph;

    printf("Modulo pH\n");

    /* ph_acido_forte */
    VERIFICA(ph_acido_forte(0.01, &ph) == PH_OK && PERTO(ph, 2.0, 1e-9),
             "acido forte 0,01 mol/L tem pH 2");
    VERIFICA(ph_acido_forte(1.0, &ph) == PH_OK && PERTO(ph, 0.0, 1e-9),
             "acido forte 1 mol/L tem pH 0");
    VERIFICA(ph_acido_forte(0.0, &ph) == PH_ERRO_CONC_INVALIDA, "acido forte: concentracao 0 e erro");
    VERIFICA(ph_acido_forte(-0.1, &ph) == PH_ERRO_CONC_INVALIDA, "acido forte: concentracao negativa e erro");
    VERIFICA(ph_acido_forte(NAN, &ph) == PH_ERRO_CONC_INVALIDA, "acido forte: NaN e erro");
    VERIFICA(ph_acido_forte(0.1, NULL) == PH_ERRO_PONTEIRO_NULO, "acido forte: ponteiro nulo e erro");
    ph = 123.0;
    ph_acido_forte(-1.0, &ph);
    VERIFICA(ph == 123.0, "acido forte: em caso de erro o resultado nao e alterado");

    /* ph_acido_fraco */
    VERIFICA(ph_acido_fraco(0.1, 1.8e-5, &ph) == PH_OK && PERTO(ph, 2.8753, 1e-3),
             "acido acetico 0,1 mol/L tem pH 2,88");
    VERIFICA(ph_acido_fraco(1e-3, 1e14, &ph) == PH_OK && PERTO(ph, 3.0, 1e-6),
             "acido fraco com Ka enorme se comporta como forte (pH 3)");
    VERIFICA(ph_acido_fraco(0.0, 1.8e-5, &ph) == PH_ERRO_CONC_INVALIDA, "acido fraco: concentracao 0 e erro");
    VERIFICA(ph_acido_fraco(0.1, 0.0, &ph) == PH_ERRO_KA_INVALIDO, "acido fraco: Ka 0 e erro");
    VERIFICA(ph_acido_fraco(0.1, -1e-5, &ph) == PH_ERRO_KA_INVALIDO, "acido fraco: Ka negativo e erro");
    VERIFICA(ph_acido_fraco(0.1, INFINITY, &ph) == PH_ERRO_KA_INVALIDO, "acido fraco: Ka infinito e erro");
    VERIFICA(ph_acido_fraco(0.1, 1.8e-5, NULL) == PH_ERRO_PONTEIRO_NULO, "acido fraco: ponteiro nulo e erro");

    /* ph_henderson_hasselbalch */
    VERIFICA(ph_henderson_hasselbalch(4.76, 0.1, 0.1, &ph) == PH_OK && PERTO(ph, 4.76, 1e-9),
             "tampao com sal = acido tem pH = pKa");
    VERIFICA(ph_henderson_hasselbalch(4.76, 1.0, 0.1, &ph) == PH_OK && PERTO(ph, 5.76, 1e-9),
             "tampao com sal 10x o acido tem pH = pKa + 1");
    VERIFICA(ph_henderson_hasselbalch(4.76, 0.0, 0.1, &ph) == PH_ERRO_CONC_INVALIDA, "tampao: sal 0 e erro");
    VERIFICA(ph_henderson_hasselbalch(4.76, 0.1, -0.1, &ph) == PH_ERRO_CONC_INVALIDA, "tampao: acido negativo e erro");
    VERIFICA(ph_henderson_hasselbalch(NAN, 0.1, 0.1, &ph) == PH_ERRO_KA_INVALIDO, "tampao: pKa NaN e erro");
    VERIFICA(ph_henderson_hasselbalch(4.76, 0.1, 0.1, NULL) == PH_ERRO_PONTEIRO_NULO, "tampao: ponteiro nulo e erro");

    /* ph_acido_diluido */
    VERIFICA(ph_acido_diluido(1e-8, &ph) == PH_OK && PERTO(ph, 6.9783, 1e-3),
             "acido 1e-8 mol/L tem pH 6,98 (e nao 8)");
    VERIFICA(ph_acido_diluido(0.1, &ph) == PH_OK && PERTO(ph, 1.0, 1e-6),
             "acido diluido concentrado da o mesmo pH do acido forte");
    VERIFICA(ph_acido_diluido(0.0, &ph) == PH_ERRO_CONC_INVALIDA, "acido diluido: concentracao 0 e erro");
    VERIFICA(ph_acido_diluido(1e-8, NULL) == PH_ERRO_PONTEIRO_NULO, "acido diluido: ponteiro nulo e erro");

    return fim_dos_testes("pH");
}
