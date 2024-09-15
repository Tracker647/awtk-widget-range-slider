# awtk-widget-range_slider

range_slider 控件是一个有两个滑块的滑条控件，可用于区间的定义。
该控件可用键盘控制滑块切换，enter选中后控制滑块滑动，esc退出控制回到滑块切换。

![](docs/images/ui.jpg)

## 准备

1. 获取 awtk 并编译

```
git clone https://github.com/zlgopen/awtk.git
cd awtk; scons; cd -
```

将自定义控件design/default/images/xx/下所有图片复制到项目的同名路径
将自定义控件design/default/styles/main.xml下规定的所有样式拷贝到项目的design/default/styles/default.xml
（为什么awtk_designer不能自动把这些搬过来？）
## 运行

1. 生成示例代码的资源

```
python scripts/update_res.py all
```
> 也可以使用 Designer 打开项目，之后点击 “打包” 按钮进行生成；
> 如果资源发生修改，则需要重新生成资源。

如果 PIL 没有安装，执行上述脚本可能会出现如下错误：
```cmd
Traceback (most recent call last):
...
ModuleNotFoundError: No module named 'PIL'
```
请用 pip 安装：
```cmd
pip install Pillow
```

2. 编译

* 编译PC版本

```
scons
```

* 编译LINUX FB版本

```
scons LINUX_FB=true
```

> 完整编译选项请参考[编译选项](https://github.com/zlgopen/awtk-widget-generator/blob/master/docs/build_options.md)

3. 运行

![GIF 2024-9-15 15-30-28](https://github.com/user-attachments/assets/214900e9-fe1d-46d4-a3a4-f49d7b0e0197)
./bin/demo
```

## 文档

[完善自定义控件](https://github.com/zlgopen/awtk-widget-generator/blob/master/docs/improve_generated_widget.md)
