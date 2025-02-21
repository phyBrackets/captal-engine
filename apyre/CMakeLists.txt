cmake_minimum_required(VERSION 3.15.7)

project(apyre VERSION 0.1.0)

include(cmake/buildsdl.cmake)

set(APYRE_SOURCES
    #Headers:
    src/apyre/config.hpp
    src/apyre/application.hpp
    src/apyre/monitor.hpp
    src/apyre/inputs.hpp
    src/apyre/window.hpp
    src/apyre/event.hpp
    src/apyre/messagebox.hpp
    src/apyre/power.hpp

    #Sources:
    src/apyre/application.cpp
    src/apyre/inputs.cpp
    src/apyre/window.cpp
    src/apyre/event.cpp
    src/apyre/messagebox.cpp
    src/apyre/power.cpp
)

if(CAPTAL_BUILD_APYRE_STATIC)
    add_library(apyre STATIC ${APYRE_SOURCES})
    target_compile_definitions(apyre PUBLIC APYRE_STATIC_BUILD)
    set(TO_EXPORT ${TO_EXPORT} apyre sdl-build-options SDL2main SDL2-static PARENT_SCOPE)
else()
    add_library(apyre SHARED ${APYRE_SOURCES})
    target_compile_definitions(apyre PUBLIC APYRE_SHARED_BUILD)
    set(TO_EXPORT ${TO_EXPORT} apyre PARENT_SCOPE)
endif()

set_target_properties(apyre PROPERTIES PREFIX "")

target_compile_definitions(apyre
    PUBLIC
        APYRE_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
        APYRE_MINOR_VERSION=${PROJECT_VERSION_MINOR}
        APYRE_PATCH_VERSION=${PROJECT_VERSION_PATCH}
)

target_include_directories(apyre
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apyre
    PRIVATE
        SDL2::SDL2main
        SDL2::SDL2-static
    PUBLIC
        captal_foundation
)

if(WIN32)
    target_sources(apyre PRIVATE src/apyre/resources.rc)
    target_link_libraries(apyre PRIVATE dwmapi)
    target_compile_definitions(apyre PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

if(CAPTAL_BUILD_APYRE_EXAMPLES)
    add_executable(apyre_test "main.cpp")
    target_link_libraries(apyre_test PRIVATE apyre)
endif()

install(TARGETS apyre
        CONFIGURATIONS Debug
        RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../libs/debug"
        ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/../libs/debug"
        COMPONENT library)

install(TARGETS apyre
        CONFIGURATIONS Release
        RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../libs/release"
        ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/../libs/release"
        COMPONENT library)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/apyre
        DESTINATION ${PROJECT_SOURCE_DIR}/include
        FILES_MATCHING PATTERN *.hpp)
