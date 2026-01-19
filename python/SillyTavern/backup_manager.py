# backup_manager.py
import json
import os
import shutil
from pathlib import Path
from datetime import datetime
from typing import Dict, Any, Optional


class BackupManager:
    """备份管理器"""
    
    def __init__(self, input_file_path: str, project_dir: str):
        self.input_file_path = Path(input_file_path).resolve()
        self.project_dir = Path(project_dir)
        self.project_dir.mkdir(parents=True, exist_ok=True)
        
        # 备份文件路径
        backup_filename = f"{self.input_file_path.stem}_{datetime.now().strftime('%Y%m%d_%H%M%S')}.bak"
        self.backup_path = self.project_dir / backup_filename
        
        # 元数据文件路径
        self.meta_path = self.project_dir / f"{self.input_file_path.stem}.meta.json"
    
    def create_backup(self) -> bool:
        """创建文件备份"""
        print(f"正在备份文件到: {self.backup_path}")
        
        try:
            # 使用分块复制处理大文件
            with open(self.input_file_path, 'rb') as src, \
                 open(self.backup_path, 'wb') as dst:
                chunk_size = 1024 * 1024  # 1MB
                while True:
                    chunk = src.read(chunk_size)
                    if not chunk:
                        break
                    dst.write(chunk)
            
            print(f"备份完成，大小: {os.path.getsize(self.backup_path) / 1024 / 1024:.2f} MB")
            return True
        except Exception as e:
            print(f"备份文件失败: {e}")
            return False
    
    def save_metadata(self, metadata: Dict[str, Any]) -> bool:
        """保存元数据"""
        try:
            with open(self.meta_path, 'w', encoding='utf-8') as f:
                json.dump(metadata, f, indent=2, ensure_ascii=False)
            print(f"元数据文件已保存: {self.meta_path}")
            return True
        except Exception as e:
            print(f"保存元数据文件失败: {e}")
            return False
    
    def load_metadata(self) -> Optional[Dict[str, Any]]:
        """加载元数据"""
        if not self.meta_path.exists():
            return None
        
        try:
            with open(self.meta_path, 'r', encoding='utf-8') as f:
                return json.load(f)
        except Exception as e:
            print(f"加载元数据文件失败: {e}")
            return None
    
    def get_backup_path(self) -> str:
        """获取备份文件路径"""
        return str(self.backup_path)
    
    def get_meta_path(self) -> str:
        """获取元数据文件路径"""
        return str(self.meta_path)