# CMakeList.txt: CMakeEnvrionmenttest 的 CMake 项目配置
cmake_minimum_required (VERSION 3.12)

# 项目名称
project(CMakeEnvrionmenttest)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 将源代码添加到可执行文件
add_executable(CMakeEnvrionmenttest 
    "CMakeEnvrionmenttest.cpp" 
    "CMakeEnvrionmenttest.h" 
    "Game.h" 
    "Game.cpp" 
    "Scenc.h" 
    "Scenc.cpp"
    "ScencMain.h" 
    "ScencMain.cpp" 
    "ScencTitle.h" 
    "ScencTitle.cpp"
    "Object.h"
    "SceneEnd.h"
    "SceneEnd.cpp")

# SDL2及扩展库配置
set(SDL2_ALL_DIR "E:/SDL2")  # SDL根目录（请确保路径正确）

# 查找SDL2库
find_package(SDL2 REQUIRED
    PATHS ${SDL2_ALL_DIR}
    NO_DEFAULT_PATH
)

# 查找SDL2扩展库
find_package(SDL2_mixer REQUIRED
    PATHS ${SDL2_ALL_DIR}
    NO_DEFAULT_PATH
)

find_package(SDL2_image REQUIRED
    PATHS ${SDL2_ALL_DIR}
    NO_DEFAULT_PATH
)

find_package(SDL2_ttf REQUIRED
    PATHS ${SDL2_ALL_DIR}
    NO_DEFAULT_PATH
)

# 链接SDL库到目标
target_link_libraries(CMakeEnvrionmenttest PRIVATE 
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_mixer::SDL2_mixer
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)

# 输出目录设置（可执行文件和资源的目标目录）
set(TARGET_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# 资源路径配置
set(ASSETS_IMAGE_SOURCE "E:/GameResources/SDL_Shooter/assets/image")
set(ASSETS_IMAGE_DEST "${TARGET_OUTPUT_DIR}/image")

set(ASSETS_MUSIC_SOURCE "E:/GameResources/SDL_Shooter/assets/music")
set(ASSETS_MUSIC_DEST "${TARGET_OUTPUT_DIR}/music")

set(ASSETS_SOUND_SOURCE "E:/GameResources/SDL_Shooter/assets/sound")
set(ASSETS_SOUND_DEST "${TARGET_OUTPUT_DIR}/sound")

set(ASSETS_TTF_SOURCE "E:/GameResources/SDL_Shooter/assets/font")  # 修正拼写错误（原ASSEST）
set(ASSETS_TTF_DEST "${TARGET_OUTPUT_DIR}/font")                  # 修正拼写错误

# 复制SDL动态库到目标目录（POST_BUILD确保编译后执行）
add_custom_command(TARGET CMakeEnvrionmenttest POST_BUILD
    # 复制SDL2核心库
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_ALL_DIR}/lib/x64/SDL2.dll"
        "${TARGET_OUTPUT_DIR}"
    
    # 复制SDL2_image库
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_ALL_DIR}/lib/x64/SDL2_image.dll"
        "${TARGET_OUTPUT_DIR}"
    
    # 复制SDL2_mixer库
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_ALL_DIR}/lib/x64/SDL2_mixer.dll"
        "${TARGET_OUTPUT_DIR}"
    
    # 复制SDL2_ttf库
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_ALL_DIR}/lib/x64/SDL2_ttf.dll"
        "${TARGET_OUTPUT_DIR}"
    
    COMMENT "已复制SDL动态库到目标目录: ${TARGET_OUTPUT_DIR}"
)

# 复制图像资源（先创建目录，再复制文件）
add_custom_command(TARGET CMakeEnvrionmenttest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_IMAGE_DEST}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_IMAGE_SOURCE}/"
        "${ASSETS_IMAGE_DEST}/"
    COMMENT "已复制图像资源到: ${ASSETS_IMAGE_DEST}"
)

# 复制音乐资源
add_custom_command(TARGET CMakeEnvrionmenttest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_MUSIC_DEST}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_MUSIC_SOURCE}/"
        "${ASSETS_MUSIC_DEST}/"
    COMMENT "已复制音乐资源到: ${ASSETS_MUSIC_DEST}"
)

# 复制音效资源
add_custom_command(TARGET CMakeEnvrionmenttest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_SOUND_DEST}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_SOUND_SOURCE}/"
        "${ASSETS_SOUND_DEST}/"
    COMMENT "已复制音效资源到: ${ASSETS_SOUND_DEST}"
)

# 复制字体资源
add_custom_command(TARGET CMakeEnvrionmenttest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_TTF_DEST}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_TTF_SOURCE}/"
        "${ASSETS_TTF_DEST}/"
    COMMENT "已复制字体资源到: ${ASSETS_TTF_DEST}"
)

# 可选：在IDE中显示资源文件（不影响构建，仅方便查看）
file(GLOB_RECURSE ASSET_FILES 
    "${ASSETS_IMAGE_SOURCE}/*"
    "${ASSETS_MUSIC_SOURCE}/*"
    "${ASSETS_SOUND_SOURCE}/*"
    "${ASSETS_TTF_SOURCE}/*"
)
add_custom_target(Assets SOURCES ${ASSET_FILES})