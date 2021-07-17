# insert_str
a tool helps you to read c/c++ code and debug


In order to clarify the calling process of the function when reading the code or debugging, 
one way is to add printing to the function. However, if the code logic is complex and the 
amount of code is large, it will be very troublesome to add printing manually.
insert_str is a tool that can automatically insert what you want to print into the file (.c .h).
After the code is cloned, $make, have a look at test.cpp, and then execute $insert_str testfile, 
and then look at the changes in test.cpp, you can know what insert_str has done 

useage:
1 backup your code
2 Prepare a file (such as testfile). Each line in the testfile is the path of a .c/.h file, 
	which can be an absolute path or a relative path.
	For example, you can get files like this: find ./ -name *.c > testfile
3 Find the main() function in insert_str.c, modify str1 and str2 to the values ​​you want, and then save and compile.
	If my_print.h can meet your needs, you don’t need to change str1/str2. If necessary, you can put my_print.h in /usr/loacl/include
4 Execute $insert_str testfile, str1 will be inserted into the first line of all files such as .c/.h, 
	and str2 will be inserted into the first line of each function definition in files such as .c/.h
5 If there is an error in some files, the errlog under the current path will record these files, and you 
	need to manually check them
6 Recompile your file



读代码时，或者debug时，为了弄清楚函数的调用过程，一种方法是向函数中添加打印， 但是如果代码逻辑很复杂而且代码量很大， 手动加打印会很麻烦。
insert_str是一个工具， 可以自动向文件中（.c .h ）插入要打印的东西.
代码clone下来后， make一下， 打开test.cpp看一下， 然后执行 $insert_str testfile， 再看一下test.cpp的变化， 就可以知道insert_str都做了什么了

使用方法是
1 备份你的代码
2 准备一个文件files（比如testfile）， testfile文件中的每一行都是一个.c/.h文件的路径， 可以是绝对路径也可以是相对路径， 
	比如可以这样得到files：  find ./ -name *.c > testfile
3 找到insert_str.c中的main函数， 修改str1 和 str2 ，改为你想要的值，然后保存编译。
	如果my_print.h可以满足你的需要， 也可以不改str1/str2, 如果有必要可以把my_print.h放进/usr/loacl/include
4 执行$insert_str testfile， str1会被插入到所有.c/.h等文件的第一行， str2会被插入到.c/.h等文件中每一个函数定义的第一行
5 如果在某些文件中出了错， 当前路径下的errlog会记录这些文件，需要手动检查errlog中的每一个文件出错的原因
6 重新编译你的文件
