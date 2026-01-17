# config_manager.py
import json
import os
import sys
from pathlib import Path
from typing import Dict, Any

def manage_config(input_file_path: str, action: str = "show", **kwargs) -> bool:
    """
    管理配置文件
    
    Args:
        input_file_path: 输入文件路径
        action: 操作类型，'show'、'set'、'reset' 或 'create'
        **kwargs: 配置参数
    """
    input_path = Path(input_file_path).resolve()
    parent_folder_name = input_path.parent.name or "root"
    project_dir = Path("workspace") / parent_folder_name
    config_file = project_dir / "config.json"
    
    default_config = {
        "clearSwipes": True,
        "noClearLine": 10,
        "clearVariables": False,
        "clearVariables_initialized": False,
        "clearIs_ejs_processed": False,
        "clearField": False,
        "clearFieldArray": ["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"]
    }
    
    if action == "show":
        if not config_file.exists():
            print(f"配置文件不存在: {config_file}")
            print("将使用默认配置:")
            for key, value in default_config.items():
                print(f"  {key}: {value}")
            return False
        
        try:
            with open(config_file, 'r', encoding='utf-8') as f:
                config = json.load(f)
            
            print(f"配置文件: {config_file}")
            print(f"配置内容:")
            for key, default_value in default_config.items():
                value = config.get(key, default_value)
                print(f"  {key}: {value}")
            return True
        except Exception as e:
            print(f"读取配置文件失败: {e}")
            return False
    
    elif action == "set":
        # 加载现有配置或使用默认配置
        if config_file.exists():
            try:
                with open(config_file, 'r', encoding='utf-8') as f:
                    config = json.load(f)
            except:
                config = default_config.copy()
        else:
            config = default_config.copy()
        
        # 更新配置
        updated = False
        
        config_params = {
            "clear_swipes": "clearSwipes",
            "no_clear_line": "noClearLine",
            "clear_variables": "clearVariables",
            "clear_variables_initialized": "clearVariables_initialized",
            "clear_is_ejs_processed": "clearIs_ejs_processed",
            "clear_field": "clearField",
            "clear_field_array": "clearFieldArray"
        }
        
        for param_key, config_key in config_params.items():
            if param_key in kwargs and kwargs[param_key] is not None:
                if param_key == "clear_field_array" and isinstance(kwargs[param_key], str):
                    try:
                        config[config_key] = json.loads(kwargs[param_key])
                    except json.JSONDecodeError:
                        print(f"错误: {param_key} 参数必须是有效的JSON数组字符串")
                        return False
                else:
                    config[config_key] = kwargs[param_key]
                updated = True
                print(f"设置 {config_key}: {kwargs[param_key]}")
        
        if not updated:
            print("没有提供要更新的配置项")
            return False
        
        # 保存配置
        try:
            project_dir.mkdir(parents=True, exist_ok=True)
            with open(config_file, 'w', encoding='utf-8') as f:
                json.dump(config, f, indent=2, ensure_ascii=False)
            print(f"配置已保存: {config_file}")
            return True
        except Exception as e:
            print(f"保存配置失败: {e}")
            return False
    
    elif action == "reset":
        try:
            project_dir.mkdir(parents=True, exist_ok=True)
            with open(config_file, 'w', encoding='utf-8') as f:
                json.dump(default_config, f, indent=2, ensure_ascii=False)
            print(f"配置已重置为默认值: {config_file}")
            print(f"配置内容:")
            for key, value in default_config.items():
                print(f"  {key}: {value}")
            return True
        except Exception as e:
            print(f"重置配置失败: {e}")
            return False
    
    elif action == "create":
        try:
            project_dir.mkdir(parents=True, exist_ok=True)
            with open(config_file, 'w', encoding='utf-8') as f:
                json.dump(default_config, f, indent=2, ensure_ascii=False)
            print(f"配置文件已创建: {config_file}")
            print(f"默认配置内容:")
            for key, value in default_config.items():
                print(f"  {key}: {value}")
            return True
        except Exception as e:
            print(f"创建配置文件失败: {e}")
            return False
    
    else:
        print(f"不支持的操作: {action}")
        return False


def main():
    """主函数"""
    import argparse
    
    parser = argparse.ArgumentParser(description='JSONL瘦身工具配置文件管理器')
    parser.add_argument('input_file', help='输入文件路径')
    parser.add_argument('--action', choices=['show', 'set', 'reset', 'create'], default='show',
                       help='操作类型: show(显示), set(设置), reset(重置), create(创建)')
    
    # 配置参数
    parser.add_argument('--clear-swipes', type=lambda x: x.lower() == 'true',
                       help='设置clearSwipes值 (true/false)')
    parser.add_argument('--no-clear-line', type=int,
                       help='设置noClearLine值')
    parser.add_argument('--clear-variables', type=lambda x: x.lower() == 'true',
                       help='设置clearVariables值 (true/false)')
    parser.add_argument('--clear-variables-initialized', type=lambda x: x.lower() == 'true',
                       help='设置clearVariables_initialized值 (true/false)')
    parser.add_argument('--clear-is-ejs-processed', type=lambda x: x.lower() == 'true',
                       help='设置clearIs_ejs_processed值 (true/false)')
    parser.add_argument('--clear-field', type=lambda x: x.lower() == 'true',
                       help='设置clearField值 (true/false)')
    parser.add_argument('--clear-field-array', type=str,
                       help='设置clearFieldArray值 (JSON数组字符串，例如：\'["field1","field2"]\')')
    
    args = parser.parse_args()
    
    success = manage_config(
        args.input_file,
        action=args.action,
        clear_swipes=args.clear_swipes,
        no_clear_line=args.no_clear_line,
        clear_variables=args.clear_variables,
        clear_variables_initialized=args.clear_variables_initialized,
        clear_is_ejs_processed=args.clear_is_ejs_processed,
        clear_field=args.clear_field,
        clear_field_array=args.clear_field_array
    )
    
    if success:
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()