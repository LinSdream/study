# restore_backup.py
import json
import os
import sys
import shutil
import argparse
from pathlib import Path
from datetime import datetime

class BackupRestorer:
    """备份恢复器"""
    
    def __init__(self, meta_file_path=None, operation_file=None):
        """
        初始化恢复器
        
        Args:
            meta_file_path: 元数据文件路径
            operation_file: 操作文件路径（用于查找元数据文件）
        """
        if meta_file_path:
            self.meta_path = Path(meta_file_path).resolve()
        elif operation_file:
            # 根据操作文件路径查找元数据文件
            op_file = Path(operation_file).resolve()
            workspace_dir = Path("workspace")
            parent_folder_name = op_file.parent.name or "root"
            meta_dir = workspace_dir / parent_folder_name
            
            # 查找最新的meta文件
            meta_files = list(meta_dir.glob(f"{op_file.stem}.meta.json"))
            if not meta_files:
                raise ValueError(f"未找到元数据文件: {meta_dir}/{op_file.stem}.meta.json")
            
            # 选择最新的meta文件（按修改时间）
            meta_files.sort(key=lambda x: x.stat().st_mtime, reverse=True)
            self.meta_path = meta_files[0]
        else:
            raise ValueError("必须提供meta_file_path或operation_file参数")
        
        # 加载元数据
        self.metadata = self.load_metadata()
        if not self.metadata:
            raise ValueError(f"无法加载元数据文件: {self.meta_path}")
        
        self.operation_file = Path(self.metadata.get('operation_file')).resolve()
        self.backup_file = Path(self.metadata.get('backup_file')).resolve()
        
        # 加载配置信息
        self.config = self.metadata.get('config', {})
    
    def load_metadata(self):
        """加载元数据"""
        if not self.meta_path.exists():
            print(f"错误: 元数据文件不存在 - {self.meta_path}")
            return None
        
        try:
            with open(self.meta_path, 'r', encoding='utf-8') as f:
                return json.load(f)
        except Exception as e:
            print(f"加载元数据文件失败: {e}")
            return None
    
    def validate_files(self):
        """验证文件是否存在"""
        if not self.backup_file.exists():
            print(f"错误: 备份文件不存在 - {self.backup_file}")
            return False
        
        return True
    
    def full_restore(self):
        """完全恢复备份"""
        print(f"正在完全恢复备份...")
        print(f"备份文件: {self.backup_file}")
        print(f"目标文件: {self.operation_file}")
        
        try:
            # 使用分块复制
            with open(self.backup_file, 'rb') as src, \
                 open(self.operation_file, 'wb') as dst:
                chunk_size = 1024 * 1024  # 1MB
                while True:
                    chunk = src.read(chunk_size)
                    if not chunk:
                        break
                    dst.write(chunk)
            
            # 更新元数据
            self.metadata['restore_type'] = 'full'
            self.metadata['restore_time'] = self.get_current_time()
            self.save_metadata()
            
            print(f"完全恢复完成!")
            print(f"恢复行数: {self.metadata.get('total_lines', '未知')}")
            print(f"原始配置: clearSwipes={self.config.get('clearSwipes', True)}, noClearLine={self.config.get('noClearLine', 10)}")
            return True
        except Exception as e:
            print(f"恢复失败: {e}")
            return False
    
    def partial_restore(self, n_lines=None):
        """部分恢复备份（覆盖前n行）"""
        print(f"正在部分恢复备份...")
        print(f"备份文件: {self.backup_file}")
        print(f"目标文件: {self.operation_file}")
        
        try:
            # 读取备份文件的所有行
            with open(self.backup_file, 'r', encoding='utf-8') as f:
                backup_lines = f.readlines()
            
            backup_line_count = len(backup_lines)
            
            # 如果指定了n_lines，使用指定的行数
            if n_lines is not None:
                if n_lines > backup_line_count:
                    print(f"警告: 指定的行数({n_lines})超过备份文件行数({backup_line_count})，使用备份文件行数")
                    n_lines = backup_line_count
            else:
                n_lines = backup_line_count
            
            print(f"备份文件行数: {backup_line_count}")
            print(f"将覆盖前 {n_lines} 行")
            
            # 读取操作文件的剩余行（如果有）
            remaining_lines = []
            if self.operation_file.exists():
                with open(self.operation_file, 'r', encoding='utf-8') as f:
                    all_lines = f.readlines()
                
                if len(all_lines) > n_lines:
                    remaining_lines = all_lines[n_lines:]
                    print(f"操作文件剩余行数: {len(remaining_lines)}")
            
            # 写入合并后的内容
            with open(self.operation_file, 'w', encoding='utf-8') as f:
                # 写入备份内容的前n_lines行
                f.writelines(backup_lines[:n_lines])
                # 写入剩余内容
                f.writelines(remaining_lines)
            
            # 更新元数据
            self.metadata['restore_type'] = 'partial'
            self.metadata['restore_time'] = self.get_current_time()
            self.metadata['backup_lines'] = backup_line_count
            self.metadata['restored_lines'] = n_lines
            self.metadata['remaining_lines'] = len(remaining_lines)
            self.save_metadata()
            
            print(f"部分恢复完成!")
            print(f"覆盖前 {n_lines} 行，保留 {len(remaining_lines)} 行")
            print(f"原始配置: clearSwipes={self.config.get('clearSwipes', True)}, noClearLine={self.config.get('noClearLine', 10)}")
            return True
        except Exception as e:
            print(f"恢复失败: {e}")
            return False
    
    def get_current_time(self):
        """获取当前时间字符串"""
        return datetime.now().isoformat()
    
    def save_metadata(self):
        """保存更新后的元数据"""
        try:
            with open(self.meta_path, 'w', encoding='utf-8') as f:
                json.dump(self.metadata, f, indent=2, ensure_ascii=False)
            print(f"元数据已更新: {self.meta_path}")
        except Exception as e:
            print(f"保存元数据失败: {e}")
    
    def print_backup_info(self):
        """打印备份信息"""
        print("\n" + "="*70)
        print("备份信息:")
        print(f"操作文件: {self.operation_file}")
        print(f"备份文件: {self.backup_file}")
        print(f"备份时间: {self.metadata.get('operation_time', '未知')}")
        print(f"配置: clearSwipes={self.config.get('clearSwipes', True)}, noClearLine={self.config.get('noClearLine', 10)}")
        print(f"总行数: {self.metadata.get('total_lines', '未知')}")
        print(f"处理行数: {self.metadata.get('processed_lines', '未知')}")
        print(f"跳过的末尾行数: {self.metadata.get('skipped_tail_lines', 0)}")
        if 'skipped_tail_lines_count' in self.metadata:
            print(f"实际跳过的末尾行数: {self.metadata.get('skipped_tail_lines_count', 0)}")
        print(f"备份大小: {self.metadata.get('backup_size', 0) / 1024 / 1024:.2f} MB")
        print("="*70)


def restore_backup(meta_file=None, operation_file=None, restore_type='full', lines=None, no_confirm=False):
    """
    恢复备份
    
    Args:
        meta_file: 元数据文件路径
        operation_file: 操作文件路径
        restore_type: 恢复类型 ('full' 或 'partial')
        lines: 部分恢复时覆盖的行数（None表示使用备份文件所有行）
        no_confirm: 跳过确认提示
    """
    try:
        # 创建恢复器
        restorer = BackupRestorer(meta_file_path=meta_file, operation_file=operation_file)
        
        # 验证文件
        if not restorer.validate_files():
            return False
        
        # 显示备份信息
        restorer.print_backup_info()
        
        # 确认操作
        if not no_confirm:
            if restore_type == 'full':
                confirm_msg = f"确认要执行完全恢复吗？(y/n): "
            else:
                line_info = f"前{lines}行" if lines is not None else "所有备份行"
                confirm_msg = f"确认要执行部分恢复（覆盖{line_info}）吗？(y/n): "
            
            confirm = input(confirm_msg)
            if confirm.lower() != 'y':
                print("操作已取消")
                return False
        
        # 执行恢复
        if restore_type == 'full':
            success = restorer.full_restore()
        elif restore_type == 'partial':
            success = restorer.partial_restore(lines)
        else:
            print(f"错误: 不支持的恢复类型: {restore_type}")
            return False
        
        return success
        
    except Exception as e:
        print(f"恢复过程中出错: {e}")
        return False


def main():
    """主函数"""
    parser = argparse.ArgumentParser(description='备份恢复工具')
    
    # 互斥参数组：必须指定元数据文件或操作文件
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--meta', help='元数据文件路径')
    group.add_argument('--operation-file', help='操作文件路径')
    
    parser.add_argument('--type', choices=['full', 'partial'], default='full',
                       help='恢复类型: full(完全恢复) 或 partial(部分恢复，覆盖前n行)')
    parser.add_argument('--lines', type=int,
                       help='部分恢复时覆盖的行数（默认使用备份文件所有行）')
    parser.add_argument('--no-confirm', action='store_true',
                       help='跳过确认提示（谨慎使用）')
    
    args = parser.parse_args()
    
    # 恢复备份
    success = restore_backup(
        meta_file=args.meta,
        operation_file=args.operation_file,
        restore_type=args.type,
        lines=args.lines,
        no_confirm=args.no_confirm
    )
    
    if success:
        print("\n恢复成功!")
        sys.exit(0)
    else:
        print("\n恢复失败!")
        sys.exit(1)


if __name__ == "__main__":
    main()