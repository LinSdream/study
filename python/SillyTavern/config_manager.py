# config_manager.py
import json
import os
from pathlib import Path
from typing import Dict, Any, Optional


class ConfigManager:
    """配置文件管理器"""
    
    def __init__(self, config_dir: str):
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
            return True
        except Exception as e:
            print(f"保存配置文件失败: {e}")
            return False
    
    def get_config_file_path(self) -> str:
        """获取配置文件路径"""
        return str(self.config_file)


class ConfigEditor:
    """配置编辑器"""
    
    def __init__(self, config_manager: ConfigManager):
        self.config_manager = config_manager
        self.config_descriptions = {
            "clearSwipes": "是否清空swipes字段（true/false）",
            "noClearLine": "跳过的末尾行数（整数）",
            "clearVariables": "是否清理variables字段（true/false）",
            "clearVariables_initialized": "是否清理variables_initialized字段（true/false）",
            "clearIs_ejs_processed": "是否清理is_ejs_processed字段（true/false）",
            "clearField": "是否删除指定字段（true/false）",
            "clearFieldArray": "要删除的字段列表（JSON数组，例如：[\"field1\",\"field2\"]）"
        }
    
    def edit_interactive(self) -> bool:
        """交互式编辑配置"""
        config = self.config_manager.load_config()
        print(f"当前配置: {config}")
        
        print("\n编辑配置:")
        for i, (key, desc) in enumerate(self.config_descriptions.items(), 1):
            print(f"{i}. {key} - {desc}")
        
        print(f"{len(self.config_descriptions)+1}. 保存并退出")
        print(f"{len(self.config_descriptions)+2}. 退出不保存")
        
        while True:
            try:
                choice = input(f"\n选择操作 (1-{len(self.config_descriptions)+2}): ").strip()
                
                if choice.isdigit():
                    choice_num = int(choice)
                    
                    if 1 <= choice_num <= len(self.config_descriptions):
                        key = list(self.config_descriptions.keys())[choice_num - 1]
                        self._edit_field(config, key)
                    
                    elif choice_num == len(self.config_descriptions) + 1:
                        return self.config_manager.save_config(config)
                    
                    elif choice_num == len(self.config_descriptions) + 2:
                        print("退出配置编辑")
                        return False
                    
                    else:
                        print(f"无效选择，请输入 1-{len(self.config_descriptions)+2}")
                else:
                    print("请输入数字")
            
            except Exception as e:
                print(f"输入错误: {e}")
        
        return False
    
    def _edit_field(self, config: Dict[str, Any], key: str) -> None:
        """编辑单个字段"""
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
    
    def show_config(self) -> bool:
        """显示配置文件内容"""
        config = self.config_manager.load_config()
        
        print(f"配置文件: {self.config_manager.get_config_file_path()}")
        print(f"配置内容:")
        for key, default_value in self.config_manager.default_config.items():
            value = config.get(key, default_value)
            print(f"  {key}: {value}")
        
        return True