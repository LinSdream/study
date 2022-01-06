//定义STB_IMAGE_IMPLEMENTATION，预处理器会修改stb_image.h头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件了具体可以看这个头文件的注解
#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"