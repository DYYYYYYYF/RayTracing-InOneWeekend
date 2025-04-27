cmake_minimum_required (VERSION 3.8)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Library)

if (WIN32)
    message("-- Current environment: Windows")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    # 1. 查找 Windows Kit 路径
    set(WINDOWS_KIT_ROOT "$ENV{WindowsSdkDir}")  # 从环境变量获取
    if (NOT WINDOWS_KIT_ROOT)
        # 默认路径（如果环境变量未设置）
        set(WINDOWS_KIT_ROOT "C:/Program Files (x86)/Windows Kits/10")
    endif()

    # 2. 查找最新版本的 Windows SDK
    file(GLOB SDK_VERSIONS "${WINDOWS_KIT_ROOT}/Include/*")
    list(SORT SDK_VERSIONS)  # 排序找到最新版本
    list(REVERSE SDK_VERSIONS)
    list(GET SDK_VERSIONS 0 LATEST_SDK_VERSION)
    get_filename_component(SDK_VERSION ${LATEST_SDK_VERSION} NAME)

    # 3. 设置 DX12 头文件和库路径
    set(DX12_INCLUDE_DIRS 
        "${WINDOWS_KIT_ROOT}/Include/${SDK_VERSION}/um"
        "${WINDOWS_KIT_ROOT}/Include/${SDK_VERSION}/shared"
    )
    set(DX12_LIBRARY_DIR "${WINDOWS_KIT_ROOT}/Lib/${SDK_VERSION}/um/${CMAKE_VS_PLATFORM_NAME}")  # e.g., x64

    # 4. 检查关键文件是否存在
    find_file(D3D12_H d3d12.h PATHS ${DX12_INCLUDE_DIRS} REQUIRED)
    find_library(D3D12_LIB d3d12.lib PATHS ${DX12_LIBRARY_DIR} REQUIRED)

    # 5. 添加到项目
    include_directories(${DX12_INCLUDE_DIRS})
    link_directories(${DX12_LIBRARY_DIR})

    set(DIRECTX12_ENABLED ON)
endif()

find_package(OpenGL QUIET)
find_package(Vulkan QUIET)

include_directories(includes includes/imgui Platform)
aux_source_directory(sources SRC)
aux_source_directory(Platform PLATFORM_SRC)
add_executable (RayTracer "main.cpp" ${SRC} ${PLATFORM_SRC})

if(WIN32)
    add_compile_definitions(-DDX12_ENABLED)
	message("-- DirectX12 enabled.")
else()
	message("-- DirectX12 disabled.")
endif()

if(Vulkan_FOUND)
	add_compile_definitions(-DVULKAN_ENABLED)
	include_directories(${Vulkan_INCLUDE_DIRS})
	target_link_libraries(RayTracer PUBLIC Vulkan::Vulkan)
	message("-- Vulkan enabled.")
else()
	message("-- Vulkan disabled.")
endif()

if(OpenGL_FOUND)
	add_compile_definitions(-DOPENGL_ENABLED)
	target_link_libraries(RayTracer PUBLIC  OpenGL::GL)
	message("-- OpenGL enabled.")
else()
	message("-- OpenGL disabled.")
endif()
