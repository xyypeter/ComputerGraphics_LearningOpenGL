#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

#本工程的名字
project(OpenGL)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

#将funcs文件夹纳入到编译系统
add_subdirectory(funcs)

#搜索所有的Cpp，加入SRCS变量中
#注意下方.之后有一个空格
#aux_source_directory(. SRCS)

#本工程所有cpp文件编译链接，生成exe
#add_executable(openglStudy "main.cpp" "func.h" "func.cpp") 不建议此种写法，文件多了之后会冗余
#add_executable(openglStudy ${SRCS})

#多目标编译写法
add_executable(openglStudy "main.cpp")
add_executable(main2 "main2.cpp")

#将funcs.lib链接到softRender
target_link_libraries(openglStudy funcs)