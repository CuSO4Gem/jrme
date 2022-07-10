# Language/语言  
[Chinese](https://github.com/CuSO4Gem/jrme/blob/master/doc/README-ZH.md)  
[English](https://github.com/CuSO4Gem/jrme/blob/master/doc/README-EN.md)

# About
With jrme, you can write, search, modify, and delete diary/notes only through the command line. Not only that, but you can also add plugins to save journals in different formats, and process them automatically.  
This project refers to the jrnl project.The jrnl project can well implement the function of writing a journal from the command line, but jrnl has several shortcomings:  
1. Requires python3.
2. Additional information of journal (such as tag, time) is not placed regularly.
3. Additional information of lacks flexibility.  
   
[More](https://github.com/CuSO4Gem/jrme/blob/master/doc/README-EN.md)

# 关于此项目  
通过jrme，你仅需要通过命令行就可以实现日记/笔记的书写，查找，修改，删除功能。不仅如此，你还可以添加插件实现不同格式的日记保存，和自动处理。  
本项目参考了jrnl项目。jrnl项目能够很好的实现命令行写日记的功能，但是jrnl有几点不足：  
1、 需要python3。  
2、 日记相关的附加信息（如tag，时间）零散。  
3、 日记相关的附加信息缺少灵活性。  
  
[更多](https://github.com/CuSO4Gem/jrme/blob/master/doc/README-ZH.md)  

# 写日记
输入jrme，将会调用系统编辑器开始写日记，默认为调用vi。编辑器中将准备好基本的文本。你将在编辑器中看到如下的内容：  
```
==========journal==========
这里输入标题
=======attributePart=======
date=2022-02-26 19:23:32
tags=
level=
==========content==========
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

## 选项
```
-T, --title 指定日记的标题
-A, --at 指定日记的时间，可以是描述性内容，如"now"，"yesterday"等
-C, --content 日定日记的内容
-b, --book 将弹出选项让用户选择日记本
-p, --path 指定日记的路径

-n, --number 制定n篇日记显示。配合--edit可以编辑日记或者配合--delete可以删除日记
-f, --from 指定搜索日记的起始时间
-t, --to 指定搜索日记的终止时间
-o, --on 指定时间搜索日记
-g, --tags 指定tag搜索日记，一次可以输入多个tags，用";"分割
-l, --level 指定level搜索日记，可以用"~"表示level区间
-G, --all_tags 显示所有的tags以及数量

-d, --default_book 弹出选项让用户选择默认日记
-a, --add_book 添加一个日基本陆军
-B, --edit_book_list 调用编辑其编辑日记本记录列表

-E, --edit 编辑所选日记
-D, --delete 删除所选日记
--force_delete 删除所选日记并且不询问
```

# 显示/搜索日记
-n, --number 搜索n篇日记。如搜索最近10篇：
```
jrme -n 10
```
-f, --from 指定搜索日记的起始时间。如：
```
jrme -f "last year"
jrme -f "2018.10"
jrme -f "last week"
```
-t, --to 指定搜索日记的终止时间。如：
```
jrme -f "2018.10" -t "2020-1-2"
jrme -f "last" -t "yesterday"
```
-o, --on 指定搜索日记的日记。如：
```
jrme -on "today"
jrme -on "last month"
```
-g, --tags 指定tag搜索日记，一次可以输入多个tags，用";"分割
```
jrme -g "tag"
jrme -g "tag1;tag2;"
```
-l, --level 指定level搜索日记，可以用"~"表示level区间。该选项默认检索日记JAttribute中的"level"关键字，如没有填写，则自动认为是0。用法示例：
```
jrme -l "2"
jrme -l "-12~13"
```
**以上选项大多可以联用，例如：**
```
jrme -f "last year" -t "yesterday" -n 5
jrme -o "today" -n 3
jrme -f "2011-1-10" -t "2022-3-1" -l "1~5" -t "jrme;jrnl;" -n 33
```
-G, --all_tags 显示所有的tags以及数量。注意：*显示的是tags的数量，而不是拥有该tag的日记的数量*

# 修改/删除日记
在搜索日记的功能基础上添加-E或者--edit编辑所选日记，如:
```
jrme -on "today" -E
jrme -f "last year" -t "yesterday" -n 5 -E
```

搜索日记的功能基础上添加-D或者--delete删除所选日记，如:
```
jrme -f "2014" -t "2018" -g "university;school" -D
```
--force_delete的功能与--delete几乎相同，所不同的是，删除时不会询问。

# JAttribute
当你开始写日记的时候，你应该注意到了日记中出现了一段特殊的字符内容，他们都是key和value的组合。这部分内容是日记的config部分，每一对key-value都是一对JAttribute，这些config将日记的关键信息同意保存。jrme自带了3个JAttribute，用户可以添加插件实现更多的JAttribute。  
## date
用于保存日记的时间，日记默认以时间降序保存。时间的形式默认为YYY-MM-DD hh:mm:ss。及时用户没有写全所有的要素，时间也能正常识别。
## level
这是对日记的重要等级进行标注，其数值可以小于0，可以认为是一个扩展版本的标星功能。如果没有填写任何数字，则认为是0。
## tags
日记的标签功能，多个标签请以;“;”进行分割。

# 配置文件
配置文件位于:~/.jrme/config.ini  
目前支持配置：
```
[base]
editor = 调用的系统编辑器

[plugin]
JAttribute = 插件，多个插件用,分割
journal format = 日记格式插件，多个插件用,分割
```

# 插件
jrme项目为了向日记添加更多更灵活的附加功能，提供了关于日记中config nod的插件和日记格式插件。插件默认保存的位置为： *~/.jrme/plugin/*。本项目提供了两个例子，分别为save_date和md_journal，位于plugin/目录下
## JAttribute插件
每一个config nod都包含默认的key和value。在用户写日记前和写完的时候，插件都能读取到全部的日记文本，并且可以进行一定的修改。  
1、每个JAttribute都有一个自己的key和默认值，在写日记前会自动准备好并展示给用户。  
2、每一个JAttribute会在用户写日记前和用户保存日记的时候获得全部的日记文本，并且可以对其进行修改。
3、jrme自带了3个JAttribute，分别是date、level和tag。请最好不要使用这3个key
  
当前工程自带一个key为"save date"的插件，可以记录保存日记的时间。工程编译后可以获得该插件，位于build/plugin/。  
注意：需要插件顺利加载，需要在配置文件中的JAttribute加入该插件的文件名。如：
```
[plugin]
JAttribute = save_date.so
```
## 日记格式插件
用户可以通过插件实现不过格式的日记存储样式，但是需要注意，无论如何，在用户输入日记的界面是不会改变的，只有保存日记的文本会发生变化。  
日记格式插件可以有自己支持的格式后缀列表，且后缀长度不超过7。系统会优先选择与第一个支持文件后缀名的日记格式插件。如果失败，依次调用插件尝试打开日记。  
  
当前工程自带一个简单的markdown格式插件，工程编译后可以获得该插件，位于build/plugin/。  
注意：需要插件顺利加载，需要在配置文件中的journal format加入该插件的文件名。如：
```
[plugin]
journal format = md_journal.so
```

# 编译与安装
## 编译
需求：gcc,g++
输出位于./build/out/jrme  
编译脚本：build.sh  

## 安装
注意，安装前请编译。安装实际上拷贝到/usr/bin/，所以需要root权限。  
安装jrme脚本：install.sh  
安装jrme插件脚本：install_plugin.sh  
卸载jrme脚本:uninstall.sh  
卸载jrme脚本:uninstall_plugin.sh
