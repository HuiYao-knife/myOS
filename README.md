# myOS
依据《操作系统真象还原》实现的一个小操作系统，实现了 ls、cd 等有限的命令，命令 help 显示可用的命令
# 构建方式
mbr 文件夹是OS的源码
cd mbr
make build
再按照 dd.sh 中的命令移动磁盘即可
sh dd.sh
随后进入 bochs 文件夹，里面存放着磁盘文件
cd ../bochs
也是运行  boch.sh 文件即可
sh boch.sh

