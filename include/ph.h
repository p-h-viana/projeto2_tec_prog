#ifndef PH_H
#define PH_H

typedef enum {
    SUCESSO_PH = 0,
    ERRO_CONC_NEGAT = 1,  /* Para concentrações <= 0 */
    ERRO_KA_NEGAT = 2     /* Para quando o Ka for <= 0 */
} PhStatus;

/**
 * @brief Calcula o pH de um ácido forte padrão.
 * 
 * @param concentracao_h Concentração do ácido H+ em mol/L.
 * @param point_ph Ponteiro onde o resultado numérico do pH será salvo.
 * @return PhStatus Retorna SUCESSO_PH em caso de sucesso ou erro apropriado.
 */
PhStatus calc_acid_forte(double concentracao_h, double *point_ph);

/**
 * @brief Calcula o pH de um ácido fraco usando a constante de acidez.
 * 
 * @param concentracao Concentração inicial do ácido em mol/L.
 * @param ka Constante de acidez Ka do ácido fraco.
 * @param point_ph Ponteiro onde o resultado numérico do pH será salvo.
 * @return PhStatus Retorna SUCESSO_PH em caso de sucesso ou erro apropriado.
 */
PhStatus calc_acid_fraco(double concentracao, double ka, double *point_ph);

/**
 * @brief Equação de Henderson-Hasselbalch.
 * 
 * @param pka Valor pKa do ácido fraco.
 * @param concentracao_sal Concentração do sal em mol/L.
 * @param concentracao_acido Concentração do ácido em mol/L.
 * @param point_ph Ponteiro onde o resultado numérico do pH será salvo.
 * @return PhStatus Retorna SUCESSO_PH em caso de sucesso ou erro apropriado.
 */
PhStatus calc_henderson_hasselbalch(double pka, double concentracao_sal, double concentracao_acido, double *point_ph);

/**
 * @brief calcula o pH de Ácido Forte Ultra-diluído
 * 
 * @param concentracao_acido Concentração do ácido em mol/L.
 * @param point_ph Ponteiro onde o resultado numérico do pH será salvo.
 * @return PhStatus Retorna SUCESSO_PH em caso de sucesso ou erro apropriado.
 */
PhStatus ph_acid_diluido(double concentracao_acido, double *point_ph);


#endif 