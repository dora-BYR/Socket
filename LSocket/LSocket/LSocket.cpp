//
// Created by 龙辉 on 16/4/5.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LSocket.h"

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

NS_LONG_END