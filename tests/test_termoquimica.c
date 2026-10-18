#include "teste.h"
#include "termoquimica.h"

int main(void)
{
    double dh;
    TipoReacao tipo;
    
    Substancia metano_o2[] = {{"CH4", 1.0}, {"O2", 2.0}};
    double ent_metano_o2[] = {-74.8, 0.0};
    
    Substancia co2_agua[] = {{"CO2", 1.0}, {"H2O", 2.0}};
    double ent_co2_agua[] = {-393.5, -285.8};
    
    Substancia so_o2[] = {{"O2", 1.0}};
    double ent_so_o2[] = {0.0};
    
    Substancia sem_nome[] = {{NULL, 1.0}};
    double ent_sem_nome[] = {0.0};
    
    Substancia mols_negativo[] = {{"CH4", -1.0}};
    double ent_mols_negativo[] = {-74.8};

    printf("Modulo termoquimica\n");

    /* termo_entalpia_reacao_bd */
    VERIFICA(termo_entalpia_reacao_bd(2, metano_o2, ent_metano_o2, 2, co2_agua, ent_co2_agua, &dh, &tipo) == TERMO_OK &&
             PERTO(dh, -890.3, 1e-9) && tipo == TERMO_REACAO_EXOTERMICA,
             "combustao do metano: dH = -890,3 kJ, exotermica");
             
    VERIFICA(termo_entalpia_reacao_bd(2, co2_agua, ent_co2_agua, 2, metano_o2, ent_metano_o2, &dh, &tipo) == TERMO_OK &&
             PERTO(dh, 890.3, 1e-9) && tipo == TERMO_REACAO_ENDOTERMICA,
             "reacao inversa: dH = +890,3 kJ, endotermica");
             
    VERIFICA(termo_entalpia_reacao_bd(1, so_o2, ent_so_o2, 1, so_o2, ent_so_o2, &dh, &tipo) == TERMO_OK &&
             tipo == TERMO_REACAO_ATERMICA,
             "O2 -> O2 e atermica");
             
    VERIFICA(termo_entalpia_reacao_bd(0, metano_o2, ent_metano_o2, 2, co2_agua, ent_co2_agua, &dh, &tipo) == TERMO_ERRO_QUANTIDADE_INVALIDA,
             "zero reagentes e erro");
             
    VERIFICA(termo_entalpia_reacao_bd(2, NULL, ent_metano_o2, 2, co2_agua, ent_co2_agua, &dh, &tipo) == TERMO_ERRO_PONTEIRO_NULO,
             "vetor de reagentes nulo e erro");
             
    VERIFICA(termo_entalpia_reacao_bd(2, metano_o2, ent_metano_o2, 2, co2_agua, ent_co2_agua, NULL, &tipo) == TERMO_ERRO_PONTEIRO_NULO,
             "ponteiro de saida nulo e erro");
             
    VERIFICA(termo_entalpia_reacao_bd(1, sem_nome, ent_sem_nome, 2, co2_agua, ent_co2_agua, &dh, &tipo) == TERMO_ERRO_PONTEIRO_NULO,
             "substancia sem nome e erro");
             
    VERIFICA(termo_entalpia_reacao_bd(1, mols_negativo, ent_mols_negativo, 2, co2_agua, ent_co2_agua, &dh, &tipo) == TERMO_ERRO_VALOR_INVALIDO,
             "quantidade de mols negativa e erro");

    /* termo_lei_de_hess */
    {
        double hs[] = {-393.5, -285.8, -890.3};
        double mult[] = {1.0, 2.0, -1.0};
        VERIFICA(termo_lei_de_hess(3, hs, mult, &dh, &tipo) == TERMO_OK &&
                 PERTO(dh, -74.8, 1e-9) && tipo == TERMO_REACAO_EXOTERMICA,
                 "Lei de Hess: formacao do CH4 da -74,8 kJ");
    }
    {
        double hs[] = {0.1, 0.2, 0.3};
        double mult[] = {1.0, 1.0, -1.0};
        VERIFICA(termo_lei_de_hess(3, hs, mult, &dh, &tipo) == TERMO_OK &&
                 tipo == TERMO_REACAO_ATERMICA,
                 "Lei de Hess: 0,1 + 0,2 - 0,3 e atermica");
    }
    {
        double hs[] = {-393.5, NAN};
        double mult[] = {1.0, 1.0};
        VERIFICA(termo_lei_de_hess(2, hs, mult, &dh, &tipo) == TERMO_ERRO_VALOR_INVALIDO,
                 "Lei de Hess: dH NaN e erro");
        VERIFICA(termo_lei_de_hess(0, hs, mult, &dh, &tipo) == TERMO_ERRO_QUANTIDADE_INVALIDA,
                 "Lei de Hess: zero equacoes e erro");
        VERIFICA(termo_lei_de_hess(2, hs, NULL, &dh, &tipo) == TERMO_ERRO_PONTEIRO_NULO,
                 "Lei de Hess: multiplicadores nulos e erro");
    }

    return fim_dos_testes("Termoquimica");
}
