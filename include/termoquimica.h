#ifndef TERMOQUIMICA_H
#define TERMOQUIMICA_H

/**
 * @brief Status de retorno para as funções de termodinâmica
 */
typedef enum {
    SUCESSO_TERMO = 0,
    ERRO_PONTEIRO_NULO = 1 
} TermoStatus;

/**
 * @brief Classificação do tipo de reação com base no Delta H
 */
typedef enum {
    REACAO_EXOTERMICA = 0, /* Delta H < 0 (Libera calor) */
    REACAO_ENDOTERMICA = 1, /* Delta H > 0 (Absorve calor) */
    REACAO_ATERMICA = 2     /* Delta H = 0 (Não há troca de calor) */
} TipoReacao;

/**
 * @brief Calcula a variação de entalpia (Delta H) de uma reação a partir 
 *        das entalpias de formação e define se é exotérmica ou endotérmica.
 * 
 * @param soma_h_produtos 
 * @param soma_h_reagentes 
 * @param point_delta_h 
 * @param point_tipo_reacao
 * @return TermoStatus Retorna SUCESSO_TERMO em caso de sucesso ou erro apropriado.
 */
TermoStatus entalpia_reacao(double soma_h_produtos, double soma_h_reagentes, double *point_delta_h, TipoReacao *point_tipo_reacao);

#endif 