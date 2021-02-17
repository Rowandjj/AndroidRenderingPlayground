# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.
project("rendering")
set(TARGET render_core)

set(SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/render_jni.cc

        ${CMAKE_SOURCE_DIR}/android_surface.cc
        ${CMAKE_SOURCE_DIR}/async_gl_render.cc
        ${CMAKE_SOURCE_DIR}/gl_render_impl.cc
        ${CMAKE_SOURCE_DIR}/render_factory.cc

        ${CMAKE_SOURCE_DIR}/thread.cc
        ${CMAKE_SOURCE_DIR}/message_loop.cc
        ${CMAKE_SOURCE_DIR}/message_loop_epoll.cc
        )

add_library( # Sets the name of the library.
             ${TARGET}
             SHARED

             ${SOURCE_FILES})

target_link_libraries( # Specifies the target library.
                     ${TARGET}
                      android
                      GLESv2
                      EGL
                      log )