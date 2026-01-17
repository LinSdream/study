# slim_jsonl.py
import json
import os
import sys
import shutil
import tempfile
import time
import argparse
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Any, Optional, Tuple, Set

class ConfigManager:
    """配置文件管理器"""
    
    def __init__(self, config_dir):
        self.config_dir = Path(config_dir)
        self.config_file = self.config_dir / "config.json"
        self.default_config = {
            "clearSwipes": True,
            "noClearLine": 10,
            "clearVariables": False,
            "clearVariables_initialized": False,
            "clearIs_ejs_processed": False,
            "clearField": False,
            "clearFieldArray": ["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"]
        }
    
    def load_config(self) -> Dict[str, Any]:
        """加载配置文件"""
        if not self.config_file.exists():
            # 如果配置文件不存在，创建默认配置
            self.save_config(self.default_config)
            print(f"使用默认配置: {self.default_config}")
            return self.default_config.copy()
        
        try:
            with open(self.config_file, 'r', encoding='utf-8') as f:
                config = json.load(f)
            
            # 合并配置，确保所有必需字段都存在
            merged_config = self.default_config.copy()
            merged_config.update(config)
            
            # 确保clearFieldArray是列表
            if 'clearFieldArray' in merged_config and not isinstance(merged_config['clearFieldArray'], list):
                merged_config['clearFieldArray'] = self.default_config['clearFieldArray']
            
            print(f"加载配置文件: {merged_config}")
            return merged_config
        except Exception as e:
            print(f"加载配置文件失败，使用默认配置: {e}")
            return self.default_config.copy()
    
    def save_config(self, config: Dict[str, Any]) -> bool:
        """保存配置文件"""
        try:
            self.config_dir.mkdir(parents=True, exist_ok=True)
            with open(self.config_file, 'w', encoding='utf-8') as f:
                json.dump(config, f, indent=2, ensure_ascii=False)
            print(f"配置文件已保存: {self.config_file}")
            return True
        except Exception as e:
            print(f"保存配置文件失败: {e}")
            return False


class JSONLSlimmer:
    """JSONL文件瘦身处理器"""
    
    def __init__(self, input_file_path: str, dry_run: bool = False, verbose: bool = False, config: Optional[Dict[str, Any]] = None):
        self.input_file_path = Path(input_file_path).resolve()
        self.dry_run = dry_run
        self.verbose = verbose
        
        # 创建workspace目录结构
        self.workspace_dir = Path("workspace")
        self.parent_folder_name = self.input_file_path.parent.name or "root"
        self.project_dir = self.workspace_dir / self.parent_folder_name
        self.project_dir.mkdir(parents=True, exist_ok=True)
        
        # 初始化配置管理器
        self.config_manager = ConfigManager(self.project_dir)
        
        # 加载配置，优先使用传入的配置
        if config is None:
            self.config = self.config_manager.load_config()
        else:
            # 确保配置包含所有必需字段
            default_config = self.config_manager.default_config.copy()
            default_config.update(config)
            
            # 确保clearFieldArray是列表
            if 'clearFieldArray' in default_config and not isinstance(default_config['clearFieldArray'], list):
                default_config['clearFieldArray'] = self.config_manager.default_config['clearFieldArray']
            
            self.config = default_config
        
        # 解析要删除的字段列表
        self.fields_to_clear = set()
        if self.config.get('clearField', False):
            field_array = self.config.get('clearFieldArray', [])
            if isinstance(field_array, list):
                self.fields_to_clear = set(field_array)
        
        # 备份文件路径
        backup_filename = f"{self.input_file_path.stem}_{datetime.now().strftime('%Y%m%d_%H%M%S')}.bak"
        self.backup_path = self.project_dir / backup_filename
        
        # meta文件路径
        self.meta_path = self.project_dir / f"{self.input_file_path.stem}.meta.json"
        
        # 统计信息
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
            'operation_time': None
        }
        
        # 元数据信息
        self.metadata = {
            'operation_file': str(self.input_file_path),
            'backup_file': str(self.backup_path),
            'config': self.config,
            'operation_time': datetime.now().isoformat(),
            'total_lines': 0,
            'processed_lines': 0,
            'skipped_tail_lines': 0,
            'variables_cleaned_count': 0,
            'variables_initialized_cleaned_count': 0,
            'is_ejs_processed_cleaned_count': 0,
            'fields_cleaned_count': 0,
            'fields_to_clear': list(self.fields_to_clear),
            'backup_size': 0,
            'status': 'pending'
        }
    
    def get_total_lines(self) -> int:
        """获取文件总行数（用于计算跳过末尾行）"""
        if self.config.get('noClearLine', 0) <= 0:
            return 0  # 如果不需要跳过末尾行，返回0表示不需要特殊处理
        
        print("正在计算文件总行数...")
        line_count = 0
        
        try:
            with open(self.input_file_path, 'r', encoding='utf-8') as f:
                for _ in f:
                    line_count += 1
            return line_count
        except Exception as e:
            print(f"计算行数失败: {e}")
            return 0
    
    def should_skip_line(self, line_num: int, total_lines: int) -> bool:
        """判断是否应该跳过这一行（在末尾行范围内）"""
        no_clear_line = self.config.get('noClearLine', 10)
        
        # 如果不需要跳过末尾行或总行数未知
        if no_clear_line <= 0 or total_lines <= 0:
            return False
        
        # 如果行号在末尾noClearLine行内，则跳过
        return line_num > (total_lines - no_clear_line)
    
    def backup_file(self) -> bool:
        """备份原始文件"""
        if self.dry_run:
            print(f"干跑模式: 备份文件位置应为 {self.backup_path}")
            return True
            
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
            
            # 获取备份文件大小
            self.metadata['backup_size'] = os.path.getsize(self.backup_path)
            print(f"备份完成，大小: {self.metadata['backup_size'] / 1024 / 1024:.2f} MB")
            return True
        except Exception as e:
            print(f"备份文件失败: {e}")
            return False
    
    def should_process_line(self, data: Dict[str, Any]) -> bool:
        """判断是否应该处理这一行（基于配置）"""
        # 检查是否满足字段处理条件
        has_swipes_and_id = 'swipes' in data and 'swipe_id' in data
        
        # 检查是否满足清理variables的条件
        should_check_variables = (
            self.config.get('clearVariables', False) or 
            self.config.get('clearVariables_initialized', False) or 
            self.config.get('clearIs_ejs_processed', False)
        )
        
        if should_check_variables:
            # 检查是否满足清理variables的条件
            is_user = data.get('is_user', True)
            has_extra = 'extra' in data
            has_variables = 'variables' in data
            
            if not is_user and has_extra and has_variables:
                return True
        
        # 检查是否应该删除指定字段
        if self.config.get('clearField', False) and self.fields_to_clear:
            for field in self.fields_to_clear:
                if field in data:
                    return True
        
        return has_swipes_and_id
    
    def clean_variables_fields(self, data: Dict[str, Any]) -> Tuple[Dict[str, Any], Dict[str, int]]:
        """
        清理variables相关字段
        
        Returns:
            Tuple[清理后的数据, 清理统计]
        """
        cleanup_stats = {
            'variables_cleaned': 0,
            'variables_initialized_cleaned': 0,
            'is_ejs_processed_cleaned': 0
        }
        
        # 清理variables字段
        if self.config.get('clearVariables', False) and 'variables' in data:
            if isinstance(data['variables'], list) and len(data['variables']) > 0:
                # 替换为[{}, {}]
                data['variables'] = [{}, {}]
                cleanup_stats['variables_cleaned'] = 1
        
        # 清理variables_initialized字段
        if self.config.get('clearVariables_initialized', False) and 'variables_initialized' in data:
            if isinstance(data['variables_initialized'], list) and len(data['variables_initialized']) > 0:
                # 替换为[true, true]
                data['variables_initialized'] = [True, True]
                cleanup_stats['variables_initialized_cleaned'] = 1
        
        # 清理is_ejs_processed字段
        if self.config.get('clearIs_ejs_processed', False) and 'is_ejs_processed' in data:
            if isinstance(data['is_ejs_processed'], list) and len(data['is_ejs_processed']) > 0:
                # 替换为[true, true]
                data['is_ejs_processed'] = [True, True]
                cleanup_stats['is_ejs_processed_cleaned'] = 1
        
        return data, cleanup_stats
    
    def clear_specified_fields(self, data: Dict[str, Any]) -> Tuple[Dict[str, Any], int]:
        """
        删除配置中指定的字段
        
        Returns:
            Tuple[清理后的数据, 删除的字段数]
        """
        fields_removed = 0
        
        if self.config.get('clearField', False) and self.fields_to_clear:
            for field in self.fields_to_clear:
                if field in data:
                    del data[field]
                    fields_removed += 1
        
        return data, fields_removed
    
    def process_swipes_fields(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """处理swipes相关字段"""
        swipes = data['swipes']
        swipe_id = data['swipe_id']
        
        # 如果swipes已经是空数组
        if len(swipes) == 0:
            self.stats['already_empty'] += 1
            if self.verbose:
                print(f"swipes已为空数组")
            # 只更新swipe_id为0
            data['swipe_id'] = 0
            return data
        
        # 检查swipe_id是否有效
        if not isinstance(swipe_id, int) or not (0 <= swipe_id < len(swipes)):
            self.stats['errors'] += 1
            if self.verbose:
                print(f"无效索引 {swipe_id} (swipes长度: {len(swipes)})")
            return data  # 返回原始数据，不做修改
        
        # 执行修改
        # 1. 将mes设置为swipes[swipe_id]
        data['mes'] = swipes[swipe_id]
        
        # 2. 根据配置决定是否清空swipes
        if self.config.get('clearSwipes', True):
            # 清空swipes
            data['swipes'] = []
            action = "swipes清空"
        else:
            # 保留swipes数组，只保留对应的index
            if 0 <= swipe_id < len(swipes):
                data['swipes'] = [swipes[swipe_id]]
                action = f"swipes保留索引{swipe_id}"
            else:
                data['swipes'] = []
                action = "swipes清空（索引无效）"
        
        # 3. 将swipe_id设置为0
        data['swipe_id'] = 0
        
        self.stats['processed'] += 1
        
        if self.verbose and self.stats['processed'] <= 10:  # 只显示前10条详细日志
            print(f"mes设置为swipes[{swipe_id}]，{action}")
        
        return data
    
    def process_data(self, data: Dict[str, Any], line_num: int) -> Dict[str, Any]:
        """处理单个JSON对象"""
        # 首先清理variables相关字段
        cleaned_data, cleanup_stats = self.clean_variables_fields(data)
        
        # 更新统计
        self.stats['variables_cleaned'] += cleanup_stats['variables_cleaned']
        self.stats['variables_initialized_cleaned'] += cleanup_stats['variables_initialized_cleaned']
        self.stats['is_ejs_processed_cleaned'] += cleanup_stats['is_ejs_processed_cleaned']
        
        # 然后删除指定字段
        cleaned_data, fields_removed = self.clear_specified_fields(cleaned_data)
        self.stats['fields_cleaned'] += fields_removed
        
        # 如果有清理操作，记录日志
        cleanup_logs = []
        if cleanup_stats['variables_cleaned']:
            cleanup_logs.append("variables清空")
        if cleanup_stats['variables_initialized_cleaned']:
            cleanup_logs.append("variables_initialized清空")
        if cleanup_stats['is_ejs_processed_cleaned']:
            cleanup_logs.append("is_ejs_processed清空")
        if fields_removed > 0:
            cleanup_logs.append(f"{fields_removed}个指定字段删除")
        
        if cleanup_logs and self.verbose and line_num <= 10:  # 只显示前10条详细日志
            print(f"第{line_num}行: {'、'.join(cleanup_logs)}")
        
        # 最后处理swipes相关字段
        if 'swipes' in cleaned_data and 'swipe_id' in cleaned_data:
            cleaned_data = self.process_swipes_fields(cleaned_data)
        
        return cleaned_data
    
    def process_file(self) -> bool:
        """处理整个文件"""
        start_time = time.time()
        
        # 获取总行数，用于计算跳过末尾行
        total_lines = self.get_total_lines()
        skip_tail_lines = self.config.get('noClearLine', 10)
        
        if skip_tail_lines > 0 and total_lines > 0:
            print(f"文件总行数: {total_lines}, 跳过末尾 {skip_tail_lines} 行")
            self.metadata['skipped_tail_lines'] = skip_tail_lines
        
        if self.dry_run:
            print("干跑模式: 只分析，不修改文件")
            temp_output = None
        else:
            # 创建临时文件
            temp_fd, temp_path = tempfile.mkstemp(
                suffix='.tmp', 
                dir=os.path.dirname(self.input_file_path) or '.'
            )
            temp_output = os.fdopen(temp_fd, 'w', encoding='utf-8')
        
        try:
            with open(self.input_file_path, 'r', encoding='utf-8') as infile:
                for line_num, line in enumerate(infile, 1):
                    self.stats['total_lines'] += 1
                    
                    # 进度显示
                    if line_num % 100000 == 0:
                        print(f"处理进度: {line_num} 行")
                        if self.dry_run and self.verbose:
                            self.print_progress_stats()
                    
                    # 检查是否应该跳过末尾行
                    if self.should_skip_line(line_num, total_lines):
                        self.stats['skipped_tail'] += 1
                        if not self.dry_run:
                            temp_output.write(line)
                        continue
                    
                    stripped_line = line.strip()
                    if not stripped_line:
                        # 空行
                        if not self.dry_run:
                            temp_output.write('\n')
                        continue
                    
                    try:
                        data = json.loads(stripped_line)
                        
                        # 检查是否需要处理
                        if self.should_process_line(data):
                            processed_data = self.process_data(data, line_num)
                            output_line = json.dumps(processed_data, ensure_ascii=False)
                        else:
                            self.stats['skipped'] += 1
                            output_line = stripped_line
                        
                        # 写入输出
                        if not self.dry_run:
                            temp_output.write(output_line + '\n')
                            
                    except json.JSONDecodeError as e:
                        self.stats['errors'] += 1
                        if self.verbose:
                            print(f"第{line_num}行: JSON解析错误 - {e}")
                        if not self.dry_run:
                            temp_output.write(line.rstrip('\n') + '\n')
                    except Exception as e:
                        self.stats['errors'] += 1
                        print(f"第{line_num}行: 处理错误 - {e}")
                        if not self.dry_run:
                            temp_output.write(line.rstrip('\n') + '\n')
            
            # 更新统计信息
            end_time = time.time()
            self.stats['operation_time'] = end_time - start_time
            
            # 更新元数据
            self.metadata['total_lines'] = self.stats['total_lines']
            self.metadata['processed_lines'] = self.stats['processed']
            self.metadata['skipped_tail_lines_count'] = self.stats['skipped_tail']
            self.metadata['variables_cleaned_count'] = self.stats['variables_cleaned']
            self.metadata['variables_initialized_cleaned_count'] = self.stats['variables_initialized_cleaned']
            self.metadata['is_ejs_processed_cleaned_count'] = self.stats['is_ejs_processed_cleaned']
            self.metadata['fields_cleaned_count'] = self.stats['fields_cleaned']
            self.metadata['operation_duration'] = self.stats['operation_time']
            
            # 如果不是干跑模式，替换原文件并保存元数据
            if not self.dry_run:
                temp_output.close()  # 确保文件已关闭
                
                # 替换原文件
                os.replace(temp_path, self.input_file_path)
                
                # 保存元数据
                self.metadata['status'] = 'completed'
                self.metadata['completion_time'] = datetime.now().isoformat()
                self.save_metadata()
                
                # 保存当前配置到配置文件
                self.config_manager.save_config(self.config)
            
            return True
            
        except Exception as e:
            print(f"处理文件时出错: {e}")
            self.metadata['status'] = 'failed'
            self.metadata['error'] = str(e)
            if not self.dry_run:
                self.save_metadata()
            return False
        finally:
            # 清理临时文件
            if not self.dry_run and 'temp_path' in locals() and os.path.exists(temp_path):
                os.remove(temp_path)
    
    def print_progress_stats(self):
        """打印进度统计信息"""
        print(f"进度统计: 已处理 {self.stats['total_lines']} 行，"
              f"清理variables: {self.stats['variables_cleaned']}，"
              f"清理variables_initialized: {self.stats['variables_initialized_cleaned']}，"
              f"清理is_ejs_processed: {self.stats['is_ejs_processed_cleaned']}，"
              f"删除字段: {self.stats['fields_cleaned']}")
    
    def save_metadata(self) -> bool:
        """保存元数据文件"""
        if self.dry_run:
            return False
            
        try:
            with open(self.meta_path, 'w', encoding='utf-8') as f:
                json.dump(self.metadata, f, indent=2, ensure_ascii=False)
            print(f"元数据文件已保存: {self.meta_path}")
            return True
        except Exception as e:
            print(f"保存元数据文件失败: {e}")
            return False
    
    def print_stats(self):
        """打印统计信息"""
        print("\n" + "="*90)
        print("JSONL文件瘦身处理统计:")
        print(f"总行数: {self.stats['total_lines']}")
        
        # 显示清理统计
        print(f"\n清理统计:")
        if self.config.get('clearSwipes', True):
            print(f"  swipes字段清理: {self.stats['processed']} 行")
            print(f"  swipes已为空的: {self.stats['already_empty']} 行")
        
        if self.config.get('clearVariables', False):
            print(f"  variables字段清理: {self.stats['variables_cleaned']} 行")
        
        if self.config.get('clearVariables_initialized', False):
            print(f"  variables_initialized字段清理: {self.stats['variables_initialized_cleaned']} 行")
        
        if self.config.get('clearIs_ejs_processed', False):
            print(f"  is_ejs_processed字段清理: {self.stats['is_ejs_processed_cleaned']} 行")
        
        if self.config.get('clearField', False) and self.fields_to_clear:
            print(f"  删除指定字段: {self.stats['fields_cleaned']} 个字段")
            if len(self.fields_to_clear) <= 5:  # 如果字段不多，显示具体字段
                print(f"    要删除的字段: {', '.join(self.fields_to_clear)}")
            else:
                print(f"    要删除的字段: {len(self.fields_to_clear)} 个字段")
        
        print(f"\n跳过统计:")
        print(f"  跳过的末尾行数: {self.stats['skipped_tail']}")
        print(f"  跳过的其他行数: {self.stats['skipped']}")
        print(f"  错误行数: {self.stats['errors']}")
        
        if self.stats['operation_time']:
            print(f"\n处理耗时: {self.stats['operation_time']:.2f} 秒")
        
        print("="*90)
        
        # 显示配置信息
        print(f"配置信息:")
        print(f"  clearSwipes: {self.config.get('clearSwipes', True)}")
        print(f"  noClearLine: {self.config.get('noClearLine', 10)}")
        print(f"  clearVariables: {self.config.get('clearVariables', False)}")
        print(f"  clearVariables_initialized: {self.config.get('clearVariables_initialized', False)}")
        print(f"  clearIs_ejs_processed: {self.config.get('clearIs_ejs_processed', False)}")
        print(f"  clearField: {self.config.get('clearField', False)}")
        if self.config.get('clearField', False):
            field_array = self.config.get('clearFieldArray', [])
            if isinstance(field_array, list):
                print(f"  clearFieldArray: {field_array}")
        print("="*90)
        
        # 计算瘦身效果（估算）
        if not self.dry_run:
            try:
                original_size = os.path.getsize(self.backup_path)
                new_size = os.path.getsize(self.input_file_path)
                reduction = original_size - new_size
                reduction_percent = (reduction / original_size * 100) if original_size > 0 else 0
                
                print(f"\n瘦身效果:")
                print(f"  原始文件大小: {original_size / 1024 / 1024:.2f} MB")
                print(f"  处理后大小: {new_size / 1024 / 1024:.2f} MB")
                print(f"  减少大小: {reduction / 1024 / 1024:.2f} MB")
                print(f"  减少比例: {reduction_percent:.2f}%")
                print("="*90)
            except Exception as e:
                print(f"无法计算瘦身效果: {e}")


def process_jsonl_file(input_file_path: str, dry_run: bool = False, verbose: bool = False, 
                      no_confirm: bool = False, config: Optional[Dict[str, Any]] = None) -> bool:
    """
    处理JSONL文件
    
    Args:
        input_file_path: 输入文件路径
        dry_run: 干跑模式，只分析不修改
        verbose: 详细输出模式
        no_confirm: 跳过确认提示
        config: 自定义配置字典
    """
    # 检查文件是否存在
    if not os.path.exists(input_file_path):
        print(f"错误: 文件不存在 - {input_file_path}")
        return False
    
    # 检查文件大小
    file_size = os.path.getsize(input_file_path)
    file_size_mb = file_size / (1024 * 1024)
    file_size_gb = file_size / (1024 * 1024 * 1024)
    
    print(f"文件: {input_file_path}")
    print(f"大小: {file_size_mb:.2f} MB ({file_size_gb:.2f} GB)")
    
    # 创建处理器
    processor = JSONLSlimmer(input_file_path, dry_run, verbose, config)
    
    # 确认操作
    if not dry_run and not no_confirm:
        print("\n" + "="*70)
        print("配置概览:")
        print(f"  clearSwipes: {processor.config.get('clearSwipes', True)}")
        print(f"  noClearLine: {processor.config.get('noClearLine', 10)}")
        print(f"  clearVariables: {processor.config.get('clearVariables', False)}")
        print(f"  clearVariables_initialized: {processor.config.get('clearVariables_initialized', False)}")
        print(f"  clearIs_ejs_processed: {processor.config.get('clearIs_ejs_processed', False)}")
        print(f"  clearField: {processor.config.get('clearField', False)}")
        if processor.config.get('clearField', False):
            field_array = processor.config.get('clearFieldArray', [])
            if isinstance(field_array, list) and field_array:
                print(f"  clearFieldArray: {field_array}")
        print("="*70)
        
        confirm = input("确认开始处理文件？(y/n): ")
        if confirm.lower() != 'y':
            print("操作已取消")
            return False
    
    # 备份文件
    if not dry_run:
        if not processor.backup_file():
            return False
    
    # 处理文件
    print("\n开始处理文件...")
    
    if processor.process_file():
        processor.print_stats()
        
        print(f"\n处理完成!")
        if not dry_run:
            print(f"原始文件已更新: {input_file_path}")
            print(f"备份文件: {processor.backup_path}")
            print(f"元数据文件: {processor.meta_path}")
            print(f"配置文件: {processor.config_manager.config_file}")
        else:
            print("干跑模式: 文件未被修改")
        return True
    else:
        print("处理失败")
        return False


def edit_config(config_file: str) -> bool:
    """编辑配置文件"""
    if not os.path.exists(config_file):
        print(f"配置文件不存在: {config_file}")
        return False
    
    try:
        with open(config_file, 'r', encoding='utf-8') as f:
            config = json.load(f)
        
        print(f"当前配置: {config}")
        
        # 配置项描述
        config_descriptions = {
            "clearSwipes": "是否清空swipes字段（true/false）",
            "noClearLine": "跳过的末尾行数（整数）",
            "clearVariables": "是否清理variables字段（true/false）",
            "clearVariables_initialized": "是否清理variables_initialized字段（true/false）",
            "clearIs_ejs_processed": "是否清理is_ejs_processed字段（true/false）",
            "clearField": "是否删除指定字段（true/false）",
            "clearFieldArray": "要删除的字段列表（JSON数组，例如：[\"field1\",\"field2\"]）"
        }
        
        # 提供编辑选项
        print("\n编辑配置:")
        for i, (key, desc) in enumerate(config_descriptions.items(), 1):
            print(f"{i}. {key} - {desc}")
        
        print(f"{len(config_descriptions)+1}. 保存并退出")
        print(f"{len(config_descriptions)+2}. 退出不保存")
        
        while True:
            try:
                choice = input(f"\n选择操作 (1-{len(config_descriptions)+2}): ").strip()
                
                if choice.isdigit():
                    choice_num = int(choice)
                    
                    if 1 <= choice_num <= len(config_descriptions):
                        key = list(config_descriptions.keys())[choice_num - 1]
                        
                        if key == "clearFieldArray":
                            current = config.get(key, ["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"])
                            print(f"{key} 当前值: {current}")
                            print("输入新的字段列表（JSON格式，例如：[\"field1\",\"field2\"]）")
                            new_value = input("新值: ").strip()
                            try:
                                if new_value:
                                    parsed = json.loads(new_value)
                                    if isinstance(parsed, list):
                                        config[key] = parsed
                                        print(f"{key} 已更新为: {config[key]}")
                                    else:
                                        print("输入无效，请输入JSON数组")
                                else:
                                    print("输入为空，保持不变")
                            except json.JSONDecodeError:
                                print("JSON解析错误，请输入有效的JSON数组")
                        
                        elif key == "noClearLine":
                            current = config.get(key, 10)
                            new_value = input(f"{key} 当前值: {current}，新值 (整数): ").strip()
                            try:
                                config[key] = int(new_value)
                                print(f"{key} 已更新为: {config[key]}")
                            except ValueError:
                                print("输入无效，请输入整数")
                        
                        else:
                            current = config.get(key, True if key == "clearSwipes" else False)
                            new_value = input(f"{key} 当前值: {current}，新值 (true/false): ").strip().lower()
                            if new_value in ["true", "false"]:
                                config[key] = new_value == "true"
                                print(f"{key} 已更新为: {config[key]}")
                            else:
                                print("输入无效，请输入 true 或 false")
                    
                    elif choice_num == len(config_descriptions) + 1:
                        with open(config_file, 'w', encoding='utf-8') as f:
                            json.dump(config, f, indent=2, ensure_ascii=False)
                        print(f"配置已保存: {config_file}")
                        return True
                    
                    elif choice_num == len(config_descriptions) + 2:
                        print("退出配置编辑")
                        return False
                    
                    else:
                        print(f"无效选择，请输入 1-{len(config_descriptions)+2}")
                else:
                    print("请输入数字")
            
            except Exception as e:
                print(f"输入错误: {e}")
    
    except Exception as e:
        print(f"编辑配置失败: {e}")
        return False


def show_config(input_file_path: str) -> bool:
    """显示配置文件内容"""
    input_path = Path(input_file_path).resolve()
    parent_folder_name = input_path.parent.name or "root"
    project_dir = Path("workspace") / parent_folder_name
    config_file = project_dir / "config.json"
    
    if not config_file.exists():
        print(f"配置文件不存在: {config_file}")
        print("将使用默认配置:")
        print("  clearSwipes: true")
        print("  noClearLine: 10")
        print("  clearVariables: false")
        print("  clearVariables_initialized: false")
        print("  clearIs_ejs_processed: false")
        print("  clearField: false")
        print("  clearFieldArray: ['TavernDB_ACU_IsolatedData', 'TavernDB_ACU_IndependentData']")
        return False
    
    try:
        with open(config_file, 'r', encoding='utf-8') as f:
            config = json.load(f)
        
        print(f"配置文件: {config_file}")
        print(f"配置内容:")
        
        default_config = {
            "clearSwipes": True,
            "noClearLine": 10,
            "clearVariables": False,
            "clearVariables_initialized": False,
            "clearIs_ejs_processed": False,
            "clearField": False,
            "clearFieldArray": ["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"]
        }
        
        for key, default_value in default_config.items():
            value = config.get(key, default_value)
            print(f"  {key}: {value}")
        return True
    except Exception as e:
        print(f"读取配置文件失败: {e}")
        return False


def analyze_file(input_file_path: str) -> bool:
    """分析文件内容，提供优化建议"""
    print(f"分析文件: {input_file_path}")
    
    if not os.path.exists(input_file_path):
        print(f"错误: 文件不存在 - {input_file_path}")
        return False
    
    stats = {
        'total_lines': 0,
        'has_swipes': 0,
        'has_variables': 0,
        'is_user_false': 0,
        'has_extra': 0,
        'meets_variables_condition': 0,
        'variables_size_sum': 0,
        'variables_initialized_size_sum': 0,
        'is_ejs_processed_size_sum': 0,
        'field_stats': {},  # 统计每个字段的出现次数和大小
        'sample_fields': set()  # 采样字段名
    }
    
    # 要检查的常见字段
    common_fields_to_check = [
        "TavernDB_ACU_IsolatedData",
        "TavernDB_ACU_IndependentData",
        "extra",
        "name",
        "character_name",
        "create_date",
        "send_date",
        "mes",
        "title",
        "swipes",
        "swipe_id",
        "swipe_info",
        "variables",
        "variables_initialized",
        "is_ejs_processed",
        "is_user",
        "is_system",
        "force_avatar",
        "gen_started",
        "gen_finished"
    ]
    
    try:
        with open(input_file_path, 'r', encoding='utf-8') as f:
            for line_num, line in enumerate(f, 1):
                stats['total_lines'] += 1
                
                # 每处理100000行输出一次进度
                if line_num % 100000 == 0:
                    print(f"分析进度: {line_num} 行")
                
                stripped_line = line.strip()
                if not stripped_line:
                    continue
                
                try:
                    data = json.loads(stripped_line)
                    
                    # 检查swipes字段
                    if 'swipes' in data and 'swipe_id' in data:
                        stats['has_swipes'] += 1
                    
                    # 检查variables字段
                    if 'variables' in data:
                        stats['has_variables'] += 1
                        # 估算variables字段大小
                        variables_str = json.dumps(data['variables'])
                        stats['variables_size_sum'] += len(variables_str.encode('utf-8'))
                    
                    # 检查其他字段
                    if data.get('is_user') == False:
                        stats['is_user_false'] += 1
                    
                    if 'extra' in data:
                        stats['has_extra'] += 1
                    
                    # 检查是否满足清理variables的条件
                    if (data.get('is_user') == False and 
                        'extra' in data and 
                        'variables' in data):
                        stats['meets_variables_condition'] += 1
                    
                    # 检查variables_initialized字段
                    if 'variables_initialized' in data:
                        var_init_str = json.dumps(data['variables_initialized'])
                        stats['variables_initialized_size_sum'] += len(var_init_str.encode('utf-8'))
                    
                    # 检查is_ejs_processed字段
                    if 'is_ejs_processed' in data:
                        is_ejs_str = json.dumps(data['is_ejs_processed'])
                        stats['is_ejs_processed_size_sum'] += len(is_ejs_str.encode('utf-8'))
                    
                    # 统计字段使用情况
                    for field in common_fields_to_check:
                        if field in data:
                            if field not in stats['field_stats']:
                                stats['field_stats'][field] = {
                                    'count': 0,
                                    'total_size': 0
                                }
                            stats['field_stats'][field]['count'] += 1
                            
                            # 估算字段大小
                            field_str = json.dumps(data[field])
                            stats['field_stats'][field]['total_size'] += len(field_str.encode('utf-8'))
                    
                    # 采样其他字段（前50行）
                    if line_num <= 50:
                        for field in data.keys():
                            if field not in common_fields_to_check:
                                stats['sample_fields'].add(field)
                
                except:
                    continue
        
        # 输出分析结果
        print("\n" + "="*90)
        print("文件分析结果:")
        print(f"总行数: {stats['total_lines']}")
        
        print(f"\nswipes字段统计:")
        print(f"  包含swipes和swipe_id的行数: {stats['has_swipes']} ({stats['has_swipes']/stats['total_lines']*100:.1f}%)")
        
        print(f"\nvariables字段统计:")
        print(f"  包含variables的行数: {stats['has_variables']} ({stats['has_variables']/stats['total_lines']*100:.1f}%)")
        if stats['has_variables'] > 0:
            avg_variables_size = stats['variables_size_sum'] / stats['has_variables']
            print(f"  平均variables字段大小: {avg_variables_size:.0f} 字节")
        
        print(f"\n清理条件统计:")
        print(f"  is_user=false的行数: {stats['is_user_false']}")
        print(f"  包含extra字段的行数: {stats['has_extra']}")
        print(f"  满足variables清理条件的行数: {stats['meets_variables_condition']}")
        
        print(f"\n其他字段大小统计:")
        if stats['variables_initialized_size_sum'] > 0:
            avg_var_init_size = stats['variables_initialized_size_sum'] / stats['has_variables']
            print(f"  平均variables_initialized字段大小: {avg_var_init_size:.0f} 字节")
        
        if stats['is_ejs_processed_size_sum'] > 0:
            avg_is_ejs_size = stats['is_ejs_processed_size_sum'] / stats['has_variables']
            print(f"  平均is_ejs_processed字段大小: {avg_is_ejs_size:.0f} 字节")
        
        # 字段使用统计
        print(f"\n字段使用统计（出现次数最多的字段）:")
        sorted_fields = sorted(
            stats['field_stats'].items(),
            key=lambda x: x[1]['total_size'],
            reverse=True
        )[:10]  # 只显示前10个
        
        for field, field_stats in sorted_fields:
            percentage = (field_stats['count'] / stats['total_lines']) * 100
            avg_size = field_stats['total_size'] / field_stats['count'] if field_stats['count'] > 0 else 0
            print(f"  {field}: {field_stats['count']} 行 ({percentage:.1f}%), "
                  f"平均大小: {avg_size:.0f} 字节")
        
        # 采样字段
        if stats['sample_fields']:
            print(f"\n采样发现的字段（前50行）:")
            sample_list = list(stats['sample_fields'])
            if len(sample_list) > 20:
                print(f"  发现 {len(sample_list)} 个其他字段，前20个: {sample_list[:20]}")
            else:
                print(f"  发现 {len(sample_list)} 个其他字段: {sample_list}")
        
        print("\n优化建议:")
        if stats['has_swipes'] > 100:
            print("  ✓ 建议启用 clearSwipes: true")
        
        if stats['meets_variables_condition'] > 100:
            print(f"  ✓ 建议启用 clearVariables: true (可清理 {stats['meets_variables_condition']} 行)")
        
        if stats['variables_initialized_size_sum'] > 1024 * 1024:  # 超过1MB
            print(f"  ✓ 建议启用 clearVariables_initialized: true (可节省约 {stats['variables_initialized_size_sum']/1024/1024:.2f} MB)")
        
        if stats['is_ejs_processed_size_sum'] > 1024 * 1024:  # 超过1MB
            print(f"  ✓ 建议启用 clearIs_ejs_processed: true (可节省约 {stats['is_ejs_processed_size_sum']/1024/1024:.2f} MB)")
        
        # 检查常见字段是否出现频繁
        for field in ["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"]:
            if field in stats['field_stats']:
                field_count = stats['field_stats'][field]['count']
                field_size = stats['field_stats'][field]['total_size']
                if field_count > 100:
                    print(f"  ✓ 建议在clearFieldArray中添加 '{field}' (出现 {field_count} 次，占用约 {field_size/1024/1024:.2f} MB)")
        
        if stats['total_lines'] > 10000:
            print(f"  ✓ 建议设置 noClearLine: 100 (跳过最后100行，保留原始数据)")
        
        print("="*90)
        return True
    
    except Exception as e:
        print(f"分析文件失败: {e}")
        return False


def main():
    """主函数"""
    parser = argparse.ArgumentParser(description='JSONL文件瘦身工具 - 清理冗余字段，减小文件体积')
    parser.add_argument('input_file', help='输入文件路径')
    
    # 处理模式选项
    group_mode = parser.add_mutually_exclusive_group()
    group_mode.add_argument('--dry-run', action='store_true', 
                           help='干跑模式，只分析不修改文件')
    group_mode.add_argument('--edit-config', action='store_true',
                           help='编辑配置文件')
    group_mode.add_argument('--show-config', action='store_true',
                           help='显示配置文件')
    group_mode.add_argument('--analyze', action='store_true',
                           help='分析文件内容，提供优化建议')
    
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='详细输出模式')
    parser.add_argument('--no-confirm', action='store_true',
                       help='跳过确认提示（谨慎使用）')
    
    # 配置参数
    parser.add_argument('--clear-swipes', type=lambda x: x.lower() == 'true',
                       help='是否清空swipes (true/false)')
    parser.add_argument('--no-clear-line', type=int,
                       help='跳过的末尾行数')
    parser.add_argument('--clear-variables', type=lambda x: x.lower() == 'true',
                       help='是否清理variables字段 (true/false)')
    parser.add_argument('--clear-variables-initialized', type=lambda x: x.lower() == 'true',
                       help='是否清理variables_initialized字段 (true/false)')
    parser.add_argument('--clear-is-ejs-processed', type=lambda x: x.lower() == 'true',
                       help='是否清理is_ejs_processed字段 (true/false)')
    parser.add_argument('--clear-field', type=lambda x: x.lower() == 'true',
                       help='是否删除指定字段 (true/false)')
    parser.add_argument('--clear-field-array', type=str,
                       help='要删除的字段列表 (JSON数组字符串，例如：\'["field1","field2"]\')')
    
    args = parser.parse_args()
    
    # 如果指定了分析模式
    if args.analyze:
        success = analyze_file(args.input_file)
        sys.exit(0 if success else 1)
    
    # 如果指定了编辑配置
    if args.edit_config:
        input_path = Path(args.input_file).resolve()
        parent_folder_name = input_path.parent.name or "root"
        project_dir = Path("workspace") / parent_folder_name
        config_file = project_dir / "config.json"
        
        edit_config(config_file)
        sys.exit(0)
    
    # 如果指定了显示配置
    if args.show_config:
        show_config(args.input_file)
        sys.exit(0)
    
    # 构建配置字典
    config = {}
    if args.clear_swipes is not None:
        config['clearSwipes'] = args.clear_swipes
    if args.no_clear_line is not None:
        config['noClearLine'] = args.no_clear_line
    if args.clear_variables is not None:
        config['clearVariables'] = args.clear_variables
    if args.clear_variables_initialized is not None:
        config['clearVariables_initialized'] = args.clear_variables_initialized
    if args.clear_is_ejs_processed is not None:
        config['clearIs_ejs_processed'] = args.clear_is_ejs_processed
    if args.clear_field is not None:
        config['clearField'] = args.clear_field
    if args.clear_field_array is not None:
        try:
            config['clearFieldArray'] = json.loads(args.clear_field_array)
        except json.JSONDecodeError:
            print(f"错误: clear-field-array 参数必须是有效的JSON数组字符串")
            sys.exit(1)
    
    # 处理文件
    success = process_jsonl_file(
        args.input_file, 
        dry_run=args.dry_run, 
        verbose=args.verbose,
        no_confirm=args.no_confirm,
        config=config if config else None
    )
    
    if success:
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()