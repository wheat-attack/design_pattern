#ifndef _C_HEADER_H_
#define _C_HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>


#define COIN_FULL_COUNT 100   // 100个币就算满了
#define COIN_CATCH_COUNT 5    // 满5个币可以抓一次


#define SET_STATE(ctx, state) do { \
    ctx->state = state; \
} while (0)}

typedef enum State {
    _WaitCoinState = 1, // 待投币状态
    _WaitOprState,      // 等待操作状态
    _BoxFullState,       // 币箱已满
    _MAX_STATE //
} State;


typedef enum ACTION {
    BACK_COIN_ACTION = 1,    // 退币动作
    ADD_COIN_ACTION,         // 投币动作
    CANCEL_ACTION,           // 取消动作
    CATCH_ACTION,            // 抓取动作
}ACTION;

typedef enum OPR_STATE {
    RUNNING,
    FINISH
}OPR_STATE;

static inline const char* action2string(ACTION action) {
    switch (action) {
    case BACK_COIN_ACTION:
        return "退币";
    case ADD_COIN_ACTION:
        return "加币";
    case CANCEL_ACTION:
        return "取消";
    case CATCH_ACTION:
        return "抓取";
    default:
        return "UNKNOWN_ACTION";
    }
}


static inline const char* state2string(State state) {
    switch (state) {
        case _WaitCoinState:
            return "【待投币】";
        case _WaitOprState:
            return "【待操作】";
        case _BoxFullState:
            return "【币箱已满】";
        default:
            return "【未知状态】";
    }
}


#define SET_STATE_CTX(state, ctx) do { \
    state->ctx=ctx; \
} while (0)

#define SAFE_FREE(p) do { \
    if (p) {  \
        free(p); \
        p=NULL; \
    } \
} while (0)

#define ACTION2FUNCTION(state, action) do { \
    switch (action) {                       \
        case BACK_COIN_ACTION:              \
            PRINT_LOG("退币动作\n");                                \
            state->action.back_coin(state->ctx);               \
            break;                               \
        case ADD_COIN_ACTION:               \
            PRINT_LOG("投币动作\n");\
            state->action.add_coin(state->ctx);               \
            break;                               \
        case CANCEL_ACTION:                 \
            PRINT_LOG("取消抓取动作\n");\
            state->action.cancel(state->ctx);               \
            break;                               \
        case CATCH_ACTION:                  \
            PRINT_LOG("抓取动作\n");                                \
            state->action.catch(state->ctx);               \
            break;                               \
        default:                                 \
            PRINT_LOG("not support action");        \
            break;                                             \
    }\
} while (0)


#define PRINT_LOG(format, ...) \
    printf("[%s:%s:%d]\t\t" format, __FILE__, __func__, __LINE__, ##__VA_ARGS__ );


typedef struct Context Context;

typedef struct Action {
    void (*back_coin)(Context* ctx);    // 退币动作
    void (*add_coin)(Context* ctx);     // 投币动作
    void (*catch)(Context* ctx);        // 抓取动作
    void (*cancel)(Context* ctx);       // 取消动作
}Action;


/*	箱满状态*/
typedef struct BoxFullState {
    Context *ctx;
    Action action;
} BoxFullState;


/*	待投币状态*/
typedef struct WaitCoinState {
    Context* ctx;
    Action action;
}WaitCoinState;


/*	待操作状态*/
typedef struct WaitOprState {
    Context* ctx;
    Action action;
    OPR_STATE opr_state;
} WaitOprState;


struct Context {

    WaitCoinState*  wait_coin_state;
    WaitOprState*   wait_opr_state;
    BoxFullState*   box_full_state;

    int coin_cnt;        // 新币数量
    int box_coin_cnt;   // 非新币数量
    State state;
};

#endif