cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(DVD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# -----------------------------------------
# fetch GLFW
# -----------------------------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)
# -----------------------------------------


# -----------------------------------------
# fetch GLM
# -----------------------------------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)
# -----------------------------------------

# -----------------------------------------
# build GLAD as a static library
# -----------------------------------------
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
# -----------------------------------------

# -----------------------------------------
# build stb_image as a static library
# -----------------------------------------
add_library(stb_image STATIC external/stb/stb_image.cpp)
target_include_directories(stb_image PUBLIC external/stb)
# -----------------------------------------

# chapters only call add_gl_target(<chapter>_<name> <file.cpp>).
function(add_gl_target target_name source_file)
    add_executable(${target_name} ${source_file})

    # linked compiled libraries
    target_link_libraries(${target_name} PRIVATE
        glfw
        glad
        glm
        stb_image
        opengl32
    )

    # tell the compiler where to find headers
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_BINARY_DIR}/_deps/glm-src
        ${CMAKE_SOURCE_DIR}/include
    )

    # absolute resources path so exes work from any working directory.
    # usage in C++: Shader(RESOURCE_DIR "shaders/vertex.vert", ...)
    # — adjacent literals concatenate.
    file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/resources" _resources_dir)
    target_compile_definitions(${target_name}
    PRIVATE "RESOURCE_DIR=\"${_resources_dir}/\"")

    # organize output per chapter when CHAPTER_NAME is
    # set by src/<chapter>/CMakeLists.txt
    if(DEFINED CHAPTER_NAME)
        set_target_properties(${target_name} PROPERTIES
            OUTPUT_NAME ${target_name}
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CHAPTER_NAME}
            FOLDER ${CHAPTER_NAME}
        )
    else()
        set_target_properties(${target_name} PROPERTIES
            OUTPUT_NAME ${target_name}
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )
    endif()
endfunction()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# playground / root entry point (kept at src/main.cpp, uses
# resources/shaders/vertex.vert)
add_gl_target(LearnOpenGL src/main.cpp)
