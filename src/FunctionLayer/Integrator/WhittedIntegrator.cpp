#include"WhittedIntegrator.h"

Spectrum WhittedIntegrator::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f), weight(1.f);    //ÿ�η����Ȩ�أ�ע�������ÿ�μ��㶼��Ҫ��ǰ�����
    Spectrum background(.1f, .2f, .4f);

    int turn = 0;       // ���ƴ���

    while(1) {      //��ͣѭ������
        ++turn;
        if(turn > max_depth){
            // return Spectrum(1.f, .0f, .0f);     //��bugʹ�ã�ʹ��ʱspp����Ϊ1��͹��Ϊ��ɫ
            return Spectrum(.0f);
        }

        auto intersectionOpt = scene.rayIntersect(ray);
        if(!intersectionOpt.has_value()){
            spectrum += weight * background;    //ѭ�����иĳ�+=����֮ǰ�Ļ����Ͻ��е��ӣ�
            break;
        }

        auto intersection = intersectionOpt.value();
        if (auto light = intersection.shape->light; light)      //�Է���
            spectrum += weight * light->evaluateEmission(intersection, -ray.direction);

        auto material = intersection.shape->material;
        auto bsdf = material->computeBSDF(intersection);
        auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler->sampler2D());

        //����Ǿ�����ʣ�ֱ���������ɷ�����ߣ�ֱ�ӽ�����һ��ѭ��
        if(bsdfSampleResult.type == BSDFType::Specular){
            ray = Ray(intersection.position, bsdfSampleResult.wi);
            weight *= bsdfSampleResult.weight;      //����Ȩ���Ըı���һ�ι��ߵ�Ȩ��
            continue;
        }
        //���Ǿ�����ʵĻ�������sample_light�ķ�����ȡ�øô��������ֵ�����˳�ѭ��
        else{
            //�����ⲿ�֣���ʱ�����ǣ��������    ��ʱû�а������ؿ�����ֹ�ʽ��ֱ��д
            // spectrum += weight * background * .15f;

            //������Դ
            float pdfLight = .0f;
            auto light = scene.sampleLight(sampler->sampler1D(), pdfLight);
            if(light && pdfLight != .0f){
                auto lightSampleResult = light->sample(intersection, sampler->sampler2D());
                Ray shadowRay(intersection.position, lightSampleResult.direction,
                              EPSILON, lightSampleResult.distance);
                auto occlude = scene.rayIntersect(shadowRay);
                if(!occlude.has_value()){
                    auto f = bsdf->f(-ray.direction, shadowRay.direction);
                    lightSampleResult.pdf *= pdfLight;      //��Դ�ϲ������õ���� * �������ù�Դ�ĸ���
                    float pdf = convertPDF(lightSampleResult);      //����pdfֵ����Ϊ�Թ�Դ���֣�
                    spectrum += weight * lightSampleResult.energy * f / pdf;
                }
            }
            break;      //ֱ���˳�ѭ��
        }
    };

    return spectrum;
}