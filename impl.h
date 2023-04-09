//
// Created by 42507 on 2023/4/8.
//

#ifndef C_IMPL_H
#define C_IMPL_H

#include "header.h"

BoxFullState* init_box_full_state(Context* ctx);
void free_box_full_state(BoxFullState* box_full_state);

WaitCoinState* init_wait_coin_state(Context* ctx);
void free_wait_coin_state(WaitCoinState* state);

WaitOprState * init_wait_opr_state(Context* ctx);
void free_wait_opr_state(WaitOprState * state);

int get_coin_cnt(Context* ctx);
int get_curr_state(Context* ctx);
void set_curr_state(Context* ctx, State state);
void call_concrete(Context *ctx, int action);

#endif //C_IMPL_H
