
cmake_minimum_required(VERSION 3.16)
project(sdl-image-processor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(subrepos/SDL       EXCLUDE_FROM_ALL)
add_subdirectory(subrepos/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(subrepos/SDL_ttf   EXCLUDE_FROM_ALL)

add_executable(main WIN32
    src/main.c
    src/image_loader.c
    src/image_processor.c
    src/ui_components.c
    src/ui_controller.c
    src/ui_state.c
    src/ui_view.c
    src/utils.c
)

option(ENABLE_SANITIZERS "Enable Address/Undefined sanitizers" OFF)

set(COMMON_WARN_FLAGS -Wall -Wextra -Wpedantic -Wformat=2 -Wshadow -Wall)
set(DEBUG_FLAGS -g -Og -fno-omit-frame-pointer)
set(SECURITY_FLAGS -D_FORTIFY_SOURCE=2 -fstack-protector-strong)
set(SANITIZERS "")

if(ENABLE_SANITIZERS)
    set(SANITIZERS -fsanitize=address,undefined)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    list(APPEND COMMON_WARN_FLAGS -Wno-unused-parameter)
endif()

target_compile_options(main PRIVATE ${COMMON_WARN_FLAGS} ${DEBUG_FLAGS} ${SECURITY_FLAGS} ${SANITIZERS})
target_link_options(main PRIVATE ${SANITIZERS})

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(main PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    m 
)
