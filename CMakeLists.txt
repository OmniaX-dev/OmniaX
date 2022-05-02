cmake_minimum_required(VERSION 3.18)
set(PROJ_NAME omniax-test)
project(${PROJ_NAME} LANGUAGES C CXX)
set(MAJOR_VER 0)
set(MINOR_VER 1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

file (STRINGS "./tools/build.nr" BUILD_NUMBER)

message("** Building ${PROJ_NAME} ${MAJOR_VER}.${MINOR_VER}.${BUILD_NUMBER}")

list(APPEND include_dirs
	${CMAKE_CURRENT_LIST_DIR}/src
	${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/GLAD
)

list(APPEND sources
    # Third Party
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/GLAD/glad.c
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/vendor/imgui/imgui_widgets.cpp

    # Utils
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/BaseObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/SineWave.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/QuadTree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/PathFinder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/Utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/Logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/StringEditor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/OutputHandlers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/Geometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/utils/DataFile.cpp

    # Graphics
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/graphics/Color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/graphics/Texture.cpp
    
    # Core
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/core/Window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/core/Errors.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/core/Shader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/core/GLBuffers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/core/Renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/core/OrthoCamera.cpp

    # Runtime
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/runtime/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/omniax/runtime/Signals.cpp
)

list(APPEND test_sources
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TestApp.cpp
)

add_library(omniax SHARED ${sources})
add_executable(omniax-test ${test_sources})
#add_compile_options(-O3 -m32 -MMD -MP -Wall -ggdb)
add_compile_options(-m32 -MMD -MP -Wall -ggdb)
target_compile_definitions(omniax PUBLIC __DEBUG__ __APP_NAME__=\"${PROJ_NAME}\" __BUILD_NUMBER__=${BUILD_NUMBER} __MAJOR_VER__=${MAJOR_VER} __MINOR_VER__=${MINOR_VER})
target_include_directories(omniax-test PUBLIC ${include_dirs})
target_include_directories(omniax PUBLIC ${include_dirs})
if (UNIX)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")
  target_link_libraries(omniax GL)
	target_link_libraries(omniax glfw)
	target_link_libraries(omniax X11)
	target_link_libraries(omniax GLU)
	target_link_libraries(omniax glut)
endif (UNIX)

target_link_libraries(omniax-test PUBLIC omniax)

add_custom_command(TARGET omniax-test POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/extra/ $<TARGET_FILE_DIR:omniax-test>
VERBATIM
)

if (WIN32)
	add_custom_command ( OUTPUT ./tools/build.nr
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/inc_bnr.exe
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/
	)
endif (WIN32)
if (UNIX)
	add_custom_command ( OUTPUT ./tools/build.nr
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/inc_bnr
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/
	)
endif (UNIX)

add_custom_target(
  IncBnr ALL
  DEPENDS ./tools/build.nr
)

set(CMAKE_INSTALL_MESSAGE NEVER)
install(TARGETS omniax-test DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Build/bin)
install(TARGETS omniax DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Build/bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extra/licences DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Build)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extra/res DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Build/bin)

add_custom_target( FinalMessage ALL
    ${CMAKE_COMMAND} -E cmake_echo_color --cyan "[100%] Built ${PROJ_NAME} ${MAJOR_VER}.${MINOR_VER}.${BUILD_NUMBER}"
    COMMENT "Final Message" )
add_dependencies( FinalMessage omniax-test )