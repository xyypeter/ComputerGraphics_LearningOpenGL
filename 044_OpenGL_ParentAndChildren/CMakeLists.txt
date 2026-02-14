add_definitions (-DDEBUG)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)
add_subdirectory(imgui)

#本工程所有cpp文件编译链接，生成exe
add_executable(openglStudy "main.cpp" "glad.c")

target_link_libraries(openglStudy PRIVATE glfw3.lib wrapper app fw imguilib)

# ========= 资源配置 =========
set(ASSETS_RUNTIME_DIR "${CMAKE_BINARY_DIR}/assets")
add_library(project_config INTERFACE)
target_compile_definitions(project_config INTERFACE
    DEBUG
    ASSETS_DIR="${ASSETS_RUNTIME_DIR}"
)
# exe 用得到
target_link_libraries(openglStudy
    PRIVATE project_config
)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_BINARY_DIR}
)