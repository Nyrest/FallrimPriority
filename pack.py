import os
import shutil
import threading

# 定义编译指令
compile_cmd = "cargo build -r --features \"{flag}\" --target-dir \"./tmp/{game_name}\""
# 定义目标数组
targets = [
    ["Skyrim AE", "x86_64-pc-windows-msvc", "skyrim_ae"],
    ["Fallout 4", "x86_64-pc-windows-msvc", "fallout_4"]
]

# 创建 "./tmp" 和 "./out" 文件夹
if not os.path.exists("./tmp"):
    os.makedirs("./tmp")
if not os.path.exists("./out"):
    os.makedirs("./out")

# 定义函数来运行编译指令
def compile_target(target):
    game_name, arch, flag = target
    cmd = compile_cmd.format(game_name=game_name, flag=flag)
    os.system(cmd)

# 多线程并行遍历数组来编译目标
threads = []
for target in targets:
    thread = threading.Thread(target=compile_target, args=(target,))
    threads.append(thread)
    thread.start()

for thread in threads:
    thread.join()

# 定义函数来处理目标文件
def process_target(target):
    game_name = target[0]
    out_dir = f"./out/{game_name}"
    tmp_dir = f"./tmp/{game_name}"

    # 创建输出目录
    os.makedirs(out_dir, exist_ok=True)

    # 复制文件
    shutil.copy(f"{tmp_dir}/release/FallrimPriority.dll", f"{out_dir}/FallrimPriority.dll")
    shutil.copy("./FallrimPriority.ini", f"{out_dir}/FallrimPriority.ini")

    # 压缩目录
    shutil.make_archive(out_dir, "zip", root_dir=out_dir)

    # 删除目录
    shutil.rmtree(out_dir)

# 多线程并行遍历数组来处理目标文件
threads = []
for target in targets:
    thread = threading.Thread(target=process_target, args=(target,))
    threads.append(thread)
    thread.start()

for thread in threads:
    thread.join()

print("Finished")