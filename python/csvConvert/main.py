import argparse
from xslConvert import csv2lua
from utils import path_format

def all_output(target_dir, tmp_dir):
    return

def single_output(csv_path, tmp_dir):
    csv2lua(csv_path, tmp_dir)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='csv convert lua')
    parser.add_argument('-i', '--input', type=str, help="输入csv")
    parser.add_argument('-o', '--output', type=str, help="输出目录")
    args = parser.parse_args()
    
    input = path_format(args.input)
    output = path_format(args.output)

    print("start parse {0}".format(input))
    output_f = csv2lua(input, output)
    print("completed output_file: {0}".format(output_f))