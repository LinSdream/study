# file_analyzer.py
import json
from typing import Dict, Any, List, Set
from collections import defaultdict


class FileAnalyzer:
    """文件分析器"""
    
    def __init__(self):
        self.stats = {
            'total_lines': 0,
            'has_swipes': 0,
            'has_variables': 0,
            'is_user_false': 0,
            'has_extra': 0,
            'meets_variables_condition': 0,
            'variables_size_sum': 0,
            'variables_initialized_size_sum': 0,
            'is_ejs_processed_size_sum': 0,
            'field_stats': defaultdict(lambda: {'count': 0, 'total_size': 0}),
            'sample_fields': set()
        }
        
        # 要检查的常见字段
        self.common_fields_to_check = [
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
    
    def analyze_line(self, line: str, line_num: int) -> None:
        """分析单行JSON数据"""
        self.stats['total_lines'] += 1
        
        stripped_line = line.strip()
        if not stripped_line:
            return
        
        try:
            data = json.loads(stripped_line)
            
            # 检查swipes字段
            if 'swipes' in data and 'swipe_id' in data:
                self.stats['has_swipes'] += 1
            
            # 检查variables字段
            if 'variables' in data:
                self.stats['has_variables'] += 1
                # 估算variables字段大小
                variables_str = json.dumps(data['variables'])
                self.stats['variables_size_sum'] += len(variables_str.encode('utf-8'))
            
            # 检查其他字段
            if data.get('is_user') == False:
                self.stats['is_user_false'] += 1
            
            if 'extra' in data:
                self.stats['has_extra'] += 1
            
            # 检查是否满足清理variables的条件
            if (data.get('is_user') == False and 
                'extra' in data and 
                'variables' in data):
                self.stats['meets_variables_condition'] += 1
            
            # 检查variables_initialized字段
            if 'variables_initialized' in data:
                var_init_str = json.dumps(data['variables_initialized'])
                self.stats['variables_initialized_size_sum'] += len(var_init_str.encode('utf-8'))
            
            # 检查is_ejs_processed字段
            if 'is_ejs_processed' in data:
                is_ejs_str = json.dumps(data['is_ejs_processed'])
                self.stats['is_ejs_processed_size_sum'] += len(is_ejs_str.encode('utf-8'))
            
            # 统计字段使用情况
            for field in self.common_fields_to_check:
                if field in data:
                    self.stats['field_stats'][field]['count'] += 1
                    
                    # 估算字段大小
                    field_str = json.dumps(data[field])
                    self.stats['field_stats'][field]['total_size'] += len(field_str.encode('utf-8'))
            
            # 采样其他字段（前50行）
            if line_num <= 50:
                for field in data.keys():
                    if field not in self.common_fields_to_check:
                        self.stats['sample_fields'].add(field)
        
        except json.JSONDecodeError:
            # 跳过无效的JSON行
            pass
    
    def get_recommendations(self) -> List[str]:
        """获取优化建议"""
        recommendations = []
        
        if self.stats['has_swipes'] > 100:
            recommendations.append("✓ 建议启用 clearSwipes: true")
        
        if self.stats['meets_variables_condition'] > 100:
            recommendations.append(f"✓ 建议启用 clearVariables: true (可清理 {self.stats['meets_variables_condition']} 行)")
        
        if self.stats['variables_initialized_size_sum'] > 1024 * 1024:  # 超过1MB
            recommendations.append(f"✓ 建议启用 clearVariables_initialized: true (可节省约 {self.stats['variables_initialized_size_sum']/1024/1024:.2f} MB)")
        
        if self.stats['is_ejs_processed_size_sum'] > 1024 * 1024:  # 超过1MB
            recommendations.append(f"✓ 建议启用 clearIs_ejs_processed: true (可节省约 {self.stats['is_ejs_processed_size_sum']/1024/1024:.2f} MB)")
        
        # 检查常见字段是否出现频繁
        for field in ["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"]:
            if field in self.stats['field_stats']:
                field_count = self.stats['field_stats'][field]['count']
                field_size = self.stats['field_stats'][field]['total_size']
                if field_count > 100:
                    recommendations.append(f"✓ 建议在clearFieldArray中添加 '{field}' (出现 {field_count} 次，占用约 {field_size/1024/1024:.2f} MB)")
        
        if self.stats['total_lines'] > 10000:
            recommendations.append(f"✓ 建议设置 noClearLine: 100 (跳过最后100行，保留原始数据)")
        
        return recommendations
    
    def print_report(self) -> None:
        """打印分析报告"""
        print("\n" + "="*90)
        print("文件分析结果:")
        print(f"总行数: {self.stats['total_lines']}")
        
        print(f"\nswipes字段统计:")
        print(f"  包含swipes和swipe_id的行数: {self.stats['has_swipes']} ({self.stats['has_swipes']/self.stats['total_lines']*100:.1f}%)")
        
        print(f"\nvariables字段统计:")
        print(f"  包含variables的行数: {self.stats['has_variables']} ({self.stats['has_variables']/self.stats['total_lines']*100:.1f}%)")
        if self.stats['has_variables'] > 0:
            avg_variables_size = self.stats['variables_size_sum'] / self.stats['has_variables']
            print(f"  平均variables字段大小: {avg_variables_size:.0f} 字节")
        
        print(f"\n清理条件统计:")
        print(f"  is_user=false的行数: {self.stats['is_user_false']}")
        print(f"  包含extra字段的行数: {self.stats['has_extra']}")
        print(f"  满足variables清理条件的行数: {self.stats['meets_variables_condition']}")
        
        print(f"\n其他字段大小统计:")
        if self.stats['variables_initialized_size_sum'] > 0:
            avg_var_init_size = self.stats['variables_initialized_size_sum'] / self.stats['has_variables']
            print(f"  平均variables_initialized字段大小: {avg_var_init_size:.0f} 字节")
        
        if self.stats['is_ejs_processed_size_sum'] > 0:
            avg_is_ejs_size = self.stats['is_ejs_processed_size_sum'] / self.stats['has_variables']
            print(f"  平均is_ejs_processed字段大小: {avg_is_ejs_size:.0f} 字节")
        
        # 字段使用统计
        print(f"\n字段使用统计（出现次数最多的字段）:")
        sorted_fields = sorted(
            self.stats['field_stats'].items(),
            key=lambda x: x[1]['total_size'],
            reverse=True
        )[:10]  # 只显示前10个
        
        for field, field_stats in sorted_fields:
            percentage = (field_stats['count'] / self.stats['total_lines']) * 100
            avg_size = field_stats['total_size'] / field_stats['count'] if field_stats['count'] > 0 else 0
            print(f"  {field}: {field_stats['count']} 行 ({percentage:.1f}%), "
                  f"平均大小: {avg_size:.0f} 字节")
        
        # 采样字段
        if self.stats['sample_fields']:
            print(f"\n采样发现的字段（前50行）:")
            sample_list = list(self.stats['sample_fields'])
            if len(sample_list) > 20:
                print(f"  发现 {len(sample_list)} 个其他字段，前20个: {sample_list[:20]}")
            else:
                print(f"  发现 {len(sample_list)} 个其他字段: {sample_list}")
        
        print("\n优化建议:")
        for recommendation in self.get_recommendations():
            print(f"  {recommendation}")
        
        print("="*90)