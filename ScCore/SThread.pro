#-------------------------------------------------
#
# Project created by QtCreator 2013-01-10T14:38:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = SThread
TEMPLATE = lib

DEFINES += OS_LINUX STHREAD_EXPORTS

INCLUDEPATH += \
    ./include \



unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}



HEADERS += \
    include/WaitCondition.h \
    include/Timer.h \
    include/Thread.h \
    include/SThread.h \
    include/Semaphore.h \
    include/Lock.h  \
    include/Mutex.h

win32 {
INCLUDEPATH += ./win32
LIBS += \
        -lUser32

HEADERS += \
    stdafx.h \
    win32/WaitCondition.h \
    win32/ThreadData.h \
    win32/MutexData.h \
     dllMain.h

SOURCES += \
    win32/WaitCondition.cpp \
    win32/ThreadData.cpp \
    win32/Thread.cpp \
    win32/Semaphore.cpp \
    win32/MutexData.cpp \
    win32/Mutex.cpp \
    win32/Lock.cpp \
    dllMain.cpp \
    StdAfx.cpp
}
unix {

INCLUDEPATH += ./posix

HEADERS += \
    posix/ThreadData.h \
    posix/MutexData.h \
	posix/SemaphoreData.h

SOURCES += \
    posix/ThreadData.cpp \
    posix/Thread.cpp \
    posix/Semaphore.cpp \
	posix/SemaphoreData.cpp \
    posix/MutexData.cpp \
    posix/Mutex.cpp
}




