/*
	抓娃娃机的状态

                   待投币                    待操作              币箱已满
	待投币          退币                持续投币，到达阈值      持续投币，到达阈值
	待操作          退币、币不够             ---              ---
	币箱已满        退币                    ---                   ---
*/

#include "header.h"
#include "impl.h"

Context* init_context() {
    BoxFullState *box_full_state;
    WaitCoinState *wait_coin_state;
    WaitOprState *wait_opr_state;
    Context *ctx;

    ctx = (Context*)calloc(1, sizeof(Context));
    box_full_state = init_box_full_state(ctx);
    wait_coin_state = init_wait_coin_state(ctx);
    wait_opr_state = init_wait_opr_state(ctx);

    ctx->state = _WaitCoinState;
    ctx->box_full_state = box_full_state;
    ctx->wait_coin_state = wait_coin_state;
    ctx->wait_opr_state = wait_opr_state;

    return ctx;
}


int main()  {
    int add_coin_cnt, action, i;
    add_coin_cnt = action = i = 0;

    printf("投币：");
    scanf("%d", &add_coin_cnt);

    Context *ctx = init_context();

    for (int i = 0; i < add_coin_cnt; i++) {
        call_concrete(ctx, ADD_COIN_ACTION);
    }

    while(1) {
        for (int i = BACK_COIN_ACTION; i <= CATCH_ACTION; i++) {
            printf("%d   -> %s\n", i, action2string(i));
        }
        printf("请输入想要的动作：");
        scanf("%d", &action);
        if (action < BACK_COIN_ACTION || action > CATCH_ACTION) {
            break;
        }

        call_concrete(ctx, action);
        action = 0;
    }
}
