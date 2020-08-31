find_package (Threads)

include_directories("${PROJECT_HEADERS}")
file(GLOB COMMON_SRC
        "*.cpp" "*.c")

add_library(common STATIC ${COMMON_SRC})
target_link_libraries(common ${CMAKE_THREAD_LIBS_INIT})