# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

#cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#add_library( # Sets the name of the library.
#             native-lib
#
#             # Sets the library as a shared library.
#             SHARED
#
#             # Provides a relative path to your source file(s).
#             native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#find_library( # Sets the name of the path variable.
#              log-lib
#
#              # Specifies the name of the NDK library that
#              # you want CMake to locate.
#              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#target_link_libraries( # Specifies the target library.
#                       native-lib
#
#                       # Links the target library to the log library
#                       # included in the NDK.
#                       ${log-lib} )

cmake_minimum_required(VERSION 3.4.1)

# 外部头文件路径，因为我们要引用 libyuv.h
include_directories(libyuv/include)

# 添加子项目，libyuv 作为一个子项目自己编译，有自己的 CMakeList.txt。
# 编译结果存放在 build 目录下，可以在里面找到生成的 .so 文件。
add_subdirectory(libyuv ./build)

# 生成动态链接库 yuvutil,  YuvJni.cpp 是我们的源代码，可以指定多个源文件。
add_library(yuvutil SHARED YuvJni.cpp)

# 添加NDK里面 编译好的  log 库
find_library(log-lib log)

# 把 yuv (这个是 libyuv 子项目生成的 yuv.so) 和 log 库链接到 yuvutil 中
target_link_libraries(yuvutil ${log-lib} yuv)
