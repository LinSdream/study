# stats_collector.py
import os
import time
from typing import Dict, Any, Optional
from datetime import datetime


class StatsCollector:
    """统计收集器"""
    
    def __init__(self):
        self.stats = {
            'total_lines': 0,
            'processed': 0,
            'skipped_tail': 0,
            'variables_cleaned': 0,
            'variables_initialized_cleaned': 0,
            'is_ejs_processed_cleaned': 0,
            'fields_cleaned': 0,
            'errors': 0,
            'skipped': 0,
            'already_empty': 0,
            'operation_time': None,
            'start_time': None,
            'end_time': None
        }
        
        self.metadata = {
            'operation_time': datetime.now().isoformat(),
            'total_lines': 0,
            'processed_lines': 0,
            'skipped_tail_lines': 0,
            'variables_cleaned_count': 0,
            'variables_initialized_cleaned_count': 0,
            'is_ejs_processed_cleaned_count': 0,
            'fields_cleaned_count': 0,
            'fields_to_clear': [],
            'backup_size': 0,
            'status': 'pending'
        }
    
    def start_timer(self) -> None:
        """开始计时"""
        self.stats['start_time'] = time.time()
    
    def stop_timer(self) -> None:
        """停止计时"""
        self.stats['end_time'] = time.time()
        self.stats['operation_time'] = self.stats['end_time'] - self.stats['start_time']
        self.metadata['operation_duration'] = self.stats['operation_time']
    
    def update_stats(self, stats_updates: Dict[str, Any]) -> None:
        """更新统计信息"""
        for key, value in stats_updates.items():
            if key in self.stats:
                if isinstance(self.stats[key], (int, float)):
                    self.stats[key] += value
                else:
                    self.stats[key] = value
    
    def update_metadata(self, metadata_updates: Dict[str, Any]) -> None:
        """更新元数据"""
        self.metadata.update(metadata_updates)
    
    def calculate_savings(self, original_file: str, processed_file: str) -> Optional[Dict[str, float]]:
        """计算瘦身效果"""
        try:
            original_size = os.path.getsize(original_file)
            new_size = os.path.getsize(processed_file)
            reduction = original_size - new_size
            reduction_percent = (reduction / original_size * 100) if original_size > 0 else 0
            
            return {
                'original_size_mb': original_size / 1024 / 1024,
                'new_size_mb': new_size / 1024 / 1024,
                'reduction_mb': reduction / 1024 / 1024,
                'reduction_percent': reduction_percent
            }
        except Exception:
            return None
    
    def print_stats(self, config: Dict[str, Any], fields_to_clear: set) -> None:
        """打印统计信息"""
        print("\n" + "="*90)
        print("JSONL文件瘦身处理统计:")
        print(f"总行数: {self.stats['total_lines']}")
        
        # 显示清理统计
        print(f"\n清理统计:")
        if config.get('clearSwipes', True):
            print(f"  swipes字段清理: {self.stats['processed']} 行")
            print(f"  swipes已为空的: {self.stats['already_empty']} 行")
        
        if config.get('clearVariables', False):
            print(f"  variables字段清理: {self.stats['variables_cleaned']} 行")
        
        if config.get('clearVariables_initialized', False):
            print(f"  variables_initialized字段清理: {self.stats['variables_initialized_cleaned']} 行")
        
        if config.get('clearIs_ejs_processed', False):
            print(f"  is_ejs_processed字段清理: {self.stats['is_ejs_processed_cleaned']} 行")
        
        if config.get('clearField', False) and fields_to_clear:
            print(f"  删除指定字段: {self.stats['fields_cleaned']} 个字段")
            if len(fields_to_clear) <= 5:  # 如果字段不多，显示具体字段
                print(f"    要删除的字段: {', '.join(fields_to_clear)}")
            else:
                print(f"    要删除的字段: {len(fields_to_clear)} 个字段")
        
        print(f"\n跳过统计:")
        print(f"  跳过的末尾行数: {self.stats['skipped_tail']}")
        print(f"  跳过的其他行数: {self.stats['skipped']}")
        print(f"  错误行数: {self.stats['errors']}")
        
        if self.stats['operation_time']:
            print(f"\n处理耗时: {self.stats['operation_time']:.2f} 秒")
        
        print("="*90)
    
    def print_config_info(self, config: Dict[str, Any]) -> None:
        """打印配置信息"""
        print(f"配置信息:")
        print(f"  clearSwipes: {config.get('clearSwipes', True)}")
        print(f"  noClearLine: {config.get('noClearLine', 10)}")
        print(f"  clearVariables: {config.get('clearVariables', False)}")
        print(f"  clearVariables_initialized: {config.get('clearVariables_initialized', False)}")
        print(f"  clearIs_ejs_processed: {config.get('clearIs_ejs_processed', False)}")
        print(f"  clearField: {config.get('clearField', False)}")
        if config.get('clearField', False):
            field_array = config.get('clearFieldArray', [])
            if isinstance(field_array, list):
                print(f"  clearFieldArray: {field_array}")
        print("="*90)
    
    def get_stats(self) -> Dict[str, Any]:
        """获取统计信息"""
        return self.stats.copy()
    
    def get_metadata(self) -> Dict[str, Any]:
        """获取元数据"""
        return self.metadata.copy()