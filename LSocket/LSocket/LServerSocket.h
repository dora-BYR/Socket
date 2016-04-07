//
// Created by 龙辉 on 16/4/7.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LSOCKET_LSERVERSOCKET_H
#define LSOCKET_LSERVERSOCKET_H

#include "LSocket.h"

#include <vector>

// 默认的最大连接等待队列长度
#define DEFAULT_MAX_WAIT_CONNECT_COUNT  5
// 默认允许的最大连接数
#define DEFAULT_MAX_CONNECTED_COUNT     1024

NS_LONG_BEGIN

	// 服务端socket
    class LServerSocket : public LSocket {
    public:
        LServerSocket();
        LServerSocket(LSocketTypeInfo * pTypeInfo);
        virtual ~LServerSocket();

        // 创建并指定socket类型
        static LServerSocket * create(LSocketTypeInfo * pTypeInfo);

        // 绑定目标地址对象
        int bindTarget(LAddressInfo * pAddress);

        // 绑定目标地址对象
        int bindTarget(std::string szIP, int nPort);

        // 监听
        // @param backlog 最大的等待队列长度
        int listenTarget(unsigned int nBacklog = DEFAULT_BACK_LOG);

        // 接受客户端的连接
        SOCKET_TYPE acceptTarget(LAddressInfo * pInfo);

    private:
        unsigned int m_nMaxConnectedCount;
    	unsigned int m_nBacklog;
    };

NS_LONG_END

#endif //LSOCKET_LSERVERSOCKET_H
