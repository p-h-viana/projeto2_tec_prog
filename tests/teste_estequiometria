/**
 * @file test_estequiometria.c
 * @brief Testes automatizados do módulo de estequiometria.
 *
 * @details Cada função pública é testada com casos normais, valores limite
 * e entradas inválidas. O programa termina com código 0 se todos os testes
 * passarem e 1 se algum falhar.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "estequiometria.h"

static int total = 0;  /**< Verificações feitas. */
static int falhas = 0; /**< Verificações que falharam. */

/** @brief Conta uma verificação e mostra a linha e a expressão se ela falhar. */
#define VERIFICA(cond)                                            \
    do {                                                          \
        total++;                                                  \
        if (!(cond)) {                                            \
            falhas++;                                             \
            printf("  FALHOU (linha %d): %s\n", __LINE__, #cond); \
        }                                                         \
    } while (0)

/** @brief Diz se @p a e @p b diferem no máximo @p tol. */
static int perto(double a, double b, double tol)
{
    double d = a - b;
    return (d < 0 ? -d : d) <= tol;
}

/** @brief Diz se estq_balancear() transforma @p entrada exatamente em @p esperado. */
static int balanceia(const char *entrada, const char *esperado)
{
    char saida[256];
    return estq_balancear(entrada, saida, sizeof saida) == ESTQ_OK
           && strcmp(saida, esperado) == 0;
}

/** @brief Testes de estq_balancear(). */
static void teste_balancear(void)
{
    char saida[256], pequeno[8], longa[ESTQ_MAX_EQUACAO + 10];

    /* uso normal */
    VERIFICA(balanceia("H2 + O2 -> H2O", "2H2 + O2 -> 2H2O"));
    VERIFICA(balanceia("Fe + O2 -> Fe2O3", "4Fe + 3O2 -> 2Fe2O3"));
    VERIFICA(balanceia("C3H8 + O2 = CO2 + H2O", "C3H8 + 5O2 -> 3CO2 + 4H2O"));
    VERIFICA(balanceia("CH4 + O2 → CO2 + H2O", "CH4 + 2O2 -> CO2 + 2H2O"));
    VERIFICA(balanceia("Cu + HNO3 => Cu(NO3)2 + NO + H2O",
                       "3Cu + 8HNO3 -> 3Cu(NO3)2 + 2NO + 4H2O"));
    VERIFICA(balanceia("Ca(OH)2 + H3PO4 -> Ca3(PO4)2 + H2O",
                       "3Ca(OH)2 + 2H3PO4 -> Ca3(PO4)2 + 6H2O"));
    VERIFICA(balanceia("K4[Fe(CN)6] + KMnO4 + H2SO4 -> KHSO4 + Fe2(SO4)3 + MnSO4 + HNO3 + CO2 + H2O",
                       "10K4[Fe(CN)6] + 122KMnO4 + 299H2SO4 -> 162KHSO4 + 5Fe2(SO4)3"
                       " + 122MnSO4 + 60HNO3 + 60CO2 + 188H2O"));
    VERIFICA(balanceia("CuSO4(H2O)5 -> CuSO4 + H2O", "CuSO4(H2O)5 -> CuSO4 + 5H2O"));
    VERIFICA(balanceia("4H2 + 4O2 -> 3H2O", "2H2 + O2 -> 2H2O")); /* coeficientes da entrada são ignorados */
    VERIFICA(balanceia("  N2+H2->NH3 ", "N2 + 3H2 -> 2NH3"));      /* espaços não importam */

    /* entradas inválidas */
    VERIFICA(estq_balancear(NULL, saida, sizeof saida) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_balancear("H2 + O2 -> H2O", NULL, 10) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_balancear("H2 + O2 -> H2O", saida, 0) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_balancear("H2 + O2", saida, sizeof saida) == ESTQ_ERRO_SINTAXE);
    VERIFICA(estq_balancear("H2 + -> H2O", saida, sizeof saida) == ESTQ_ERRO_SINTAXE);
    VERIFICA(estq_balancear("Ca(OH2 -> CaO + H2O", saida, sizeof saida) == ESTQ_ERRO_SINTAXE);
    VERIFICA(estq_balancear("h2 + o2 -> h2o", saida, sizeof saida) == ESTQ_ERRO_SINTAXE);
    VERIFICA(estq_balancear("H0 + O2 -> H2O", saida, sizeof saida) == ESTQ_ERRO_SINTAXE);
    VERIFICA(estq_balancear("Xy + O2 -> XyO", saida, sizeof saida) == ESTQ_ERRO_ELEMENTO);
    VERIFICA(estq_balancear("H2O -> H2O2", saida, sizeof saida) == ESTQ_ERRO_BALANCEAMENTO);
    VERIFICA(estq_balancear("H2 + O2 -> H2O + H2O2", saida, sizeof saida) == ESTQ_ERRO_BALANCEAMENTO);
    VERIFICA(estq_balancear("NaCl -> Na + Cl2 + He", saida, sizeof saida) == ESTQ_ERRO_BALANCEAMENTO);

    /* limites */
    VERIFICA(estq_balancear("H2 + O2 -> H2O", pequeno, sizeof pequeno) == ESTQ_ERRO_LIMITE);
    VERIFICA(pequeno[0] == '\0'); /* em caso de erro a saída fica vazia */
    VERIFICA(estq_balancear("H + H + H + H + H + H + H + H + H + H + H + H + H + H + H + H -> H2",
                            saida, sizeof saida) == ESTQ_ERRO_LIMITE); /* 17 substâncias */
    memset(longa, ' ', sizeof longa - 1);
    longa[sizeof longa - 1] = '\0';
    memcpy(longa, "H2 + O2 -> H2O", 14);
    VERIFICA(estq_balancear(longa, saida, sizeof saida) == ESTQ_ERRO_LIMITE);
}

/** @brief Testes de estq_massa_produto(). */
static void teste_massa_produto(void)
{
    const double m_agua[] = {10.0, 32.0};
    const double m_agua2[] = {4.0, 64.0};
    const double m_ferro[] = {160.0, 100.0};
    const double m_calcario[] = {100.0};
    const double m_zero[] = {0.0, 32.0};
    const double m_neg[] = {10.0, -1.0};
    const double m_nan[] = {10.0, NAN};
    double massa = -1.0;
    size_t lim = 99;

    /* O2 limitante: 10 g de H2 + 32 g de O2 formam cerca de 36,03 g de água */
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_agua, 2, "H2O", &massa, &lim) == ESTQ_OK);
    VERIFICA(perto(massa, 36.03, 0.01) && lim == 1);

    /* H2 limitante */
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_agua2, 2, "H2O", &massa, &lim) == ESTQ_OK);
    VERIFICA(perto(massa, 35.74, 0.01) && lim == 0);

    /* equação não balanceada e produto que não é o primeiro */
    VERIFICA(estq_massa_produto("Fe2O3 + CO -> CO2 + Fe", m_ferro, 2, "Fe", &massa, &lim) == ESTQ_OK);
    VERIFICA(perto(massa, 111.91, 0.01) && lim == 0);

    /* um reagente só, sem pedir o limitante */
    VERIFICA(estq_massa_produto("CaCO3 -> CaO + CO2", m_calcario, 1, "CaO", &massa, NULL) == ESTQ_OK);
    VERIFICA(perto(massa, 56.03, 0.01));

    /* limite: reagente com massa zero não forma produto */
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_zero, 2, "H2O", &massa, &lim) == ESTQ_OK);
    VERIFICA(massa == 0.0 && lim == 0);

    /* entradas inválidas: a saída não pode ser alterada */
    massa = -1.0;
    VERIFICA(estq_massa_produto(NULL, m_agua, 2, "H2O", &massa, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", NULL, 2, "H2O", &massa, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_agua, 2, NULL, &massa, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_agua, 2, "H2O", NULL, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_agua, 1, "H2O", &massa, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_neg, 2, "H2O", &massa, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_nan, 2, "H2O", &massa, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_agua, 2, "O2", &massa, NULL) == ESTQ_ERRO_PRODUTO);
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m_agua, 2, "H2O2", &massa, NULL) == ESTQ_ERRO_PRODUTO);
    VERIFICA(estq_massa_produto("H2 + O2", m_agua, 2, "H2O", &massa, NULL) == ESTQ_ERRO_SINTAXE);
    VERIFICA(estq_massa_produto("H2O -> H2O2", m_calcario, 1, "H2O2", &massa, NULL) == ESTQ_ERRO_BALANCEAMENTO);
    VERIFICA(massa == -1.0);
}

/** @brief Testes de estq_rendimento(). */
static void teste_rendimento(void)
{
    const double m[] = {10.0, 32.0};
    double teorica = 0.0, r = 0.0;

    VERIFICA(estq_rendimento(27.0, 36.0, &r) == ESTQ_OK && perto(r, 75.0, 1e-9));
    VERIFICA(estq_rendimento(36.0, 36.0, &r) == ESTQ_OK && perto(r, 100.0, 1e-9)); /* limite superior */
    VERIFICA(estq_rendimento(0.0, 36.0, &r) == ESTQ_OK && perto(r, 0.0, 1e-9));    /* limite inferior */

    /* junto com estq_massa_produto: 27 g obtidos de 36,03 g teóricos */
    VERIFICA(estq_massa_produto("H2 + O2 -> H2O", m, 2, "H2O", &teorica, NULL) == ESTQ_OK);
    VERIFICA(estq_rendimento(27.0, teorica, &r) == ESTQ_OK && perto(r, 74.93, 0.01));

    /* entradas inválidas: a saída não pode ser alterada */
    r = -1.0;
    VERIFICA(estq_rendimento(40.0, 36.0, &r) == ESTQ_ERRO_PARAMETRO); /* acima de 100% */
    VERIFICA(estq_rendimento(-1.0, 36.0, &r) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_rendimento(10.0, 0.0, &r) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_rendimento(10.0, -5.0, &r) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_rendimento(NAN, 36.0, &r) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_rendimento(10.0, INFINITY, &r) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(estq_rendimento(10.0, 36.0, NULL) == ESTQ_ERRO_PARAMETRO);
    VERIFICA(r == -1.0);
}

/** @brief Testes de estq_mensagem_erro(). */
static void teste_mensagem_erro(void)
{
    int c;

    VERIFICA(strcmp(estq_mensagem_erro(ESTQ_OK), "sucesso") == 0);
    for (c = ESTQ_OK; c <= ESTQ_ERRO_LIMITE; c++) {
        const char *msg = estq_mensagem_erro((EstqStatus)c);
        VERIFICA(msg != NULL && strcmp(msg, "erro desconhecido") != 0);
    }
    VERIFICA(strcmp(estq_mensagem_erro(ESTQ_ERRO_SINTAXE), estq_mensagem_erro(ESTQ_ERRO_ELEMENTO)) != 0);
    VERIFICA(strcmp(estq_mensagem_erro((EstqStatus)99), "erro desconhecido") == 0);
}

/** @brief Roda um grupo de testes e mostra quantas verificações ele fez. */
static void rodar(const char *nome, void (*teste)(void))
{
    int total_antes = total, falhas_antes = falhas;

    teste();
    printf("%-20s %3d verificações  %s\n", nome, total - total_antes,
           falhas == falhas_antes ? "ok" : "FALHOU");
}

int main(void)
{
    printf("Testes do módulo de estequiometria\n");
    rodar("estq_balancear", teste_balancear);
    rodar("estq_massa_produto", teste_massa_produto);
    rodar("estq_rendimento", teste_rendimento);
    rodar("estq_mensagem_erro", teste_mensagem_erro);

    printf("Resultado: %d de %d verificações passaram -> %s\n", total - falhas, total,
           falhas == 0 ? "APROVADO" : "REPROVADO");
    return falhas == 0 ? 0 : 1;
}
