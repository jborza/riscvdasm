#pragma once
#include "types.h"
#include "state.h"




word read_csr(State *state, int csr);
void write_csr(State *state, int csr, word value);