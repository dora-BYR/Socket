//
// Created by 龙辉 on 16/4/7.
// Copyright (c) 2016 yuemo studio. All rights reserved.
//

#ifndef LSOCKET_LTEST_H
#define LSOCKET_LTEST_H

#include "define_l.h"

NS_LONG_BEGIN

    class LTest {
    public:
        LTest(){}
        virtual ~LTest(){}
        static void startServer();
        static void startCliennt();
    };

NS_LONG_END

#endif //LSOCKET_LTEST_H
