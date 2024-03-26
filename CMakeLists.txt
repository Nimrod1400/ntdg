cmake_minimum_required(VERSION 3.10)

project(ntdg)

add_compile_options(-ggdb)
add_compile_definitions(TB_OPT_ATTR_W=64)
add_executable(ntdg src/main.c)
