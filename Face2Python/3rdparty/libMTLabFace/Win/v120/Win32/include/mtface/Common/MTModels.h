#pragma once

#include <mtface/Common/MTDefine.h>
#include <stddef.h>

namespace mtface
{

// 模型文件封装
class MEITU_EXPORT MTModels
{
public:
    // 模型类型
    enum class Type
    {
        MEITU_FD_MODEL_TYPE,          // FD模型
        MEITU_FA_MODEL_TYPE,          // FA模型
        MEITU_GENDER_MODEL_TYPE,      // Gender模型
        MEITU_TRACK_MODEL_TYPE,       // Tracking模型
        MEITU_FDV2_RPN_MODEL_TYPE,    // FDV2模型RPN
        MEITU_FDV2_FA_MODEL_TYPE,     // FDV2模型FA
        UNKNOWN_MODEL_TYPE
    };
    
    // 模型数据
    class Model
    {
    public:
        Model();
        ~Model();
        Model(const Model &other);
        Model(Model&& other);
        
        Model& operator=(const Model &other);
        Model& operator=(Model &&other);
        
        // 不拷贝数据, 外部自己管理传入pData的生命周期
        bool SetData(char *pData, unsigned long size, Type type);
        // 拷贝数据
        bool SetDataCpy(const char *pData, unsigned long size, Type type);
        // 从文件载入数据
        bool LoadFromFile(const char *filePath, Type type);
        
    public:
        Type m_type;
        size_t m_size;
        char *m_modelData;
        
    private:
        char *m_modelDataReal;
    };
    
public:
    MTModels();
    ~MTModels();
    
    // 从文件载入数据
    bool PushModelFromFile(const char *filePath, Type type);
    // 不拷贝数据, 外部自己管理传入pData的生命周期
    bool PushModelFromData(char *pData, unsigned long size, Type type);
    // 拷贝数据
    bool PushModelFromDataCpy(const char *pData, unsigned long size, Type type);
    
    // 获取指定类型的模型数据
    Model *GetModelByType(Type type);
    
private:
    void* models;
};

} // namespace mtface
