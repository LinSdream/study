import socket
import struct

class custom_client:
    __host:str
    __port:int
    __c:socket.socket

    def __init__(self, host="127.0.0.1", port=23548):
        self.__host=host
        self.__port=port

    def __recv_data(self):
        if not self.__c:
            print("[client][E] client not connect ")

        header = self.__c.recv(4)
        if not header or len(header) != 4:
            return None
        #解析接收到的 4 字节头部数据，获取后续数据的总长度。
        data_len = struct.unpack('!I', header)[0]
        #创建一个 可变的字节数组
        received = bytearray()
        while len(received) < data_len:
            remaining = data_len - len(received)
            chunk = self.__c.recv(min(1024, remaining))
            if not chunk:
                break

            received.extend(chunk)

        return received.decode('utf-8')
        
    def __send_data(self, data:str):
        if not self.__c:
            print("[client][E] client not connect ")

        data=str(data)
        encoded_data = data.encode('utf-8')
        data_len = len(encoded_data)
        bytes_len = struct.pack('!I', data_len)
        self.__c.sendall(bytes_len)
        self.__c.sendall(encoded_data)

    def send(self, command_list:list[str]):
        command=""
        for s in command_list:
            if " " in s:
                command+=f"\'{s}\'"
            else:
                command+=s
            command+=" "
        self.__c=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__c.connect((self.__host, self.__port))
        print(f"[client][I] 已连接到 {self.__host}:{self.__port}")
        try:
            self.__send_data(command)
            resp = self.__recv_data()
            if resp:
                print(resp)
        except KeyboardInterrupt:
            print("[client][I] 中断连接")
        finally:
            self.__c.close()
            self.__c = None  


if __name__ == "__main__":
    c = custom_client()
    from sys import argv
    c.send(argv[1:])