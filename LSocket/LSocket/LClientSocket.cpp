//
// Created by 龙辉 on 16/4/7.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LClientSocket.h"

NS_LONG_BEGIN

    LClientSocket::LClientSocket():
        LSocket()
    {

    }

    LClientSocket::LClientSocket(LSocketTypeInfo *pTypeInfo) :
        LSocket(pTypeInfo)
    {

    }

    LClientSocket::~LClientSocket()
    {

    }

    LClientSocket* LClientSocket::create(LSocketTypeInfo * pTypeInfo)
    {
        auto pSocket = new LClientSocket(pTypeInfo);
        return pSocket;
    }

    int LClientSocket::connectServer(LAddressInfo *pInfo)
    {
        if (!pInfo)
        {
            return FAILED_RET_VALUE;
        }
        return this->connectServer(pInfo->getIP(), pInfo->getPort());
    }

    int LClientSocket::connectServer(std::string szIP, int nPort)
    {
        if (!getIsIPv6()) {
            struct sockaddr_in addr;
            memset(&addr, NULL, sizeof(addr));
            addr.sin_family = this->getSocketTypeInfo()->getFamily();
            addr.sin_addr.s_addr = inet_addr(szIP.c_str());
            addr.sin_port = htons(nPort);
            return connect(this->getSocket(), (struct sockaddr *)&addr, sizeof(addr));
        } else {
            struct sockaddr_in6 addr;
            memset(&addr, NULL, sizeof(addr));
            addr.sin6_family = AF_INET6;
            inet_pton(AF_INET6, szIP.c_str(), &(addr.sin6_addr.s6_addr));
            addr.sin6_port = htons(nPort);
            return connect(this->getSocket(), (struct sockaddr *)&addr, sizeof(addr));
        }
    }

    int LClientSocket::sendData(const char * pData, int nFlag)
    {
        return LSocket::sendData(this->getSocket(), pData, nFlag);
    }

    int LClientSocket::recvData(char * pBuf, int nFlag)
    {
        return LSocket::recvData(this->getSocket(), pBuf, nFlag);
    }

    int LClientSocket::setNotBlockMode()
    {
        return LSocket::setNotBlockMode(this->getSocket());
    }

    int LClientSocket::close()
    {
        return LSocket::close(this->getSocket());
    }

NS_LONG_END
