# ****************************************************************************
# CUI
#
# The Advanced Framework for Simulation, Integration, and Modeling (AFSIM)
#
# The use, dissemination or disclosure of data in this file is subject to
# limitation or restriction. See accompanying README and LICENSE for details.
# ****************************************************************************
#
# WSF CMake Configuration
#
# This file creates the engine executable

project(engine)

# Set the shared library "paths" before calling "add_executable"
# The actual lib names can just be added, in the modules cmake file,
# using 'target_link_libraries'
if (COMMAND set_3rd_party_shared_paths)
   set_3rd_party_shared_paths()
endif()


if(MSVC)
   # 设置编译器选项以支持UTF-8
   add_compile_options(/utf-8)
endif()

set(THIRD_PARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party)

# 配置 Boost路径
set(Boost_ROOT ${THIRD_PARTY_PATH}/boost-1.87.0/msvc_x64/lib/cmake/Boost-1.87.0)

set(Boost_INCLUDE_DIR ${THIRD_PARTY_PATH}/boost-1.87.0/msvc_x64/include)
#set(Boost_LIBRARY_DIR ${THIRD_PARTY_PATH}/boost-1.87.0/msvc_x64/lib/debug)
set(Boost_LIBRARY_DIR ${THIRD_PARTY_PATH}/boost-1.87.0/msvc_x64/lib/release)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})


# wsf_p6dof
include_directories(${CMAKE_BINARY_DIR}/build_wsf_p6dof)
include_directories(${CMAKE_BINARY_DIR}/build_wsf_p6dof/p6dof)
include_directories(${THIRD_PARTY_PATH}/../wsf_plugins/wsf_p6dof/p6dof/source)
include_directories(${THIRD_PARTY_PATH}/../wsf_plugins/wsf_p6dof/source)

# wsf_six_dof
include_directories(${CMAKE_BINARY_DIR}/build_wsf_six_dof)
include_directories(${THIRD_PARTY_PATH}/../wsf_plugins/wsf_six_dof/source)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/application)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/commands)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/data)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/events)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/inputs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/signals)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/utils)


file(GLOB ENGINE_HPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/source/**/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/**/*/*.hpp
        )

file(GLOB ENGINE_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/source/**/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/**/*/*.cpp
        )

message(STATUS "PY ENGINE_HPP_SRCS: ${ENGINE_HPP_SRCS}")
message(STATUS "PY ENGINE_CXX_SRCS: ${ENGINE_CXX_SRCS}")

wsf_project_template(${PROJECT_NAME} EXECUTABLE
HDRS
   ${ENGINE_HPP_SRCS}
   ${WSF_EXTENSION_HEADER_DIR}/wsf_extensions.hpp
SRCS
   ${ENGINE_CXX_SRCS}
   source/engine.cpp
NOTICES
   README.md
)

# 添加 wsf_p6dof 支持
target_link_libraries(${PROJECT_NAME} wsf_p6dof)
target_link_libraries(${PROJECT_NAME} wsf_six_dof)


# *****************************************************************************
# Configure version header and resource files
include(GenerateVersionInfo)
generate_version_header(${PROJECT_NAME})
if(MSVC)
   generate_resource(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/source/engine_icon.ico")
endif()

link_wsf_optional_projects(${PROJECT_NAME})
link_sockets(${PROJECT_NAME})

# Install all WSF grammar to get "main" grammar file and any files defining
# commands that may be used by engine but not included in engine.ag
get_property(WSF_GRAMMAR_FILES GLOBAL PROPERTY WSF_GRAMMAR_FILES)
install(FILES ${WSF_GRAMMAR_FILES} DESTINATION bin/${PROJECT_NAME}_grammar COMPONENT Runtime)
