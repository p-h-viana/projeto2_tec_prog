#include <stdio.h>
#include "elementos.h"
#include "estequiometria.h"
#include "ph.h"
#include "termoquimica.h"

int main(void)
{
    char eq[128];
    double massas[] = {10.0, 32.0};
    double massa, rendimento, ph, dh;
    size_t limitante;
    EstqStatus st;
    TipoReacao tipo;
    Substancia reagentes[] = {{"CH4", 1.0}, {"O2", 2.0}};
    Substancia produtos[] = {{"CO2", 1.0}, {"H2O", 2.0}};

    printf("Fe: %s, Z = %d, massa atomica = %.3f u\n",
           elemento_nome("Fe"), elemento_numero_atomico("Fe"), elemento_massa_atomica("Fe"));

    if (estq_balancear("Fe + O2 -> Fe2O3", eq, sizeof eq) == ESTQ_OK)
        printf("Equacao balanceada: %s\n", eq);

    st = estq_balancear("H2 + O2", eq, sizeof eq);
    printf("Equacao \"H2 + O2\": %s\n", estq_mensagem_erro(st));

    if (estq_massa_produto("H2 + O2 -> H2O", massas, 2, "H2O", &massa, &limitante) == ESTQ_OK &&
        estq_rendimento(27.0, massa, &rendimento) == ESTQ_OK)
        printf("10 g de H2 + 32 g de O2: %.2f g de H2O (limitante: reagente %zu); 27 g obtidos = %.1f%%\n",
               massa, limitante, rendimento);

    if (ph_acido_fraco(0.1, 1.8e-5, &ph) == PH_OK)
        printf("pH do acido acetico 0,1 mol/L: %.2f\n", ph);

    if (termo_entalpia_reacao_bd(2, reagentes, 2, produtos, &dh, &tipo) == TERMO_OK)
        printf("Combustao do metano: dH = %.1f kJ (%s)\n",
               dh, tipo == TERMO_REACAO_EXOTERMICA ? "exotermica" : "nao exotermica");

    return 0;
}
