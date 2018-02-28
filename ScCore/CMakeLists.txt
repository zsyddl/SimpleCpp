PROJECT(SThread)
set (SThread_headers
	${PROJECT_SOURCE_DIR}/include/WaitCondition.h
    ${PROJECT_SOURCE_DIR}/include/Timer.h
    ${PROJECT_SOURCE_DIR}/include/Thread.h
    ${PROJECT_SOURCE_DIR}/include/SThread.h
    ${PROJECT_SOURCE_DIR}/include/Semaphore.h
    ${PROJECT_SOURCE_DIR}/include/Lock.h
    ${PROJECT_SOURCE_DIR}/include/Mutex.h
    ${PROJECT_SOURCE_DIR}/include/Event.h
	${PROJECT_SOURCE_DIR}/posix/EventData.h
    ${PROJECT_SOURCE_DIR}/posix/ThreadData.h
    ${PROJECT_SOURCE_DIR}/posix/MutexData.h
)

set (SThread_sources
	${PROJECT_SOURCE_DIR}/posix/ThreadData.cpp
    ${PROJECT_SOURCE_DIR}/posix/Thread.cpp
    ${PROJECT_SOURCE_DIR}/posix/MutexData.cpp
    ${PROJECT_SOURCE_DIR}/posix/Mutex.cpp
    ${PROJECT_SOURCE_DIR}/posix/Event.cpp
    ${PROJECT_SOURCE_DIR}/posix/EventData.cpp
)

#LINK_DIRECTORIES()#3rdparty链接库目录

ADD_LIBRARY(SThread SHARED ${SThread_sources}) #生成动态库
#TARGET_LINK_LIBRARIES(ExosipPlus ${PROJECT_SOURCE_DIR}/3rdparty/exosip2/lib/libeXosip2.so)#包含3rdparty动态库
#TARGET_LINK_LIBRARIES(ExosipPlus ${PROJECT_SOURCE_DIR}/3rdparty/sthread/lib/libSThread.so)

include_directories( "${PROJECT_SOURCE_DIR}/include" )#包含头文件
include_directories( "${PROJECT_SOURCE_DIR}/posix" )#包含头文件2
include_directories( "${PROJECT_SOURCE_DIR}" )#包含头文件3
ADD_DEFINITIONS(-DOS_LINUX) #OS宏
set_target_properties(SThread PROPERTIES LINKER_LANGUAGE CXX)