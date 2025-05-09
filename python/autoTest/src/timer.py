import time
import asyncio
from typing import *


class Timer:
    @property
    def started(self):
        return bool(self.__cur)

    @property
    def current(self):
        if self.started:
            return time.time() - self.__cur
        else:
            return 0.

    def __init__(self, limit, count=0):
        self.limit = limit
        self.count = count
        self.__cur = 0
        self.__reach_count = count

    def start(self):
        if not self.started:
            self.__cur = time.time()
            self.__reach_count = 0

        return self

    def reached(self):
        self.__reach_count += 1
        return time.time() - self.__cur > self.limit and self.__reach_count > self.count

    def reset(self):
        self.__cur = time.time()
        self.__reach_count = 0
        return self

    def clear(self):
        self.__cur = 0
        self.__reach_count = self.count
        return self

    def reached_and_reset(self):
        if self.reached():
            self.reset()
            return True
        else:
            return False

    def wait(self):
        diff = self.__cur + self.limit - time.time()
        if diff > 0:
            time.sleep(diff)

    def __str__(self):
        return f'Timer(limit={round(self.current(), 3)}/{self.limit}, count={self.__reach_count}/{self.count})'

    __repr__ = __str__


class LoopTimer:
    __interval:float
    __cur:float
    __f_map:dict[str, Tuple[Callable[[float], NoReturn], Optional[Timer]]]
    stop_event:Optional[Callable[[], bool]]
    
    @property
    def started(self):
        return bool(self.__cur)

    def __init__(self, interval):
        self.__interval = interval
        self.__cur = 0
        self.__f_map=dict({})
        self.stop_event = None

    def start(self):
        self.__cur = time.time()

    def registerFunc(self, key:str, func:Callable, timer:Timer=None):
        if key in self.__f_map:
            print("[loopTimer] key already exist! key:", key)
            return
        self.__f_map[key] = (func, timer)

    def unregisterFunc(self, key:str):
        if key not in self.__f_map:
            return
        val = self.__f_map[key]
        if val[1]:
            val[1].clear()
        del self.__f_map[key]

    def reached(self):
        return time.time() - self.__cur > self.__interval

    def update(self):
        while 1:
            if self.stop_event and self.stop_event():
                self.stop()
                return
            if not self.started:
                continue
            if not self.reached():
                time.sleep(self.__interval + self.__cur - time.time())
                continue
            dt = time.time() - self.__cur
            self.__cur = time.time()
            for key, info in self.__f_map.items():
                try:
                    if info[1] != None:
                        timer = info[1]
                        if not timer.started:
                            timer.start()
                        if timer.reached_and_reset():
                            info[0](dt)
                    else:
                        info[0](dt)
                except Exception as e:
                    print("loopTimer execute func err! key:", key, e)

    async def asyncUpdate(self):
        while 1:
            if self.stop_event and self.stop_event():
                self.stop()
                return
            if not self.started:
                continue
            if not self.reached():
                await asyncio.sleep(self.__interval + self.__cur - time.time())
                continue
            dt = time.time() - self.__cur
            self.__cur = time.time()
            for key, info in self.__f_map.items():
                try:
                    if info[1] != None:
                        timer = info[1]
                        if not timer.started:
                            timer.start()
                        if timer.reached_and_reset():
                            info[0](dt)
                    else:
                        info[0](dt)
                except Exception as e:
                    print("loopTimer execute func err! key:", key, e)
    
    def stop(self):
        self.__cur = 0