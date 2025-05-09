from adbutils import AdbDevice, AdbClient, adb
from typing import Optional
import re
import subprocess
import multiprocessing
import utils
from platform import system

if system() == 'Windows':
    ADB_PATH = "assets/adb.exe"
else:
    ADB_PATH = "assets/adb"

class Device:
    __adb: AdbClient
    __device: AdbDevice
    __serial: str
    
    @property
    def adb(self) -> AdbClient:
        return self.__adb

    @property
    def serial(self) -> str:
        return self.__serial
    
    @property
    def device(self) -> AdbDevice:
        return self.__device

    @property
    def device_state(self)-> str:
        """{offline,bootloader,device}"""
        return self.__device.get_state()

    def __init__(self, adbClient:Optional[AdbClient], serial:str):
        self.__adb = adbClient
        if not self.__adb:
            self.__adb = adb
        self.__device = self.__adb.device(serial= serial)
        self.__serial = serial

    def shell(self, cmd, stream=False, recvall=True, timeout=10, rstrip=True):
        if stream:
            res = self.__device.shell(cmd, stream=stream, timeout=timeout, rstrip=rstrip)
            if recvall:
                # 返回的是bytes
                return utils.recv_all(res)
            else:
                # 返回的是socket
                return res
        else:
            return self.__device.shell(cmd, stream=stream, timeout=timeout, rstrip=rstrip)

    def get_cpu_abi(self):
        return self.get_getprop("ro.product.cpu.abi")
    
    def get_sdk_version(self):
        return self.get_getprop("ro.build.version.sdk")
    
    def get_getprop(self, name):
        return self.shell(["getprop", name]).strip()
    
    def file_exist(self, file_path):
        res = self.shell(["ls", file_path])
        return res == file_path
    
    def push_file(self, local, remote):
        return self.__device.sync.push(local,remote)

    def install_app(self, url, nolaunch:bool = True):
        self.__device.install(url,nolaunch = nolaunch)

    def open_app(self, packageName):
        self.__device.app_start(package_name=packageName)

    def adb_commond(self, cmd, shell=False, timeout=None):
        process_info = [utils.get_full_path(ADB_PATH), "-s", self.__serial]
        process_info.extend(list(map(str, cmd)))
        
        process = subprocess.Popen(process_info,stdout=subprocess.PIPE, shell=shell)
        try:
            stdout, stderr = process.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            process.kill()
            stdout, stderr = process.communicate()
        return stdout, stderr


    def adb_get_pid(self, find_str):
        out, err= self.adb_commond(["shell", "ps","|","grep", find_str])
        line = out.decode()
        if line == "":
            return None
        res = re.search(r"[0-9]+", line)
        if res == None:
            return None
        #第一串是pid
        return int(res.group())

    def adb_kill_pid(self, pid:int):
        self.shell(["kill", "-9", str(pid)])

    def forward(self, remote_tcp, local_tcp):
        self.__device.forward(local=str(local_tcp), remote=str(remote_tcp))

    def forward_remove(self, tcp:str):
        self.adb_commond(["forward", "--remove", "tcp:" + str(tcp)])