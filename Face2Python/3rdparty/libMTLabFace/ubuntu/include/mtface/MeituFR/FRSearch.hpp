//
//  FRSearch.hpp
//  MeituFRDemo
//
//  Created by meitu on 2017/6/26.
//  Copyright © 2017年 com.meitu.mlab. All rights reserved.
//

#ifndef mtface_MeituFR_FRSearch_hpp
#define mtface_MeituFR_FRSearch_hpp

#include "mtface/Common/MTVector.h"
#include "mtface/Common/MTFaceFeature.h"
#include "mtface/Common/MTDefine.h"
#include "mtface/Common/MTTypes.h"

namespace mtface
{    
    class MEITU_EXPORT FRSearch
    {
    public:
        /// @brief 以脸搜脸, 输入的人脸列表和搜索target人脸**必须**提取完成faceCode 提取
        /// @param [in] faces       人脸列表
        /// @param [in] target      要搜索的人脸
        /// @param [out] indexes    返回与target相似的人脸在faces中的序号, 其长度代表相似人脸的个数
        /// @param [in] threshold   相似人脸的阈值, 默认值为评测集 FAR@0.1% 的时候的阈值
        /// @return MTRET 执行的状态
        ///               - 如果target没有faceCode, 返回 MTRET::MTRET_ERR_PARAMS
        static MTRET Run(const mtface::MTVector<mtface::MTFaceFeature>& faces,
            const mtface::MTFaceFeature& target,
            mtface::MTVector<int>& match_indexes,
            float threshold = 0.4618f);

        /// @brief 一脸搜脸, 输入的人脸列表和搜索target人脸**必须**提取完成faceCode 提取
        /// @param [in] faces             人脸列表
        /// @param [in] target_index      要搜索的人脸在faces的序号
        /// @param [out] indexes    返回与target相似的人脸在faces中的序号, 其长度代表相似人脸的个数
        /// @param [in] threshold   相似人脸的阈值, 默认值为评测集 FAR@0.1% 的时候的阈值
        /// @return MTRET 执行的状态
        ///               - 如果target没有faceCode, 返回 MTRET::MTRET_ERR_PARAMS
        static MTRET Run(const mtface::MTVector<mtface::MTFaceFeature>& faces,
            size_t target_index,
            mtface::MTVector<int>& match_indexes,
            float threshold = 0.4618f);

    };
}

#endif /* mtface_MeituFR_FRSearch_hpp */
