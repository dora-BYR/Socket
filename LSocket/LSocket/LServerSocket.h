//
// Created by 龙辉 on 16/4/7.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LSOCKET_LSERVERSOCKET_H
#define LSOCKET_LSERVERSOCKET_H

#include "LSocket.h"

#include <vector>
#include <functional>

#include <sys/epoll.h>

// 默认的最大连接等待队列长度
#define DEFAULT_MAX_WAIT_CONNECT_COUNT  5
// 默认允许的最大连接数
#define DEFAULT_MAX_CONNECTED_COUNT     1024
// epoll mode监听socket的最大数目
#define COUNT_MAX_LISTEN_SOCKET_EPOLL   10000
// epoll mode 一次处理的最大事件数目
#define COUNT_MAX_EVENT_HANDLED         20
// epoll mode 超时

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

        // epoll mode
        int epollControl(int cmdType = EPOLL_CTL_ADD, int nEpSize = COUNT_MAX_LISTEN_SOCKET_EPOLL, int events = EPOLLIN|EPOLLET, int nTimeout = -1);
        int epollWait(int nMaxEventCountHandled = COUNT_MAX_EVENT_HANDLED, int nTimeout = -1);

    private:
        unsigned int m_nMaxConnectedCount;
    	unsigned int m_nBacklog;
        SOCKET_TYPE m_epollFd;
        struct epoll_event m_epEvent;
        struct epoll_event m_epEvents[COUNT_MAX_EVENT_HANDLED];
    };

NS_LONG_END

#endif //LSOCKET_LSERVERSOCKET_H
