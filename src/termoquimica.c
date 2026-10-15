#include "termoquimica.h"

TermoStatus entalpia_reacao(double soma_h_produtos, double soma_h_reagentes, double *ptr_delta_h, TipoReacao *ptr_tipo_reacao) {
    if (ptr_delta_h == 0 || ptr_tipo_reacao == 0) {
        return ERRO_PONTEIRO_NULO;
    }

    *ptr_delta_h = soma_h_produtos - soma_h_reagentes;

    if (*ptr_delta_h < 0.0) {
        *ptr_tipo_reacao = REACAO_EXOTERMICA;
    } else if (*ptr_delta_h > 0.0) {
        *ptr_tipo_reacao = REACAO_ENDOTERMICA;
    } else {
        *ptr_tipo_reacao = REACAO_ATERMICA;
    }

    return SUCESSO_TERMO;
}