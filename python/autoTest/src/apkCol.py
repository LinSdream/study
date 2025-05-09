from typing import Callable, Optional
from devices import Device
import utils
import re

APK_PATH = "assets/apk/1.apk"
    
def apk_exist(device:Device, package_name:str):
    out, err = res = device.adb_commond(["shell", "pm", "list", "package", "-f" "|", "grep", package_name])
    line = out.decode()
    if line == "":
        return False
    res = re.search("apk={}".format(package_name), line)
    if res == None:
        return False
    return True
    
def apk_uninstall(device:Device, package_name:str):
    device.device.uninstall(package_name)

def apk_install(device:Device, url:str, callback:Optional[Callable[[str], None]]=None):
    device.device.install_remote
    device.device.install(path_or_url=url, flags=['-r', '-t', '-g', '-d'], nolaunch=True, callback=callback)

if __name__ == "__main__":
    from adbutils import AdbClient
    adb_client = AdbClient(host="127.0.0.1", port=5037)
    device = Device(adb_client, "127.0.0.1:16416")
    package_name="and.matrix.pos"
    print(apk_exist(device,package_name))
    apk_uninstall(device,package_name)
    def test(state):
        if state == 'BEFORE_INSTALL':
            print("111111111111111111111")
        elif state == 'FINALLY':
            print("222222222222222222222222")
    apk_install(device, utils.get_full_path(APK_PATH), callback=test)
