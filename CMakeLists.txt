cmake_minimum_required(VERSION 3.23)



###############################################
## Tricks
###############################################
# Generate the make files:
#	cd bin
#	cmake ../
# Run the unit tests
#	cd bin
#	cmake ../
#	make
#	ctest


###############################################
## Definitions
###############################################
set(                        DESCRIPTION                         "Creed & Bear assessment")
project(                    test                                VERSION 1.1
                            DESCRIPTION                         ${DESCRIPTION}
                            LANGUAGES                           CXX)
set(                        CMAKE_CXX_STANDARD                  17)
set(                        CMAKE_CXX                           g++)
set(                        CMAKE_CXX_STANDARD_REQUIRED         ON)
set(                        CMAKE_CXX_EXTENSIONS                OFF)

set(                        DIR_SRC                             src)
set(                        DIR_APP                             app)
set(                        DIR_HEADER                          header)
set(                        DIR_TEST                            test)

set(                        EXTENSION_SRC                       .cpp)
set(                        EXTENSION_HEADER                    .h)

set(                        DIR_APP                             ${CMAKE_SOURCE_DIR}/app)
set(                        DIR_TEST                            ${CMAKE_SOURCE_DIR}/test)
set(                        DIR_CODE                            ${CMAKE_SOURCE_DIR}/code)
set(                        DIR_RESOURCE                        ${CMAKE_SOURCE_DIR}/resource)

set(                        EXEC_LIST                           main_ordersManager)
set(                        SRC_TEST_MAIN                       test_main${EXTENSION_SRC})

set(                        CMAKE_CXX_FLAGS                     "-O3 -Wall -Werror -Wextra -g")
SET(                        FLAG_PROD                           "")
SET(                        FLAG_VERBOSE                        "-DLOG")
SET(                        FLAG_TEST                           "-DTEST")



#=======================================================
# Manage the config of the project
#=======================================================
message(STATUS "==================================")
message(STATUS "Configurations : " ${DESCRIPTION})
message(STATUS "==================================")
message(STATUS "C++ standard          : " ${CMAKE_CXX_STANDARD})
message(STATUS "C++ compiler          : " ${CMAKE_CXX})
message(STATUS "Flags                 : " ${CMAKE_CXX_FLAGS})


#=======================================================
# Include the headers of the project
#=======================================================
MACRO(HEADER_DIR_LIST lib)
    message(STATUS "\tAdd sub directory header: " ${lib})
    include_directories(${lib})

    FILE(GLOB children ${lib}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${child})
            HEADER_DIR_LIST(${child})
        ENDIF()
    ENDFOREACH()
ENDMACRO()

HEADER_DIR_LIST(${DIR_CODE}/${DIR_HEADER})


#=======================================================
# Compile custom code
#=======================================================
SET(libCustomList "")

message(STATUS "\n\nCompile custom libraries: ")
file(GLOB_RECURSE libFileList ${DIR_CODE}/${DIR_SRC}/*${EXTENSION_SRC})     # Gather the source files of the custom libraries
message(STATUS "\tAdd source files")
foreach(sourceFile ${libFileList})
    message(STATUS "\t\t${sourceFile}")
    LIST(APPEND libCustomList ${sourceFile})
endforeach(sourceFile ${libFileList})

message(STATUS "\n\nCompile tests:")
file(GLOB_RECURSE testFileList ${DIR_TEST}/*${EXTENSION_SRC})               # Gather the test source files
SET(testList "")
foreach(sourceFile ${testFileList})
    message(STATUS "\t\t${sourceFile}")
    LIST(APPEND testList ${sourceFile})
endforeach(sourceFile ${testFileList})


#=======================================================
# Compile custom applications
#=======================================================
message(STATUS "\n\nGenerate app executables:")
foreach(exec ${EXEC_LIST})

    SET(execName ${exec})
    message(STATUS "\t\t\"${execName}\" (no logs, no routines for tests)")
    SET (execSourceFile ${DIR_APP}/${exec}${EXTENSION_SRC})
    add_executable(${execName} ${libCustomList} ${execSourceFile})
    target_compile_options( ${execName} PUBLIC ${FLAG_PROD})

    SET(execName ${exec}_verbose)
    message(STATUS "\t\t\"${execName}\" (no logs, no routines for tests)")
    SET (execSourceFile ${DIR_APP}/${exec}${EXTENSION_SRC})
    add_executable(${execName} ${libCustomList} ${execSourceFile})
    target_compile_options( ${execName} PUBLIC ${FLAG_VERBOSE})

endforeach(exec ${EXEC_LIST})


#=======================================================
# Compile custom tests
#=======================================================
include(CTest)                                                              # Part of CMake, no extra frameworks / lib
enable_testing()


message(STATUS "\n\nGenerate CTest executables:")
foreach(execTest ${testList})
    message(STATUS "\t\t\"${execTest}\"")
    get_filename_component(testName ${execTest} NAME_WE)
    add_executable(${testName} ${libCustomList} ${execTest})
    target_compile_options( ${testName} PUBLIC ${FLAG_TEST})
   add_test(NAME ${testName} COMMAND ${testName})
endforeach(execTest ${testList})


