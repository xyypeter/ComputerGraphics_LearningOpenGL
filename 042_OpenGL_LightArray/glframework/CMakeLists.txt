file(GLOB_RECURSE FW ./  *.cpp)

add_library(fw ${FW} )

# fw 自己需要 ASSETS_DIR（Renderer.cpp 在这里）
target_link_libraries(fw
    PUBLIC project_config
)