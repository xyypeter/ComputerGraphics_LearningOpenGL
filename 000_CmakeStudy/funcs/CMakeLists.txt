#递归(RECURSE)将本文件夹下的所有Cpp放到FUNCS中
file(GLOB_RECURSE FUNCS ./ *.cpp)

#将FUNCS中的所有CPP编译为funcs这个lib库
add_library(funcs ${FUNCS})
