from os import path, makedirs, getcwd

def get_full_path(tarPath):
    return getcwd().replace("\\", "/") + "/" + tarPath

def path_format(targetPath):
    return targetPath.replace("\\", "/")

def get_file_name(file_path):
    stem, suffix = path.splitext(path.basename(file_path))
    return stem