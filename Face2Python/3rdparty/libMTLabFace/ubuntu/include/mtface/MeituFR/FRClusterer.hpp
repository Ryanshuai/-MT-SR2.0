//
//  FRCluster.hpp
//  MeituFRDemo
//
//  Created by meitu on 2017/6/10.
//  Copyright © 2017年 com.meitu.mlab. All rights reserved.
//

#ifndef mtface_MeituFR_FRCluster_hpp
#define mtface_MeituFR_FRCluster_hpp

#include "mtface/Common/MTFaceFeature.h"
#include "mtface/Common/MTVector.h"
#include "mtface/Common/MTDefine.h"

namespace mtface
{
    
    class MEITU_EXPORT FRClusterer
    {
    public:
        /// \brief 将具有 faceCode 的人脸进行聚类
        /// \param features [in|out] 输入含有faceCode的人脸列表, 输出填充 clusterID 的人脸列表
        ///                          完成聚类后， hasClusterID = true
        ///                          clusterID 对应类别 ID,  离群的人脸  clusterID = -1
        /// \return 状态码
        MTRET Cluster(MTVector<MTFaceFeature>& features);

		struct MEITU_EXPORT Config{
			int min_samples;        /// 类别最少样本个数
			float eps1;             /// 类内距离阈值
			float eps2;             /// 合并大簇阈值
			float eps3;             /// 合并离群阈值
			Config() :
				min_samples(2),
				eps1(0.3f),
				eps2(0.36f),
				eps3(0.46f)
			{}

		}_config;
    };
}

#endif /* mtface_MeituFR_FRCluster_hpp */
