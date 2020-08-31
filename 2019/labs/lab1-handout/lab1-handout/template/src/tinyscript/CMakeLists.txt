include_directories("${PROJECT_HEADERS}")
file(GLOB TINYSCRIPT_SRC
        "*.c" "*.cpp"
        )

find_package(BISON)
find_package(FLEX)

BISON_TARGET(TinyParser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(TinyScanner parser/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(TinyScanner TinyParser)

add_library(tinyscript STATIC ${TINYSCRIPT_SRC} ${BISON_TinyParser_OUTPUTS} ${FLEX_TinyScanner_OUTPUTS})
target_link_libraries(tinyscript common)
add_executable(interp interp_main.c)
target_link_libraries(interp common)
target_link_libraries(interp tinyscript)
target_link_libraries(interp ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(interp PUBLIC -Wl,--section-start=__sandbox=0x18000000)
target_link_libraries(interp -Wl,--section-start=__sandbox=0x18000000)