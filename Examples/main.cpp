// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEConcurrency.h"

int main()
{
    IESpinOnWriteObject SpinOnWriteObject(2);
    return 0;
}