#ifndef TERMOQUIMICA_H
#define TERMOQUIMICA_H

/**
 * @brief Status de retorno para as funções de termodinâmica.
 */
typedef enum {
    SUCESSO_TERMO = 0,
    ERRO_PONTEIRO_NULO = 1,
    ERRO_QUANTIDADE_INVALIDA = 2,
    ERRO_SUBSTANCIA_NAO_ENCONTRADA = 3 
} TermoStatus;

/**
 * @brief Classificação do tipo de reação com base no Delta H.
 */
typedef enum {
    REACAO_EXOTERMICA = 0, /* Delta H < 0 (Libera calor) */
    REACAO_ENDOTERMICA = 1, /* Delta H > 0 (Absorve calor) */
    REACAO_ATERMICA = 2     /* Delta H = 0 (Não há troca de calor) */
} TipoReacao;

/**
 * @brief Estrutura para armazenar os dados de cada participante da reação.
 */
typedef struct {
    char nome[50]; /* Nome da molécula (ex: "H2O") */
    double mols;   /* Coeficiente estequiométrico */
} Substancia;

/**
 * @brief Simulação da busca no Banco de Dados.
 * 
 * @param nome_substancia Nome da molécula para buscar.
 * @param encontrou Ponteiro que recebe 1 se achou e 0 se não achou.
 * @return double Retorna o valor da entalpia da substância.
 */
double buscar_entalpia_bd(const char *nome_substancia, int *encontrou);

/**
 * @brief Calcula o Delta H buscando as entalpias automaticamente no Banco de Dados.
 * 
 * @param qtd_reagentes Número de substâncias nos reagentes.
 * @param reagentes Array com as estruturas dos reagentes.
 * @param qtd_produtos Número de substâncias nos produtos.
 * @param produtos Array com as estruturas dos produtos.
 * @param point_delta_h Ponteiro onde o valor numérico do Delta H será salvo.
 * @param point_tipo_reacao Ponteiro onde a classificação da reação será salva.
 * @return TermoStatus Retorna SUCESSO_TERMO em caso de sucesso ou erro apropriado.
 */
TermoStatus entalpia_reacao_bd(
    int qtd_reagentes, Substancia reagentes[],
    int qtd_produtos, Substancia produtos[],
    double *point_delta_h, TipoReacao *point_tipo_reacao
);

/**
 * @brief Calcula o Delta H final de uma reação usando a Lei de Hess.
 * 
 * @param qtd_equacoes Número de equações intermediárias.
 * @param delta_h_equacoes Array com os valores de Delta H de cada equação intermediária.
 * @param multiplicadores Array com os fatores de multiplicação de cada equação.
 * @param point_delta_h Ponteiro onde o valor numérico do Delta H final será salvo.
 * @param point_tipo_reacao Ponteiro onde a classificação da reação global será salva.
 * @return TermoStatus Retorna SUCESSO_TERMO em caso de sucesso ou erro apropriado.
 */
TermoStatus lei_de_hess(
    int qtd_equacoes, double delta_h_equacoes[], double multiplicadores[], 
    double *point_delta_h, TipoReacao *point_tipo_reacao
);

#endif 