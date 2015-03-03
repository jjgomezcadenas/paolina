# -*- cmake -*-
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(PAOLINA)

MESSAGE("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
#Set compilier flags
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   SET(CMAKE_CXX_FLAGS "-Wall -g -O2 -std=c++11" )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux specific code
    SET(CMAKE_CXX_FLAGS "-Wall -g -O2 -std=c++0x" )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


SET(CMAKE_VERBOSE_MAKEFILE ON)
#SET(CMAKE_BUILD_TYPE ${CMAKE_CXX_FLAGS})

MESSAGE("CMAKE source dir: ${CMAKE_SOURCE_DIR}")
MESSAGE("CMAKE binary dir: ${CMAKE_BINARY_DIR}")

MESSAGE("CMAKE compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
MESSAGE("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Set where to look for external packages 
set (CMAKE_MODULE_PATH $ENV{CMODULES}) 

MESSAGE("CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

SET(CMAKE_INSTALL_PREFIX $ENV{INSTALL})


MESSAGE("CMAKE_INSTALL_PREFIX set to: ${CMAKE_INSTALL_PREFIX}")


# Look for the needed libraries

find_package(ROOT REQUIRED)

if(CMAKE_SYSTEM_NAME MATCHES Darwin)
  # To avoid to set ld_Library_path to locate the test libraries
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,@loader_path/.")
endif()


MESSAGE("ROOT include directories: ${ROOT_INCLUDE_DIR}")
MESSAGE("ROOT libraries: ${ROOT_LIBRARIES}")

MESSAGE("ROOT Found: ${ROOT_FOUND}")
MESSAGE("ROOT CINT: ${ROOTCINT_EXECUTABLE}")

include(RootNewMacros)
#Make symbolic links in paolina/ to all header files

EXECUTE_PROCESS(
  COMMAND mkdir -p paolina)


FILE(GLOB i_headers . src/*.h )

FOREACH(file ${i_headers})
  EXECUTE_PROCESS(
    COMMAND ln -s -f ${file} paolina)
ENDFOREACH(file)

set (PAOLINA_SRC ${CMAKE_SOURCE_DIR}/src)

# Directories that will be searched for headers during compilation
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${PAOLINA_SRC} 
          ${ROOT_INCLUDE_DIR})

#set everything needed for the root dictonary and create the dictionary


set(PAOLINA_DICTIONARY ${CMAKE_BINARY_DIR}/paolinaDict)  # set dictionary path

MESSAGE("Generate Paolina dictionary")

SET(PAOLINA_HEADERS ${PAOLINA_SRC}/Track.h ${PAOLINA_SRC}/Voxel.h ${PAOLINA_SRC}/Blob.h 
  ${PAOLINA_SRC}/TopoEvent.h ${PAOLINA_SRC}/BlobBuilder.h ${PAOLINA_SRC}/TrackBuilder.h
  ${PAOLINA_SRC}/VoxelBuilder.h)



set(IRENE_DICTIONARY ${CMAKE_BINARY_DIR}/ireneDict)  # set dictionary path
set(LINKDEF_FILE ${PAOLINA_SRC}/LinkDef.h)  # set dictionary path



MESSAGE("Generate Paolina dictionary")


ROOT_GENERATE_DICTIONARY(${PAOLINA_DICTIONARY} ${PAOLINA_HEADERS} MODULE paolina LINKDEF ${LINKDEF_FILE})

#---------------------------------------------------------------------------------------------------
#---ROOT_GENERATE_DICTIONARY( dictionary headerfiles MODULE module DEPENDENCIES dep1 dep2
#                                                    STAGE1 LINKDEF linkdef OPTIONS opt1 opt2 ...)
#---------------------------------------------------------------------------------------------------


FILE(GLOB paolina_src ${PAOLINA_SRC}/*.cc)


# # # # Finally, create it!

MESSAGE("Creating Paolina library")

ROOT_LINKER_LIBRARY(paolina ${paolina_src} ${PAOLINA_DICTIONARY} LIBRARIES ${ROOT_LIBRARIES})

#
INSTALL(TARGETS paolina DESTINATION lib)
INSTALL(FILES ${PAOLINA_HEADERS} DESTINATION include/paolina)
INSTALL(FILES ${CMAKE_BINARY_DIR}/libpaolina.rootmap DESTINATION lib)
INSTALL(FILES ${CMAKE_BINARY_DIR}/libpaolina_rdict.pcm DESTINATION lib)



#Make symbolic links in paolina/ to all header files


# set(PAOLINA_DICTIONARY ${CMAKE_BINARY_DIR}/paolinaDict.cxx)  # set dictionary path

# MESSAGE("Generate Paolina dictionary")

# SET(PAOLINA_HEADERS ${PAOLINA_SRC}/Track.h ${PAOLINA_SRC}/Voxel.h ${PAOLINA_SRC}/Blob.h 
#   ${PAOLINA_SRC}/TopoEvent.h ${PAOLINA_SRC}/BlobBuilder.h ${PAOLINA_SRC}/TrackBuilder.h
#   ${PAOLINA_SRC}/VoxelBuilder.h)


# ROOT_GENERATE_DICTIONARY(
#            "${PAOLINA_DICTIONARY}"            # path to dictionary to generate
#            "${PAOLINA_SRC}"  # list of includes
#            "${PAOLINA_HEADERS}" 
#            "${PAOLINA_SRC}/LinkDef.h"   # ROOT linkDef file
#          )

#  FILE(GLOB paolina_src . src/*.cc)


# # # # # Finally, create it!

# MESSAGE("Creating Paolina library")

# ADD_LIBRARY(paolina SHARED ${paolina_src} ${PAOLINA_DICTIONARY})


# SET_TARGET_PROPERTIES(paolina PROPERTIES VERSION 1.1.0 SOVERSION 1)
# TARGET_LINK_LIBRARIES(paolina ${ROOT_LIBRARIES})



# # Installation destinations (just for the lib, not the python module)
# INSTALL(TARGETS alex LIBRARY DESTINATION lib)
# INSTALL(FILES ${alex_headers} DESTINATION include/alex)

# # Get ready for creating a package
# INCLUDE(CPack)
