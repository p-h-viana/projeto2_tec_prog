#include "elementos.h"
typedef struct {
    char simbolo[3];
    char nome[30];
    int numero_atomico;
    double massa_atomica;
    int grupo;
    int periodo;
} Elemento;

/*
 * Tabela periódica dos 118 elementos químicos.
 *
 * Campos:
 * simbolo, nome, numero_atomico, massa_atomica, grupo, periodo
 *
 * Para elementos sem peso atomico padrao, massa_atomica armazena
 * um numero de massa de referencia de um isotopo.
 *
 * Convencao adotada: lantanideos e actinideos recebem grupo 3.
 */
static const Elemento elementos[] = {
    {"H",  "Hidrogenio",       1,   1.008,    1, 1},
    {"He", "Helio",            2,   4.0026,  18, 1},

    {"Li", "Litio",            3,   6.94,     1, 2},
    {"Be", "Berilio",          4,   9.0122,   2, 2},
    {"B",  "Boro",             5,  10.81,    13, 2},
    {"C",  "Carbono",          6,  12.011,   14, 2},
    {"N",  "Nitrogenio",       7,  14.007,   15, 2},
    {"O",  "Oxigenio",         8,  15.999,   16, 2},
    {"F",  "Fluor",            9,  18.998,   17, 2},
    {"Ne", "Neonio",          10,  20.180,   18, 2},

    {"Na", "Sodio",           11,  22.990,    1, 3},
    {"Mg", "Magnesio",        12,  24.305,    2, 3},
    {"Al", "Aluminio",        13,  26.982,   13, 3},
    {"Si", "Silicio",         14,  28.085,   14, 3},
    {"P",  "Fosforo",         15,  30.974,   15, 3},
    {"S",  "Enxofre",         16,  32.06,    16, 3},
    {"Cl", "Cloro",           17,  35.45,    17, 3},
    {"Ar", "Argonio",         18,  39.948,   18, 3},

    {"K",  "Potassio",        19,  39.098,    1, 4},
    {"Ca", "Calcio",          20,  40.078,    2, 4},
    {"Sc", "Escandio",        21,  44.956,    3, 4},
    {"Ti", "Titanio",         22,  47.867,    4, 4},
    {"V",  "Vanadio",         23,  50.942,    5, 4},
    {"Cr", "Cromo",           24,  51.996,    6, 4},
    {"Mn", "Manganes",        25,  54.938,    7, 4},
    {"Fe", "Ferro",           26,  55.845,    8, 4},
    {"Co", "Cobalto",         27,  58.933,    9, 4},
    {"Ni", "Niquel",          28,  58.693,   10, 4},
    {"Cu", "Cobre",           29,  63.546,   11, 4},
    {"Zn", "Zinco",           30,  65.38,    12, 4},
    {"Ga", "Galio",           31,  69.723,   13, 4},
    {"Ge", "Germanio",        32,  72.630,   14, 4},
    {"As", "Arsenio",         33,  74.922,   15, 4},
    {"Se", "Selenio",         34,  78.971,   16, 4},
    {"Br", "Bromo",           35,  79.904,   17, 4},
    {"Kr", "Criptonio",       36,  83.798,   18, 4},

    {"Rb", "Rubidio",         37,  85.468,    1, 5},
    {"Sr", "Estroncio",       38,  87.62,     2, 5},
    {"Y",  "Itrio",           39,  88.906,    3, 5},
    {"Zr", "Zirconio",        40,  91.224,    4, 5},
    {"Nb", "Niobio",          41,  92.906,    5, 5},
    {"Mo", "Molibdenio",      42,  95.95,     6, 5},
    {"Tc", "Tecnecio",        43,  98.0,      7, 5},
    {"Ru", "Rutenio",         44, 101.07,     8, 5},
    {"Rh", "Rodio",           45, 102.91,     9, 5},
    {"Pd", "Paladio",         46, 106.42,    10, 5},
    {"Ag", "Prata",           47, 107.87,    11, 5},
    {"Cd", "Cadmio",          48, 112.41,    12, 5},
    {"In", "Indio",           49, 114.82,    13, 5},
    {"Sn", "Estanho",         50, 118.71,    14, 5},
    {"Sb", "Antimonio",       51, 121.76,    15, 5},
    {"Te", "Telurio",         52, 127.60,    16, 5},
    {"I",  "Iodo",            53, 126.90,    17, 5},
    {"Xe", "Xenonio",         54, 131.29,    18, 5},

    {"Cs", "Cesio",           55, 132.91,     1, 6},
    {"Ba", "Bario",           56, 137.33,     2, 6},
    {"La", "Lantanio",        57, 138.91,     3, 6},
    {"Ce", "Cerio",           58, 140.12,     3, 6},
    {"Pr", "Praseodimio",     59, 140.91,     3, 6},
    {"Nd", "Neodimio",        60, 144.24,     3, 6},
    {"Pm", "Promecio",        61, 145.0,      3, 6},
    {"Sm", "Samario",         62, 150.36,     3, 6},
    {"Eu", "Europio",         63, 151.96,     3, 6},
    {"Gd", "Gadolinio",       64, 157.25,     3, 6},
    {"Tb", "Terbio",          65, 158.93,     3, 6},
    {"Dy", "Disprosio",       66, 162.50,     3, 6},
    {"Ho", "Holmio",          67, 164.93,     3, 6},
    {"Er", "Erbio",           68, 167.26,     3, 6},
    {"Tm", "Tulio",           69, 168.93,     3, 6},
    {"Yb", "Iterbio",         70, 173.05,     3, 6},
    {"Lu", "Lutecio",         71, 174.97,     3, 6},
    {"Hf", "Hafnio",          72, 178.49,     4, 6},
    {"Ta", "Tantalo",         73, 180.95,     5, 6},
    {"W",  "Tungstenio",      74, 183.84,     6, 6},
    {"Re", "Renio",           75, 186.21,     7, 6},
    {"Os", "Osmio",           76, 190.23,     8, 6},
    {"Ir", "Iridio",          77, 192.22,     9, 6},
    {"Pt", "Platina",         78, 195.08,    10, 6},
    {"Au", "Ouro",            79, 196.97,    11, 6},
    {"Hg", "Mercurio",        80, 200.59,    12, 6},
    {"Tl", "Talio",            81, 204.38,    13, 6},
    {"Pb", "Chumbo",          82, 207.2,     14, 6},
    {"Bi", "Bismuto",         83, 208.98,    15, 6},
    {"Po", "Polonio",         84, 209.0,     16, 6},
    {"At", "Astato",          85, 210.0,     17, 6},
    {"Rn", "Radonio",         86, 222.0,     18, 6},

    {"Fr", "Francio",         87, 223.0,      1, 7},
    {"Ra", "Radio",           88, 226.0,      2, 7},
    {"Ac", "Actinio",         89, 227.0,      3, 7},
    {"Th", "Torio",           90, 232.04,     3, 7},
    {"Pa", "Protactinio",     91, 231.04,     3, 7},
    {"U",  "Uranio",          92, 238.03,     3, 7},
    {"Np", "Neptunio",        93, 237.0,      3, 7},
    {"Pu", "Plutonio",        94, 244.0,      3, 7},
    {"Am", "Americio",        95, 243.0,      3, 7},
    {"Cm", "Curio",            96, 247.0,      3, 7},
    {"Bk", "Berquelio",       97, 247.0,      3, 7},
    {"Cf", "Californio",      98, 251.0,      3, 7},
    {"Es", "Einstenio",       99, 252.0,      3, 7},
    {"Fm", "Fermio",         100, 257.0,      3, 7},
    {"Md", "Mendelevio",     101, 258.0,      3, 7},
    {"No", "Nobelio",        102, 259.0,      3, 7},
    {"Lr", "Laurencio",      103, 266.0,      3, 7},
    {"Rf", "Rutherfordio",   104, 267.0,      4, 7},
    {"Db", "Dubnio",         105, 268.0,      5, 7},
    {"Sg", "Seaborgio",      106, 269.0,      6, 7},
    {"Bh", "Bohrio",         107, 270.0,      7, 7},
    {"Hs", "Hassio",         108, 269.0,      8, 7},
    {"Mt", "Meitnerio",      109, 278.0,      9, 7},
    {"Ds", "Darmstadtio",    110, 281.0,     10, 7},
    {"Rg", "Roentgenio",     111, 282.0,     11, 7},
    {"Cn", "Copernicio",     112, 285.0,     12, 7},
    {"Nh", "Nihonio",        113, 286.0,     13, 7},
    {"Fl", "Flerovio",       114, 289.0,     14, 7},
    {"Mc", "Moscovio",       115, 290.0,     15, 7},
    {"Lv", "Livermorio",     116, 293.0,     16, 7},
    {"Ts", "Tenessino",      117, 294.0,     17, 7},
    {"Og", "Oganessonio",    118, 294.0,     18, 7}
};
