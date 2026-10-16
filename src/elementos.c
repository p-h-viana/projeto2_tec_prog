#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "elementos.h"

typedef struct {
    char simbolo[3];
    char nome[30];
    int numero_atomico;
    double massa_atomica;
    int grupo;
    int periodo;
    char classificacao[30];
} Elemento;

/*
 * Tabela periódica dos 118 elementos químicos.
 *
 * Campos:
 * simbolo, nome, numero_atomico, massa_atomica, grupo, periodo, classificacao
 *
 * Para elementos sem peso atomico padrao, massa_atomica armazena
 * um numero de massa de referencia de um isotopo.
 *
 * Convencao adotada: lantanideos e actinideos recebem grupo 3.
 */
static const Elemento elementos[] = {
    {"H",  "Hidrogenio",       1,   1.008,    1, 1, "Ametal"},
    {"He", "Helio",            2,   4.0026,  18, 1, "Gas nobre"},
    {"Li", "Litio",            3,   6.94,     1, 2, "Metal alcalino"},
    {"Be", "Berilio",          4,   9.0122,   2, 2, "Metal alcalino-terroso"},
    {"B",  "Boro",             5,  10.81,    13, 2, "Semimetal"},
    {"C",  "Carbono",          6,  12.011,   14, 2, "Ametal"},
    {"N",  "Nitrogenio",       7,  14.007,   15, 2, "Ametal"},
    {"O",  "Oxigenio",         8,  15.999,   16, 2, "Ametal"},
    {"F",  "Fluor",            9,  18.998,   17, 2, "Halogenio"},
    {"Ne", "Neonio",          10,  20.180,   18, 2, "Gas nobre"},
    {"Na", "Sodio",           11,  22.990,    1, 3, "Metal alcalino"},
    {"Mg", "Magnesio",        12,  24.305,    2, 3, "Metal alcalino-terroso"},
    {"Al", "Aluminio",        13,  26.982,   13, 3, "Metal pos-transicao"},
    {"Si", "Silicio",         14,  28.085,   14, 3, "Semimetal"},
    {"P",  "Fosforo",         15,  30.974,   15, 3, "Ametal"},
    {"S",  "Enxofre",         16,  32.06,    16, 3, "Ametal"},
    {"Cl", "Cloro",           17,  35.45,    17, 3, "Halogenio"},
    {"Ar", "Argonio",         18,  39.948,   18, 3, "Gas nobre"},
    {"K",  "Potassio",        19,  39.098,    1, 4, "Metal alcalino"},
    {"Ca", "Calcio",          20,  40.078,    2, 4, "Metal alcalino-terroso"},
    {"Sc", "Escandio",        21,  44.956,    3, 4, "Metal de transicao"},
    {"Ti", "Titanio",         22,  47.867,    4, 4, "Metal de transicao"},
    {"V",  "Vanadio",         23,  50.942,    5, 4, "Metal de transicao"},
    {"Cr", "Cromo",           24,  51.996,    6, 4, "Metal de transicao"},
    {"Mn", "Manganes",        25,  54.938,    7, 4, "Metal de transicao"},
    {"Fe", "Ferro",           26,  55.845,    8, 4, "Metal de transicao"},
    {"Co", "Cobalto",         27,  58.933,    9, 4, "Metal de transicao"},
    {"Ni", "Niquel",          28,  58.693,   10, 4, "Metal de transicao"},
    {"Cu", "Cobre",           29,  63.546,   11, 4, "Metal de transicao"},
    {"Zn", "Zinco",           30,  65.38,    12, 4, "Metal de transicao"},
    {"Ga", "Galio",           31,  69.723,   13, 4, "Metal pos-transicao"},
    {"Ge", "Germanio",        32,  72.630,   14, 4, "Semimetal"},
    {"As", "Arsenio",         33,  74.922,   15, 4, "Semimetal"},
    {"Se", "Selenio",         34,  78.971,   16, 4, "Ametal"},
    {"Br", "Bromo",           35,  79.904,   17, 4, "Halogenio"},
    {"Kr", "Criptonio",       36,  83.798,   18, 4, "Gas nobre"},
    {"Rb", "Rubidio",         37,  85.468,    1, 5, "Metal alcalino"},
    {"Sr", "Estroncio",       38,  87.62,     2, 5, "Metal alcalino-terroso"},
    {"Y",  "Itrio",           39,  88.906,    3, 5, "Metal de transicao"},
    {"Zr", "Zirconio",        40,  91.224,    4, 5, "Metal de transicao"},
    {"Nb", "Niobio",          41,  92.906,    5, 5, "Metal de transicao"},
    {"Mo", "Molibdenio",      42,  95.95,     6, 5, "Metal de transicao"},
    {"Tc", "Tecnecio",        43,  98.0,      7, 5, "Metal de transicao"},
    {"Ru", "Rutenio",         44, 101.07,     8, 5, "Metal de transicao"},
    {"Rh", "Rodio",           45, 102.91,     9, 5, "Metal de transicao"},
    {"Pd", "Paladio",         46, 106.42,    10, 5, "Metal de transicao"},
    {"Ag", "Prata",           47, 107.87,    11, 5, "Metal de transicao"},
    {"Cd", "Cadmio",          48, 112.41,    12, 5, "Metal de transicao"},
    {"In", "Indio",           49, 114.82,    13, 5, "Metal pos-transicao"},
    {"Sn", "Estanho",         50, 118.71,    14, 5, "Metal pos-transicao"},
    {"Sb", "Antimonio",       51, 121.76,    15, 5, "Semimetal"},
    {"Te", "Telurio",         52, 127.60,    16, 5, "Semimetal"},
    {"I",  "Iodo",            53, 126.90,    17, 5, "Halogenio"},
    {"Xe", "Xenonio",         54, 131.29,    18, 5, "Gas nobre"},
    {"Cs", "Cesio",           55, 132.91,     1, 6, "Metal alcalino"},
    {"Ba", "Bario",           56, 137.33,     2, 6, "Metal alcalino-terroso"},
    {"La", "Lantanio",        57, 138.91,     3, 6, "Lantanideo"},
    {"Ce", "Cerio",           58, 140.12,     3, 6, "Lantanideo"},
    {"Pr", "Praseodimio",     59, 140.91,     3, 6, "Lantanideo"},
    {"Nd", "Neodimio",        60, 144.24,     3, 6, "Lantanideo"},
    {"Pm", "Promecio",        61, 145.0,      3, 6, "Lantanideo"},
    {"Sm", "Samario",         62, 150.36,     3, 6, "Lantanideo"},
    {"Eu", "Europio",         63, 151.96,     3, 6, "Lantanideo"},
    {"Gd", "Gadolinio",       64, 157.25,     3, 6, "Lantanideo"},
    {"Tb", "Terbio",          65, 158.93,     3, 6, "Lantanideo"},
    {"Dy", "Disprosio",       66, 162.50,     3, 6, "Lantanideo"},
    {"Ho", "Holmio",          67, 164.93,     3, 6, "Lantanideo"},
    {"Er", "Erbio",           68, 167.26,     3, 6, "Lantanideo"},
    {"Tm", "Tulio",           69, 168.93,     3, 6, "Lantanideo"},
    {"Yb", "Iterbio",         70, 173.05,     3, 6, "Lantanideo"},
    {"Lu", "Lutecio",         71, 174.97,     3, 6, "Lantanideo"},
    {"Hf", "Hafnio",          72, 178.49,     4, 6, "Metal de transicao"},
    {"Ta", "Tantalo",         73, 180.95,     5, 6, "Metal de transicao"},
    {"W",  "Tungstenio",      74, 183.84,     6, 6, "Metal de transicao"},
    {"Re", "Renio",           75, 186.21,     7, 6, "Metal de transicao"},
    {"Os", "Osmio",           76, 190.23,     8, 6, "Metal de transicao"},
    {"Ir", "Iridio",          77, 192.22,     9, 6, "Metal de transicao"},
    {"Pt", "Platina",         78, 195.08,    10, 6, "Metal de transicao"},
    {"Au", "Ouro",            79, 196.97,    11, 6, "Metal de transicao"},
    {"Hg", "Mercurio",        80, 200.59,    12, 6, "Metal de transicao"},
    {"Tl", "Talio",           81, 204.38,    13, 6, "Metal pos-transicao"},
    {"Pb", "Chumbo",          82, 207.2,     14, 6, "Metal pos-transicao"},
    {"Bi", "Bismuto",         83, 208.98,    15, 6, "Metal pos-transicao"},
    {"Po", "Polonio",         84, 209.0,     16, 6, "Metal pos-transicao"},
    {"At", "Astato",          85, 210.0,     17, 6, "Halogenio"},
    {"Rn", "Radonio",         86, 222.0,     18, 6, "Gas nobre"},
    {"Fr", "Francio",         87, 223.0,      1, 7, "Metal alcalino"},
    {"Ra", "Radio",           88, 226.0,      2, 7, "Metal alcalino-terroso"},
    {"Ac", "Actinio",         89, 227.0,      3, 7, "Actinideo"},
    {"Th", "Torio",           90, 232.04,     3, 7, "Actinideo"},
    {"Pa", "Protactinio",     91, 231.04,     3, 7, "Actinideo"},
    {"U",  "Uranio",           92, 238.03,     3, 7, "Actinideo"},
    {"Np", "Neptunio",         93, 237.0,      3, 7, "Actinideo"},
    {"Pu", "Plutonio",         94, 244.0,      3, 7, "Actinideo"},
    {"Am", "Americio",         95, 243.0,      3, 7, "Actinideo"},
    {"Cm", "Curio",            96, 247.0,      3, 7, "Actinideo"},
    {"Bk", "Berquelio",        97, 247.0,      3, 7, "Actinideo"},
    {"Cf", "Californio",       98, 251.0,      3, 7, "Actinideo"},
    {"Es", "Einstenio",        99, 252.0,      3, 7, "Actinideo"},
    {"Fm", "Fermio",          100, 257.0,      3, 7, "Actinideo"},
    {"Md", "Mendelevio",      101, 258.0,      3, 7, "Actinideo"},
    {"No", "Nobelio",         102, 259.0,      3, 7, "Actinideo"},
    {"Lr", "Laurencio",       103, 266.0,      3, 7, "Actinideo"},
    {"Rf", "Rutherfordio",    104, 267.0,      4, 7, "Metal de transicao"},
    {"Db", "Dubnio",          105, 268.0,      5, 7, "Metal de transicao"},
    {"Sg", "Seaborgio",       106, 269.0,      6, 7, "Metal de transicao"},
    {"Bh", "Bohrio",          107, 270.0,      7, 7, "Metal de transicao"},
    {"Hs", "Hassio",          108, 269.0,      8, 7, "Metal de transicao"},
    {"Mt", "Meitnerio",       109, 278.0,      9, 7, "Metal de transicao"},
    {"Ds", "Darmstadtio",     110, 281.0,     10, 7, "Metal de transicao"},
    {"Rg", "Roentgenio",      111, 282.0,     11, 7, "Metal de transicao"},
    {"Cn", "Copernicio",      112, 285.0,     12, 7, "Metal de transicao"},
    {"Nh", "Nihonio",         113, 286.0,     13, 7, "Metal pos-transicao"},
    {"Fl", "Flerovio",        114, 289.0,     14, 7, "Metal pos-transicao"},
    {"Mc", "Moscovio",        115, 290.0,     15, 7, "Metal pos-transicao"},
    {"Lv", "Livermorio",      116, 293.0,     16, 7, "Metal pos-transicao"},
    {"Ts", "Tenessino",       117, 294.0,     17, 7, "Halogenio"},
    {"Og", "Oganessonio",     118, 294.0,     18, 7, "Gas nobre"}
};

/* 
 * Variável e 2 funções que não entram na API pública:
 * Fazem parte da busca (no array elementos[]) de uma informação, que interessa ao usuário,
 * no struct Elemento, que não deve ser público.
 */

// Contabiliza o número de elementos químicos na tabela periódica em O(1). 
// Usada como limite superior para os loops de busca. 
static const int numero_de_elementos = sizeof(elementos) / sizeof(Elemento);

// Retorna um ponteiro para o elemento químico com o símbolo fornecido.
// O símbolo é o parâmetro para as principais buscas na tabela
static const Elemento* ponteiro_por_simbolo(const char* simbolo) {
    if (simbolo != NULL) {
        for (int i = 0; i < numero_de_elementos; i++) {
            if (strcmp(elementos[i].simbolo, simbolo) == 0) {
                return &elementos[i];
            }
        }
    }
    return NULL;
}

// Retorna um ponteiro para o elemento químico com o nome fornecido.
// Usado apenas para buscar o símbolo do elemento químico.
static const Elemento* ponteiro_por_nome(const char* nome) {
    if (nome != NULL) {
        for (int i = 0; i < numero_de_elementos; i++) {
            if (strcmp(elementos[i].nome, nome) == 0) {
                return &elementos[i];
            }
        }
    }
    return NULL;
}

/* Funções públicas para consulta de dados da lista de elementos */

const char* elemento_nome(const char* simbolo) {
    const Elemento* elemento = ponteiro_por_simbolo(simbolo);
    if (elemento != NULL) {
        return elemento->nome;
    }
    return NULL;
}

int elemento_numero_atomico(const char* simbolo) {
    const Elemento* elemento = ponteiro_por_simbolo(simbolo);
    if (elemento != NULL) {
        return elemento->numero_atomico;
    }
    return -1;
}

double elemento_massa_atomica(const char* simbolo) {
    const Elemento* elemento = ponteiro_por_simbolo(simbolo);
    if (elemento != NULL) {
        return elemento->massa_atomica;
    }
    return -1;
}

int elemento_grupo(const char* simbolo) {
    const Elemento* elemento = ponteiro_por_simbolo(simbolo);
    if (elemento != NULL) {
        return elemento->grupo;
    }
    return -1;
}

int elemento_periodo(const char* simbolo) {
    const Elemento* elemento = ponteiro_por_simbolo(simbolo);
    if (elemento != NULL) {
        return elemento->periodo;
    }
    return -1;
}

const char* elemento_classificacao(const char* simbolo) {
    const Elemento* elemento = ponteiro_por_simbolo(simbolo);
    if (elemento != NULL) {
        return elemento->classificacao;
    }
    return NULL;
}

const char* elemento_simbolo(const char* nome) {
    const Elemento* elemento = ponteiro_por_nome(nome);
    if (elemento != NULL) {
        return elemento->simbolo;
    }
    return NULL;
}