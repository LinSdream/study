from adbutils import AdbConnection, AdbTimeout
from typing import Optional
from os import getcwd, path, makedirs, remove
import requests
import socket
import time, datetime

def recv_all(stream, chunk_size=4096, recv_interval=0.000) -> bytes:
    if isinstance(stream, AdbConnection):
        stream = stream.conn
        stream.settimeout(10)
    else:
        stream.settimeout(10)

    try:
        fragments = []
        while 1:
            chunk = stream.recv(chunk_size)
            if chunk:
                fragments.append(chunk)
                # https://stackoverflow.com/questions/23837827/python-server-program-has-high-cpu-usage/41749820#41749820
                time.sleep(recv_interval)
            else:
                break
        return remove_shell_warning(b''.join(fragments))
    except socket.timeout:
        raise AdbTimeout('adb read timeout')


def remove_shell_warning(s):
    if isinstance(s, bytes):
        if s.startswith(b'WARNING'):
            try:
                s = s.split(b'\n', maxsplit=1)[1]
            except IndexError:
                pass
        return s
    elif isinstance(s, str):
        if s.startswith('WARNING'):
            try:
                s = s.split('\n', maxsplit=1)[1]
            except IndexError:
                pass
    return s

def download_file(url:str, dest:Optional[str], chunk=2048, convert=True):
    if not path.exists(dest):
        makedirs(dest)
    try:
        resp = requests.get(url=url,stream=True)
        resp.raise_for_status()
        len = int(resp.headers.get("Content-Length", 0))
        r = resp.raw

        time_00 = int(time.mktime(datetime.date.today().timetuple()))
        download_pre = 1
        file_path = "{}/{}.apk".format(dest, time_00)

        if path.exists(file_path):
            print("目标文件已存在!")
            if convert:
                remove(file_path)
            else:
                return file_path
        file = open(file_path, "wb")
        for i in resp.iter_content(chunk_size=chunk):
            file.write(i)
            print(download_pre * chunk, "/", len)
            download_pre += 1
    except Exception as e:
        print(e)
    
    return file_path

def get_full_path(path):
    return getcwd().replace("\\", "/") + "/" + path


if __name__ == "__main__":
    dest = get_full_path("temp")
    download_file("http://devops.api.onemt.co/bundle/download/84987",dest=dest,chunk=5120)