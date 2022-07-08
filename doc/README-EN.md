# Aboult
With jrme, you can write, search, modify, and delete jrounal/notes only through the command line. Not only that, but you can also add plugins to save journals in different formats, and process them automatically.  
This project refers to the jrnl project.The jrnl project can well implement the function of writing a journal from the command line, but jrnl has several shortcomings:  
1. Requires python3.
2. Additional information of journal (such as tag, time) is not placed regularly.
3. Additional information of lacks flexibility.

# Write a Journal
In your terminal, input `jrme`, the editor (`vi` by default) will be launched. There will be templates ready for editing. It's expected to see the following content

```
==========journal==========
<input title here>
=======attributePart=======
date=2022-02-26 19:23:32
tags=
level=
==========content==========
<input body here>
```

`:wq` to save and quit.

## Other Way to Write a Journal

jrme [-A date] [-T title] [-Cbody]  
例如：
```
jrme -A today -T hello world.what a nice day.
jrme -A today
jrme -T hello world. -C what a nice day.
jrme -T hello world.
```

## Options
```
-T, --title Specifies the title of jrounal
-A, --at Specifies the jrounal date, can be a description, e.g. 'now' or 'yesterday'
-C, --content The jrounal content
-b, --book Will prompt user to select the jrounal book
-p, --path Specifies the jrounal path

-n, --number Create a collection of diaries of size `n`. It can be used together with `--edit` to edit a jrounal or `--delete` to delete one.
-f, --from The start date for jrounal search
-t, --to The end date for jrounal search
-o, --on The exact date for jrounal search
-g, --tags Search jrounal with tags, you can specify multiple tags, seperated by `;`
-l, --level Search jrounal by level. You may use `~` to specify the level range 
-G, --all_tags Display all tags and their count

-d, --default_book Prompt user to select the default jrounal book
-a, --add_book Add a jrounal book
-B, --edit_book_list Edit the jrounal book list

-E, --edit Edit the selected jrounal
-D, --delete Delete the selected jrounal
--force_delete Force delete the selected jrounal without prompt
```

# Browse & Search Journals
-n, --number, browse n journals recently. For example, Browse 10 journals recently:
```
jrme -n 10
```
-f, --from, browse journals since specified time point. For example:
```
jrme -f "last year"
jrme -f "2018.10"
jrme -f "last week"
```
-t, --to, browse journals before specified time point. For example:
```
jrme -f "2018.10" -t "2020-1-2"
jrme -f "last" -t "yesterday"
```
-o, --on browse journals on specified time point. For example:
```
jrme -on "today"
jrme -on "last month"
```
-g, --tags, browse journals, that with specified tags. Tags devide by ";". For example:
```
jrme -g "tag"
jrme -g "tag1;tag2;"
```
-l, --level, browse journals, with the level between. For example:
```
jrme -l "2"
jrme -l "-12~13"
```
**Most of the above options can be combined， For example:**
```
jrme -f "last year" -t "yesterday" -n 5
jrme -o "today" -n 3
jrme 
```
-G, --all_tags, show all tags. Notice: *It will show the number of tags, not the number of journal with the tags.*  

# Modify or Delete Journals
The journals can be edited by appending -E or --edit option while browsing journal.  For example:
```
jrme -on "today" -E
jrme -f "last year" -t "yesterday" -n 5 -E
```
The journals can be deleted by appending -D or --delete option while browsing journal.  For example:
```
jrme -f "2014" -t "2018" -g "university;school" -D
```
--force_delete option is nearly the same as --delete, but jrme will delete without quering.

# JAttribute
You may have token notice of that there some special words while you are writing journal. They are pairs of "key" and "value". All the keys and values construct "config" as part of journal. The config will be save with journal. Jrme has 3 initial JAttribute. They are "data", level and tags. More JAttribute can be implement join as plugin.
## date
The date of journal. Format is YY-MM-DD hh:mm:ss.
## level
The importance of journal. The default value is 0.
## tags
Tags of journal. Tags should be divide by ";".

# Config file
The config file is ~/.jrme/config.ini  
```
[base]
editor = Default editor

[plugin]
JAttribute = JAttribute plugins. Should be divide by ","
Formate = Formate support plugins. Should be divide by ","
```
# Plugins
Jrme is flexible. It support JAttributes plugin and Formate plugin to extend function. Plugins are save at *~/.jrme/plugin/*
## JAttribute Plugin
Every JAttribute include "key" and "valus". Plugins can get whole journal befor user type in journal and while user saving journal. Plugin can modify journal at that time.  
1. Every plugin should have a default value to set while user prepare to type in journal.
2. Every plugin can get whole journal befor user type in journal and while user saving journal. Plugin can modify journal at that time.   
3. jrme has build in 3 config nod. They are date, level and tag. Don`t use the three words in plugin.  
  
Jrme project provide an JAttribute plugin as example which is named as "save data". Run build.sh script and you can get it in ./build/plugin/. Move save_data.so to *~/.jrme/plugin/* and modify *~/.jrme/config.ini* for journal to load plugin.   
~/.jrme/config.ini example：
```
[plugin]
JAttribute = save_date.so
```
## Formate Plugin
Users can save journal as different kind of format. Notice that the interface for typ in journal will not change, no matter which kind of format has be select. The format will only change while user save journal. Plugin can provide a list of supporting postfix, but each postfix should not longer then 7 char. Jrme will auto select plugin to open journal.  
Jrme project provide  an JAttribute plugin as example which support markdown format. Run build.sh script and you can get it in ./build/plugin/. Move save_data.so to *~/.jrme/plugin/* and modify *~/.jrme/config.ini* for journal to load plugin.   
~/.jrme/config.ini example：
```
[plugin]
Formate = md_journal.so
```  

# Compile & Install
## Compile
Need：gcc,g++  
Output: ./build/out/jrme  
Build script: ：build.sh 

## Install
Note: The install processes is acturly copy jrme to /usr/bin/. So you need to compile before install, and you also need root permission.  
install jrme: install.sh  
install plugin: install_plugin.sh  
uninstall: uninstall.sh  
uninstall plugins: uninstall_plugin.sh