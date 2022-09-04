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
#	ctest -j<#parallel cores>


###############################################
## Definitions
###############################################
set(                        DESCRIPTION                         "Creed & Bear assessment")
project(					test								VERSION 1.1
                            DESCRIPTION							${DESCRIPTION}
                            LANGUAGES							CXX)
set(						CMAKE_CXX_STANDARD					17)
set(						CMAKE_CXX							g++)
set(						CMAKE_CXX_STANDARD_REQUIRED			ON)
set(						CMAKE_CXX_EXTENSIONS				OFF)

enable_testing()

set(						DIR_SRC                             src)
set(						DIR_HEADER                          header)

set(						EXTENSION_SRC						.cpp)
set(						EXTENSION_HEADER					.h)

set(						DIR_APP								${CMAKE_SOURCE_DIR}/app)
set(						DIR_TEST							${CMAKE_SOURCE_DIR}/test)
set(						DIR_CODE							${CMAKE_SOURCE_DIR}/code)
set(						DIR_RESOURCE						${CMAKE_SOURCE_DIR}/resource)

set(						EXEC_LIST        					main_ordersManager)

#set(						LIB_LIST_TEST_UNIT_TEST				${LIB_TOOLS_CC} ${LIB_CUSTOM_ALLOCATOR} ${LIB_HTTP_SERVER} ${LIB_JOURNAL} ${LIB_HTTP_SERVER_CORE})

#set(						SRC_TEST_MAIN						test_main${EXTENSION_SRC})


set(						CMAKE_CXX_FLAGS						"-O3 -Wall -Werror -Wextra -g")
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
# List the header sub directories relative to lib
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


#=======================================================
# Compile custom code
#=======================================================
FILE(GLOB libList RELATIVE ${DIR_CODE} ${DIR_CODE}/*)						# List the custom libraries to compile
SET(libCustomList "")
message(STATUS "\n\nCompile library: " ${lib})
HEADER_DIR_LIST(${DIR_CODE}/${DIR_HEADER})									#		Set the headers into the project

file(GLOB_RECURSE sourceFileList ${DIR_CODE}/${DIR_SRC}/*${EXTENSION_SRC})	#		Gather the source files: The file(GLOB...) allows for wildcard additions
message(STATUS "\tAdd source files")
foreach(sourceFile ${sourceFileList})
    message(STATUS "\t\t${sourceFile}")
    LIST(APPEND libCustomList ${sourceFile})
endforeach(sourceFile ${sourceFileList})

IF (sourceFileList)
#    add_library(AAAAA SHARED ${sourceFileList})					#		Generate the shared library from the sources
    #target_link_libraries(${lib} ${sourceFileList})
    #install(TARGETS LIB_CUSTOM DESTINATION /usr/lib)					#		Set the location for library installation -- i.e., /usr/lib in this case not really necessary in this example. Use "sudo make install" to apply
ENDIF()


#=======================================================
# Compile custom applications (no debug log)
#=======================================================
foreach(exec ${EXEC_LIST})

    SET(execName ${exec})
    message(STATUS "\n\nGenerate production executable \"${execName}\" (no logs, no routines for tests)")
    SET (execPath ${DIR_CODE}/${DIR_SRC}/${exec})
    add_executable(${execName} ${libCustomList} ${execPath})
    target_compile_options( ${execName} PUBLIC ${FLAG_PROD})

    SET(execName ${exec}_verbose)
    message(STATUS "\n\nGenerate verbose executable \"${execName}\" (print the logs of the code)")
    SET (execPath ${DIR_CODE}/${DIR_SRC}/${exec})
    add_executable(${execName} ${libCustomList} ${execPath})
    target_compile_options( ${execName} PUBLIC ${FLAG_VERBOSE})

    SET(execName ${exec}_test)
    message(STATUS "\n\nGenerate test executable \"${execName}\" (execute the routines for tests)")
    SET (execPath ${DIR_CODE}/${DIR_SRC}/${exec})
    add_executable(${execName} ${libCustomList} ${execPath})
    target_compile_options( ${execName} PUBLIC ${FLAG_TEST})

endforeach(exec ${EXEC_LIST})






