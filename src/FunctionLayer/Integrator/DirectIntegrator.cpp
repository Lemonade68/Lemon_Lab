#include"DirectIntegrator.h"

Spectrum DirectIntegrator_SampleBSDF::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f);
    Spectrum background(.1f, .2f, .4f);

    auto intersectionOpt = scene.rayIntersect(ray);    //�볡����

    //���û�д򵽣�ֱ�ӷ��ػ�����(�����Ϊֱ�ӷ��ر�����ɫ)
    if(!intersectionOpt.has_value()){
        //�ȴ�����
        return background;
    }

    //�����Ǵ�����������
    auto intersection = intersectionOpt.value();

    //�ж������Ƿ�������
    if (auto light = intersection.shape->light; light){
        //�����wo�Ǵ�shading point�������
        spectrum += light->evaluateEmission(intersection, -ray.direction);
    }

    //��Դ�ļ���(ֱ�Ӳ���BSDF�Ļ��ǲ������һ�������ж��Ƿ�򵽹�Դ���򵽣����ϣ�û�򵽣�����)
    auto material = intersection.shape->material;
    auto bsdf = material->computeBSDF(intersection);
    auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler);     //�����˲���

    //TODO���ж��Ƿ�ֱ�ӻ��й�Դ����������ֱ�ӷ��أ�

    //ֻ����һ������ ���� Path tracing����ֹ��������ָ������ը
    Ray sampleRay{intersection.position, bsdfSampleResult.wi};      //�����õ��Ĺ���

    auto findLight = scene.rayIntersect(sampleRay);
    if(!findLight.has_value())
        spectrum += bsdfSampleResult.weight * background;           //���㻷���⣨�ȴ����ƣ�
    else{
        auto result = findLight.value();
        if (auto light = result.shape->light; light){               //���е����巢������Ϊ��Դ
            //��ʱ�����ǹ���˥�������ǵĻ����ÿ�
            spectrum += bsdfSampleResult.weight *
                        light->evaluateEmission(result, -sampleRay.direction);
        }
    }

    return spectrum;
}


//TODO��������Դ�ķ���
Spectrum DirectIntegrator_SampleLight::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f);
    Spectrum background(.1f, .2f, .4f);

    auto intersectionOpt = scene.rayIntersect(ray);    //�볡����

    //���û�д򵽣�ֱ�ӷ��ػ�����(�����Ϊֱ�ӷ��ر�����ɫ)
    if(!intersectionOpt.has_value()){
        //�ȴ�����
        return background;
    }

    //�����Ǵ�����������
    auto intersection = intersectionOpt.value();


    //�ж������Ƿ�������(��ʱֱ�ӵ��ɹ�Դ)
    if (auto light = intersection.shape->light; light){
        //�����wo�Ǵ�shading point�������
        spectrum += light->evaluateEmission(intersection, -ray.direction);
    }

    //������˳�����Դ��������Χ����������Զ��Դ�������⣩��TODO(�����ģ��������)
    // spectrum += background;              //����֮����Կ�����Դ��������Щ����

    //���濪ʼ���ܻ����segmentation fault

    //����������������е�һ����Դ
    float pdfLight = .0f;

    // segmentation�����������
    auto light = scene.sampleLight(sampler->sampler1D(), pdfLight);

    //����й�Դ�ҿ��Ա�������
    if(light && pdfLight != 0){
        //�ڹ�Դ�ϲ���һ���㣬������shading point�͸õ㣬����һ������
        auto lightSampleResult = light->sample(intersection, sampler->sampler2D());
        Ray shadowRay(intersection.position, lightSampleResult.direction, EPSILON, lightSampleResult.distance);
        //�������������Ƿ��赲
        auto occlude = scene.rayIntersect(shadowRay);
        if(!occlude.has_value()){
            auto material = intersection.shape->material;
            auto bsdf = material->computeBSDF(intersection);
            Spectrum f = bsdf->f(-ray.direction, shadowRay.direction);      //�����Ѿ������cos��
            lightSampleResult.pdf *= pdfLight;                          //�������ù�Դ�ĸ��� * �ù�Դ�ϲ������õ�ĸ���
            float pdf = convertPDF(lightSampleResult);                  //ת���������Դ����
            spectrum += lightSampleResult.energy * f / pdf;
        }
    }
    return spectrum;
}