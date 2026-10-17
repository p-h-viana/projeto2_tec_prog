/**
 * @file ph.h
 * @brief Cálculo de pH de soluções ácidas e tampões (a 25 °C).
 *
 * @details Todas as funções devolvem o resultado por ponteiro e retornam um
 * ::PhStatus. Em caso de erro, o valor apontado não é alterado.
 */
#ifndef PH_H
#define PH_H

/** @brief Códigos de retorno do módulo de pH. */
typedef enum {
    PH_OK = 0,             /**< Sucesso. */
    PH_ERRO_PONTEIRO_NULO, /**< Ponteiro de saída é NULL. */
    PH_ERRO_CONC_INVALIDA, /**< Concentração zero, negativa ou não finita. */
    PH_ERRO_KA_INVALIDO    /**< Ka (ou pKa) inválido. */
} PhStatus;

/**
 * @brief pH de um ácido forte monoprótico, supondo ionização total.
 * @param concentracao_h Concentração de H+ em mol/L (> 0).
 * @param point_ph       Recebe o pH calculado.
 * @return ::PH_OK, ::PH_ERRO_PONTEIRO_NULO ou ::PH_ERRO_CONC_INVALIDA.
 * @note Para soluções muito diluídas (abaixo de ~1e-6 mol/L) use ph_acido_diluido().
 */
PhStatus ph_acido_forte(double concentracao_h, double *point_ph);

/**
 * @brief pH de um ácido fraco monoprótico, resolvendo x² + Ka·x − Ka·C = 0.
 * @param concentracao Concentração inicial do ácido em mol/L (> 0).
 * @param ka           Constante de acidez (> 0).
 * @param point_ph     Recebe o pH calculado.
 * @return ::PH_OK, ::PH_ERRO_PONTEIRO_NULO, ::PH_ERRO_CONC_INVALIDA ou
 *         ::PH_ERRO_KA_INVALIDO.
 * @note A autoionização da água é desprezada.
 */
PhStatus ph_acido_fraco(double concentracao, double ka, double *point_ph);

/**
 * @brief pH de uma solução tampão pela equação de Henderson-Hasselbalch.
 * @param pka                pKa do ácido (qualquer valor finito).
 * @param concentracao_sal   Concentração da base conjugada (sal) em mol/L (> 0).
 * @param concentracao_acido Concentração do ácido em mol/L (> 0).
 * @param point_ph           Recebe o pH calculado.
 * @return ::PH_OK, ::PH_ERRO_PONTEIRO_NULO, ::PH_ERRO_CONC_INVALIDA ou
 *         ::PH_ERRO_KA_INVALIDO (pKa não finito).
 */
PhStatus ph_henderson_hasselbalch(double pka, double concentracao_sal,
                                  double concentracao_acido, double *point_ph);

/**
 * @brief pH de um ácido forte muito diluído, levando em conta a autoionização da água.
 * @param concentracao_acido Concentração do ácido em mol/L (> 0).
 * @param point_ph           Recebe o pH calculado.
 * @return ::PH_OK, ::PH_ERRO_PONTEIRO_NULO ou ::PH_ERRO_CONC_INVALIDA.
 * @note Usa Kw = 1,0·10⁻¹⁴ (25 °C).
 */
PhStatus ph_acido_diluido(double concentracao_acido, double *point_ph);

#endif /* PH_H */
