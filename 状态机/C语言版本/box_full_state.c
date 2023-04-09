/*
 *  箱满的所有动作
 *
 * */

#include "header.h"


static void back_coin(Context* ctx) {
    if (!ctx->coin_cnt) {
        return;
    }
    PRINT_LOG("退出%d个币", ctx->coin_cnt);
    ctx->box_coin_cnt = ctx->box_coin_cnt - ctx->coin_cnt;
    if (ctx->box_coin_cnt < 0) {
        ctx->box_coin_cnt = 0;
    }
    ctx->coin_cnt = 0;
    return;
}

static void add_coin(Context* ctx) {
    PRINT_LOG("币箱已满，请退币或联系工作人员清空币箱\n");
}

static void cancel(Context* ctx) {
    PRINT_LOG("不允许取消\n");
}

static void catch(Context* ctx) {
    PRINT_LOG("抓取动作不支持\n");
}


BoxFullState* init_box_full_state(Context* ctx) {
    BoxFullState* box_full_state = (BoxFullState*)malloc(sizeof(BoxFullState));

    box_full_state->action.add_coin = add_coin;
    box_full_state->action.back_coin = back_coin;
    box_full_state->action.cancel = cancel;
    box_full_state->action.catch = catch;
    box_full_state->ctx = ctx;
    return box_full_state;
}

void free_box_full_state(BoxFullState* box_full_state) {
    SAFE_FREE(box_full_state);
}
