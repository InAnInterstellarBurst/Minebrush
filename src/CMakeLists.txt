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

include (${CMAKE_MODULE_PATH}/targetcommon.cmake)

file(GLOB SRCS
    *.cpp *.hpp
)

add_library(Minebrush STATIC ${SRCS})
target_precompile_headers(Minebrush PRIVATE pch.hpp)
configure_compile_flags(Minebrush)
configure_common_target_properties(Minebrush)

target_link_libraries(Minebrush
    PUBLIC
        spdlog::spdlog
        wx::core wx::base
        "$<$<PLATFORM_ID:LINUX>:m dl>"
)

target_include_directories(Minebrush SYSTEM
    PUBLIC
        ${CMAKE_SOURCE_DIR}/dep
    INTERFACE
        ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(Minebrush
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
