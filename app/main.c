#include <stdio.h>
#include "quimica.h"

static void demo_elementos(void) {
    printf("[ Modulo: Elementos ]\n");
    printf("Consulta Fe: Z=%d, Massa=%.3f u, Classe=%s\n\n",
           elemento_numero_atomico("Fe"),
           elemento_massa_atomica("Fe"),
           elemento_classificacao("Fe"));
}

static void demo_estequiometria(void) {
    char eq[128];
    double massas[] = {10.0, 32.0};
    double massa_prod, rendimento;
    size_t limitante;

    printf("[ Modulo: Estequiometria ]\n");
    
    if (estq_balancear("Fe + O2 -> Fe2O3", eq, sizeof(eq)) == ESTQ_OK) {
        printf("Balanceamento: %s\n", eq);
    }

    if (estq_massa_produto("H2 + O2 -> H2O", massas, 2, "H2O", &massa_prod, &limitante) == ESTQ_OK) {
        estq_rendimento(27.0, massa_prod, &rendimento);
        printf("Rendimento (10g H2 + 32g O2 -> 27g H2O): %.1f%%\n", rendimento);
        printf("Reagente limitante (indice): %zu\n\n", limitante);
    }
}

static void demo_ph(void) {
    double ph;
    printf("[ Modulo: pH ]\n");
    
    if (ph_acido_fraco(0.1, 1.8e-5, &ph) == PH_OK) {
        printf("pH (acido acetico 0.1M): %.2f\n\n", ph);
    }
}

static void demo_termoquimica(void) {
    Substancia reagentes[] = {{"CH4", 1.0}, {"O2", 2.0}};
    Substancia produtos[]  = {{"CO2", 1.0}, {"H2O", 2.0}};
    double dh;
    TipoReacao tipo;

    printf("[ Modulo: Termoquimica ]\n");
    
    if (termo_entalpia_reacao_bd(2, reagentes, 2, produtos, &dh, &tipo) == TERMO_OK) {
        const char *classificacao = (tipo == TERMO_REACAO_EXOTERMICA) ? "Exotermica" : "Nao exotermica";
        printf("Combustao CH4: dH = %.1f kJ (%s)\n\n", dh, classificacao);
    }
}

int main(void) {
    demo_elementos();
    demo_estequiometria();
    demo_ph();
    demo_termoquimica();

    return 0;
}