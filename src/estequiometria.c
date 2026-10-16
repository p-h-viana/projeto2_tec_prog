/**
 * @file estequiometria.c
 * @brief Implementação do módulo de estequiometria.
 *
 * @details Tudo o que é @c static aqui é interno ao módulo:
 * - a tabela com os 118 elementos e suas massas atômicas;
 * - a leitura das fórmulas, que conta os átomos de cada elemento;
 * - a leitura da equação, que separa reagentes e produtos;
 * - o balanceamento, que monta a matriz elemento × substância (produtos com
 *   sinal negativo) e resolve A·x = 0 por eliminação de Gauss-Jordan só com
 *   inteiros, para não ter erro de arredondamento.
 */

#include "estequiometria.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define NUM_ELEMENTOS    118        
#define MAX_PROFUNDIDADE 8          
#define MAX_INDICE       9999L      
#define MAX_ATOMOS       99999L      
#define MAX_VALOR        (1LL << 30) 

/** @brief Elemento da tabela periódica. */
typedef struct {
    const char *simbolo; /**< Símbolo químico, ex.: "Fe". */
    double massa;        /**< Massa atômica em g/mol. */
} Elemento;

/**
 * @brief Tabela periódica em ordem de número atômico.
 *
 * Massas atômicas padrão (valores convencionais da IUPAC). Para os elementos
 * sem massa padrão, usa-se o número de massa do isótopo mais estável.
 */
static const Elemento TABELA[NUM_ELEMENTOS] = {
    {"H", 1.008},   {"He", 4.0026}, {"Li", 6.94},   {"Be", 9.0122}, {"B", 10.81},
    {"C", 12.011},  {"N", 14.007},  {"O", 15.999},  {"F", 18.998},  {"Ne", 20.180},
    {"Na", 22.990}, {"Mg", 24.305}, {"Al", 26.982}, {"Si", 28.085}, {"P", 30.974},
    {"S", 32.06},   {"Cl", 35.45},  {"Ar", 39.95},  {"K", 39.098},  {"Ca", 40.078},
    {"Sc", 44.956}, {"Ti", 47.867}, {"V", 50.942},  {"Cr", 51.996}, {"Mn", 54.938},
    {"Fe", 55.845}, {"Co", 58.933}, {"Ni", 58.693}, {"Cu", 63.546}, {"Zn", 65.38},
    {"Ga", 69.723}, {"Ge", 72.630}, {"As", 74.922}, {"Se", 78.971}, {"Br", 79.904},
    {"Kr", 83.798}, {"Rb", 85.468}, {"Sr", 87.62},  {"Y", 88.906},  {"Zr", 91.224},
    {"Nb", 92.906}, {"Mo", 95.95},  {"Tc", 98.0},   {"Ru", 101.07}, {"Rh", 102.91},
    {"Pd", 106.42}, {"Ag", 107.87}, {"Cd", 112.41}, {"In", 114.82}, {"Sn", 118.71},
    {"Sb", 121.76}, {"Te", 127.60}, {"I", 126.90},  {"Xe", 131.29}, {"Cs", 132.91},
    {"Ba", 137.33}, {"La", 138.91}, {"Ce", 140.12}, {"Pr", 140.91}, {"Nd", 144.24},
    {"Pm", 145.0},  {"Sm", 150.36}, {"Eu", 151.96}, {"Gd", 157.25}, {"Tb", 158.93},
    {"Dy", 162.50}, {"Ho", 164.93}, {"Er", 167.26}, {"Tm", 168.93}, {"Yb", 173.05},
    {"Lu", 174.97}, {"Hf", 178.49}, {"Ta", 180.95}, {"W", 183.84},  {"Re", 186.21},
    {"Os", 190.23}, {"Ir", 192.22}, {"Pt", 195.08}, {"Au", 196.97}, {"Hg", 200.59},
    {"Tl", 204.38}, {"Pb", 207.2},  {"Bi", 208.98}, {"Po", 209.0},  {"At", 210.0},
    {"Rn", 222.0},  {"Fr", 223.0},  {"Ra", 226.0},  {"Ac", 227.0},  {"Th", 232.04},
    {"Pa", 231.04}, {"U", 238.03},  {"Np", 237.0},  {"Pu", 244.0},  {"Am", 243.0},
    {"Cm", 247.0},  {"Bk", 247.0},  {"Cf", 251.0},  {"Es", 252.0},  {"Fm", 257.0},
    {"Md", 258.0},  {"No", 259.0},  {"Lr", 266.0},  {"Rf", 267.0},  {"Db", 268.0},
    {"Sg", 269.0},  {"Bh", 270.0},  {"Hs", 269.0},  {"Mt", 278.0},  {"Ds", 281.0},
    {"Rg", 282.0},  {"Cn", 285.0},  {"Nh", 286.0},  {"Fl", 289.0},  {"Mc", 290.0},
    {"Lv", 293.0},  {"Ts", 294.0},  {"Og", 294.0}
};

/** @brief Substância de uma equação. */
typedef struct {
    const char *formula;        /**< Fórmula sem coeficiente (aponta para dentro de Equacao::texto). */
    long atomos[NUM_ELEMENTOS]; /**< Átomos de cada elemento, na ordem da tabela. */
} Especie;

/** @brief Equação já separada em substâncias. */
typedef struct {
    char texto[ESTQ_MAX_EQUACAO + 1];  /**< Cópia da entrada, cortada em pedaços. */
    Especie esp[ESTQ_MAX_ESPECIES];    /**< Reagentes primeiro, depois produtos. */
    size_t n_reagentes;                /**< Quantas substâncias são reagentes. */
    size_t n_especies;                 /**< Total de substâncias. */
    long long coef[ESTQ_MAX_ESPECIES]; /**< Coeficientes calculados por balancear(). */
} Equacao;

/**
 * @brief Procura um símbolo na tabela.
 * @param sim Início do símbolo.
 * @param tam Número de caracteres do símbolo.
 * @return Posição do elemento na tabela, ou -1 se não existir.
 */
static int buscar_elemento(const char *sim, size_t tam)
{
    int i;

    for (i = 0; i < NUM_ELEMENTOS; i++)
        if (strlen(TABELA[i].simbolo) == tam && strncmp(TABELA[i].simbolo, sim, tam) == 0)
            return i;
    return -1;
}

/**
 * @brief Lê o índice que vem depois de um símbolo ou de um grupo (o 2 de H2O).
 * @param p Posição atual na fórmula; avança sobre os dígitos.
 * @param n Recebe o índice lido, ou 1 se não houver dígitos.
 * @return ::ESTQ_OK, ::ESTQ_ERRO_SINTAXE se o índice for 0 ou ::ESTQ_ERRO_LIMITE
 *         se passar de #MAX_INDICE.
 */
static EstqStatus ler_indice(const char **p, long *n)
{
    *n = 1;
    if (!isdigit((unsigned char)**p))
        return ESTQ_OK;

    *n = 0;
    while (isdigit((unsigned char)**p)) {
        *n = *n * 10 + (**p - '0');
        if (*n > MAX_INDICE)
            return ESTQ_ERRO_LIMITE;
        (*p)++;
    }
    return *n == 0 ? ESTQ_ERRO_SINTAXE : ESTQ_OK;
}

/**
 * @brief Lê elementos e grupos até encontrar o caractere @p fecha.
 *
 * Grupos entre parênteses ou colchetes são lidos por recursão e somados
 * multiplicados pelo índice que vem depois deles.
 *
 * @param p      Posição atual na fórmula; avança durante a leitura.
 * @param fecha  ')' ou ']' dentro de um grupo; no nível de fora, o fim da string.
 * @param atomos Vetor onde as quantidades de átomos são somadas.
 * @param prof   Profundidade atual de parênteses.
 * @return ::ESTQ_OK ou o código do erro encontrado.
 */
static EstqStatus ler_grupo(const char **p, char fecha, long atomos[], int prof)
{
    int vazio = 1;
    long n;
    EstqStatus st;

    while (**p != fecha) {
        if (isupper((unsigned char)**p)) {
            const char *ini = (*p)++;
            int el;

            if (islower((unsigned char)**p))
                (*p)++;
            el = buscar_elemento(ini, (size_t)(*p - ini));
            if (el < 0)
                return ESTQ_ERRO_ELEMENTO;
            if ((st = ler_indice(p, &n)) != ESTQ_OK)
                return st;
            atomos[el] += n;
            if (atomos[el] > MAX_ATOMOS)
                return ESTQ_ERRO_LIMITE;
        } else if (**p == '(' || **p == '[') {
            long sub[NUM_ELEMENTOS] = {0};
            char fim = (**p == '(') ? ')' : ']';
            int i;

            if (prof == MAX_PROFUNDIDADE)
                return ESTQ_ERRO_LIMITE;
            (*p)++;
            if ((st = ler_grupo(p, fim, sub, prof + 1)) != ESTQ_OK)
                return st;
            (*p)++; /* pula o ')' ou ']' */
            if ((st = ler_indice(p, &n)) != ESTQ_OK)
                return st;
            for (i = 0; i < NUM_ELEMENTOS; i++) {
                atomos[i] += sub[i] * n;
                if (atomos[i] > MAX_ATOMOS)
                    return ESTQ_ERRO_LIMITE;
            }
        } else {
            return ESTQ_ERRO_SINTAXE; /* inclui grupo que não foi fechado */
        }
        vazio = 0;
    }
    return vazio ? ESTQ_ERRO_SINTAXE : ESTQ_OK;
}

/**
 * @brief Tira os espaços do começo e do fim de uma string.
 * @param s String a ser aparada (é modificada).
 * @return Ponteiro para o primeiro caractere que não é espaço.
 */
static char *aparar(char *s)
{
    char *fim;

    while (isspace((unsigned char)*s))
        s++;
    fim = s + strlen(s);
    while (fim > s && isspace((unsigned char)fim[-1]))
        fim--;
    *fim = '\0';
    return s;
}
/**
 * @brief Lê um termo da equação (ex.: "2H2O") e acrescenta a substância em @p eq.
 * @param termo Texto do termo; o coeficiente na frente é ignorado.
 * @param eq    Equação que recebe a substância.
 * @return ::ESTQ_OK ou o código do erro encontrado.
 */
static EstqStatus ler_termo(char *termo, Equacao *eq)
{
    Especie *e;
    const char *p;
    EstqStatus st;

    termo = aparar(termo);
    while (isdigit((unsigned char)*termo)) /* coeficiente digitado: ignorado */
        termo++;
    while (isspace((unsigned char)*termo))
        termo++;
    if (*termo == '\0')
        return ESTQ_ERRO_SINTAXE;
    if (eq->n_especies == ESTQ_MAX_ESPECIES)
        return ESTQ_ERRO_LIMITE;

    e = &eq->esp[eq->n_especies];
    e->formula = termo;
    memset(e->atomos, 0, sizeof e->atomos);
    p = termo;
    if ((st = ler_grupo(&p, '\0', e->atomos, 0)) != ESTQ_OK)
        return st;
    eq->n_especies++;
    return ESTQ_OK;
}
/**
 * @brief Lê um lado da equação, com os termos separados por '+'.
 * @param lado Texto do lado (é modificado).
 * @param eq   Equação que recebe as substâncias.
 * @return ::ESTQ_OK ou o código do erro encontrado.
 */
static EstqStatus ler_lado(char *lado, Equacao *eq)
{
    for (;;) {
        char *mais = strchr(lado, '+');
        EstqStatus st;

        if (mais != NULL)
            *mais = '\0';
        st = ler_termo(lado, eq);
        if (st != ESTQ_OK || mais == NULL)
            return st;
        lado = mais + 1;
    }
}
/**
 * @brief Separa a equação em reagentes e produtos e lê todas as substâncias.
 * @param texto Equação digitada.
 * @param eq    Estrutura preenchida com as substâncias.
 * @return ::ESTQ_OK ou o código do erro encontrado.
 */
static EstqStatus ler_equacao(const char *texto, Equacao *eq)
{
    /* "->", "=>", a seta em UTF-8 e "="; o "=" fica por último porque faz parte de "=>" */
    static const char *const separadores[] = {"->", "=>", "\xE2\x86\x92", "="};
    char *sep = NULL;
    size_t i, tam_sep = 0;
    EstqStatus st;

    if (strlen(texto) > ESTQ_MAX_EQUACAO)
        return ESTQ_ERRO_LIMITE;
    strcpy(eq->texto, texto);
    eq->n_especies = 0;

    for (i = 0; i < sizeof separadores / sizeof separadores[0] && sep == NULL; i++) {
        sep = strstr(eq->texto, separadores[i]);
        tam_sep = strlen(separadores[i]);
    }
    if (sep == NULL)
        return ESTQ_ERRO_SINTAXE;
    *sep = '\0';

    if ((st = ler_lado(eq->texto, eq)) != ESTQ_OK)
        return st;
    eq->n_reagentes = eq->n_especies;
    return ler_lado(sep + tam_sep, eq);
}
