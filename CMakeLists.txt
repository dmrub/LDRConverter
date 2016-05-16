cmake_minimum_required(VERSION 2.8)

project(LDConv)

# User options

#list(APPEND CMAKE_MODULE_PATH
#  "${PROJECT_SOURCE_DIR}/cmake"
# "${PROJECT_SOURCE_DIR}/cmake/modules")

# Detect compiler

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(C_IS_GNU   1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(C_IS_GNU   1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  set(C_IS_INTEL 1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(C_IS_MSVC  1)
endif()

# Enable C99 mode
#if(C_IS_GNU)
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
#endif()

#if(C_IS_INTEL)
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -restrict")
#endif()

# Enable C++11 mode
# include(CheckCXXCompilerFlag)
# CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
# CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
# if(COMPILER_SUPPORTS_CXX11)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# elseif(COMPILER_SUPPORTS_CXX0X)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# else()
#   message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
# endif()


if (UNIX AND NOT WIN32 AND NOT APPLE)
  add_definitions(-D_OSMESA)
endif()

if (UNIX)
  add_definitions(-DPOSIX)
endif()

add_definitions(-DEXPORT_3DS)

# Find packages
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
#find_package(Lib3ds REQUIRED)

include_directories(${PNG_INCLUDE_DIR} ${JPEG_INCLUDE_DIR})

include_directories(
  src
  3rdParty/tinyxml
  3rdParty/lib3ds/src
  ${CMAKE_CURRENT_BINARY_DIR})

# Build lib3ds

#ADD_SUBDIRECTORY(3rdParty/lib3ds/src)
ADD_LIBRARY(3ds
    3rdParty/lib3ds/src/lib3ds.h
    3rdParty/lib3ds/src/lib3ds_impl.h
    3rdParty/lib3ds/src/lib3ds_atmosphere.c
    3rdParty/lib3ds/src/lib3ds_background.c
    3rdParty/lib3ds/src/lib3ds_camera.c
    3rdParty/lib3ds/src/lib3ds_chunk.c
    3rdParty/lib3ds/src/lib3ds_chunktable.c
    3rdParty/lib3ds/src/lib3ds_file.c
    3rdParty/lib3ds/src/lib3ds_io.c
    3rdParty/lib3ds/src/lib3ds_light.c
    3rdParty/lib3ds/src/lib3ds_material.c
    3rdParty/lib3ds/src/lib3ds_math.c
    3rdParty/lib3ds/src/lib3ds_matrix.c
    3rdParty/lib3ds/src/lib3ds_mesh.c
    3rdParty/lib3ds/src/lib3ds_node.c
    3rdParty/lib3ds/src/lib3ds_quat.c
    3rdParty/lib3ds/src/lib3ds_shadow.c
    3rdParty/lib3ds/src/lib3ds_track.c
    3rdParty/lib3ds/src/lib3ds_util.c
    3rdParty/lib3ds/src/lib3ds_vector.c
    3rdParty/lib3ds/src/lib3ds_viewport.c
    3rdParty/lib3ds/src/lib3ds.rc)

add_library(tinyxml
  # TinyXml
  3rdParty/tinyxml/tinyxml.cpp
  3rdParty/tinyxml/tinyxmlerror.cpp
  3rdParty/tinyxml/tinyxmlparser.cpp)

# TCFoundation
add_library(TCFoundation
  src/TCFoundation//ConvertUTF.c
  src/TCFoundation/TCBmpImageFormat.cpp
  src/TCFoundation/TCNetworkClient.cpp
  src/TCFoundation/TCImageFormat.cpp
  src/TCFoundation/TCNetwork.cpp
  src/TCFoundation/TCObjectTree.cpp
  src/TCFoundation/TCImage.cpp
  src/TCFoundation/TCSortedStringArray.cpp
  src/TCFoundation/TCObjectArray.cpp
  src/TCFoundation/TCAlertManager.cpp
  src/TCFoundation/TCPngImageFormat.cpp
  src/TCFoundation/TCJpegImageFormat.cpp
  src/TCFoundation/TCProgressAlert.cpp
  src/TCFoundation/TCAlert.cpp
  src/TCFoundation/TCJpegOptions.cpp
  src/TCFoundation/TCStringObjectTree.cpp
  src/TCFoundation/TCVector.cpp
  src/TCFoundation/TCObject.cpp
  src/TCFoundation/TCArray.cpp
  src/TCFoundation/TCWebClient.cpp
  src/TCFoundation/TCDictionary.cpp
  src/TCFoundation/TCStringArray.cpp
  src/TCFoundation/TCUnzip.cpp
  src/TCFoundation/TCImageOptions.cpp
  src/TCFoundation/TCLocalStrings.cpp
  src/TCFoundation/TCUserDefaults.cpp
  src/TCFoundation/mystring.cpp
  src/TCFoundation/TCAutoreleasePool.cpp
  src/TCFoundation/TCBinaryTree.cpp
  src/TCFoundation/TCBinaryObjectTree.cpp)

# LDLoader
add_library(LDLoader
  src/LDLoader/LDrawIni.c
  src/LDLoader/LDLModelLine.cpp
  src/LDLoader/LDLCamera.cpp
  src/LDLoader/LDLActionLine.cpp
  src/LDLoader/LDLFindFileAlert.cpp
  src/LDLoader/LDLMainModel.cpp
  src/LDLoader/LDLPrimitiveCheck.cpp
  src/LDLoader/LDLTriangleLine.cpp
  src/LDLoader/LDLPalette.cpp
  src/LDLoader/LDLUnknownLine.cpp
  src/LDLoader/LDLConditionalLineLine.cpp
  src/LDLoader/LDLFileLine.cpp
  src/LDLoader/LDLCommentLine.cpp
  src/LDLoader/LDLModel.cpp
  src/LDLoader/LDLLineLine.cpp
  src/LDLoader/LDLError.cpp
  src/LDLoader/LDLShapeLine.cpp
  src/LDLoader/LDLQuadLine.cpp
  src/LDLoader/LDLAutoCamera.cpp
  src/LDLoader/LDLFacing.cpp
  src/LDLoader/LDLEmptyLine.cpp)

# LDExporter
add_library(LDExporter
  src/LDExporter/LD3dsExporter.cpp
  #src/LDExporter/LDPovExporter.cpp
  src/LDExporter/LDExporter.cpp
  #src/LDExporter/LDLdrExporter.cpp
  #src/LDExporter/LDStlExporter.cpp
  src/LDExporter/LDExporterSetting.cpp)

add_executable(Headerize src/tools/Headerize.cpp ${LIBHEADERS})
target_link_libraries(Headerize TCFoundation)

add_custom_command(
  OUTPUT LDViewMessages.h
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/LDViewMessages.ini LDViewMessages.ini
  COMMAND Headerize LDViewMessages.ini
  DEPENDS src/LDViewMessages.ini
)

add_executable(UnMirrorStuds
  src/UnMirrorStuds/UnMirrorStuds.cpp
  src/UnMirrorStuds/UnMirrorStudsMain.cpp)
target_link_libraries(UnMirrorStuds LDLoader TCFoundation ${PNG_LIBRARY} ${JPEG_LIBRARY})

add_executable(ldrconvert
  src/Converter/Converter.cpp
  src/Converter/ConverterMain.cpp)
target_link_libraries(ldrconvert LDExporter LDLoader TCFoundation 3ds ${PNG_LIBRARY} ${JPEG_LIBRARY})
