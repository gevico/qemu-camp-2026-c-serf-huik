#!/usr/bin/bash

# 获取脚本所在的目录（这里是 tests/）
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# 计算相对路径：从 tests/ 到 exercises/20_mybash/
EXERCISE_DIR="$(cd "$SCRIPT_DIR/../exercises/20_mybash" && pwd)"
MYBASH_CMD="$(cd "$SCRIPT_DIR" && pwd)/mybash_cmd.sh"

# 切换到 exercise 目录执行，这样 mybash_cmd.sh 中的相对路径才能正确
cd "$EXERCISE_DIR"

# 使用绝对路径调用 mybash 和命令脚本
"$EXERCISE_DIR/bin/mybash" "$MYBASH_CMD"