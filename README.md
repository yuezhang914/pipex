# pipex 

A small utility that simulates shell pipe behavior.

## What this project is

The goal of `pipex` is to reproduce the core behavior of UNIX shell pipelines: send the output of one command through a pipe to the next command, and finally write the result to a target file. Common usage example:

```
./pipex infile "cmd1 arg" "cmd2 arg" outfile
# equivalent to shell: < infile cmd1 | cmd2 > outfile
```

For the bonus part I also implemented a `here_doc`-like mode and support for pipelines of arbitrary length (multi-stage pipelines), for example:

```
./pipex_bonus here_doc LIMITER "cmd1" "cmd2" outfile
# uses heredoc input as stdin for the first command
```

The project is written in C and directly uses low-level system calls such as `fork`, `pipe`, `dup2`, and `execve` to manage processes and pipes. I integrated my own `libft` and included a rich set of test scripts and example usages.

---

## What I did

I implemented `pipex` from scratch, including bonus features. My work includes but is not limited to:

* Designing and implementing process/pipe creation and management: use `pipe()` to create pipes, `fork()` to create child processes, `dup2()` to bind stdin/stdout, and `execve()` to run commands.
* Implementing command parsing and `PATH` lookup: split incoming command strings into argv arrays and locate executable files using the `PATH` environment variable (checking with `access(..., F_OK)`).
* Managing parent/child resources and error handling: print meaningful error messages on failure, properly free allocated memory, close file descriptors, and return appropriate exit statuses (propagating child exit codes).
* Implementing the mandatory scenario (two commands + infile/outfile) and bonus scenarios (`here_doc`, multi-command pipelines).
* Writing a `Makefile` that supports `make` and `make bonus`, and integrating `libft` into the build.
* Creating a large set of test cases in `test/` that cover normal flows and many error cases (command not found, permission issues, wrong arguments, etc.) for manual or automated verification.

---

## Completed features (detailed list)

### Mandatory (core features)

* Read data from the specified `infile` and provide it as stdin for the first command.
* Support a two-command pipeline (`cmd1 | cmd2`): the parent creates two child processes, each runs one of the commands with stdin/stdout redirected as needed.
* Correctly parse command strings into argument arrays (using `ft_split` / `libft`) and find command executables from `PATH`.
* Print errors and return appropriate exit codes when command lookup or `execve` fails.
* Parent process waits for child processes to finish and returns the exit status of the last command.
* `Makefile`: common targets such as `make`, `make re`, `make clean`, and `make fclean`.
* Integrated `libft`: a custom helper library used by the project (string, memory, IO functions, etc.).

### Bonus (extensions)

* `here_doc` support: when the first argument is `here_doc`, read from standard input until the limiter string and use that input for the first command instead of reading from an infile.
* Multi-command pipelines (N-stage pipelines): support `./pipex_bonus infile cmd1 cmd2 ... cmdN outfile` by chaining multiple pipes and connecting child processes in sequence.
* Improved resource management: close unused pipe descriptors and properly reap child processes in multi-command scenarios.
* Extra error and edge-case handling (e.g., command not found, PATH permission issues, fallback logic when `dup2`/`pipe`/`fork` fail).
* The test directory includes bonus and common failure cases to help verify correctness.

---

## Project structure (directory overview)

```
gitpipex/                       # repository root
├─ src/                         # mandatory implementation sources (children, cmd, main, helpers, error, etc.)
├─ bonus/                       # bonus-specific implementation (here_doc, multi-command, bonus utils)
├─ includes/                    # header files (pipex.h, pipex_bonus.h)
├─ libft/                       # my integrated libft (ft_split, ft_printf, etc.)
├─ Makefile                     # build script (targets: all / bonus / clean / fclean / re)
```

---

## Usage

### Dependencies

* Linux or any UNIX-like system that supports `fork`, `pipe`, and `execve`.
* `gcc` or another compatible C compiler, and `make`.
* `libft` is included in this repository, so building only requires running `make` (no external library dependency).

### Build

From the repository root run:

```bash
# build the mandatory version
make

# build the bonus executable (if you want to run bonus features)
make bonus
```

After successful build you will get:

* `pipex`
* `pipex_bonus`

### Examples

Mandatory (two commands):

```bash
# equivalent to shell: < infile cmd1 | cmd2 > outfile
./pipex infile "grep foo" "wc -l" outfile
```

Bonus (`here_doc`):

```bash
# read heredoc input; LIMITER is the ending delimiter
./pipex_bonus here_doc LIMITER "cat -e" "wc -c" outfile
```

Multi-command (if multi-stage pipelines are implemented):

```bash
./pipex_bonus infile "cmd1" "cmd2" "cmd3" outfile
# equivalent to: < infile cmd1 | cmd2 | cmd3 > outfile
```

---------------------中文版---------------------

# pipex (gitpipex) — 项目说明

一个模拟 shell 管道行为的小工具。


## 这个项目是什么

`pipex` 的目标是复现 UNIX shell 管道的核心行为：将一个命令的输出通过管道传给下一个命令，最终把结果写入目标文件。常见用法示例：

```
./pipex infile "cmd1 arg" "cmd2 arg" outfile
# 等价于 shell: < infile cmd1 | cmd2 > outfile
```

在 Bonus 里我还实现了类 `here_doc` 的行为与支持任意长度的命令管道（多段管道），例如：

```
./pipex_bonus here_doc LIMITER "cmd1" "cmd2" outfile
# 使用 heredoc 输入作为第一个命令的 stdin
```

项目使用 C 语言，直接调用 `fork` / `pipe` / `dup2` / `execve` 等底层系统调用来实现进程与管道管理；同时集成了我实现的 `libft`，并提供了丰富的测试脚本与样例用法。

---

## 我做了什么

我从零实现了 `pipex` 的完整功能与 bonus 扩展，工作包含但不限于：

* 设计并实现进程/管道的创建与管理：使用 `pipe()` 创建管道、`fork()` 创建子进程、`dup2()` 绑定标准输入/输出、`execve()` 执行命令。
* 实现命令解析与 `PATH` 查找：把传入的命令字符串切分成 argv 数组，使用环境变量 `PATH` 去定位可执行文件（`access(..., F_OK)` 判断）。
* 实现父/子进程的资源管理与错误处理：失败时打印合理错误信息、正确释放分配的内存、关闭文件描述符并返回合适的退出状态（传递子进程退出码）。
* 实现必做场景（两个命令 + infile/outfile）以及 bonus 场景（`here_doc`、多命令管道）。
* 编写 `Makefile` 支持 `make` 与 `make bonus` 两个构建目标，并集成 `libft` 的构建。
* 编写大量测试用例（`test/`）以覆盖正常路径和各种错误情况（命令不存在、权限问题、参数错误等），用于手工或自动化验证。

---

## 已完成的功能（精确列举）

### 必做（基本功能）

* 从指定 `infile` 读取数据，为第一个命令提供 stdin。
* 支持两个命令的管道（`cmd1 | cmd2`）：父进程创建两个子进程，分别执行第一个与第二个命令，按需重定向 stdin/stdout。
* 正确解析命令字符串为参数数组（使用 `ft_split` / `libft`），并从 `PATH` 中查找命令可执行文件。
* 在命令查找失败或 `execve` 失败时，打印错误并返回合适的退出码。
* 父进程等待子进程完成，并返回最后一个命令的退出状态。
* `Makefile`：`make` / `make re` / `make clean` / `make fclean` 等常用目标。
* 集成 `libft`：自写的基础库函数供主程序使用（字符串、内存、io 等）。

### Bonus（扩展）

* `here_doc` 支持：当第一个参数是 `here_doc` 时，从标准输入读取直到限定符，作为第一个命令的输入（不再从 infile 读）。
* 多命令管道（N 段管道）：支持 `./pipex_bonus infile cmd1 cmd2 ... cmdN outfile`，链式创建多组管道并按顺序连接各个子进程。
* 更完善的资源管理：在多命令场景下正确关闭不再使用的 pipe 描述符，并回收子进程资源。
* 额外的错误/边界场景处理（例如命令不存在、路径权限问题、dup2/pipe/fork 失败的回退逻辑）。
* 测试目录包含 `bonus` 与常见失败用例，便于验证实现正确性。

---

## 整体项目模块构成（目录说明）

```
gitpipex/                       # 仓库根
├─ src/                         # 必做实现源码（children、cmd、main、helpers、error 等）
├─ bonus/                       # bonus 相关实现（here_doc、多命令、bonus-specific utils）
├─ includes/                    # 头文件（pipex.h, pipex_bonus.h）
├─ libft/                       # 我实现并集成的 libft（字符串、ft_split、ft_printf 等）
├─ Makefile                     # 构建脚本（targets: all / bonus / clean / fclean / re）
                  
```

## 使用说明

### 依赖

* Linux 或其他类 UNIX 系统（支持 `fork` / `pipe` / `execve`）
* `gcc` 或兼容的 C 编译器、`make`
* 本仓库已经包含 `libft`，所以只需运行 `make` 即可（不依赖系统库）

### 编译

在仓库根目录运行：

```bash
# 编译必做版本
make

# 编译含 bonus 的可执行（如果你想运行 bonus）
make bonus
```

编译成功会生成：

* `pipex` 
* `pipex_bonus`

### 运行示例

必做（两个命令）：

```bash
# 等价于 shell: < infile cmd1 | cmd2 > outfile
./pipex infile "grep foo" "wc -l" outfile
```

bonus（here_doc）：

```bash
# 通过 heredoc 输入，LIMITER 为结束标记
./pipex_bonus here_doc LIMITER "cat -e" "wc -c" outfile
```

多命令（若实现了多段管道）：

```bash
./pipex_bonus infile "cmd1" "cmd2" "cmd3" outfile
# 等价于: < infile cmd1 | cmd2 | cmd3 > outfile
```
