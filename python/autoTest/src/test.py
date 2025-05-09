from wrapper.decorator import run_once
import asyncio
from devices import Device
from adbutils import AdbClient
from minitouch import Minitouch
from timer import LoopTimer, Timer

@run_once
def ttt():
    print("ttttttttttttt")

if __name__ == "__main__":
    looptimer = LoopTimer(1)
    looptimer.start()

    looptimer.registerFunc("test1", lambda dt:print("test11111", dt))
    looptimer.registerFunc("test2", lambda dt:print("test222222", dt), timer=Timer(3))

    ttt()
    ttt()

    looptimer.update()
    # loop = asyncio.new_event_loop()
    # asyncio.set_event_loop(loop=loop)

    # taks = asyncio.create_task(looptimer.asyncUpdate())
    # asyncio.wait(taks)