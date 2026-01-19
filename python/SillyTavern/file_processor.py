# file_processor.py
import json
import tempfile
import os
from typing import Dict, Any, Optional, Generator
from pathlib import Path


class FileProcessor:
    """文件处理器"""
    
    @staticmethod
    def get_total_lines(file_path: str) -> int:
        """获取文件总行数"""
        print("正在计算文件总行数...")
        line_count = 0
        
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for _ in f:
                    line_count += 1
            return line_count
        except Exception as e:
            print(f"计算行数失败: {e}")
            return 0
    
    @staticmethod
    def should_skip_line(line_num: int, total_lines: int, no_clear_line: int) -> bool:
        """判断是否应该跳过这一行（在末尾行范围内）"""
        # 如果不需要跳过末尾行或总行数未知
        if no_clear_line <= 0 or total_lines <= 0:
            return False
        
        # 如果行号在末尾noClearLine行内，则跳过
        return line_num > (total_lines - no_clear_line)
    
    @staticmethod
    def read_jsonl_file(file_path: str) -> Generator[tuple, None, None]:
        """读取JSONL文件，返回(行号, 行内容)的生成器"""
        with open(file_path, 'r', encoding='utf-8') as f:
            for line_num, line in enumerate(f, 1):
                stripped_line = line.strip()
                if stripped_line:
                    yield line_num, stripped_line
    
    @staticmethod
    def parse_json_line(line: str) -> Optional[Dict[str, Any]]:
        """解析JSON行"""
        try:
            return json.loads(line)
        except json.JSONDecodeError:
            return None
    
    @staticmethod
    def create_temp_file(input_file_path: str) -> Optional[tuple]:
        """创建临时文件"""
        try:
            temp_fd, temp_path = tempfile.mkstemp(
                suffix='.tmp', 
                dir=os.path.dirname(input_file_path) or '.'
            )
            return temp_fd, temp_path
        except Exception as e:
            print(f"创建临时文件失败: {e}")
            return None
    
    @staticmethod
    def replace_file(temp_path: str, input_file_path: str) -> bool:
        """用临时文件替换原文件"""
        try:
            os.replace(temp_path, input_file_path)
            return True
        except Exception as e:
            print(f"替换文件失败: {e}")
            return False