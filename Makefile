CC       = gcc
CFLAGS   = -std=c99 -Wall -Wextra -pedantic -Iinclude
DEPFLAGS = -MMD -MP
LDLIBS   = -lm
AR       = ar
PREFIX  ?= /usr/local

MODULOS = ph termoquimica estequiometria elementos

OBJ_ESTATICO = $(MODULOS:%=build/estatico/%.o)
OBJ_DINAMICO = $(MODULOS:%=build/dinamico/%.o)
OBJ_TESTES   = $(MODULOS:%=build/tests/test_%.o) build/tests/teste.o
OBJ_APP      = build/app/main.o
HEADERS      = $(MODULOS:%=include/%.h)

LIB_A  = lib/libquimica.a
LIB_SO = lib/libquimica.so
TESTES = $(MODULOS:%=bin/test_%)

.PHONY: all clean run test install

all: $(LIB_A) $(LIB_SO) bin/app_estatica bin/app_dinamica

# Objetos da biblioteca estática
$(OBJ_ESTATICO): build/estatico/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Objetos da biblioteca dinâmica (precisam de -fPIC)
$(OBJ_DINAMICO): build/dinamico/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPFLAGS) -fPIC -c $< -o $@

$(LIB_A): $(OBJ_ESTATICO)
	@mkdir -p $(@D)
	$(AR) rcs $@ $^

$(LIB_SO): $(OBJ_DINAMICO)
	@mkdir -p $(@D)
	$(CC) -shared $^ $(LDLIBS) -o $@

$(OBJ_APP): build/app/%.o: app/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

bin/app_estatica: $(OBJ_APP) $(LIB_A)
	@mkdir -p $(@D)
	$(CC) $< $(LIB_A) $(LDLIBS) -o $@

# O rpath faz o executável achar lib/libquimica.so sem LD_LIBRARY_PATH
bin/app_dinamica: $(OBJ_APP) $(LIB_SO)
	@mkdir -p $(@D)
	$(CC) $< -Llib -lquimica $(LDLIBS) -Wl,-rpath,'$$ORIGIN/../lib' -o $@

$(OBJ_TESTES): build/tests/%.o: tests/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TESTES): bin/%: build/tests/%.o build/tests/teste.o $(LIB_A)
	@mkdir -p $(@D)
	$(CC) $< build/tests/teste.o $(LIB_A) $(LDLIBS) -o $@

run: bin/app_estatica bin/app_dinamica
	@echo "== Aplicacao com a biblioteca estatica =="
	@./bin/app_estatica
	@echo
	@echo "== Aplicacao com a biblioteca dinamica =="
	@./bin/app_dinamica

test: $(TESTES)
	@falhou=0; \
	for t in $(TESTES); do ./$$t || falhou=1; echo; done; \
	if [ $$falhou -eq 0 ]; then echo "RESULTADO: APROVADO (todos os testes passaram)"; \
	else echo "RESULTADO: FALHOU (ha testes com erro)"; exit 1; fi

install: $(LIB_A) $(LIB_SO)
	install -d $(PREFIX)/lib $(PREFIX)/include
	install -m 644 $(LIB_A) $(LIB_SO) $(PREFIX)/lib
	install -m 644 $(HEADERS) $(PREFIX)/include

clean:
	rm -rf build lib bin

# Dependências dos headers, geradas pelo gcc (-MMD)
-include $(OBJ_ESTATICO:.o=.d) $(OBJ_DINAMICO:.o=.d) $(OBJ_TESTES:.o=.d) $(OBJ_APP:.o=.d)
