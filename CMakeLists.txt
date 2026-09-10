cmake_minimum_required(VERSION 3.20)
project(BiohazardV120SplitScreenDecomp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(re5_split120 STATIC
    src/split_localcoop_120.cpp
    src/split_state13_120.cpp
    src/split_render_120.cpp
    src/split_resource_120.cpp
    src/split_resource_geometry_120.cpp
    src/split_resource_geometry_blocks_120.cpp
    src/split_rebuild_120.cpp
    src/split_a1be00_120.cpp
    src/split_a22c00_120.cpp
    src/split_cursor_120.cpp
    src/split_state_change_120.cpp
    src/split_aspect_correction_120.cpp
    src/split_visibility_guard_120.cpp
    src/player_state_accessor_120.cpp
    src/split_table_fallback_120.cpp
    src/split_table_draw_120.cpp
    src/split_explicit_fallback_cases_120.cpp
    src/split_ui_dispatch_120.cpp
    src/split_transform_120.cpp
    src/split_hit_test_120.cpp
    src/split_panel_transform_120.cpp
    src/split_ui_metrics_120.cpp
    src/split_draw_dispatch_120.cpp
    src/split_centered_draw_120.cpp
    src/split_extent_shift_draw_120.cpp
    src/split_ui_primary_120.cpp
    src/split_ui_draw_120.cpp
    src/split_top22_120.cpp
)

target_include_directories(re5_split120 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(RE5_FREESTANDING_CROSS "Build Win32 COFF objects with Clang without a Windows SDK" OFF)
if(RE5_FREESTANDING_CROSS)
    target_compile_options(re5_split120 PRIVATE
        -ffreestanding
        -fno-exceptions
        -fno-rtti
        -nostdinc++
        -I${CMAKE_CURRENT_SOURCE_DIR}/cmake/freestanding
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(re5_split120 PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(re5_split120 PROPERTIES
    OUTPUT_NAME "BiohazardV_1.2.0_splitscreen_decomp_win32"
)
