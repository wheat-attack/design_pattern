/*
 *  等待币的所有操作
 * */

#include "header.h"
#include "impl.h"


static void back_coin(Context* ctx) {
    if (!ctx->coin_cnt) {
        PRINT_LOG("没有币，没有办法退\n");
        return;
    }
    PRINT_LOG("退出%d个币\n", ctx->coin_cnt);
    ctx->box_coin_cnt = ctx->box_coin_cnt - ctx->coin_cnt;
    if (ctx->box_coin_cnt < 0) {
        ctx->box_coin_cnt = 0;
    }

    ctx->coin_cnt = 0;
    return;
}

static void add_coin(Context* ctx) {

    if (ctx->box_coin_cnt == COIN_FULL_COUNT) {
        set_curr_state(ctx, _BoxFullState);
        PRINT_LOG("当前币箱已满，请退币或者联系工作人员清空币箱\n");
        return;
    }

    ctx->coin_cnt++;
    ctx->box_coin_cnt++;

    if (ctx->coin_cnt >= COIN_CATCH_COUNT) {
        set_curr_state(ctx, _WaitOprState);
        PRINT_LOG("当前的币有%d个, 状态转换成待操作状态\n", ctx->coin_cnt);
        return;
    }

    PRINT_LOG("当前的币有%d个\n", ctx->coin_cnt);
}

static void cancel(Context* ctx) {
    PRINT_LOG("没有正在抓取，不能取消\n");
}

static void catch(Context* ctx) {
    PRINT_LOG("币不够，请继续投币\n");
}

WaitCoinState* init_wait_coin_state(Context* ctx) {
    WaitCoinState* wait_coin_state = (WaitCoinState*)calloc(1, sizeof(WaitCoinState));

    wait_coin_state->action.add_coin = add_coin;
    wait_coin_state->action.back_coin = back_coin;
    wait_coin_state->action.cancel = cancel;
    wait_coin_state->action.catch = catch;
    wait_coin_state->ctx = ctx;
    return wait_coin_state;
}

void free_wait_coin_state(WaitCoinState* state) {
    SAFE_FREE(state);
}
