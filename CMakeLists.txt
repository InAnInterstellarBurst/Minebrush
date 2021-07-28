# CMakeLists.txt is part of this program. Copyright Connor M
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.16)

project(Minebrush)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(BUILD_SHARED_LIBS Off)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Just to be sure
add_compile_definitions($<$<CONFIG:RELEASE>:NDEBUG>)

set(wxUSE_STL On)
add_subdirectory(dep/spdlog)
add_subdirectory(dep/wxwidgets)
set_property(TARGET spdlog PROPERTY FOLDER "Dependencies")

add_subdirectory(src)

if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    if(BUILD_TESTAPP)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TestApp)
        set_property(TARGET TestApp PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:TestApp>)
    endif()
endif()
