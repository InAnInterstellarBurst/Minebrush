# targetcommon.cmake is part of this program. Copyright Connor M
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

function(configure_common_target_properties target)
    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    )

    set_target_properties(${target} PROPERTIES CXX_STANDARD 20)
    set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
    endif()
endfunction()

function(configure_compile_flags target)
    if(MSVC)
        target_compile_definitions(${target} PRIVATE NOMINMAX)
        add_compile_options(${target} PRIVATE
            # If you ever wonder why I shit on msvcc, just look at these wonderfully descriptive names
            /permissive-
            /W4 /w14640 /w14242 /w14254 /w14263 /w14265
            /w14287 /we4289 /w14296 /w14311 /w14545 /w14546
            /w14547 /w14549 /w14555 /w14619 /w14640 /w14826
            /w14905 /w14906 /w14928
            /wd6031 # Oddly enough the one warning msvc turns on by default is fucking awful (ignored return)
            "$<$<CONFIG:RELEASE>:/fp:fast>"
        )
    else() # Assume clang/gcc
        target_link_options(${target} PRIVATE "$<$<CONFIG:DEBUG>:-fsanitize=address,undefined>")
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
            -Wold-style-cast -Wcast-align -Wunused -Wformat=2
            -Woverloaded-virtual -Wconversion -Wsign-conversion
            -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches
            -Wlogical-op -Wnull-dereference -Wdouble-promotion
            $<$<CONFIG:RELEASE>:-Ofast -s>
            "$<$<CONFIG:DEBUG>:-fsanitize=address,undefined>"
        )
    endif()
endfunction()
