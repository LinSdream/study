import csv
from utils import get_file_name
import sys

def csv2lua(csv_path, target_path):
    try:
        with open(csv_path,encoding='utf-8') as f:
            reader = csv.reader(f)
            data = list(reader)
        
            file_name = get_file_name(csv_path)
            f = None
            if "_constant" not in file_name:
                f = normal_file_parse
                (data, target_path, file_name)
            else:
                f = constant_parse
            f(data, target_path, file_name)
            return target_path + '/'+ file_name + ".lua"
    except Exception as e:
        sys.stderr.write("conver failed. file: {0}\n err: {1}".format(csv_path, e))

        
def constant_parse(data, target_path, file_name):
    with open(target_path + '/'+ file_name + ".lua", 'w', encoding='utf-8') as w:
        w.write("local {0} = {{\n".format(file_name))
        for arr in data[7:]:
            res = "\t[\"{0}\"] = {1},\n".format(arr[0], type_parse(arr[3], arr[1]))
            w.write(res)
        w.write("}\n")
        w.write("return {0}".format(file_name))


def normal_file_parse(data, target_path, file_name):
        key_dir = data[1]
        type_dir = data[5]

        with open(target_path + '/'+ file_name + ".lua", 'w', encoding='utf-8') as w:
            w.write("local {0} = {{\n".format(file_name))
            row = 0
            for arr in data[7:]:
                str = "\t[{0}] = {{ ".format(row)
                row = row + 1
                for i in range(len(key_dir)):
                    str += "{0} = {1}, ".format(key_dir[i], type_parse(type_dir[i], arr[i]))
                str = str[:-1]
                str += " },\n"
                w.write(str)
            w.write("}\n")
            w.write("return {0}".format(file_name))


def type_parse(type, content):
    if type == 'string':
        content = content.replace('\\', '/')
        return "\"{0}\"".format(content)
    elif type == 'int array':
        if content == "":
            return r"{}"
        arr = content.split(';')
        res = '{ '
        for element in arr:
            res += "{0}, ".format(element)
        res = res[:-2]
        res += ' }'
        return res
    elif type == 'string array':
        if content == "":
            return r"{}"
        arr = content.split(';')
        res = '{ '
        for element in arr:
            element = element.replace('\\', '/')
            res += "\"{0}\", ".format(element)
        res = res[:-2]
        res += ' }'
        return res
    else:
        return content