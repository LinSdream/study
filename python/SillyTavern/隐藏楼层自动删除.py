import re
import os
import shutil

def clear_screen():
    """清屏函数"""
    os.system('cls' if os.name == 'nt' else 'clear')

def show_menu():
    """显示菜单"""
    print("=" * 50)
    print("   隐藏楼层清除器 - 删除隐藏楼层")
    print("*聊天文件位于SillyTavern\data\default-user\chats目录下")
    print("=" * 50)
    print("1. 选择文件并处理")
    print("2. 撤销上次操作（恢复备份）")
    print("3. 清屏")
    print("4. 退出程序")
    print("=" * 50)

def remove_matching_lines_optimized(file_path):
    """
    高效删除匹配正则表达式 .*is_system":true.* 的行
    """
    pattern = re.compile(r'.*is_system":true.*', re.IGNORECASE)
    temp_file = file_path + '.tmp'
    
    try:
        removed_count = 0
        total_lines = 0
        
        # 先统计总行数和匹配行数
        with open(file_path, 'r', encoding='utf-8') as infile:
            for line in infile:
                total_lines += 1
                if pattern.match(line):
                    removed_count += 1
        
        if removed_count == 0:
            print("没有找到隐藏楼层")
            return False
        
        print(f"文件总行数: {total_lines}")
        print(f"匹配到 {removed_count} 层需要删除")
        confirm = input("确认删除？(y/n): ")
        if confirm.lower() != 'y':
            print("操作已取消")
            return False
        
        # 创建备份
        backup_path = file_path + '.backup'
        shutil.copy2(file_path, backup_path)
        print(f"已创建备份文件: {backup_path}")
        
        # 处理文件
        with open(file_path, 'r', encoding='utf-8') as infile, \
             open(temp_file, 'w', encoding='utf-8') as outfile:
            
            processed_count = 0
            for line in infile:
                if not pattern.match(line):
                    outfile.write(line)
                processed_count += 1
                # 显示进度
                if processed_count % 1000 == 0:
                    print(f"处理进度: {processed_count}/{total_lines} 层", end='\r')
        
        # 替换原文件
        os.replace(temp_file, file_path)
        print(f"\n成功删除了 {removed_count} 层匹配内容")
        return True
        
    except Exception as e:
        # 清理临时文件
        try:
            if os.path.exists(temp_file):
                os.remove(temp_file)
        except:
            pass
        print(f"处理文件时出错: {e}")
        return False

def undo_last_operation(file_path):
    """撤销上次操作，恢复备份"""
    backup_path = file_path + '.backup'
    
    if not os.path.exists(backup_path):
        print("找不到备份文件，无法撤销")
        return False
    
    try:
        # 恢复备份
        shutil.copy2(backup_path, file_path)
        # 删除备份文件
        os.remove(backup_path)
        print("成功撤销上次操作，文件已恢复")
        return True
    except Exception as e:
        print(f"撤销操作失败: {e}")
        return False

def get_file_path():
    """获取文件路径，支持拖拽和手动输入"""

    print("\n请选择文件（可以直接拖拽文件到窗口，或输入文件路径）")

    file_path = input("文件路径: ").strip()
    
    # 处理拖拽可能带来的引号
    if file_path.startswith('"') and file_path.endswith('"'):
        file_path = file_path[1:-1]
    elif file_path.startswith("'") and file_path.endswith("'"):
        file_path = file_path[1:-1]
    
    # 检查文件是否存在
    if not os.path.exists(file_path):
        print("文件不存在，请检查路径")
        return None
    
    # 检查文件大小
    file_size = os.path.getsize(file_path)
    print(f"文件大小: {file_size / 1024 / 1024:.2f} MB")
    
    return file_path

def main():
    current_file = None
    
    while True:
        clear_screen()
        show_menu()
        
        if current_file:
            print(f"当前文件: {current_file}")
        
        choice = input("\n请选择操作 (1-4): ").strip()
        
        if choice == '1':
            # 选择文件并处理
            file_path = get_file_path()
            if file_path:
                current_file = file_path
                remove_matching_lines_optimized(file_path)
            
        elif choice == '2':
            # 撤销操作
            if current_file:
                undo_last_operation(current_file)
            else:
                print("请先选择文件")
            
        elif choice == '3':
            # 清屏，继续循环
            continue
            
        elif choice == '4':
            # 退出程序
            print("感谢使用，再见！")
            break
            
        else:
            print("无效选择，请重新输入")
        
        input("\n按回车键继续...")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n程序被用户中断")
    except Exception as e:
        print(f"程序运行出错: {e}")
        input("按回车键退出...")
