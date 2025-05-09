import socket
import struct
from sys import stderr, exit
import traceback

class custom_server:
    __host:str
    __port:int
    __s:socket.socket
    __backlog:int
    __command_listen_fun:None

    def command_listen(self, command_func):
        self.__command_listen_fun = command_func

    def __init__(self, host="127.0.0.1", port=23548, backlog=5):
        self.__host = host
        self.__port = port
        self.__backlog = backlog
        self.__s = None

    def __recv_data(self, client:socket.socket):
        header = client.recv(4)
        if not header or len(header) != 4:
            return None
        #解析接收到的 4 字节头部数据，获取后续数据的总长度。
        data_len = struct.unpack('!I', header)[0]
        #创建一个 可变的字节数组
        received = bytearray()
        while len(received) < data_len:
            remaining = data_len - len(received)
            chunk = client.recv(min(1024, remaining))
            if not chunk:
                break

            received.extend(chunk)

        return received.decode('utf-8')
        
    def __send_data(self, client:socket.socket, data:str):
        data=str(data)
        encoded_data = data.encode('utf-8')
        data_len = len(encoded_data)
        bytes_len = struct.pack('!I', data_len)
        client.sendall(bytes_len)
        client.sendall(encoded_data)

    def run(self):
        self.__s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__s.bind((self.__host, self.__port))
        self.__s.listen(self.__backlog)
        print(f"[server][I] 服务启动")
        try:
            while True:
                client, addr = self.__s.accept()
                print(f"[server][I] 客户端连接 addr: {addr}")
                try:
                    while True:
                        command = self.__recv_data(client)
                        if not command:
                            break
                        if self.__command_listen_fun:
                            result = self.__command_listen_fun(command)
                            self.__send_data(client, result)
                        else:
                            self.__send_data(client, "faild! no command can exec")
                except ConnectionResetError:
                    print(f"[server][I] 客户端断开连接 addr:{addr}")
                finally:
                    print(f"[server][I] 关闭客户端 addr:{addr}")
                    client.close()
        except KeyboardInterrupt:
            print("关闭服务端")
        except Exception as e:
          err=f"{e.args}\n{traceback.format_exc()}"
          stderr.write(err)
          exit(1)
        finally:
            self.__s.close()


if __name__ == "__main__":
    s = custom_server()
    from command import exec
    s.command_listen(exec)
    s.run()