# list_backups.py
import json
import os
import sys
from pathlib import Path
from datetime import datetime

def list_available_backups():
    """列出所有可用的备份"""
    workspace_dir = Path("workspace")
    
    if not workspace_dir.exists():
        print("workspace目录不存在，没有可用的备份")
        return []
    
    backups = []
    
    # 遍历workspace目录
    for parent_dir in workspace_dir.iterdir():
        if not parent_dir.is_dir():
            continue
        
        # 查找元数据文件
        for meta_file in parent_dir.glob("*.meta.json"):
            try:
                with open(meta_file, 'r', encoding='utf-8') as f:
                    metadata = json.load(f)
                
                backups.append({
                    'meta_file': str(meta_file),
                    'operation_file': metadata.get('operation_file', '未知'),
                    'backup_file': metadata.get('backup_file', '未知'),
                    'backup_time': metadata.get('operation_time', '未知'),
                    'total_lines': metadata.get('total_lines', 0),
                    'processed_lines': metadata.get('processed_lines', 0),
                    'status': metadata.get('status', '未知')
                })
            except Exception as e:
                print(f"读取元数据文件失败 {meta_file}: {e}")
    
    return backups

def print_backup_table(backups):
    """以表格形式打印备份信息"""
    if not backups:
        print("没有找到备份文件")
        return
    
    print("\n" + "="*120)
    print(f"{'序号':<4} | {'操作文件':<40} | {'备份时间':<20} | {'行数':<8} | {'状态':<10} | {'元数据文件'}")
    print("-" * 120)
    
    for i, backup in enumerate(backups, 1):
        # 截断操作文件名
        op_file = backup['operation_file']
        if len(op_file) > 40:
            op_file = "..." + op_file[-37:]
        
        # 格式化备份时间
        backup_time = backup['backup_time']
        if 'T' in backup_time:
            backup_time = backup_time.split('T')[0]
        
        # 格式化元数据文件名
        meta_file = Path(backup['meta_file']).name
        
        print(f"{i:<4} | {op_file:<40} | {backup_time:<20} | "
              f"{backup['total_lines']:<8} | {backup['status']:<10} | {meta_file}")
    
    print("="*120)
    print(f"找到 {len(backups)} 个备份")

def main():
    """主函数"""
    backups = list_available_backups()
    print_backup_table(backups)
    
    if backups:
        print("\n使用以下命令恢复备份:")
        print("1. 使用元数据文件恢复:")
        for i, backup in enumerate(backups, 1):
            print(f"   备份{i}: python restore_backup.py --meta \"{backup['meta_file']}\" --type full")
        
        print("\n2. 使用操作文件恢复:")
        for i, backup in enumerate(backups, 1):
            print(f"   备份{i}: python restore_backup.py --operation-file \"{backup['operation_file']}\" --type full")
        
        print("\n提示: 使用 --type partial 进行部分恢复")

if __name__ == "__main__":
    main()