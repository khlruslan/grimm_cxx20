include_directories(${cond_var_SOURCE_DIR}/inc)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


add_executable(cond_var cond_var.cxx)
add_executable(atomic_bool atomic_bool.cxx)
add_executable(atomic_flag atomic_flag.cxx)
add_executable(atomic_flag_one atomic_flag_one.cxx)
add_executable(empty_loop empty_loop.cxx)
add_executable(semaphore semaphore.cxx)

target_link_libraries(cond_var PRIVATE Threads::Threads)
target_link_libraries(atomic_bool PRIVATE Threads::Threads)
target_link_libraries(atomic_flag PRIVATE Threads::Threads)
target_link_libraries(atomic_flag_one PRIVATE Threads::Threads)
target_link_libraries(semaphore PRIVATE Threads::Threads)
