cmake_minimum_required(VERSION 3.20)
project(SharkDB CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MYSQL_DIR "C:/Program Files/MySQL/MySQL Server 9.1")

include_directories("${MYSQL_DIR}/include")

add_executable(SharkDB main.cpp)
add_executable(SharkDB_OOP shark_oops.cpp)

get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

foreach(target SharkDB SharkDB_OOP)
    target_link_libraries(${target} "${MYSQL_DIR}/lib/libmysql.lib")

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MYSQL_DIR}/lib/libmysql.dll"
            "${MYSQL_DIR}/bin/libssl-3-x64.dll"
            "${MYSQL_DIR}/bin/libcrypto-3-x64.dll"
            $<TARGET_FILE_DIR:${target}>
    )

    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra)
        target_link_options(${target} PRIVATE -static-libgcc -static-libstdc++)
        target_link_libraries(${target} ws2_32)
        if (EXISTS "${MINGW_BIN_DIR}/libwinpthread-1.dll")
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN_DIR}/libwinpthread-1.dll"
                    $<TARGET_FILE_DIR:${target}>
            )
        endif()
    endif()
endforeach()
