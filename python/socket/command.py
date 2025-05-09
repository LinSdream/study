import subprocess
import shlex

def process_exec(command, shell=False, cwd=None, timeout=None):
    process_info = list(map(str, command))
    print(process_info)
    process = subprocess.Popen(process_info,stdout=subprocess.PIPE,stderr=subprocess.PIPE, shell=shell, cwd=cwd)
    try:
        stdout, stderr = process.communicate(timeout=timeout)
        # print(f"out: {stdout.decode()} \n err: {stderr.decode()}")
        return process.returncode, stdout.decode(), stderr.decode()
    except subprocess.TimeoutExpired:
        process.kill()
        return 1

def update_config(args:list[str]):
    try:
        #任何的进程都不能更改另一个进程的工作目录，因此cd指令是无法使用的，得一开始就指定工作目录
        return process_exec(["/bin/bash", "./autoUpdate.sh"], cwd="/mnt/e/work/PD/tools/cfg2lua")[0]
    except Exception as e:
        print(e)
        return 1

def search_config(args:list[str]):
    try:
        print("find argv: ", args)
        cmd=["/bin/bash", "./search.sh"]
        cmd.extend(list(map(str,args)))
        return process_exec(cmd, cwd="/mnt/e/work/PD/tools/cfg2lua")[1]
    except Exception as e:
        print(e)
        return 1


def exec(command_str:str):
    result = shlex.split(command_str)
    command = result[0]
    args = result[1:]
    if not command:
        return "no send command"
    
    if command == "update_config":
        return update_config(args)
    if command == "search_config":
        return search_config(args)
    
    return "no define command"