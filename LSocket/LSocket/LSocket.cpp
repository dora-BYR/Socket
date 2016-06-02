//
// Created by 龙辉 on 16/4/5.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LSocket.h"

#ifdef __WIN32
    #pragma comment(lib, "ws2_32")
#else
    #include <sys/time.h>
    #include <fcntl.h>
#endif

NS_LONG_BEGIN

    LSocketTypeInfo::LSocketTypeInfo(int nFamily, int nTtype, int nProtocol):
        m_nFamily(nFamily),
        m_nType(nTtype),
        m_nProtocol(nProtocol)
    {

    }

    LSocketTypeInfo::~LSocketTypeInfo()
    {

    }

    int LSocketTypeInfo::getFamily()
    {
        return m_nFamily;
    }

    void LSocketTypeInfo::setFamily(int nFamily)
    {
        m_nFamily = nFamily;
    }

    int LSocketTypeInfo::getType()
    {
        return m_nType;
    }

    void LSocketTypeInfo::setType(int nTtype)
    {
        m_nType = nTtype;
    }

    int LSocketTypeInfo::getProtocol()
    {
        return m_nProtocol;
    }

    void LSocketTypeInfo::setProtocol(int nProtocol)
    {
        m_nProtocol = nProtocol;
    }

    LAddressInfo::LAddressInfo():
        m_szIP(""),
        m_nPort(0)
    {

    }

    LAddressInfo::LAddressInfo(std::string szIP, int nPort):
        m_szIP(szIP),
        m_nPort(nPort)
    {
    }

    LAddressInfo::~LAddressInfo()
    {

    }

    void LAddressInfo::setIP(std::string szIP)
    {
        m_szIP = szIP;
    }

    std::string LAddressInfo::getIP()
    {
        return m_szIP;
    }

    void LAddressInfo::setPort(int nPort)
    {
        m_nPort = nPort;
    }

    int LAddressInfo::getPort()
    {
        return m_nPort;
    }

    LSocket::LSocket():
        m_pSctTpInfo(nullptr),
        m_stSocket(NULL),
        m_pAddressInfo(nullptr)
    {

    }

    LSocket::LSocket(LSocketTypeInfo * pTypeInfo):
        m_pSctTpInfo(pTypeInfo),
        m_pAddressInfo(nullptr)
    {
        if (pTypeInfo)
        {
            m_stSocket = socket(pTypeInfo->getFamily(), pTypeInfo->getType(), pTypeInfo->getProtocol());
        }
    }

    LSocket::~LSocket()
    {

    }

    int LSocket::sendData(SOCKET_TYPE socket, const char *pData, int nFlag)
    {
        int ret = FAILED_RET_VALUE;
        if (!pData)
        {
            return ret;
        }


        size_t nRealSize = strlen(pData);
        while(nRealSize > 0)
        {
            ssize_t nSendedSize = send(socket, (const void *)pData, nRealSize, nFlag);
            if(nSendedSize < 0)
            {
#ifdef __WIN32
                if(WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    continue;
                }
#else
                if(errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
                {
                    continue;
                }
#endif
                return (int)nSendedSize;
            }
            nRealSize = nRealSize - nSendedSize;
        }
        return ret;
    }

    int LSocket::recvData(SOCKET_TYPE socket, char * pBuf, int nFlag)
    {
        ssize_t nReadSize = READ_BUFFER_SIZE;
        while(nReadSize > 0)
        {
            nReadSize = recv(this->getSocket(), (void *)pBuf, READ_BUFFER_SIZE, nFlag);
            if(nReadSize < 0)
            {
#ifdef __WIN32
                if(WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    continue;
                }
#else
                if(errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
                {
                    continue;
                }
#endif
                return (int)nReadSize;
            }
            else
            {
                if (nReadSize == 0 && errno != EINTR)
                {
                    return FAILED_RET_VALUE;
                }
            }
            break;
        }
        return nReadSize;
    }

    void LSocket::setSocketTypeInfo(LSocketTypeInfo *stypeInfo)
    {
        m_pSctTpInfo = stypeInfo;
    }

    LSocketTypeInfo* LSocket::getSocketTypeInfo()
    {
        return m_pSctTpInfo;
    }

    SOCKET_TYPE LSocket::getSocket()
    {
        return m_stSocket;
    }

    void LSocket::setAddressInfo(LAddressInfo *pInfo)
    {
        m_pAddressInfo = pInfo;
    }

    LAddressInfo* LSocket::getAddressInfo()
    {
        return m_pAddressInfo;
    }

    int LSocket::selectMode(SOCKET_TYPE socket, int nFdMode, int nTimeout) {
        fd_set *pReadfds = NULL;
        fd_set *pWritefds = NULL;
        fd_set *pErrorfds = NULL;
        if (nFdMode & FD_READ == FD_READ)
        {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(socket, &readfds);
            pReadfds = &readfds;
        }
        if (nFdMode & FD_WRITE == FD_WRITE)
        {
            fd_set writefds;
            FD_ZERO(&writefds);
            FD_SET(socket, &writefds);
            pWritefds = &writefds;
        }
        if (nFdMode & FD_ERROR == FD_ERROR)
        {
            fd_set errorfds;
            FD_ZERO(&errorfds);
            FD_SET(socket, &errorfds);
            pErrorfds = &errorfds;
        }

        timeval * pTimeOut = NULL;
        if( nTimeout < 0)
        {
            timeval timeout;
            timeout.tv_sec = nTimeout / 1000;
            timeout.tv_usec = (nTimeout % 1000) * 1000;
            pTimeOut = &timeout;
        }

#ifdef __WIN32
        int rtn = select(1, pReadfds, pWritefds, pErrorfds, pTimeOut);
        if(rtn == 0)
        {
            return 0;
        }
        else if(rtn == SOCKET_ERROR)
        {
            return -1;
        }
        return 1;
#else
        return select(socket + 1, pReadfds, pWritefds, pErrorfds, pTimeOut);
#endif
    }

    int LSocket::close(SOCKET_TYPE socket)
    {
#ifdef __WIN32
        return closesocket(socket);
#else
        return close(socket);
#endif
    }

    int LSocket::setNotBlockMode(SOCKET_TYPE socket)
    {
#ifdef __WIN32
        unsigned long nRet = 1;
	    ioctlsocket(socket, FIONBIO, &nRet);
        return nRet;
#else
        int nFlag = fcntl(socket, F_GETFL);
        if((nFlag & O_NONBLOCK) == 0)
        {
            return fcntl(socket, F_SETFL, O_NONBLOCK | nFlag);
        }
        return nFlag;
#endif
    }

    int LSocket::setSendTimeout(int nTimeout) {
        return setsockopt(m_stSocket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&nTimeout, sizeof(int));
    }

    int LSocket::setRecvTimeout(int nTimeout) {
        return setsockopt(m_stSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&nTimeout, sizeof(int));
    }

    std::unordered_map<std::string, int> LSocket::getFamilyTypeByIPOrHostName(std::string szIPorHostname) {
        struct addrinfo *answer, hint, *curr;
        bzero(&hint, sizeof(hint));
        // char ipstr[128];
        char * ipstr = new char[128];
        hint.ai_family = AF_UNSPEC;
        hint.ai_socktype = SOCK_STREAM;;
        struct sockaddr_in  *sockaddr_ipv4;
        struct sockaddr_in6 *sockaddr_ipv6;
        std::unordered_map<std::string, int> result;

        int ret = getaddrinfo(szIPorHostname.c_str(), NULL,&hint, &answer);
        if (ret != 0) {
            return result;
        }
        for (curr = answer; curr != NULL; curr = curr->ai_next) {
            switch (curr->ai_family){
                case AF_UNSPEC:
                    result[szIPorHostname] = AF_UNSPEC;
                    break;
                case AF_INET:
                    sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>( curr->ai_addr);
                    inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstr, sizeof(ipstr));
                    result[ipstr] = AF_INET;
                    break;
                case AF_INET6:
                    sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>( curr->ai_addr);
                    inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstr, sizeof(ipstr));
                    result[ipstr] = AF_INET6;
                    break;
            }
        }
        freeaddrinfo(answer);
        return result;
    }

    bool LSocket::getIsIPv6() {
        return (m_pSctTpInfo->getFamily() == AF_INET6);
    }

NS_LONG_END