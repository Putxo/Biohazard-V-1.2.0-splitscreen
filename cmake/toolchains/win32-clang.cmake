set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS_INIT "--target=i686-pc-windows-msvc")
# GNU ar/ranlib accept Intel COFF .obj members and are installed on GitHub Ubuntu runners.
set(CMAKE_AR ar)
set(CMAKE_RANLIB ranlib)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(RE5_FREESTANDING_CROSS ON CACHE BOOL "" FORCE)
