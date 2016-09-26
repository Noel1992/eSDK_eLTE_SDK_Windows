##############################################################################
#Copyright 2012,TD-Tech. Co., Ltd.
#Filename: Itf - CMakeList.txt 
#Author:	WuGang
#Created: 2012/07/21 14:50
#Description: 
##############################################################################

##############################################################################
#                         source files                                       #
##############################################################################
ADD_DEFINITIONS(-DDLL_PB_MSG_EXPORTS -DMQ_EXPORT)

set(PROTO_FILE_NAME "platform_msg.proto")
set(PROTO_FILE_FULL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_FILE_NAME}")
set(CPP_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

string(REGEX REPLACE "(.*)\\..*$" "\\1.pb.cpp" targetCppFile ${PROTO_FILE_NAME})	
string(REGEX REPLACE "(.*)\\..*$" "\\1.pb.cc" intermCcFile ${PROTO_FILE_NAME})	
string(REGEX REPLACE "(.*)\\..*$" "\\1.pb.h" targetHFile ${PROTO_FILE_NAME})	

EXEC_PROGRAM(${Protobuf_EXECUTABLE_PROTOC} ARGS "-I=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=dllexport_decl=PB_MSG_IMPORT_EXPORT:${CPP_OUT_DIR} ${PROTO_FILE_FULL_DIR}")
EXEC_PROGRAM(${CMAKE_COMMAND} ARGS "-E copy ${CMAKE_CURRENT_BINARY_DIR}/${intermCcFile}	${CMAKE_CURRENT_BINARY_DIR}/${targetCppFile}")
EXEC_PROGRAM(${CMAKE_COMMAND} ARGS "-E remove -f ${CMAKE_CURRENT_BINARY_DIR}/${intermCcFile}")

set(PROTO_FILE_NAME "sca_msg.proto")
set(PROTO_FILE_FULL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_FILE_NAME}")
set(CPP_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

string(REGEX REPLACE "(.*)\\..*$" "\\1.pb.cpp" targetCppFile ${PROTO_FILE_NAME})	
string(REGEX REPLACE "(.*)\\..*$" "\\1.pb.cc" intermCcFile ${PROTO_FILE_NAME})	
string(REGEX REPLACE "(.*)\\..*$" "\\1.pb.h" targetHFile ${PROTO_FILE_NAME})	

EXEC_PROGRAM(${Protobuf_EXECUTABLE_PROTOC} ARGS "-I=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=dllexport_decl=PB_MSG_IMPORT_EXPORT:${CPP_OUT_DIR} ${PROTO_FILE_FULL_DIR}")
EXEC_PROGRAM(${CMAKE_COMMAND} ARGS "-E copy ${CMAKE_CURRENT_BINARY_DIR}/${intermCcFile}	${CMAKE_CURRENT_BINARY_DIR}/${targetCppFile}")
EXEC_PROGRAM(${CMAKE_COMMAND} ARGS "-E remove -f ${CMAKE_CURRENT_BINARY_DIR}/${intermCcFile}")


##############################################################################
#                          plt_pb_msg                                
##############################################################################

if (WIN32)
ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
endif()

#header files
set(PLT_MSG_HEADER_FILES
	${UBP_INTERFACE_PATH}/platform/modules/mq/message_factory.hpp
    ${UBP_INTERFACE_BINARY_PATH}/platform/platform_msg.pb.h
	${UBP_INTERFACE_BINARY_PATH}/platform/sca_msg.pb.h)
#source files
set(PLT_MSG_SOURCE_FILES
	${SOURCE_PATH}/platform/modules/mq/message_factory.cpp
    ${UBP_INTERFACE_PATH}/platform/platform_msg.proto
	${UBP_INTERFACE_PATH}/platform/sca_msg.proto
    ${UBP_INTERFACE_BINARY_PATH}/platform/platform_msg.pb.cpp
	${UBP_INTERFACE_BINARY_PATH}/platform/sca_msg.pb.cpp)

if(MSVC_IDE)
    source_group("Resource Files" REGULAR_EXPRESSION ".*\\.proto$")	
endif()

set(PLT_MSG_THIRD_PARTY_LIBS ${ACE_LIBRARY}
                            ${PROTOBUF_LIBRARY}
							${LOG4CXX_LIBRARY})

set(PLT_MSG_CUSTOM_LIBS )

include_directories(BEFORE  ${ACE_INCLUDE_DIRS}
                            ${PROTOBUF_INCLUDE_DIRS}
                            ${LOG4CXX_INCLUDE_DIRS} 
							${UBP_INTERFACE_PATH})

add_library(ubp_plt_pb_msg SHARED ${PLT_MSG_SOURCE_FILES} ${PLT_MSG_HEADER_FILES})

target_link_libraries(ubp_plt_pb_msg ${PLT_MSG_THIRD_PARTY_LIBS})

install(TARGETS ubp_plt_pb_msg DESTINATION lib)