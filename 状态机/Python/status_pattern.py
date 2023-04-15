# coding: utf-8

"""
状态模式
"""

COIN_FULL_COUNT = 100
COIN_CATCH_COUNT = 5

ACTION = {
    1: 'add_coin',
    2: 'back_coin',
    3: 'catch',
}


class DollMachine:
    """
    娃娃机
    """
    def __init__(self):
        self.box_coin_cnt = 0
        self.coin_cnt = 0
        self.wait_coin_state = WaitCoinStatus(self)
        self.wait_opr_state = WaitOprStatus(self)
        self.box_full_state = BoxFullStatus(self)
        self.state = self.wait_coin_state

    def call_action(self, action, *args, **kwargs):
        """
        执行动作
        :param action:
        :return:
        """
        action = ACTION.get(action)
        if not action:
            raise Exception("无效的动作")

        func = self.state.__getattribute__(action)
        func(*args, **kwargs)


class Action:
    """
    动作基类
    """

    def __init__(self, ctx: DollMachine):
        self.ctx = ctx

    def add_coin(self):
        """
        添加硬币
        :return:
        """
        if self.ctx.box_coin_cnt >= COIN_FULL_COUNT:
            self.ctx.state = self.ctx.box_full_state
            raise Exception("不允许投币了")

        self.ctx.coin_cnt += 1
        self.ctx.box_coin_cnt += 1
        print("有币 {}个".format(self.ctx.coin_cnt))

    def back_coin(self):
        """
        退币
        :return:
        """
        if self.ctx.coin_cnt == 0:
            print("没币了退不了")
            return

        print("退币 {}个".format(self.ctx.coin_cnt))
        self.ctx.box_coin_cnt = self.ctx.box_coin_cnt - self.ctx.coin_cnt
        if self.ctx.box_coin_cnt < 0:
            self.ctx.box_coin_cnt = 0

        self.ctx.coin_cnt = 0

    def catch(self):
        """
        抓取
        :return:
        """
        raise Exception("币不够，不允许抓取")


class BoxFullStatus(Action):
    """
    娃娃机已满
    """
    def __init__(self, ctx: DollMachine):
        super().__init__(ctx)

    def catch(self):
        if self.ctx.coin_cnt < COIN_CATCH_COUNT:
            self.ctx.state = self.ctx.wait_coin_state
            raise Exception("币不够了，不允许继续抓")

        self.ctx.coin_cnt -= COIN_CATCH_COUNT
        print("抓一次。。。。")


class WaitOprStatus(Action):
    """
    等待操作状态
    """
    def __init__(self, ctx: DollMachine):
        super().__init__(ctx)

    def catch(self):
        if self.ctx.coin_cnt < COIN_CATCH_COUNT:
            self.ctx.state = self.ctx.wait_coin_state
            raise Exception("币不够了，不允许继续抓")

        self.ctx.coin_cnt -= COIN_CATCH_COUNT
        print("抓一次。。。。")


class WaitCoinStatus(Action):
    """
    等待硬币状态
    """
    def __init__(self, ctx: DollMachine):
        super().__init__(ctx)

    def add_coin(self):
        super().add_coin()
        if self.ctx.coin_cnt > COIN_CATCH_COUNT:
            self.ctx.state = self.ctx.wait_opr_state


if __name__ == '__main__':
    doll = DollMachine()
    for i in range(10):
        doll.call_action(1)

    doll.call_action(3)
    doll.call_action(3)
    doll.call_action(2)
