#pragma once
/*
SPDX-FileCopyrightText: © 2020 DongHyuk Park< SnowFleur0128@gamil.com>
SPDX-License-Identifier: BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
#include"Global.h"

class CNode;
using CNodeSharePtr = std::shared_ptr<CNode>;

class CNode {
public:
    Position        position_;
    CNodeSharePtr   next_;
};