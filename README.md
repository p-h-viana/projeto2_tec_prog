# projeto2_tec_prog
# libquimica

Biblioteca em C para cálculos de química geral, feita para o Projeto 2 de Técnicas de Programação I (IME-USP).

A biblioteca tem 17 funções públicas, divididas em quatro módulos. O módulo de elementos (`elementos.h`) consulta dados dos 118 elementos da tabela periódica. O de estequiometria (`estequiometria.h`) balanceia equações químicas e calcula a massa de produto, o reagente limitante e o rendimento. O de pH (`ph.h`) calcula o pH de ácidos fortes, fracos e muito diluídos e de soluções tampão. O de termoquímica (`termoquimica.h`) calcula a variação de entalpia de uma reação, a partir de uma tabela de entalpias de formação ou pela Lei de Hess.

A documentação de cada função (parâmetros, retorno e erros) está nos próprios headers, em `include/`.

## Pré-requisitos

Para compilar o projeto é preciso de um ambiente Linux com GCC, GNU Make e Git. No Ubuntu/Debian, tudo pode ser instalado com:

```
sudo apt install build-essential git
```

## Estrutura do projeto

```
A pasta `app/` contém a aplicação cliente, `include/` contém a API pública, `src/` contém a implementação dos módulos e `tests/` contém os testes automatizados.

As pastas `build/`, `lib/` e `bin/` são criadas pelo `make` e não ficam no repositório. A `build/` recebe os arquivos objeto (`.o`) e de dependência (`.d`), a `lib/` recebe as bibliotecas e a `bin/` recebe os executáveis da aplicação e dos testes.

## Compilação

Na raiz do projeto, rode:

```
make
```

Esse comando gera a biblioteca estática `lib/libquimica.a`, a biblioteca dinâmica `lib/libquimica.so` e duas versões da aplicação: `bin/app_estatica`, ligada à biblioteca estática, e `bin/app_dinamica`, ligada à dinâmica.

A compilação é incremental: ao alterar um módulo, só ele é recompilado. As dependências dos headers são geradas automaticamente pelo GCC, com as opções `-MMD -MP`.

## Alvos do Makefile

O `make` (ou `make all`) compila a biblioteca estática, a dinâmica e as duas versões da aplicação.

O `make run` compila o que for necessário e executa as duas versões da aplicação, uma depois da outra.

O `make test` compila e executa os testes de todos os módulos.

O `make install` instala as bibliotecas e os headers, como descrito na seção de instalação.

O `make clean` apaga as pastas `build/`, `lib/` e `bin/`.

## Execução

Para compilar e executar as duas versões da aplicação:

```
make run
```

Depois de compilar, também é possível executar cada versão separadamente:

```
./bin/app_estatica
./bin/app_dinamica
```

A versão dinâmica encontra a `libquimica.so` sozinha, sem precisar de `LD_LIBRARY_PATH`, porque o caminho da biblioteca fica gravado no executável com a opção `-Wl,-rpath`. Para confirmar que ela usa a biblioteca dinâmica, rode:

```
ldd bin/app_dinamica
```

## Testes

Para compilar e executar os testes:

```
make test
```

Cada módulo tem seu próprio programa de teste: `bin/test_elementos`, `bin/test_estequiometria`, `bin/test_ph` e `bin/test_termoquimica`. Os testes cobrem todas as funções públicas, com casos de uso normal, valores limite e entradas inválidas.

No final aparece `RESULTADO: APROVADO (todos os testes passaram)` ou `RESULTADO: FALHOU (ha testes com erro)`. Se algum teste falhar, o `make` também termina com erro.

## Instalação

Por padrão, a instalação é feita em `/usr/local`, o que exige `sudo`:

```
sudo make install
```

Para instalar sem privilégios de administrador, basta trocar o prefixo:

```
make install PREFIX=$HOME/.local
```

As bibliotecas são copiadas para `PREFIX/lib` e os headers para `PREFIX/include`.

Para compilar outro programa usando a biblioteca instalada:

```
gcc programa.c -I$HOME/.local/include -L$HOME/.local/lib -lquimica -lm -o programa
```

## Autores

Carolina Thomaz Ferreira, Eric Zucherato Narbutis, Gabriel Moura Moreira e Pedro Henrique Viana.
