//
// Created by 龙辉 on 16/4/7.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#include "LTest.h"
#include "LServerSocket.h"
#include "LClientSocket.h"

NS_LONG_BEGIN

    void LTest::startServer()
    {
        LLOG("启动服务端...");
        bool bIsStart = false;
        // 创建服务端socket
        LSocketTypeInfo typeInfo(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        auto serverSocket = LServerSocket::create(&typeInfo);
        // 绑定地址
        LAddressInfo addrInfo("127.0.0.1", 12580);
        serverSocket->bindTarget(&addrInfo);
        // 开启监听, 最大等待队列长度设置为10
        serverSocket->listenTarget(10);
        // 开始接收
        LAddressInfo info;
        while(true)
        {
            if (!bIsStart)
            {
                LLOG("服务端启动成功");
            }
            SOCKET_TYPE socket = serverSocket->acceptTarget(&info);
            if (socket != L_INVALID_SOCKET)
            {
                bIsStart = true;
                LLOG("成功收到客户端的连接, ID = %d", (int)socket);
                // 向客户端发送欢迎数据
                std::string data = "dora, 欢迎回来!";
                serverSocket->sendData(socket, data.c_str());
                LLOG("向客户端发送数据: %s", data.c_str());
            }
        }
    }

    void LTest::startCliennt()
    {
        LLOG("启动客户端...");
        bool bIsStart = false;
        // 创建服务端socket
        LSocketTypeInfo typeInfo(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        auto clientSocket = LClientSocket::create(&typeInfo);
        // 连接服务器
        LAddressInfo addrInfo("127.0.0.1", 12580);
        int ret = clientSocket->connectServer(&addrInfo);
        if (ret == 0)
        {
            char buf[READ_BUFFER_SIZE] = {0};
            while (true)
            {
                memset(buf, NULL, sizeof(buf));
                // 开始接收数据
                ret = clientSocket->recvData(buf);
                if (ret > 0)
                {
                    LLOG("收到服务端数据:%s", buf);
                }
            }
        }

    }

NS_LONG_END