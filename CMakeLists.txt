cmake_minimum_required(VERSION 3.11)
project(raygui-container
    DESCRIPTION "raygui-container: An extention to raygui"
    HOMEPAGE_URL "https://github.com/robloach/raygui-container"
    VERSION 0.0.1
)

# Register the cmake folder for find_package()
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(RAYGUI_CONTAINER_IS_SOURCE_DIR TRUE)
else()
    set(RAYGUI_CONTAINER_IS_SOURCE_DIR FALSE)
endif()
option(RAYGUI_CONTAINER_BUILD_EXAMPLE "${PROJECT_NAME}: Example" ${RAYGUI_CONTAINER_IS_SOURCE_DIR})

# raygui-container
add_library(raygui-container INTERFACE)
# TODO: Make raygui download via cmake or something.
target_include_directories(raygui-container INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raygui
)

# raygui-container-example
if (RAYGUI_CONTAINER_BUILD_EXAMPLE)
    add_executable(raygui-container-example
        example/raygui-container-example.c
    )

    find_package(raylib)
    target_link_libraries(raygui-container-example PUBLIC
        raylib
        raygui-container
    )
endif()