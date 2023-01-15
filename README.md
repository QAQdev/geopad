# GeoPad

详细设计请查看`面向对象程序设计报告`，这里只简要说明构建方法。

此份代码为 Windows 版本，仅支持在 Windows 上编译运行。构建方法如下：

- 在工程目录下执行 `mkdir build`
- `cd build`
- `cmake .. -G "MingGW Makefiles"`
- `make`

执行完上述命令后，会在 `build` 文件夹内产生可执行文件。
