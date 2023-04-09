package main

import (
	"fmt"
)

const (
	CoinFullCount  = 100 // 100个币就算满了
	CoinCatchCount = 5   // 满5个币可以抓一次
)

const (
	AddCoin  = 1
	BackCoin = 2
	Catch    = 3
)

type DollMachine struct {
	boxFullState  *BoxFullState  // 已满状态
	waitCoinState *WaitCoinState // 等待投币状态
	waitOprState  *WaitOprState  // 等待操作状态
	currState     Action         // 当前状态

	coinCnt    int // 已投币数量
	boxCoinCnt int // 币箱已有币数量
}

func (d *DollMachine) callAction(action int) {
	switch action {
	case AddCoin:
		d.currState.addCoin()
		break
	case BackCoin:
		d.currState.backCoin()
		break
	case Catch:
		d.currState.catch()
		break
	default:
		fmt.Println("不支持的动作")
	}
}

type Action interface {
	addCoin()
	backCoin()
	catch()
	setDoll(ctx *DollMachine)
}

type BoxFullState struct {
	ctx *DollMachine
}

func (b *BoxFullState) addCoin() {
	fmt.Printf("币箱已满，请退币或联系工作人员清空币箱\n")
}

func (b *BoxFullState) backCoin() {
	if b.ctx.coinCnt == 0 {
		return
	}

	fmt.Printf("币箱满状态：退币%d\n", b.ctx.coinCnt)
	b.ctx.boxCoinCnt = b.ctx.boxCoinCnt - b.ctx.coinCnt
	if b.ctx.boxCoinCnt < 0 {
		b.ctx.boxCoinCnt = 0
	}
	b.ctx.coinCnt = 0
}

func (b *BoxFullState) catch() {
	fmt.Println("币箱满状态，不允许抓取")
}

func (b *BoxFullState) setDoll(ctx *DollMachine) {
	b.ctx = ctx
}

type WaitCoinState struct {
	ctx *DollMachine
}

func (w *WaitCoinState) addCoin() {
	if w.ctx.coinCnt == CoinFullCount {
		w.ctx.currState = w.ctx.boxFullState
		fmt.Println("投币状态：当前币箱已满，请退币或者联系工作人员清空币箱")
		return
	}

	w.ctx.coinCnt++
	w.ctx.boxCoinCnt++

	if w.ctx.coinCnt == CoinCatchCount {
		w.ctx.currState = w.ctx.waitOprState
		fmt.Printf("当前的币有%d个, 状态转换成待操作状态\n", w.ctx.coinCnt)
		return
	}

	fmt.Printf("投币状态：币有%d个\n", w.ctx.coinCnt)
}

func (w *WaitCoinState) backCoin() {
	if w.ctx.coinCnt == 0 {
		fmt.Println("投币状态：未投币，无法退币")
		return
	}

	fmt.Printf("投币状态：退出%d个币\n", w.ctx.coinCnt)
	w.ctx.boxCoinCnt = w.ctx.boxCoinCnt - w.ctx.coinCnt
	if w.ctx.boxCoinCnt < 0 {
		w.ctx.boxCoinCnt = 0
	}
	w.ctx.coinCnt = 0
}

func (w *WaitCoinState) catch() {
	fmt.Println("投币状态：不允许抓取，币不够")
}

func (w *WaitCoinState) setDoll(ctx *DollMachine) {
	w.ctx = ctx
}

type WaitOprState struct {
	ctx *DollMachine
}

func (w *WaitOprState) addCoin() {
	if w.ctx.boxCoinCnt == CoinFullCount {
		fmt.Println("当前币箱已经满了，不允许投币了，已有币可以继续抓取娃娃")
		return
	}

	fmt.Println("待操作状态，继续投币")
	w.ctx.boxCoinCnt++
	w.ctx.coinCnt++
}

func (w *WaitOprState) backCoin() {
	if w.ctx.coinCnt == 0 {
		fmt.Println("待操作状态：未投币，无法退币")
		goto state
	}

	fmt.Printf("待操作状态：退出%d个币\n", w.ctx.coinCnt)
	w.ctx.boxCoinCnt = w.ctx.boxCoinCnt - w.ctx.coinCnt
	if w.ctx.boxCoinCnt < 0 {
		w.ctx.boxCoinCnt = 0
	}
	w.ctx.coinCnt = 0

state:
	w.ctx.currState = w.ctx.waitCoinState
}

func (w *WaitOprState) catch() {
	if w.ctx.coinCnt/CoinCatchCount > 0 {
		fmt.Println("抓取娃娃")
		w.ctx.coinCnt = w.ctx.coinCnt - CoinCatchCount
		return
	}

	fmt.Println("币不够了，请投币")
	w.ctx.currState = w.ctx.waitCoinState
}

func (w *WaitOprState) setDoll(ctx *DollMachine) {
	w.ctx = ctx
}

func main() {
	var addCoinCnt int
	var action int

	d := &DollMachine{
		boxFullState:  &BoxFullState{},
		waitCoinState: &WaitCoinState{},
		waitOprState:  &WaitOprState{},
		currState:     nil,
		coinCnt:       0,
		boxCoinCnt:    0,
	}
	d.waitOprState.setDoll(d)
	d.waitCoinState.setDoll(d)
	d.waitOprState.setDoll(d)
	d.currState = d.waitCoinState

	fmt.Println("请投币: ")
	fmt.Scanln(&addCoinCnt)
	for i := 0; i < addCoinCnt; i++ {
		d.currState.addCoin()
	}

	for true {
		fmt.Println("请输入想要的动作：")
		fmt.Println("1  -> 投币")
		fmt.Println("2  -> 退币")
		fmt.Println("3  -> 抓取")

		fmt.Scanln(&action)
		d.callAction(action)
		action = 0
	}
}
