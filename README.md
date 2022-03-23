# 关于此项目  
一个通过命令行写日记的程序，参考了jrnl项目。jrnl项目能够很好的实现命令行写日记的功能，但是有几个重要缺陷：  
1、需要python3。  
2、日记相关的辅助信息（如tag，时间）没有被规则的放置。  
3、日记相关的辅助信息缺少灵活性。

# 写日记
输入jrme，将会调用系统编辑器开始写日记，默认为调用vi。编辑器中将准备好基本的文本。你将在编辑器中看到如下的内容：  
```
==========journal==========
这里输入标题
==========config==========
date=2022-02-26 19:23:32
tag=
level=
==========coload plntent==========
这里输入正文
```
保存并退出后将会保存日记。
## 写日记的其他方式

jrme [-A 时间] [-T 标题] [-C正文]  
例如：
```
jrme -A today -T hello world.what a nice day.
jrme -A today
jrme -T hello world. -C what a nice day.
jrme -T hello world.
```

# 配置文件
配置文件位于:~/.jrme/config.ini  
目前支持配置：
```
[base]
editor = 调用的系统编辑器

[journal books]
default = 日记保存的位置

[plugin]
config node = 插件，多个插件用,分割
journal format = 日记格式插件，多个插件用,分割
```

# 插件
jrme项目为了向日记添加更多更灵活的附加功能，提供了关于日记中config章节的插件。每一项目的config都包含默认的key和value。在写日记前和完成日记书写的时候，插件都能读取到全部的日记文本，并且可以进行一定的修改。  
## Config Node插件
1、每个Config Node都有一个自己的key和默认值，在写日记前会自动准备好并展示给用户。  
2、每一个config node会在用户写日记前和用户保存日记的时候获得全部的日记文本，并且可以对其进行修改。
  
当前工程自带一个key为"save date"的插件，可以记录保存日记的时间。运行：*make plugin*可以获得该插件，保存位置为：*~/.jrme/plugin/*。  
注意：需要插件顺利加载，需要在配置文件中的config node加入该插件的文件名。如：
```
[plugin]
config node = save_date.so
```
## 日记格式插件
用户可以通过插件实现不过格式的日记存储样式，但是需要注意，无论如何，在用户输入日记的界面是不会改变的，只有保存日记的文本会发生变化。  
日记格式插件可以有自己支持的格式后缀列表，且后缀长度不超过7。系统会优先选择与第一个支持文件后缀名的日记格式插件。如果失败，依次调用插件尝试打开日记。  
  
当前工程自带一个简单的markdown格式插件，运行：*make plugin*可以获得该插件，保存位置为：*~/.jrme/plugin/*。  
注意：需要插件顺利加载，需要在配置文件中的journal format加入该插件的文件名。如：
```
[plugin]
journal format = md_journal.so
```

# 编译
需求：gcc,g++,gtest(如果需要运行test)  
输出位于./bin/jrme.elf  

编译：make  
编译插件：make plugin  
编译测试：make test (注意，编译测试需要先安装gtest)  
清除：make clean

编译（带插件）：make PLUGIN=y  
清除（带插件）：make clean PLUGIN=y

