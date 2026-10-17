#include "teste.h"
#include "elementos.h"

int main(void)
{
    printf("Modulo elementos\n");

    /* elemento_nome */
    VERIFICA(IGUAIS(elemento_nome("O"), "Oxigenio"), "nome de O e Oxigenio");
    VERIFICA(IGUAIS(elemento_nome("Og"), "Oganessonio"), "nome do ultimo elemento (Og)");
    VERIFICA(elemento_nome("Xx") == NULL, "nome de simbolo inexistente e NULL");
    VERIFICA(elemento_nome("o") == NULL, "simbolo em minuscula nao e encontrado");
    VERIFICA(elemento_nome(NULL) == NULL, "nome com argumento NULL e NULL");

    /* elemento_numero_atomico */
    VERIFICA(elemento_numero_atomico("H") == 1, "numero atomico de H e 1");
    VERIFICA(elemento_numero_atomico("Sc") == 21, "numero atomico de Sc e 21");
    VERIFICA(elemento_numero_atomico("Og") == 118, "numero atomico de Og e 118");
    VERIFICA(elemento_numero_atomico("Xx") == -1, "numero atomico de simbolo inexistente e -1");
    VERIFICA(elemento_numero_atomico(NULL) == -1, "numero atomico com argumento NULL e -1");

    /* elemento_massa_atomica */
    VERIFICA(PERTO(elemento_massa_atomica("O"), 15.999, 1e-9), "massa atomica de O e 15,999");
    VERIFICA(PERTO(elemento_massa_atomica("Ar"), 39.948, 1e-9), "massa atomica de Ar e 39,948");
    VERIFICA(elemento_massa_atomica("Xx") == -1.0, "massa de simbolo inexistente e -1");
    VERIFICA(elemento_massa_atomica(NULL) == -1.0, "massa com argumento NULL e -1");

    /* elemento_grupo */
    VERIFICA(elemento_grupo("He") == 18, "grupo de He e 18");
    VERIFICA(elemento_grupo("Na") == 1, "grupo de Na e 1");
    VERIFICA(elemento_grupo("La") == 3 && elemento_grupo("U") == 3, "lantanideos e actinideos ficam no grupo 3");
    VERIFICA(elemento_grupo("Xx") == -1, "grupo de simbolo inexistente e -1");

    /* elemento_periodo */
    VERIFICA(elemento_periodo("H") == 1, "periodo de H e 1");
    VERIFICA(elemento_periodo("Og") == 7, "periodo de Og e 7");
    VERIFICA(elemento_periodo(NULL) == -1, "periodo com argumento NULL e -1");

    /* elemento_classificacao */
    VERIFICA(IGUAIS(elemento_classificacao("He"), "Gas nobre"), "He e gas nobre");
    VERIFICA(IGUAIS(elemento_classificacao("Cl"), "Halogenio"), "Cl e halogenio");
    VERIFICA(IGUAIS(elemento_classificacao("Fe"), "Metal de transicao"), "Fe e metal de transicao");
    VERIFICA(elemento_classificacao("Xx") == NULL, "classificacao de simbolo inexistente e NULL");

    /* elemento_simbolo */
    VERIFICA(IGUAIS(elemento_simbolo("Fosforo"), "P"), "simbolo de Fosforo e P");
    VERIFICA(IGUAIS(elemento_simbolo("Ferro"), "Fe"), "simbolo de Ferro e Fe");
    VERIFICA(elemento_simbolo("fosforo") == NULL, "nome em minuscula nao e encontrado");
    VERIFICA(elemento_simbolo("Fósforo") == NULL, "nome com acento nao e encontrado");
    VERIFICA(elemento_simbolo(NULL) == NULL, "simbolo com argumento NULL e NULL");

    return fim_dos_testes("Elementos");
}
