//
// Created by 龙辉 on 16/4/7.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LSOCKET_LCLIENTSOCKET_H
#define LSOCKET_LCLIENTSOCKET_H

#include "LSocket.h"

NS_LONG_BEGIN

    class LClientSocket : public LSocket {
    public:
        LClientSocket();
        LClientSocket(LSocketTypeInfo * pTypeInfo);
        virtual ~LClientSocket();

        // 创建并指定socket类型
        static LClientSocket * create(LSocketTypeInfo * pTypeInfo);

        // 连接目标服务器
        int connectServer(LAddressInfo * pInfo);

        // 连接目标服务器
        int connectServer(std::string szIP, int nPort);

        // 发送数据
        int sendData(const char * pData, int nFlag = NULL);

        // 接收数据
        int recvData(char * pBuf, int nFlag = NULL);

        // 设置非阻塞模式
        int setNotBlockMode();

        // 关闭套接字
        int close();

    private:

    };

NS_LONG_END

#endif //LSOCKET_LCLIENTSOCKET_H
