# Aboult
With jrme, you can write, search, modify, and delete diary/notes only through the command line. Not only that, but you can also add plugins to save journals in different formats, and process them automatically.  
This project refers to the jrnl project.The jrnl project can well implement the function of writing a journal from the command line, but jrnl has several shortcomings:  
1. Requires python3.
2. Additional information of journal (such as tag, time) is not placed regularly.
3. Additional information of lacks flexibility.

# Write a diary
In your terminal, input `jrme`, the editor (`vi` by default) will be launched. There will be templates ready for editing. It's expected to see the following content

```
==========journal==========
<input title here>
==========config==========
date=2022-02-26 19:23:32
tags=
level=
==========content==========
<input body here>
```

`:wq` to save and quit.

## Other way to write a diary

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
-T, --title Specifies the title of diary
-A, --at Specifies the diary date, can be a description, e.g. 'now' or 'yesterday'
-C, --content The diary content
-b, --book Will prompt user to select the diary book
-p, --path Specifies the diary path

-n, --number Create a collection of diaries of size `n`. It can be used together with `--edit` to edit a diary or `--delete` to delete one.
-f, --from The start date for diary search
-t, --to The end date for diary search
-o, --on The exact date for diary search
-g, --tags Search diary with tags, you can specify multiple tags, seperated by `;`
-l, --level Search diary by level. You may use `~` to specify the level range 
-G, --all_tags Display all tags and their count

-d, --default_book Prompt user to select the default diary book
-a, --add_book Add a diary book
-B, --edit_book_list Edit the diary book list

-E, --edit Edit the selected diary
-D, --delete Delete the selected diary
--force_delete Force delete the selected diary without prompt
```

# compile&sintall
## compile
Need：gcc,g++  
Output: ./build/out/jrme  
Build script: ：build.sh 

## install
Note: The install processes is acturly copy jrme to /usr/bin/. So you need to compile before install, and you also need root permission.  
install jrme: install.sh  
install plugin: install_plugin.sh  
uninstall: uninstall.sh  
uninstall plugins: uninstall_plugin.sh