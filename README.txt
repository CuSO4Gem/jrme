这是一个还在开发中的工程，为了能够顺利加载插件，请在环境变量中添加如下内容：
export JRME_PLUGIN=你希望保存插件的位置
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$JRME_PLUGIN
这是一个还在开发中的工程，为了能够顺利加载插件，请在环境变量中添加如下内容：
export JRME_PLUGIN=你希望保存插件的位置
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$JRME_PLUGIN

编译:make
编译插件:make plugin
编译测试:make test
清除:make clean

编译（带插件）:make PLUGIN=y
清除（带插件）:make clean PLUGIN=y