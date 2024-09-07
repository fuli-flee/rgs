#pragma once
#define LOG( ...)//设置日志输出为空
#define ASSERT(x,... ) 
//{if(!(x)) {LOG(__VA_ARGS__); __debugbreak();}}
//debugbreak 断点调试    __VA_ARGS__ 代表可变参数