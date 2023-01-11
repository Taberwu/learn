CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
project(sp_dr)

#set include files: *.h, *.hpp, *.hh in this project
FILE(GLOB_RECURSE PROJECT_HEADERS  "include/*.h" "../message/*.h")
#set source files:*.cpp, *.c, *.cc in this project
FILE(GLOB_RECURSE PROJECT_SOURCES  "src/*.cpp" "../message/*.cc")
#set library files: *.lib in this project
if(USE_STATIC_COMPILE)
    if (USE_CC_QNX)
        FILE(GLOB_RECURSE PROJECT_LIBRARIES
        "../../bin_aarch64_qnx/libWorldModel_s.a"
        "../../bin_aarch64_qnx/libNode_s.a"
        "../../bin_aarch64_qnx/libBasicIpc_s.a"
        "../../bin_aarch64_qnx/libCommon_s.a"
        )
    elseif (USE_CC_PEGASUS)
        FILE(GLOB_RECURSE PROJECT_LIBRARIES
        "../../bin_aarch64_pegasus/libWorldModel_s.a"
        "../../bin_aarch64_pegasus/libNode_s.a"
        "../../bin_aarch64_pegasus/libBasicIpc_s.a"
        "../../bin_aarch64_pegasus/libCommon_s.a"
        )
    else()
        FILE(GLOB_RECURSE PROJECT_LIBRARIES
        "../../bin/libWorldModel_s.a"
        "../../bin/libNode_s.a"
        "../../bin/libBasicIpc_s.a"
        "../../bin/libCommon_s.a"
        )
    endif()

    add_library(${PROJECT_NAME}_s STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}_s  ${PROJECT_LIBRARIES_KOTEI} ${PROJECT_LIBRARIES}  ${KOTEI_LIBRARIES} -lc protobuf-lite lcm -lzmq)

else()
    if (USE_CC_QNX)
        FILE(GLOB_RECURSE PROJECT_LIBRARIES
        "../../bin_aarch64_qnx/libWorldModel.so"
        "../../bin_aarch64_qnx/libNode.so"
        "../../bin_aarch64_qnx/libBasicIpc.so"
        "../../bin_aarch64_qnx/libCommon.so"
        )
    elseif (USE_CC_PEGASUS)
        FILE(GLOB_RECURSE PROJECT_LIBRARIES
        "../../bin_aarch64_pegasus/libWorldModel.so"
        "../../bin_aarch64_pegasus/libNode.so"
        "../../bin_aarch64_pegasus/libBasicIpc.so"
        "../../bin_aarch64_pegasus/libCommon.so"
        )
    else()
        FILE(GLOB_RECURSE PROJECT_LIBRARIES
        "../../bin/libWorldModel.so"
        "../../bin/libNode.so"
        "../../bin/libBasicIpc.so"
        "../../bin/libCommon.so"
        )
    endif()

    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
    target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES} protobuf-lite lcm -lzmq )
endif()
#add_executable(app_${PROJECT_NAME} "src/main.cc" ${PROJECT_SOURCES} ${PROJECT_HEADERS})
#target_link_libraries(app_${PROJECT_NAME} ${PROJECT_LIBRARIES} protobuf-lite lcm pthread)
