from devices import Device
import utils
from threading import Thread

MINITOUCH_PATH = "/data/local/tmp/minitouch"
LOCAL_MINITOUCH_DIR = "assets/minitouch/"
MINITOUCH_SOCKET = "127.0.0.1:11165"
MINITOUCH_TCP_PORT = "11166"

class Minitouch:
    __device: Device
    __inte: bool
    __minitouch_thread: Thread

    @property
    def minitouch_running(self):
        if self.__minitouch_thread == None:
            return False
        return self.__minitouch_thread.is_alive()

    def __init__(self, device: Device, inte_model=True):
        self.__device = device
        self.__inte = inte_model
        self.__minitouch_thread = None

    def init_minitouch(self):
        #先判断是否有文件存在
        if not self.__device.file_exist(MINITOUCH_PATH):
            abi = self.__device.get_cpu_abi()
            path = utils.get_full_path(LOCAL_MINITOUCH_DIR) + abi + "/minitouch"
            push_size = self.__device.push_file(path, MINITOUCH_PATH)
            if push_size <= 0:
                raise Exception("push minitouch file faild! file path: " + path)

        #判断服务是否已经启用
        pid = self.__device.adb_get_pid("minitouch")
        if pid != None:
            self.__device.adb_kill_pid(pid)

        #权限判断，先简单处理，全部 777
        self.__device.shell(["chmod", "777", MINITOUCH_PATH])

    def reInstall(self):
        self.__device.adb_commond(["shell", "rm", "-f", MINITOUCH_PATH])
        self.init_minitouch()

    def connect(self, nohup=False):
        self.__minitouch_thread = Thread(target=self.__start_minitouch, args=[nohup, subProcess])
        self.__minitouch_thread.start()


    def disconnect(self):
        if not self.__inte:
            self.__device.forward_remove(MINITOUCH_TCP_PORT)
        
        pid = self.__device.adb_get_pid("minitouch")
        if pid != None:
            self.__device.adb_kill_pid(pid)


    def __start_minitouch(self, nohup:bool):
        if self.__inte:
            if nohup:
                self.__device.adb_commond(["shell", "nohup", MINITOUCH_PATH, "-s", MINITOUCH_SOCKET])
            else:
                self.__device.adb_commond(["shell", MINITOUCH_PATH, "-s", MINITOUCH_SOCKET])
            ##会显示连接超时
            #self.__device.shell([MINITOUCH_PATH, "-s", MINITOUCH_SOCKET])
        else:
            #端口重定向
            self.__device.forward_remove(MINITOUCH_TCP_PORT)
            self.__device.forward(MINITOUCH_TCP_PORT, "localabstract:minitouch")
            self.__device.adb_commond(["shell", MINITOUCH_PATH])



if __name__ == "__main__":
    from getopt import getopt
    from sys import argv
    
    argv = argv[1:]
    serial = None
    remove = False
    reInstall = False
    run = False
    opts, args = getopt(argv, "s:rmg")
    for opt, arg in opts:
        if opt == '-s':
            serial = arg
        elif opt == '-m':
            remove = True
        elif opt == '-r':
            reInstall = True
        elif opt == '-g':
            run = True

    if serial == None:
        exit(1)

    device = Device(adbClient=None, serial=serial)

    device.adb.connect(serial)

    if device.device_state != 'device':
        exit(2)

    minitouch = Minitouch(device=device)

    if remove or reInstall:
        minitouch.disconnect()
    if remove:
        device.adb_commond(["shell", "rm", "-f", MINITOUCH_PATH])
    if reInstall:
        minitouch.reInstall()

    
    if run:
        minitouch.init_minitouch()
        minitouch.connect()