# slim_jsonl.py
import os
import sys
import argparse
import json
from pathlib import Path
from typing import Dict, Any, Optional

# 导入模块
from config_manager import ConfigManager, ConfigEditor
from backup_manager import BackupManager
from field_cleaner import FieldCleaner
from file_analyzer import FileAnalyzer
from stats_collector import StatsCollector
from file_processor import FileProcessor


class JSONLSlimmer:
    """JSONL文件瘦身处理器（主程序）"""
    
    def __init__(self, input_file_path: str, dry_run: bool = False, verbose: bool = False, 
                 config: Optional[Dict[str, Any]] = None):
        self.input_file_path = Path(input_file_path).resolve()
        self.dry_run = dry_run
        self.verbose = verbose
        
        # 创建workspace目录结构
        self.workspace_dir = Path("workspace")
        self.parent_folder_name = self.input_file_path.parent.name or "root"
        self.project_dir = self.workspace_dir / self.parent_folder_name
        
        # 初始化各个模块
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
        
        # 初始化其他模块
        self.backup_manager = BackupManager(input_file_path, self.project_dir)
        self.field_cleaner = FieldCleaner(self.config)
        self.stats_collector = StatsCollector()
        self.file_processor = FileProcessor()
        
        # 更新元数据中的配置信息
        self.stats_collector.update_metadata({
            'config': self.config,
            'fields_to_clear': list(self.field_cleaner.get_fields_to_clear())
        })
    
    def process_file(self) -> bool:
        """处理整个文件"""
        # 开始计时
        self.stats_collector.start_timer()
        
        # 获取总行数，用于计算跳过末尾行
        total_lines = self.file_processor.get_total_lines(str(self.input_file_path))
        skip_tail_lines = self.config.get('noClearLine', 10)
        
        if skip_tail_lines > 0 and total_lines > 0:
            print(f"文件总行数: {total_lines}, 跳过末尾 {skip_tail_lines} 行")
            self.stats_collector.update_metadata({'skipped_tail_lines': skip_tail_lines})
        
        if self.dry_run:
            print("干跑模式: 只分析，不修改文件")
            temp_output = None
        else:
            # 创建临时文件
            temp_result = self.file_processor.create_temp_file(str(self.input_file_path))
            if not temp_result:
                return False
            temp_fd, temp_path = temp_result
            temp_output = os.fdopen(temp_fd, 'w', encoding='utf-8')
        
        try:
            # 处理文件
            for line_num, line in self.file_processor.read_jsonl_file(str(self.input_file_path)):
                self.stats_collector.update_stats({'total_lines': 1})
                
                # 进度显示
                if line_num % 100000 == 0:
                    print(f"处理进度: {line_num} 行")
                    if self.dry_run and self.verbose:
                        self._print_progress_stats()
                
                # 检查是否应该跳过末尾行
                if self.file_processor.should_skip_line(line_num, total_lines, skip_tail_lines):
                    self.stats_collector.update_stats({'skipped_tail': 1})
                    if not self.dry_run:
                        temp_output.write(line + '\n')
                    continue
                
                # 解析JSON行
                data = self.file_processor.parse_json_line(line)
                if data is None:
                    self.stats_collector.update_stats({'errors': 1})
                    if not self.dry_run:
                        temp_output.write(line + '\n')
                    continue
                
                # 检查是否需要处理
                if self.field_cleaner.should_process_line(data):
                    # 处理数据
                    processed_data, line_stats = self.field_cleaner.process_data(data)
                    self.stats_collector.update_stats(line_stats)
                    
                    # 如果有清理操作，记录日志
                    if self.verbose and any(line_stats.values()) and line_num <= 10:
                        self._log_line_processing(line_num, line_stats)
                    
                    output_line = json.dumps(processed_data, ensure_ascii=False)
                else:
                    self.stats_collector.update_stats({'skipped': 1})
                    output_line = line
                
                # 写入输出
                if not self.dry_run:
                    temp_output.write(output_line + '\n')
            
            # 停止计时
            self.stats_collector.stop_timer()
            
            # 更新元数据中的统计信息
            stats = self.stats_collector.get_stats()
            self.stats_collector.update_metadata({
                'total_lines': stats['total_lines'],
                'processed_lines': stats['processed'],
                'skipped_tail_lines_count': stats['skipped_tail'],
                'variables_cleaned_count': stats['variables_cleaned'],
                'variables_initialized_cleaned_count': stats['variables_initialized_cleaned'],
                'is_ejs_processed_cleaned_count': stats['is_ejs_processed_cleaned'],
                'fields_cleaned_count': stats['fields_cleaned']
            })
            
            # 如果不是干跑模式，替换原文件并保存元数据
            if not self.dry_run:
                temp_output.close()  # 确保文件已关闭
                
                # 替换原文件
                if not self.file_processor.replace_file(temp_path, str(self.input_file_path)):
                    return False
                
                # 保存元数据
                self.stats_collector.update_metadata({
                    'status': 'completed',
                    'completion_time': self.stats_collector.metadata['operation_time']
                })
                self.backup_manager.save_metadata(self.stats_collector.get_metadata())
                
                # 保存当前配置到配置文件
                self.config_manager.save_config(self.config)
            
            return True
            
        except Exception as e:
            print(f"处理文件时出错: {e}")
            self.stats_collector.update_metadata({
                'status': 'failed',
                'error': str(e)
            })
            if not self.dry_run:
                self.backup_manager.save_metadata(self.stats_collector.get_metadata())
            return False
    
    def _print_progress_stats(self):
        """打印进度统计信息"""
        stats = self.stats_collector.get_stats()
        print(f"进度统计: 已处理 {stats['total_lines']} 行，"
              f"清理variables: {stats['variables_cleaned']}，"
              f"清理variables_initialized: {stats['variables_initialized_cleaned']}，"
              f"清理is_ejs_processed: {stats['is_ejs_processed_cleaned']}，"
              f"删除字段: {stats['fields_cleaned']}")
    
    def _log_line_processing(self, line_num: int, line_stats: Dict[str, int]):
        """记录行处理日志"""
        cleanup_logs = []
        if line_stats.get('variables_cleaned'):
            cleanup_logs.append("variables清空")
        if line_stats.get('variables_initialized_cleaned'):
            cleanup_logs.append("variables_initialized清空")
        if line_stats.get('is_ejs_processed_cleaned'):
            cleanup_logs.append("is_ejs_processed清空")
        if line_stats.get('fields_cleaned'):
            cleanup_logs.append(f"{line_stats['fields_cleaned']}个指定字段删除")
        
        if cleanup_logs:
            print(f"第{line_num}行: {'、'.join(cleanup_logs)}")
    
    def run(self, no_confirm: bool = False) -> bool:
        """运行瘦身处理"""
        # 检查文件是否存在
        if not os.path.exists(self.input_file_path):
            print(f"错误: 文件不存在 - {self.input_file_path}")
            return False
        
        # 检查文件大小
        file_size = os.path.getsize(self.input_file_path)
        file_size_mb = file_size / (1024 * 1024)
        file_size_gb = file_size / (1024 * 1024 * 1024)
        
        print(f"文件: {self.input_file_path}")
        print(f"大小: {file_size_mb:.2f} MB ({file_size_gb:.2f} GB)")
        
        # 确认操作
        if not self.dry_run and not no_confirm:
            print("\n" + "="*70)
            print("配置概览:")
            print(f"  clearSwipes: {self.config.get('clearSwipes', True)}")
            print(f"  noClearLine: {self.config.get('noClearLine', 10)}")
            print(f"  clearVariables: {self.config.get('clearVariables', False)}")
            print(f"  clearVariables_initialized: {self.config.get('clearVariables_initialized', False)}")
            print(f"  clearIs_ejs_processed: {self.config.get('clearIs_ejs_processed', False)}")
            print(f"  clearField: {self.config.get('clearField', False)}")
            if self.config.get('clearField', False):
                field_array = self.config.get('clearFieldArray', [])
                if isinstance(field_array, list) and field_array:
                    print(f"  clearFieldArray: {field_array}")
            print("="*70)
            
            confirm = input("确认开始处理文件？(y/n): ")
            if confirm.lower() != 'y':
                print("操作已取消")
                return False
        
        # 备份文件
        if not self.dry_run:
            if not self.backup_manager.create_backup():
                return False
            
            # 更新元数据中的备份文件大小
            backup_size = os.path.getsize(self.backup_manager.get_backup_path())
            self.stats_collector.update_metadata({'backup_size': backup_size})
        
        # 处理文件
        print("\n开始处理文件...")
        
        if self.process_file():
            # 打印统计信息
            self.stats_collector.print_stats(self.config, self.field_cleaner.get_fields_to_clear())
            self.stats_collector.print_config_info(self.config)
            
            # 计算瘦身效果
            if not self.dry_run:
                savings = self.stats_collector.calculate_savings(
                    self.backup_manager.get_backup_path(),
                    str(self.input_file_path)
                )
                if savings:
                    print(f"\n瘦身效果:")
                    print(f"  原始文件大小: {savings['original_size_mb']:.2f} MB")
                    print(f"  处理后大小: {savings['new_size_mb']:.2f} MB")
                    print(f"  减少大小: {savings['reduction_mb']:.2f} MB")
                    print(f"  减少比例: {savings['reduction_percent']:.2f}%")
                    print("="*90)
            
            print(f"\n处理完成!")
            if not self.dry_run:
                print(f"原始文件已更新: {self.input_file_path}")
                print(f"备份文件: {self.backup_manager.get_backup_path()}")
                print(f"元数据文件: {self.backup_manager.get_meta_path()}")
                print(f"配置文件: {self.config_manager.get_config_file_path()}")
            else:
                print("干跑模式: 文件未被修改")
            return True
        else:
            print("处理失败")
            return False


def analyze_file(input_file_path: str) -> bool:
    """分析文件内容，提供优化建议"""
    analyzer = FileAnalyzer()
    
    try:
        with open(input_file_path, 'r', encoding='utf-8') as f:
            for line_num, line in enumerate(f, 1):
                # 每处理100000行输出一次进度
                if line_num % 100000 == 0:
                    print(f"分析进度: {line_num} 行")
                
                analyzer.analyze_line(line, line_num)
        
        analyzer.print_report()
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
    
    # 如果指定了编辑配置或显示配置
    if args.edit_config or args.show_config:
        input_path = Path(args.input_file).resolve()
        parent_folder_name = input_path.parent.name or "root"
        project_dir = Path("workspace") / parent_folder_name
        
        config_manager = ConfigManager(project_dir)
        
        if args.edit_config:
            config_editor = ConfigEditor(config_manager)
            success = config_editor.edit_interactive()
        else:
            config_editor = ConfigEditor(config_manager)
            success = config_editor.show_config()
        
        sys.exit(0 if success else 1)
    
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
    
    # 创建瘦身处理器并运行
    slimmer = JSONLSlimmer(
        args.input_file, 
        dry_run=args.dry_run, 
        verbose=args.verbose,
        config=config if config else None
    )
    
    success = slimmer.run(no_confirm=args.no_confirm)
    
    if success:
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()