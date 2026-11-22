cmake_minimum_required(VERSION 3.20)
project(SharkDB CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MYSQL_DIR "C:/Program Files/MySQL/MySQL Server 9.1")

include_directories("${MYSQL_DIR}/include")

add_executable(SharkDB main.cpp)

target_link_libraries(SharkDB
    "${MYSQL_DIR}/lib/libmysql.lib"
)

add_custom_command(TARGET SharkDB POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MYSQL_DIR}/lib/libmysql.dll"
        "${MYSQL_DIR}/bin/libssl-3-x64.dll"
        "${MYSQL_DIR}/bin/libcrypto-3-x64.dll"
        $<TARGET_FILE_DIR:SharkDB>
)

if (MSVC)
    target_compile_options(SharkDB PRIVATE /W4 /utf-8)
else()
    target_compile_options(SharkDB PRIVATE -Wall -Wextra)
    target_link_options(SharkDB PRIVATE -static-libgcc -static-libstdc++)
    target_link_libraries(SharkDB ws2_32)
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    if (EXISTS "${MINGW_BIN_DIR}/libwinpthread-1.dll")
        add_custom_command(TARGET SharkDB POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MINGW_BIN_DIR}/libwinpthread-1.dll"
                $<TARGET_FILE_DIR:SharkDB>
        )
    endif()
endif()
