#!/bin/bash

# 定义服务文件路径
SERVICE_FILE="/etc/systemd/system/combined_program.service"

# 检查是否有权限创建服务文件
if [ "$(id -u)" != "0" ]; then
  echo "请使用 sudo 权限运行此脚本。"
  exit 1
fi

# 创建 systemd 服务文件
echo "创建 systemd 服务文件..."

cat <<EOL > $SERVICE_FILE
[Unit]
Description=Combined Program Server
After=network.target

[Service]
Type=simple
ExecStart=/home/firefly/Dev/combined_program server --daemon --send-test
Restart=on-failure
# 如果需要以 root 用户运行，可以取消下一行的注释
User=root
# 如果需要使用 sudo 执行特定命令
# ExecStart=/usr/bin/sudo /home/firefly/Dev/combined_program server --daemon
WorkingDirectory=/home/firefly/Dev

[Install]
WantedBy=multi-user.target
EOL

echo "服务文件创建完成：$SERVICE_FILE"

# 重新加载 systemd
echo "重新加载 systemd 守护进程..."
systemctl daemon-reload

# 启用服务在开机时启动
echo "启用 combined_program.service 在系统启动时自动运行..."
systemctl enable combined_program.service

# 启动服务
echo "立即启动 combined_program.service..."
systemctl start combined_program.service

# 检查服务状态
echo "服务状态："
systemctl status combined_program.service --no-pager

echo "脚本执行完成。服务已自动化设置。"
