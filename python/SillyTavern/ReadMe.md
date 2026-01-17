# README_SLIM_JSONL.md
# JSONL文件瘦身工具

## 概述

`slim_jsonl.py` 是一个专门用于处理JSONL文件瘦身的工具，通过清理冗余字段来减小文件体积，提高处理效率。

## 主要功能

### 1. 字段删除功能
新增了两个配置项来控制字段删除：
- `clearField`: 布尔值，是否启用字段删除功能（默认：false）
- `clearFieldArray`: 字符串数组，要删除的字段名列表（默认：["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"]）

### 2. 处理逻辑
当 `clearField` 为 `true` 且 `clearFieldArray` 不为空时，工具会删除每个JSON对象中指定的字段。

## 配置文件结构

```json
{
  "clearSwipes": true,
  "noClearLine": 10,
  "clearVariables": false,
  "clearVariables_initialized": false,
  "clearIs_ejs_processed": false,
  "clearField": false,
  "clearFieldArray": ["TavernDB_ACU_IsolatedData", "TavernDB_ACU_IndependentData"]
}
```


# 使用默认配置处理文件
python slim_jsonl.py "path/to/file.jsonl"

# 干跑模式（只分析不修改）
python slim_jsonl.py "path/to/file.jsonl" --dry-run

# 详细输出模式
python slim_jsonl.py "path/to/file.jsonl" --verbose

# 跳过确认提示
python slim_jsonl.py "path/to/file.jsonl" --no-confirm

# 分析文件内容，提供优化建议
python slim_jsonl.py "path/to/file.jsonl" --analyze

# 显示配置文件
python slim_jsonl.py "path/to/file.jsonl" --show-config

# 编辑配置文件
python slim_jsonl.py "path/to/file.jsonl" --edit-config

# 使用命令行参数覆盖配置
python slim_jsonl.py "path/to/file.jsonl" \
  --clear-swipes true \
  --no-clear-line 20 \
  --clear-variables true \
  --clear-variables-initialized true \
  --clear-is-ejs-processed true


# 显示配置
python config_manager.py "path/to/file.jsonl"

# 设置配置
python config_manager.py "path/to/file.jsonl" --action set \
  --clear-swipes false \
  --no-clear-line 50

# 重置配置
python config_manager.py "path/to/file.jsonl" --action reset

# 创建配置文件
python config_manager.py "path/to/file.jsonl" --action create


# 完全恢复备份
python restore_backup.py --operation-file "path/to/file.jsonl" --type full

# 部分恢复备份（覆盖前1000行）
python restore_backup.py --operation-file "path/to/file.jsonl" --type partial --lines 1000

# 文件结构
workspace/
├── <父文件夹名1>/
│   ├── config.json                    # 配置文件
│   ├── <原始文件名>_<时间戳>.bak         # 备份文件
│   └── <原始文件名>.meta.json           # 元数据文件
├── <父文件夹名2>/
│   ├── config.json
│   ├── ...
│   └── ...
└── ...