cmake_minimum_required(VERSION 3.16)
project(raytracer)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)

add_library(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/thirdparty/stb_image_write.h
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/bitmap.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/bitmap.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/color.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/mat.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/math.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/ray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/vec.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/world.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib)

option(RAYTRACER_BUILD_GUI "Build GUI" ON)
if(RAYTRACER_BUILD_GUI)
    include(${CMAKE_CURRENT_SOURCE_DIR}/gui/cmake_scripts/Qt.cmake)

    add_executable(${PROJECT_NAME}_gui
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/res/resource.qrc
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/window.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/window.hpp)
    target_link_libraries(${PROJECT_NAME}_gui PUBLIC ${PROJECT_NAME})
    target_include_directories(${PROJECT_NAME}_gui PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/gui)

    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
    target_link_libraries(${PROJECT_NAME}_gui PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)
    target_include_directories(${PROJECT_NAME}_gui PRIVATE ${QT_INCLUDE} ${QT_INCLUDE}/QtCore ${QT_INCLUDE}/QtWidgets ${QT_INCLUDE}/QtGui)

    if(WIN32)
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            configure_file("${QT_BASEDIR}/bin/Qt6Cored.dll"    "${CMAKE_BINARY_DIR}/Qt6Cored.dll"    COPYONLY)
            configure_file("${QT_BASEDIR}/bin/Qt6Guid.dll"     "${CMAKE_BINARY_DIR}/Qt6Guid.dll"     COPYONLY)
            configure_file("${QT_BASEDIR}/bin/Qt6Widgetsd.dll" "${CMAKE_BINARY_DIR}/Qt6Widgetsd.dll" COPYONLY)

            configure_file("${QT_BASEDIR}/plugins/platforms/qwindowsd.dll" "${CMAKE_BINARY_DIR}/platforms/qwindowsd.dll" COPYONLY)

            configure_file("${QT_BASEDIR}/plugins/styles/qwindowsvistastyled.dll" "${CMAKE_BINARY_DIR}/styles/qwindowsvistastyled.dll" COPYONLY)
        else()
            configure_file("${QT_BASEDIR}/bin/Qt6Core.dll"    "${CMAKE_BINARY_DIR}/Qt6Core.dll"    COPYONLY)
            configure_file("${QT_BASEDIR}/bin/Qt6Gui.dll"     "${CMAKE_BINARY_DIR}/Qt6Gui.dll"     COPYONLY)
            configure_file("${QT_BASEDIR}/bin/Qt6Widgets.dll" "${CMAKE_BINARY_DIR}/Qt6Widgets.dll" COPYONLY)

            configure_file("${QT_BASEDIR}/plugins/platforms/qwindows.dll" "${CMAKE_BINARY_DIR}/platforms/qwindows.dll" COPYONLY)

            configure_file("${QT_BASEDIR}/plugins/styles/qwindowsvistastyle.dll" "${CMAKE_BINARY_DIR}/styles/qwindowsvistastyle.dll" COPYONLY)
        endif()
    endif()
endif()

option(RAYTRACER_BUILD_TESTS "Build Tests" ON)
if(RAYTRACER_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1)
    FetchContent_MakeAvailable(googletest)
    enable_testing()

    add_executable(${PROJECT_NAME}_test
            ${CMAKE_CURRENT_SOURCE_DIR}/test/bitmap.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/color.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mat.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/math.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/ray.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/vec.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/world.cpp)
    target_link_libraries(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME} gtest_main)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_test)
endif()
