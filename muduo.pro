TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Timestamp.cc \
    EventLoop.cc \
    Epoller.cc \
    Channel.cc \
    TimeQueue.cc \
    Thread.cc \
    Acceptor.cc \
    Buffer.cc \
    EventLoopThread.cc \
    EventLoopThreadPool.cc \
    InetAddress.cc \
    Poller.cc \
    Socket.cc \
    SocketsOps.cc \
    TcpConnection.cc \
    TcpServer.cc

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Atomic.h \
    CurrentThread.h \
    Thread.h \
    Timer.h \
    Timestamp.h \
    Epoller.h \
    EventLoop.h \
    Poller.h \
    TimeQueue.h \
    Channel.h \
    Inetaddr.h \
    Logging.h \
    Acceptor.h \
    BlockingQueue.h \
    BoundBlockingQueue.h \
    Buffer.h \
    Callbacks.h \
    Condition.h \
    CountDownLatch.h \
    EventLoopThread.h \
    EventLoopThreadPool.h \
    InetAddress.h \
    LogFile.h \
    Mutex.h \
    ProcessInfo.h \
    Singleton.h \
    Socket.h \
    SocketsOps.h \
    TcpConnection.h \
    TcpServer.h \
    ThreadLocal.h \
    Endian.h

LIBS += -lpthread

DISTFILES += \
    muduo.pro.user
