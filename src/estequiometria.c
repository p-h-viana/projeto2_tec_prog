/**
 * @file estequiometria.c
 * @brief Implementação do módulo de estequiometria.
 *
 * @details Tudo o que é @c static aqui é interno ao módulo:
 * - a leitura das fórmulas, que conta os átomos de cada elemento;
 * - a leitura da equação, que separa reagentes e produtos;
 * - o balanceamento, que monta a matriz elemento × substância (produtos com
 *   sinal negativo) e resolve A·x = 0 por eliminação de Gauss-Jordan só com
 *   inteiros, para não ter erro de arredondamento.
 *
 * Os símbolos e as massas atômicas vêm do módulo de elementos, para que a
 * biblioteca inteira use a mesma tabela periódica.
 */
#include "estequiometria.h"
#include "elementos.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define NUM_ELEMENTOS    118         /* tamanho da tabela de elementos.c */
#define MAX_PROFUNDIDADE 8
#define MAX_INDICE       9999L
#define MAX_ATOMOS       99999L
#define MAX_VALOR        (1LL << 30)

/** @brief Substância de uma equação. */
typedef struct {
    const char *formula;        /**< Fórmula sem coeficiente (aponta para dentro de Equacao::texto). */
    long atomos[NUM_ELEMENTOS]; /**< Átomos de cada elemento; a posição é o número atômico - 1. */
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
 * @brief Procura um símbolo na tabela periódica.
 * @param sim Início do símbolo.
 * @param tam Número de caracteres do símbolo.
 * @return Número atômico - 1 (posição em Especie::atomos), ou -1 se não existir.
 */
static int buscar_elemento(const char *sim, size_t tam)
{
    char simbolo[3];
    int z;

    if (tam > 2)
        return -1;
    memcpy(simbolo, sim, tam);
    simbolo[tam] = '\0';
    z = elemento_numero_atomico(simbolo);
    return z > 0 ? z - 1 : -1;
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

/**
 * @brief Máximo divisor comum (sempre não negativo).
 * @param a Primeiro número.
 * @param b Segundo número.
 * @return mdc(|a|, |b|); vale 0 só se os dois forem 0.
 */
static long long mdc(long long a, long long b)
{
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/**
 * @brief Calcula os coeficientes da equação.
 *
 * Monta a matriz A (uma linha por elemento, uma coluna por substância, com os
 * produtos negativos) e procura x com A·x = 0. Depois da eliminação, a equação
 * só tem balanceamento único se sobrar exatamente uma variável livre; os
 * coeficientes saem dela e precisam ser todos positivos.
 *
 * @param eq Equação já lida; o resultado vai para @c eq->coef.
 * @return ::ESTQ_OK, ::ESTQ_ERRO_BALANCEAMENTO ou ::ESTQ_ERRO_LIMITE.
 */
static EstqStatus balancear(Equacao *eq)
{
    long long m[NUM_ELEMENTOS][ESTQ_MAX_ESPECIES];
    size_t piv[ESTQ_MAX_ESPECIES]; /* coluna do pivô de cada linha */
    int eh_pivo[ESTQ_MAX_ESPECIES] = {0};
    size_t n = eq->n_especies, linhas = 0, posto = 0, livre = 0;
    size_t i, j, k, col;
    long long mmc = 1, g;
    int el;

    /* uma linha para cada elemento que aparece na equação */
    for (el = 0; el < NUM_ELEMENTOS; el++) {
        int presente = 0;

        for (j = 0; j < n; j++) {
            long long v = eq->esp[j].atomos[el];

            m[linhas][j] = (j < eq->n_reagentes) ? v : -v;
            if (v != 0)
                presente = 1;
        }
        if (presente)
            linhas++;
    }

    for (col = 0; col < n && posto < linhas; col++) {
        i = posto;
        while (i < linhas && m[i][col] == 0)
            i++;
        if (i == linhas)
            continue; /* coluna sem pivô: variável livre */

        for (k = 0; k < n; k++) { /* troca a linha i com a linha do pivô */
            long long t = m[i][k];
            m[i][k] = m[posto][k];
            m[posto][k] = t;
        }

        for (i = 0; i < linhas; i++) { /* zera a coluna nas outras linhas */
            long long a = m[posto][col], b = m[i][col];

            if (i == posto || b == 0)
                continue;
            g = 0;
            for (k = 0; k < n; k++) {
                m[i][k] = m[i][k] * a - m[posto][k] * b;
                g = mdc(g, m[i][k]);
            }
            for (k = 0; k < n; k++) {
                if (g > 1)
                    m[i][k] /= g;
                if (m[i][k] > MAX_VALOR || m[i][k] < -MAX_VALOR)
                    return ESTQ_ERRO_LIMITE;
            }
        }
        piv[posto++] = col;
        eh_pivo[col] = 1;
    }

    if (n - posto != 1)
        return ESTQ_ERRO_BALANCEAMENTO; /* nenhuma solução ou várias independentes */
    while (eh_pivo[livre])
        livre++;

    /* a variável livre vale o mmc dos pivôs, assim as outras saem inteiras */
    for (i = 0; i < posto; i++) {
        long long p = m[i][piv[i]] < 0 ? -m[i][piv[i]] : m[i][piv[i]];

        mmc = mmc / mdc(mmc, p) * p;
        if (mmc > MAX_VALOR)
            return ESTQ_ERRO_LIMITE;
    }
    eq->coef[livre] = mmc;
    for (i = 0; i < posto; i++)
        eq->coef[piv[i]] = -m[i][livre] * (mmc / m[i][piv[i]]);

    /* simplifica e confere se ninguém ficou com coeficiente zero ou negativo */
    g = 0;
    for (j = 0; j < n; j++)
        g = mdc(g, eq->coef[j]);
    for (j = 0; j < n; j++) {
        eq->coef[j] /= g;
        if (eq->coef[j] <= 0)
            return ESTQ_ERRO_BALANCEAMENTO;
    }
    return ESTQ_OK;
}

/**
 * @brief Escreve a equação balanceada em @p saida.
 * @param eq    Equação com os coeficientes já calculados.
 * @param saida Buffer de saída.
 * @param tam   Tamanho de @p saida em bytes.
 * @return ::ESTQ_OK ou ::ESTQ_ERRO_LIMITE se o resultado não couber.
 */
static EstqStatus escrever(const Equacao *eq, char *saida, size_t tam)
{
    size_t j, usado = 0;

    for (j = 0; j < eq->n_especies; j++) {
        const char *sep = (j == 0) ? "" : (j == eq->n_reagentes) ? " -> " : " + ";
        int k;

        if (eq->coef[j] == 1)
            k = snprintf(saida + usado, tam - usado, "%s%s", sep, eq->esp[j].formula);
        else
            k = snprintf(saida + usado, tam - usado, "%s%lld%s", sep, eq->coef[j],
                         eq->esp[j].formula);
        if (k < 0 || (size_t)k >= tam - usado) {
            saida[0] = '\0';
            return ESTQ_ERRO_LIMITE;
        }
        usado += (size_t)k;
    }
    return ESTQ_OK;
}

/**
 * @brief Massa molar de uma substância.
 *
 * Os símbolos são lidos de novo da fórmula (que já foi validada) e cada
 * elemento entra na soma uma vez só, com o total de átomos já contado.
 *
 * @param e Substância já lida.
 * @return Massa molar em g/mol.
 */
static double massa_molar(const Especie *e)
{
    int visto[NUM_ELEMENTOS] = {0};
    const char *p;
    double m = 0.0;

    for (p = e->formula; *p != '\0'; p++) {
        char simbolo[3] = {0};
        int el;

        if (!isupper((unsigned char)*p))
            continue;
        simbolo[0] = p[0];
        if (islower((unsigned char)p[1]))
            simbolo[1] = p[1];
        el = elemento_numero_atomico(simbolo) - 1;
        if (el >= 0 && !visto[el]) {
            visto[el] = 1;
            m += e->atomos[el] * elemento_massa_atomica(simbolo);
        }
    }
    return m;
}

EstqStatus estq_balancear(const char *equacao, char *saida, size_t tam_saida)
{
    Equacao eq;
    EstqStatus st;

    if (equacao == NULL || saida == NULL || tam_saida == 0)
        return ESTQ_ERRO_PARAMETRO;
    saida[0] = '\0';

    if ((st = ler_equacao(equacao, &eq)) != ESTQ_OK)
        return st;
    if ((st = balancear(&eq)) != ESTQ_OK)
        return st;
    return escrever(&eq, saida, tam_saida);
}

EstqStatus estq_massa_produto(const char *equacao, const double massas[], size_t n_massas,
                              const char *produto, double *massa_produto, size_t *limitante)
{
    Equacao eq;
    EstqStatus st;
    size_t j, p, lim = 0;
    double menor = 0.0;

    if (equacao == NULL || massas == NULL || produto == NULL || massa_produto == NULL)
        return ESTQ_ERRO_PARAMETRO;
    if ((st = ler_equacao(equacao, &eq)) != ESTQ_OK)
        return st;
    if ((st = balancear(&eq)) != ESTQ_OK)
        return st;
    if (n_massas != eq.n_reagentes)
        return ESTQ_ERRO_PARAMETRO;

    for (p = eq.n_reagentes; p < eq.n_especies; p++)
        if (strcmp(eq.esp[p].formula, produto) == 0)
            break;
    if (p == eq.n_especies)
        return ESTQ_ERRO_PRODUTO;
    for (j = 0; j < n_massas; j++) {
        double x;

        if (!isfinite(massas[j]) || massas[j] < 0.0)
            return ESTQ_ERRO_PARAMETRO;
        x = massas[j] / massa_molar(&eq.esp[j]) / (double)eq.coef[j];
        if (j == 0 || x < menor) {
            menor = x;
            lim = j;
        }
    }

    *massa_produto = menor * (double)eq.coef[p] * massa_molar(&eq.esp[p]);
    if (limitante != NULL)
        *limitante = lim;
    return ESTQ_OK;
}

EstqStatus estq_rendimento(double massa_real, double massa_teorica, double *rendimento)
{
    if (rendimento == NULL || !isfinite(massa_real) || !isfinite(massa_teorica))
        return ESTQ_ERRO_PARAMETRO;
    if (massa_teorica <= 0.0 || massa_real < 0.0 || massa_real > massa_teorica)
        return ESTQ_ERRO_PARAMETRO;

    *rendimento = massa_real / massa_teorica * 100.0;
    return ESTQ_OK;
}

const char *estq_mensagem_erro(EstqStatus status)
{
    switch (status) {
    case ESTQ_OK:                 return "sucesso";
    case ESTQ_ERRO_PARAMETRO:     return "parametro invalido";
    case ESTQ_ERRO_SINTAXE:       return "equacao ou formula mal escrita";
    case ESTQ_ERRO_ELEMENTO:      return "elemento quimico desconhecido";
    case ESTQ_ERRO_BALANCEAMENTO: return "nao foi possivel balancear a equacao";
    case ESTQ_ERRO_PRODUTO:       return "produto nao encontrado na equacao";
    case ESTQ_ERRO_LIMITE:        return "limite do modulo excedido";
    }
    return "erro desconhecido";
}