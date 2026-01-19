# field_cleaner.py
import json
from typing import Dict, Any, Tuple, Set, List


class FieldCleaner:
    """字段清理器"""
    
    def __init__(self, config: Dict[str, Any]):
        self.config = config
        
        # 解析要删除的字段列表
        self.fields_to_clear = set()
        if self.config.get('clearField', False):
            field_array = self.config.get('clearFieldArray', [])
            if isinstance(field_array, list):
                self.fields_to_clear = set(field_array)
    
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
            # 只更新swipe_id为0
            data['swipe_id'] = 0
            return data
        
        # 检查swipe_id是否有效
        if not isinstance(swipe_id, int) or not (0 <= swipe_id < len(swipes)):
            # 返回原始数据，不做修改
            return data
        
        # 执行修改
        # 1. 将mes设置为swipes[swipe_id]
        data['mes'] = swipes[swipe_id]
        
        # 2. 根据配置决定是否清空swipes
        if self.config.get('clearSwipes', True):
            # 清空swipes
            data['swipes'] = []
        else:
            # 保留swipes数组，只保留对应的index
            if 0 <= swipe_id < len(swipes):
                data['swipes'] = [swipes[swipe_id]]
            else:
                data['swipes'] = []
        
        # 3. 将swipe_id设置为0
        data['swipe_id'] = 0
        
        return data
    
    def process_data(self, data: Dict[str, Any]) -> Tuple[Dict[str, Any], Dict[str, int]]:
        """处理单个JSON对象，返回清理后的数据和统计信息"""
        stats = {
            'variables_cleaned': 0,
            'variables_initialized_cleaned': 0,
            'is_ejs_processed_cleaned': 0,
            'fields_cleaned': 0,
            'swipes_processed': 0,
            'already_empty': 0
        }
        
        # 首先清理variables相关字段
        cleaned_data, cleanup_stats = self.clean_variables_fields(data)
        stats.update(cleanup_stats)
        
        # 然后删除指定字段
        cleaned_data, fields_removed = self.clear_specified_fields(cleaned_data)
        stats['fields_cleaned'] = fields_removed
        
        # 最后处理swipes相关字段
        if 'swipes' in cleaned_data and 'swipe_id' in cleaned_data:
            if len(cleaned_data['swipes']) == 0:
                stats['already_empty'] = 1
            cleaned_data = self.process_swipes_fields(cleaned_data)
            stats['swipes_processed'] = 1
        
        return cleaned_data, stats
    
    def get_fields_to_clear(self) -> Set[str]:
        """获取要删除的字段列表"""
        return self.fields_to_clear.copy()