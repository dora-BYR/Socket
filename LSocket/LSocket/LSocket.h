//
// Created by 龙辉 on 16/4/5.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LSOCKET_LSOCKET_H
#define LSOCKET_LSOCKET_H

#include "define_l.h"

#ifdef __WIN32
    #include <WinSock2.h>
    #include <Ws2tcpip.h>
    typedef SOCKET SOCKET_TYPE;
    typedef int socklen_t;
    #define L_EWOULDBLOCK WSAEWOULDBLOCK
    #define L_EAGAIN WSAEWOULDBLOCK
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    typedef int SOCKET_TYPE;
    #define L_INVALID_SOCKET -1
    #define L_EWOULDBLOCK EWOULDBLOCK
    #define L_EAGAIN EAGAIN
#endif

#include <string>

#define FAILED_RET_VALUE            -1
#define READ_BUFFER_SIZE            1024*1024
#define DEFAULT_BACK_LOG            20

NS_LONG_BEGIN
    // socket协议信息对象
    class LSocketTypeInfo {
    public:
        // 默认为IPv4-AF_INET
        LSocketTypeInfo(int nFamily = AF_INET, int nType = SOCK_STREAM, int nProtocol = IPPROTO_TCP);
        virtual ~LSocketTypeInfo();

        void setFamily(int nFamily);
        int getFamily();
        void setType(int nType);
        int getType();
        void setProtocol(int nProtocol);
        int getProtocol();

    private:
        int m_nFamily;
        int m_nType;
        int m_nProtocol;
    };

    // 地址对象
    class LAddressInfo {
    public:
        LAddressInfo();
        LAddressInfo(std::string szIP, int nPort);
        virtual ~LAddressInfo();

        void setIP(std::string szIP);
        std::string getIP();
        void setPort(int nPort);
        int getPort();

    private:
        std::string m_szIP;
        int m_nPort;
    };

    // 套接字
    class LSocket {
    public:
        LSocket();
        LSocket(LSocketTypeInfo * pTypeInfo);
        virtual ~LSocket();

        // select模型的文件描述符集合类型
        enum {
            FD_READ = 0x01,
            FD_WRITE = 0X10,
            FD_ERROR = 0x100,
        };

        // 发送数据
        int sendData(SOCKET_TYPE socket, const char * pData, int nFlag = NULL);

        // 接收数据
        int recvData(SOCKET_TYPE socket, char *pBuf, int nFlag = NULL);


        // 设置非阻塞模式 - I/O非阻塞模型
        int setNotBlockMode(SOCKET_TYPE socket);

        // 设置阻塞模式 - I/O复用模型 (select/poll/epoll)
        // @mode default select
        // @param nTimeout 毫秒
        //       (select-blocked will be enabled when nTimeout < 0, while non-block when nTimeout == 0)
        int selectMode(SOCKET_TYPE socket, int nFdMode = FD_READ, int nTimeout = NULL);

        // 关闭套接字
        int close(SOCKET_TYPE socket);

    public:
        void setSocketTypeInfo(LSocketTypeInfo * pTypeInfo);
        LSocketTypeInfo * getSocketTypeInfo();
        SOCKET_TYPE getSocket();
        void setAddressInfo(LAddressInfo * pInfo);
        LAddressInfo * getAddressInfo();

    private:
        SOCKET_TYPE m_stSocket;
        LSocketTypeInfo * m_pSctTpInfo;
        LAddressInfo * m_pAddressInfo;
    };

NS_LONG_END

#endif //LSOCKET_LSOCKET_H
