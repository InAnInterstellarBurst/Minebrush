# shader.cmake is part of this program. Copyright Connor M
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

function(add_shader TARGET SHADER TYPE)
    find_program(GLSLC glslangValidator REQUIRED)

    set(src ${CMAKE_SOURCE_DIR}/src/testapp/res/shader/${SHADER})
    set(opt $<TARGET_FILE_DIR:${TARGET}>/res/shader/${SHADER}.spv)

    get_filename_component(optdir ${opt} DIRECTORY)
    file(MAKE_DIRECTORY ${optdir})

    add_custom_command(
           OUTPUT ${opt}
           COMMAND ${GLSLC} -S ${TYPE} -V -o ${opt} ${src}
           DEPENDS ${src}
           IMPLICIT_DEPENDS CXX ${src}
           VERBATIM)

    set_source_files_properties(${opt} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${opt})
    source_group(res/shader/bin FILES ${opt})
endfunction()
