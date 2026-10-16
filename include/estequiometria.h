/**
 * @file estequiometria.h
 * @brief Interface pública do módulo de estequiometria.
 *
 * @details As funções recebem a equação química como texto, no formato
 * `reagentes -> produtos`, com as substâncias de cada lado separadas por `+`.
 *
 * Regras de escrita:
 * - o separador pode ser `->`, `=>`, `=` ou `→`;
 * - cada substância pode ter um coeficiente na frente (`2H2O`), mas ele é
 *   ignorado, porque o balanceamento é sempre recalculado;
 * - as fórmulas aceitam parênteses e colchetes, inclusive aninhados
 *   (`Ca3(PO4)2`, `K4[Fe(CN)6]`);
 * - hidratos são escritos com parênteses (`CuSO4(H2O)5`);
 * - estados físicos (`(aq)`, `(s)`...) e cargas de íons não são aceitos.
 *
 * Todas as funções que podem falhar devolvem um ::EstqStatus. Os resultados
 * saem por parâmetros de saída, que só são alterados em caso de sucesso
 * (a exceção é o buffer de estq_balancear(), que fica vazio em caso de erro).
 */

#ifndef ESTEQUIOMETRIA_H
#define ESTEQUIOMETRIA_H

#include <stddef.h>

/**
 * @defgroup estequiometria Estequiometria
 * @brief Balanceamento de equações, massa de produto e rendimento.
 * @{
 */

/** @brief Número máximo de caracteres de uma equação. */
#define ESTQ_MAX_EQUACAO 512

/** @brief Número máximo de substâncias (reagentes + produtos) em uma equação. */
#define ESTQ_MAX_ESPECIES 16

/** @brief Códigos de retorno das funções do módulo. */
typedef enum {
    ESTQ_OK = 0,             /**< Operação concluída com sucesso. */
    ESTQ_ERRO_PARAMETRO,     /**< Ponteiro nulo, tamanho inválido ou valor fora do domínio. */
    ESTQ_ERRO_SINTAXE,       /**< Equação ou fórmula mal escrita. */
    ESTQ_ERRO_ELEMENTO,      /**< Símbolo que não corresponde a nenhum elemento químico. */
    ESTQ_ERRO_BALANCEAMENTO, /**< Equação impossível de balancear ou com mais de um balanceamento. */
    ESTQ_ERRO_PRODUTO,       /**< Substância pedida não está entre os produtos da equação. */
    ESTQ_ERRO_LIMITE         /**< Algum limite do módulo foi excedido (ver cada função). */
} EstqStatus;

/**
 * @brief Balanceia uma equação química.
 *
 * @details Encontra os menores coeficientes inteiros positivos que conservam
 * o número de átomos de cada elemento. A equação balanceada é escrita com
 * ` -> ` como separador, sem o coeficiente 1 e com as fórmulas do jeito que
 * foram digitadas.
 *
 * @code
 * char eq[128];
 * if (estq_balancear("Fe + O2 -> Fe2O3", eq, sizeof eq) == ESTQ_OK)
 *     printf("%s\n", eq);                     // 4Fe + 3O2 -> 2Fe2O3
 * @endcode
 *
 * @param[in]  equacao   Equação a balancear.
 * @param[out] saida     Buffer que recebe a equação balanceada.
 * @param[in]  tam_saida Tamanho de @p saida em bytes, contando o terminador.
 *
 * @return ::ESTQ_OK se a equação foi balanceada; caso contrário, um dos erros abaixo.
 * @retval ESTQ_ERRO_PARAMETRO     @p equacao ou @p saida nulo, ou @p tam_saida igual a 0.
 * @retval ESTQ_ERRO_SINTAXE       Falta o separador, há termo vazio, parêntese sem par,
 *                                 índice 0 ou caractere inválido.
 * @retval ESTQ_ERRO_ELEMENTO      Uma fórmula usa símbolo inexistente (ex.: `Xy`).
 * @retval ESTQ_ERRO_BALANCEAMENTO Nenhum conjunto de coeficientes positivos balanceia a
 *                                 equação, ou existe mais de um independente
 *                                 (ex.: `H2 + O2 -> H2O + H2O2`).
 * @retval ESTQ_ERRO_LIMITE        Equação com mais de ::ESTQ_MAX_EQUACAO caracteres ou mais
 *                                 de ::ESTQ_MAX_ESPECIES substâncias, parênteses aninhados
 *                                 demais, índices ou coeficientes grandes demais, ou
 *                                 @p saida pequeno demais para o resultado.
 *
 * @note Em caso de erro, @p saida recebe uma string vazia (quando não é nulo e
 *       @p tam_saida é maior que 0).
 */
EstqStatus estq_balancear(const char *equacao, char *saida, size_t tam_saida);

/**
 * @brief Calcula a massa de um produto a partir das massas dos reagentes.
 *
 * @details A equação é balanceada internamente. Para cada reagente calcula-se
 * a quantidade de matéria (n = m / M) dividida pelo seu coeficiente; o menor
 * valor indica o reagente limitante, que define quanto produto se forma.
 * A reação é considerada completa, então o valor devolvido é a massa teórica
 * (rendimento de 100 por cento). As massas molares são calculadas com as
 * massas atômicas padrão dos elementos.
 *
 * @code
 * double massas[] = {10.0, 32.0};             // 10 g de H2 e 32 g de O2
 * double agua;
 * size_t lim;
 * if (estq_massa_produto("H2 + O2 -> H2O", massas, 2, "H2O", &agua, &lim) == ESTQ_OK)
 *     printf("%.2f g (limitante: %zu)\n", agua, lim);   // 36.03 g (limitante: 1)
 * @endcode
 *
 * @param[in]  equacao       Equação da reação, balanceada ou não.
 * @param[in]  massas        Massas dos reagentes em gramas, na ordem em que aparecem na equação.
 * @param[in]  n_massas      Quantidade de valores em @p massas; deve ser igual ao número de reagentes.
 * @param[in]  produto       Fórmula do produto desejado, escrita como na equação e sem coeficiente.
 * @param[out] massa_produto Massa do produto formada, em gramas.
 * @param[out] limitante     Índice, em @p massas, do reagente limitante (em caso de empate,
 *                           o de menor índice). Pode ser NULL se não interessar.
 *
 * @return ::ESTQ_OK em caso de sucesso; caso contrário, um dos erros abaixo.
 * @retval ESTQ_ERRO_PARAMETRO     @p equacao, @p massas, @p produto ou @p massa_produto nulo;
 *                                 @p n_massas diferente do número de reagentes; ou alguma
 *                                 massa negativa, infinita ou NaN.
 * @retval ESTQ_ERRO_PRODUTO       @p produto não aparece do lado dos produtos.
 * @retval ESTQ_ERRO_SINTAXE       Equação mal escrita (ver estq_balancear()).
 * @retval ESTQ_ERRO_ELEMENTO      Símbolo inexistente (ver estq_balancear()).
 * @retval ESTQ_ERRO_BALANCEAMENTO Equação sem balanceamento único (ver estq_balancear()).
 * @retval ESTQ_ERRO_LIMITE        Limites da equação excedidos (ver estq_balancear()).
 */
EstqStatus estq_massa_produto(const char *equacao, const double massas[], size_t n_massas,
                              const char *produto, double *massa_produto, size_t *limitante);

/**
 * @brief Calcula o rendimento percentual de uma reação.
 *
 * @details rendimento = massa real / massa teórica × 100. As duas massas devem
 * estar na mesma unidade; a massa teórica normalmente vem de estq_massa_produto().
 *
 * @code
 * double r;
 * if (estq_rendimento(27.0, 36.03, &r) == ESTQ_OK)
 *     printf("%.1f%%\n", r);                  // 74.9%
 * @endcode
 *
 * @param[in]  massa_real    Massa obtida na prática (maior ou igual a zero).
 * @param[in]  massa_teorica Massa prevista pela estequiometria (maior que zero).
 * @param[out] rendimento    Rendimento em porcentagem, entre 0 e 100.
 *
 * @return ::ESTQ_OK em caso de sucesso; caso contrário, ::ESTQ_ERRO_PARAMETRO.
 * @retval ESTQ_ERRO_PARAMETRO @p rendimento nulo; massa infinita ou NaN; @p massa_teorica
 *                             menor ou igual a zero; @p massa_real negativa; ou
 *                             @p massa_real maior que @p massa_teorica (rendimento acima
 *                             de 100 por cento, que indica erro de medida ou produto impuro).
 */
EstqStatus estq_rendimento(double massa_real, double massa_teorica, double *rendimento);

/**
 * @brief Devolve uma descrição em texto de um código de retorno.
 *
 * @code
 * EstqStatus st = estq_balancear("H2 + O2", eq, sizeof eq);
 * if (st != ESTQ_OK)
 *     fprintf(stderr, "erro: %s\n", estq_mensagem_erro(st));
 * @endcode
 *
 * @param[in] status Código devolvido por uma função do módulo.
 *
 * @return String constante com a descrição; nunca é NULL. Para um código
 *         desconhecido devolve "erro desconhecido". A string não deve ser
 *         alterada nem liberada.
 */
const char *estq_mensagem_erro(EstqStatus status);

/** @} */

#endif /* ESTEQUIOMETRIA_H */
