/*
 *  抓娃娃机代码上下文
 * */

#include "header.h"

/*获取币的数量*/
int get_coin_cnt(Context* ctx) {
    return ctx->coin_cnt;
}

/* 设置当前状态 */
void set_curr_state(Context* ctx, State state) {
    ctx->state = state;
}

/* 获取当前的状态 */
static State get_curr_state(Context* ctx) {
    return ctx->state;
}

/* 实际调用最终的状态 */
void call_concrete(Context *ctx, int action) {
    State state = get_curr_state(ctx);

    PRINT_LOG("当前状态为: %s\n", state2string(state));

    switch (state) {
        case _WaitCoinState:
            ACTION2FUNCTION(ctx->wait_coin_state, action);
            break;
        case _WaitOprState:
            ACTION2FUNCTION(ctx->wait_opr_state, action);
            break;
        case _BoxFullState:
            ACTION2FUNCTION(ctx->box_full_state, action);
            break;
        default:
            break;
    }
}
