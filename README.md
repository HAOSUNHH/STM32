# STM32
 STM32

vscode开发stm32教程:

# 用vscode编辑keil-STM32

vscode非常的漂亮,用它写代码总体来说是比keil好.

## **keil assistant**

![image-20220918134639503](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918134639503.png)

鼠标右键设置keil assistant插件,就是把你安装的keil路径写进去,你也可以看这个插件的简介,里边也写了用法

![image-20220918134728514](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918134728514.png)

3 设置编码问题,不设置的话中文注释是没有办法显示的,会乱码.keil的编码是GB2312.在齿轮那里找到设置,输入encoding,把第一个打钩.他就会自动检测编码,并改变了.

![image-20220918134804870](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918134804870.png)

**在keil中构建你的工程,这个都应该会的,不会的话你也不会看到这个文章(先在keil或者STM**

**STM32CUBMX生成,然后在vscode开发)**

这个很重要就是在keil项目,在vscode编辑代码.

keil的环境一定要配置好

![image-20220918121237710](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918121237710.png)

![image-20220918121302503](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918121302503.png)

![image-20220918121332452](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918121332452.png)

![image-20220918121349944](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918121349944.png)

下载后可能会有一个.net平台的下载进行下载

![image-20220918120108229](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918120108229.png)

------

不出现就不用加

但是可能会有波浪线报错,因为,vscode他找不到一些定义的变量,你可以跳转,但是他报错,看着很别扭,所以在你编辑器列表中找到c_cpp_properties.json文件,添加两句话.

```
"MYSYMBOL",

"MYVALUE=1"
```

![image-20220918121512043](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918121512043.png)

---

## 我遇到的问题

在烧录程序时，出现以下图片提示

Error: Flash Download failed - Target DLL has been cancelled

![img](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/20200405095555716.jpg)
![img](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/20200405095555292.jpg)

选择 Debug——>settings

![image-20220918135328695](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918135328695.png)

![image-20220918135444713](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918135444713.png)

看你自己我用的DAP-LINK,看情况选择

![image-20220918135708791](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918135708791.png)

![image-20220918135735991](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/image-20220918135735991.png)

选择 Flash Download——>勾选上Rest and Run——>点击确定

![img](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/20200405095556613.png)

选择 Utilities——>勾选中Use Debug Driver——>点击OK

![img](https://hongecho.oss-cn-hangzhou.aliyuncs.com/images/20200405095556315.png)

重新编译以下就好了，此时可以正常烧录，不会有报错了

---

## Embedded IDE
