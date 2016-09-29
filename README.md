# rcsh

[![License](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](LICENSE)

完成 [中科大《软件工程》MOOC](http://mooc.study.163.com/course/USTC-1000002006) 中的可重用命令行菜单小程序，其实相当于实现了个非常非常简易的 shell。

其中，`CLIMenu` 及其所依赖的 `Callback`、`CallbackMap` 可以单独拿来用在其它各类需要接受用户输入命令的命令行程序中（如 ftp、telnet），`main.c` 则是实现类似 shell 的效果。

## 运行示例

```
~ $ hello
Hello! Welcome to rcsh!
~ $ help
Commands may be abbreviated. Commands are:
help		print help information
hello		print hello message
echo		echo a string
cd		enter a directory
pwd		print current directory
ls		list items in current directory
sys		run command in system shell
exit		exit the shell
~ $ pwd
/Users/richard
~ $ cd ..
/Users $ ls
Guest
richard
Shared
/Users $ uname -a
rcsh: uname: no such command, using system shell
Darwin Richard-Mac.local 15.6.0 Darwin Kernel Version 15.6.0: Mon Aug 29 20:21:34 PDT 2016; root:xnu-3248.60.11~1/RELEASE_X86_64 x86_64
/Users $ sys uname -a
Darwin Richard-Mac.local 15.6.0 Darwin Kernel Version 15.6.0: Mon Aug 29 20:21:34 PDT 2016; root:xnu-3248.60.11~1/RELEASE_X86_64 x86_64
/Users $ echo "test"
test
/Users $ exit
```

## CLIMenu 的接口说明

该部分可在其它程序中重用，因此这里给出它的接口，方便以后使用。

__重要：__`_` 和 `__` 开头的任何变量、函数，使用者都不应该自己更改或调用。

### 创建和销毁 CLIMenu

```c
CLIMenu *menu = newCLIMenu();
deleteCLIMenu(&menu);
```

### 成员变量

| 变量名 | 变量类型 | 说明 |
| ------ | --------- | ---- |
| data | void * | 可自由使用的数据指针，默认为 NULL，如果使用中分配了内存，可以在 `finalizeCallback` 中释放内存 |
| initializeCallback | SimpleCallback | 用于初始化，在 `run` 函数的开头调用 |
| finalizeCallback | SimpleCallback | 用于做收尾处理，在 `run` 函数结尾调用（也就是跳出接受输入的循环之后） |
| promptCallback | SimpleCallback | 用于输出提示符，如果不设置，默认输出 `>>>` 作为提示符 |
| noSuchCommandCallback | Callback | 没找到命令对应的回调函数时调用，默认值为 `CLIMenu.c` 中的 `defaultNoSuchCommandCallback` |

### 成员函数

注意：每一个成员函数的第一个参数，都是 CLIMenu 自身的结构体指针，用于访问数据（毕竟 C 不是面向对象的），后面不再列出。

| 函数名 | 返回类型 | 参数列表 | 说明 |
| ------ | --------- | -------- | ---- |
| add | void | const char \*command, Callback callback | 添加一个命令 |
| remove | void | const char \*command | 删除一个命令 |
| clear | void | | 清除所有命令 |
| run | void | const char \*exitCommand | 运行命令行菜单 |
