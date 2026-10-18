/**
 * @file termoquimica.h
 * @brief Variação de entalpia de reações (tabela de entalpias e Lei de Hess).
 *
 * @details As entalpias de formação ficam numa tabela interna pequena, com
 * valores padrão a 25 °C em kJ/mol: H2O (líquida), CO2, CH4 e O2. Os nomes
 * são comparados exatamente como escritos (ex.: "CO2").
 *
 * Uma reação com |ΔH| menor que 1e-6 kJ é classificada como atérmica, para
 * não depender de a soma em ponto flutuante dar exatamente zero.
 */
#ifndef TERMOQUIMICA_H
#define TERMOQUIMICA_H

/** @brief Códigos de retorno do módulo de termoquímica. */
typedef enum {
    TERMO_OK = 0,                        /**< Sucesso. */
    TERMO_ERRO_PONTEIRO_NULO,            /**< Algum ponteiro (ou nome de substância) é NULL. */
    TERMO_ERRO_QUANTIDADE_INVALIDA,      /**< Quantidade de substâncias ou equações <= 0. */
    TERMO_ERRO_SUBSTANCIA_NAO_ENCONTRADA,/**< Substância fora da tabela de entalpias. */
    TERMO_ERRO_VALOR_INVALIDO            /**< Número não finito ou quantidade em mol negativa. */
} TermoStatus;

/** @brief Classificação da reação pelo sinal de ΔH. */
typedef enum {
    TERMO_REACAO_EXOTERMICA,  /**< ΔH < 0. */
    TERMO_REACAO_ENDOTERMICA, /**< ΔH > 0. */
    TERMO_REACAO_ATERMICA     /**< ΔH = 0 (dentro da tolerância). */
} TipoReacao;

/** @brief Substância de uma reação e sua quantidade. */
typedef struct {
    const char *nome; /**< Fórmula, ex.: "CO2". */
    double mols;      /**< Quantidade em mol (coeficiente da equação), >= 0. */
} Substancia;

/**
 * @brief Calcula ΔH = Σ(n·ΔHf produtos) − Σ(n·ΔHf reagentes) usando a tabela interna.
 * @param qtd_reagentes     Número de reagentes (> 0).
 * @param reagentes         Vetor com os reagentes.
 * @param qtd_produtos      Número de produtos (> 0).
 * @param produtos          Vetor com os produtos.
 * @param point_delta_h     Recebe ΔH em kJ.
 * @param point_tipo_reacao Recebe a classificação da reação.
 * @return ::TERMO_OK, ou:
 *         - ::TERMO_ERRO_PONTEIRO_NULO se algum ponteiro ou nome for NULL;
 *         - ::TERMO_ERRO_QUANTIDADE_INVALIDA se alguma quantidade for <= 0;
 *         - ::TERMO_ERRO_VALOR_INVALIDO se algum @c mols for negativo ou não finito;
 *         - ::TERMO_ERRO_SUBSTANCIA_NAO_ENCONTRADA se uma substância não estiver na tabela.
 */
TermoStatus termo_entalpia_reacao_bd(
    int qtd_reagentes, Substancia reagentes[], double ent_reagentes[],
    int qtd_produtos, Substancia produtos[], double ent_produtos[],
    double *point_delta_h, TipoReacao *point_tipo_reacao);

/**
 * @brief Calcula ΔH pela Lei de Hess: Σ ΔH_i · multiplicador_i.
 * @param qtd_equacoes      Número de equações (> 0).
 * @param delta_h_equacoes  ΔH de cada equação, em kJ.
 * @param multiplicadores   Fator de cada equação (negativo para equação invertida).
 * @param point_delta_h     Recebe ΔH em kJ.
 * @param point_tipo_reacao Recebe a classificação da reação.
 * @return ::TERMO_OK, ::TERMO_ERRO_PONTEIRO_NULO, ::TERMO_ERRO_QUANTIDADE_INVALIDA
 *         ou ::TERMO_ERRO_VALOR_INVALIDO (algum valor não finito).
 */
TermoStatus termo_lei_de_hess(
    int qtd_equacoes, double delta_h_equacoes[], double multiplicadores[],
    double *point_delta_h, TipoReacao *point_tipo_reacao);

#endif /* TERMOQUIMICA_H */
