#pragma once

#include"CoreLayer/Math/Math.h"

class NDF{
public:
    virtual ~NDF() noexcept = default;      //˼��������ʲô

    //�ο�moer-lite��ֱ�ӽ��Է��߷ֲ�����D����Ӱ�ڱ���G�ļ���ȫ������NDF��
    //�����ߵļ�������ض���ndf�����й�
    virtual float getD(const Vector3f &whLocal, float alpha) const noexcept = 0;
    virtual float getG(const Vector3f &woLocal, const Vector3f &wiLocal, float alpha) const noexcept = 0;

    //����΢����ģ�ͣ���Ҫ�Բ����������Ƿ��ߵķ���wh�Լ���pdf����Ҫ�ں������ת��
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/  ��  https://zhuanlan.zhihu.com/p/475748352
    virtual Vector3f sampleWh(const Vector3f &woLocal, const float &alpha, const Vector2f &sampler) const noexcept = 0;
    virtual float getPDF_reflect_global(const Vector3f &woLocal, const Vector3f &whLocal, const float &alpha) const noexcept = 0;
    //����wh��wo����wi�����淴�䣩
    virtual Vector3f calcWiLocalReflect(const Vector3f &woLocal, const Vector3f &whLocal) const noexcept {
        //���õ���������(������������Ϊ1)
        return 2 * dot(woLocal, whLocal) * whLocal - woLocal;
    }
};
