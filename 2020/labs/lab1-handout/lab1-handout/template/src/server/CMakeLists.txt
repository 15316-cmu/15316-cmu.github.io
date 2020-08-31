find_package (Threads)

add_executable(server tiny.c)
target_link_libraries(server common)
target_link_libraries(server tinyscript)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

add_executable(client client.c)
target_link_libraries(client common)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})

