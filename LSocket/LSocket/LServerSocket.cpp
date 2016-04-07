//
// Created by 龙辉 on 16/4/7.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LServerSocket.h"

NS_LONG_BEGIN

    LServerSocket::LServerSocket():
            LSocket(),
            m_nBacklog(DEFAULT_MAX_WAIT_CONNECT_COUNT),
            m_nMaxConnectedCount(DEFAULT_MAX_CONNECTED_COUNT)
    {

    }

    LServerSocket::LServerSocket(LSocketTypeInfo * pTypeInfo):
            LSocket(pTypeInfo),
            m_nBacklog(DEFAULT_MAX_WAIT_CONNECT_COUNT),
            m_nMaxConnectedCount(DEFAULT_MAX_CONNECTED_COUNT)
    {

    }

    LServerSocket::~LServerSocket()
    {

    }

    LServerSocket* LServerSocket::create(LSocketTypeInfo * pTypeInfo)
    {
        auto pServerSocket = new LServerSocket(pTypeInfo);
        return pServerSocket;
    }

    int LServerSocket::bindTarget(LAddressInfo * pInfo)
    {
        if (!pInfo)
        {
            return FAILED_RET_VALUE;
        }
        this->LSocket::setAddressInfo(pInfo);
        return this->bindTarget(pInfo->getIP(), pInfo->getPort());
    }

    int LServerSocket::bindTarget(std::string szIP, int nPort)
    {
        struct sockaddr_in addr;
        memset(&addr, NULL, sizeof(addr));
        addr.sin_family = this->getSocketTypeInfo()->getFamily();
        addr.sin_addr.s_addr = inet_addr(szIP.c_str());
        addr.sin_port = htons(nPort);
        return bind(this->getSocket(), (struct sockaddr*)&addr, sizeof(addr));
    }

    int LServerSocket::listenTarget(unsigned int nBacklog)
    {
        m_nBacklog = nBacklog;
        return listen(this->getSocket(), nBacklog);
    }

    SOCKET_TYPE LServerSocket::acceptTarget(LAddressInfo * pInfo)
    {
        struct sockaddr_in addr;
        socklen_t size = sizeof(addr);
        SOCKET_TYPE clientSocket = accept(this->getSocket(), (struct sockaddr*)&addr, &size);
        pInfo->setIP(inet_ntoa(addr.sin_addr));
        pInfo->setPort(ntohs(addr.sin_port));
        return clientSocket;
    }

NS_LONG_END