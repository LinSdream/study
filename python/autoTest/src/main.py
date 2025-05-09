from wrapper.decorator import run_once
from devices import Device
from adbutils import AdbClient
from minitouch import Minitouch
from pywebio.output import *
from pywebio.input import *
from pywebio.pin import *
from pywebio.session import defer_call
from utils import get_full_path, download_file
import threading
from timer import LoopTimer, Timer


serial_int_map:dict[str, int] = dict({})
serial_uid:int = 0

def serial2int(serial):
    global serial_int_map
    global serial_uid
    if serial in serial_int_map:
        return serial_int_map[serial]
    else:
        serial_uid = serial_uid + 1
        uid = serial_uid
        serial_int_map[serial] = uid
        return uid


class DeviceCol:
    minitouch:Minitouch
    device:Device
    scope_set:set[str] = set()
    __uid:int

    @property
    def uid(self):
        return self.__uid

    def __init__(self, device:Device, minitouch:Minitouch):
        self.minitouch = minitouch
        self.device = device
        self.__uid = serial2int(self.device.serial)

    # def set_scope_content(self, name:str, parent:str, content:str):
    #     key = str.format("{}_{}",self.__uid , name)
    #     if key not in self.scope_set:
    #         self.scope_set.add(key)
    #         set_scope(name=key, container_scope=parent)
        
    #     with use_scope(name=key, clear=True):
    #         put_markdown(content, sanitize=True, scope=key)
        

class AutoTest:
    stop_event: threading.Event = None
    devices: dict[str, DeviceCol] = dict({})
    loopTimer = LoopTimer(0.02)

    exclude: list[str] = ["127.0.0.1:16384","127.0.0.1:7555"]
    include:list[str] = ["127.0.0.1:12121212","127.0.0.1:16416"]
    adb_client:AdbClient

    def __init__(self):
        self.adb_client = AdbClient(host="127.0.0.1", port=5037)
        # defer_call(self.clear_up)
        for d in self.adb_client.device_list():
            if d.serial not in self.exclude:
                self.init_device_server(d.serial)

        for serial in self.include:
            if serial not in self.devices:
                #尝试连接设备
                res = self.adb_client.connect(serial, 3)
                if res.find("connected") != -1:
                    self.init_device_server(serial)
                else:
                    print("connect failed, serial: ",serial)

    def init_device_server(self, serial:str):
        if serial in self.devices:
            print(serial+" 已初始化")
            return
        device = Device(self.adb_client, serial)
        minitouch = Minitouch(device=device)
        minitouch.init_minitouch()
        self.devices[serial] = DeviceCol(minitouch = minitouch, device = device)

    def install_app(self, path):
        for device in self.devices.values():
            device.device.install_app(path)

    @run_once
    def startServer(self):
        for device in self.devices.values():
            device.minitouch.connect()

    def stopServer(self):
        for device in self.devices.values():
            device.minitouch.disconnect()

    @run_once
    def loop(self):
        self.startServer()

        # put_input(name='input_connect_txt', label='手动连接需要连接的设备号')
        # put_actions(name='input_connect', buttons={"连接"})

        # put_markdown("设备minitouch运行状态")
        # put_scrollable(put_scope('minitouch_scroll'), height=300, keep_bottom=True)

        
        # self.loopTimer.registerFunc("draw", self.draw, timer=Timer(1))
        self.loopTimer.registerFunc("draw", self.cmd_ouput, timer=Timer(1))
        self.loopTimer.start()
        self.loopTimer.update()
       
    # def draw(self, dt):
    #     if pin.input_connect_txt:
    #         print(pin.input_connect_txt)
    #         pin_update("input_connect_txt",spec={})

    #     for device in self.devices.values():
    #         print("设备号： {}, minitouch 状态: {}".format(device.device.serial, device.minitouch.minitouch_running))
    #         device.set_scope_content(name="minitouch_state", parent="minitouch_scroll",content="设备号： {}, minitouch 状态: {}".format(device.device.serial, device.minitouch.minitouch_running))

    def cmd_ouput(self, dt):
        for device in self.devices.values():
            print("设备号： {}, minitouch 状态: {}".format(device.device.serial, device.minitouch.minitouch_running))

    def clear_up(self):
        self.loopTimer.stop()

def parse_argv(argv):
    from getopt import getopt
    params = dict({})
    opts, args = getopt(argv, "o:f:sr")
    for opt, arg in opts:
        if opt == '-f':
            params["apk"] = arg
        elif opt == '-s':
            params["stopServer"] = True
        elif opt == '-r':
            params["startServer"] = True
        elif opt == '-o':
            params["openAkp"] = arg

    return params

def install_apk(params:dict, autoTest:AutoTest):
    apk = params.get("apk")
    if apk != None:
        dest = get_full_path("assets/apk")
        apk = download_file(apk,dest=dest,chunk=5120)
        t = Timer(1)
        t.wait()
        autoTest.install_app(apk)

def stop_server(params:dict, autoTest:AutoTest):
    stopServer = params.get("stopServer")
    if stopServer != None:
        autoTest.stopServer()

def start_server(params:dict, autoTest:AutoTest):
    stopServer = params.get("startServer")
    if stopServer != None:
        autoTest.startServer()

def open_apk(params:dict, autoTest:AutoTest):
    packageName = params.get("openAkp")
    if packageName != None:
        for device in autoTest.devices.values():
            device.device.open_app(packageName)

if __name__ == '__main__':
    from sys import argv
    argv = argv[1:]

    params = dict({})
    if argv:
        params = parse_argv(argv=argv)

    #启动服务
    autoTest = AutoTest()

    install_apk(params=params, autoTest=autoTest)
    stop_server(params=params, autoTest=autoTest)
    start_server(params=params, autoTest=autoTest)
    open_apk(params=params, autoTest=autoTest)

    # loopTimer = LoopTimer(0.02)
    # loopTimer.registerFunc("cmdOutput", autoTest.cmd_ouput, timer=Timer(1))
    # loopTimer.start()
    # loopTimer.update()