cmake_minimum_required(VERSION 3.10)
project(foro VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

set(src_include
    include/lista.hpp
    include/lista_doble.hpp
    include/cola_prioridad.hpp
    include/vector.hpp
    include/heap.hpp
)

set(src
    ${src_include}
    #src/source.cpp
)


#add_executable(foro ${src})
add_executable(source ${src} src/source.cpp)
target_include_directories(source PRIVATE include)


if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
