/**
 * @file elementos.h
 * @brief Consulta de informações sobre os elementos químicos da tabela periódica.
 *
 * Todas as funções recebem uma string entre aspas duplas e devolvem um dado
 * do elemento correspondente. A tabela é interna ao módulo e não pode ser
 * alterada pelo usuário.
 *
 * Convenções de entrada (a busca diferencia maiúsculas de minúsculas):
 * - Símbolo: no máximo duas letras, a primeira maiúscula e a segunda
 *   minúscula, sem acentos (ex.: "H", "Sc", "Og").
 * - Nome: apenas letras, sem acentos nem caracteres especiais, começando
 *   com maiúscula e o restante minúsculo (ex.: "Fosforo").
 *
 * Convenção de erro: se o elemento não for encontrado (ou o argumento for
 * NULL), as funções devolvem NULL para `const char*` e -1 para `int` e `double`.
 */

#ifndef ELEMENTOS_H
#define ELEMENTOS_H

/**
 * @brief Devolve o nome do elemento a partir do símbolo.
 * @param simbolo Símbolo do elemento (ex.: "O").
 * @return Nome do elemento (ex.: "Oxigenio"), ou NULL se não existir.
 */
const char* elemento_nome(const char* simbolo);

/**
 * @brief Devolve o número atômico do elemento.
 * @param simbolo Símbolo do elemento (ex.: "Sc").
 * @return Número atômico (ex.: 21), ou -1 se não existir.
 *
 * @code
 * printf("%d\n", elemento_numero_atomico("Sc"));   // imprime 21
 * @endcode
 */
int elemento_numero_atomico(const char* simbolo);

/**
 * @brief Devolve a massa atômica do elemento, em u.
 * @param simbolo Símbolo do elemento (ex.: "O").
 * @return Massa atômica (ex.: 15.999), ou -1 se não existir.
 */
double elemento_massa_atomica(const char* simbolo);

/**
 * @brief Devolve o grupo do elemento na tabela periódica.
 * @param simbolo Símbolo do elemento (ex: "He").
 * @return Grupo de 1 a 18 (ex: 18), ou -1 se não existir.
 * @note Lantanídeos e Actinídeos foram convencionalmente associados ao grupo 3.
 */
int elemento_grupo(const char* simbolo);

/**
 * @brief Devolve o período do elemento na tabela periódica.
 * @param simbolo Símbolo do elemento (ex: "Og").
 * @return Período de 1 a 7 (ex.: 7), ou -1 se não existir.
 */
int elemento_periodo(const char* simbolo);

/**
 * @brief Devolve a classificação do elemento (ex.: gás nobre, halogênio).
 * @param simbolo Símbolo do elemento (ex: "He").
 * @return Classificação química od elemento (ex: "Gas nobre"), ou NULL se não existir.
 */
const char* elemento_classificacao(const char* simbolo);

/**
 * @brief Devolve o símbolo do elemento a partir do nome.
 *
 * É a única função que recebe o nome, e não o símbolo.
 *
 * @param nome Nome do elemento (ex.: "Fosforo").
 * @return Símbolo do elemento (ex.: "P"), ou NULL se não existir.
 *
 * @code
 * printf("%s\n", elemento_simbolo("Fosforo"));   // imprime P
 * @endcode
 */
const char* elemento_simbolo(const char* nome);

#endif