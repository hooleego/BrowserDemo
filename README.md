# 项目说明
---
基于CEF3的浏览器Demo


# 相关依赖
---
1. 安装Visual Studio 2017 Community

2. 安装Qt5.12.6(使用32位开发平台)

> Qt安装完成后，要添加两个系统环境变量
> 
> 1. QTDIR：QT安装目录，例：C:\Qt\Qt5.12.6\5.12.6\msvc2017
> 2. 将Qt可执行文件目录添加入Path，例：C:\Qt\Qt5.12.6\5.12.6\msvc2017\bin

3. 安装Qt Visual Studio Tools，下载地址为：https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123

4. 安装CMake 3.19.2+

5. 下载CEF开发库，并编译libcef_dll_wrapper，编译步骤请参考
```
compile_libcef_dll_wrapper.pdf
```

6. 拷贝头文件、lib文件和bin文件
```
拷贝头文件
拷贝CEF开发库目录下的include文件夹拷贝到本项目的thirdparty/cef3/目录中

拷贝lib文件
拷贝CEF开发库目录下的Debug/libcef.lib文件拷贝到本项目的thirdparty/cef3/debug/lib目录中
拷贝(5)步骤中编译生成的Debug版本的libcef_dll_wrapper.lib文件拷贝到本项目的thirdparty/cef3/debug/lib目录中
拷贝CEF开发库目录下的Release/libcef.lib文件拷贝到本项目的thirdparty/cef3/lib目录中
拷贝(5)步骤中编译生成的Release版本的libcef_dll_wrapper.lib文件拷贝到本项目的thirdparty/cef3/lib目录中

拷贝bin文件
拷贝CEF开发库目录下的Debug里面的所有文件拷贝到本项目的thirdparty/cef3/debug/bin目录中
拷贝CEF开发库目录下的Resources里面的所有文件拷贝到本项目的thirdparty/cef3/debug/bin目录中
拷贝CEF开发库目录下的Release里面的所有文件拷贝到本项目的thirdparty/cef3/bin目录中
拷贝CEF开发库目录下的Resources里面的所有文件拷贝到本项目的thirdparty/cef3/bin目录中
```


# 构建
---




# 打包
---
1. 打开cmd终端，进入INSTALL目录，然后执行如下命令

```
windeployqt BrowserDemo.exe --no-quick-import --no-translations --no-serialport --no-svg --pdb
```


# Troubleshooting
---
1. 只有Release版本能正常运行
2. Debug版本也能编译和运行，但是运行后无法打开加载页面


