/*
 *  等待操作的所有操作
 * */

#include "header.h"
#include "impl.h"


static void set_opr_state(Context* ctx, OPR_STATE opr_state) {
    ctx->wait_opr_state->opr_state = opr_state;
}


static void back_coin(Context* ctx) {
    if (!ctx->coin_cnt) {
        PRINT_LOG("没有币，没有办法退\n");
        return;
    }
    PRINT_LOG("退出%d个币\n", ctx->coin_cnt);
    ctx->box_coin_cnt = ctx->box_coin_cnt - ctx->coin_cnt;
    ctx->coin_cnt = 0;
    set_curr_state(ctx, _WaitCoinState);
    return;
}

static void add_coin(Context* ctx) {
    if (ctx->box_coin_cnt == COIN_FULL_COUNT) {
        PRINT_LOG("当前币箱已经满了，不允许投币了，已有币可以继续抓取娃娃\n");
        return;
    }

    PRINT_LOG("待操作状态，仍加币\n");
    ctx->coin_cnt++;
    ctx->box_coin_cnt++;
}

static void cancel(Context* ctx) {
    if (ctx->wait_opr_state->opr_state != RUNNING) {
        PRINT_LOG("当前状态不是正在运行的，不允许取消\n");
        return;
    }

    ctx->coin_cnt = ctx->coin_cnt + COIN_CATCH_COUNT;
}

static void catch(Context* ctx) {

    if (ctx->coin_cnt / COIN_CATCH_COUNT > 0) {

        PRINT_LOG("catch ing .... \n");
        set_opr_state(ctx, RUNNING);
        PRINT_LOG("end catch\n");
        ctx->coin_cnt = ctx->coin_cnt - COIN_CATCH_COUNT;
        return;
    }

    PRINT_LOG("币不够了，请投币\n");
    set_curr_state(ctx, _WaitCoinState);
}


WaitOprState * init_wait_opr_state(Context* ctx) {
    WaitOprState* wait_opr_state = (WaitOprState*)calloc(1, sizeof(WaitOprState));

    wait_opr_state->action.add_coin = add_coin;
    wait_opr_state->action.back_coin = back_coin;
    wait_opr_state->action.cancel = cancel;
    wait_opr_state->action.catch = catch;
    wait_opr_state->ctx = ctx;
    wait_opr_state->opr_state = FINISH;
    return wait_opr_state;
}

void free_wait_opr_state(WaitOprState* state) {
    SAFE_FREE(state);
}
