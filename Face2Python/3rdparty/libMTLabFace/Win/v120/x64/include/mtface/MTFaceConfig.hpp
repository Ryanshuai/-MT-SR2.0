#pragma once

#include "mtface/Common/MTDefine.h"
#define MTFACE_VERSION "MTFace-3.0.0"
/* #undef ENABLE_PLATFORM_CHECK */

namespace mtface
{

// 版本号信息, 返回宏定义MTFACE_VERSION字符串
MEITU_EXPORT const char *GetMTFaceVersion();

} // mtface

#define MTFACE_BUILD_TIMEZ 2017-08-25T08:44:23Z