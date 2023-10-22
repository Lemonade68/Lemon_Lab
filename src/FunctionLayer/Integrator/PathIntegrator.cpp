#include "PathIntegrator.h"

//pathIntegrator����д�����ݹ����ѭ����ѭ��Ӧ���������� L=E+KE+K��E+... �Ľ��ۣ�
//�˴�ʹ��ѭ����д��

Spectrum PathIntegrator::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f), weight(1.f);
    Spectrum background(.0f, .0f, .0f);         //û��hdr��ֻ��������������
    int depth = 0;

    bool firstTime = true, SpecularBounce = false;

    //����ѭ����ֱ�����ߴ򵽱�������������ﵽ���޻��ߴﵽRR������ʱ�˳�ѭ��
    while(1){
        ++depth;
        if(depth >= max_depth)
            break;

        auto intersectionOpt = scene.rayIntersect(ray);

        if(!intersectionOpt.has_value()){
            //ֱ�ӷ���Ȩ��*����������
            spectrum += weight * background;
            break;
        }

        auto intersection = intersectionOpt.value();

        //��������Ĺ�(����ǵ�һ�δ򵽹�Դ��ֱ�ӷ��ع�Դ��energy)
        //�����Ƿ�ֹ�����������ڶ��μ����ϵ��䣬������������Է����ݲ����ǣ�ֻ�о���������Ĺ�Դ�ſ���
        if(depth == 1 || SpecularBounce){       
            if (auto light = intersection.shape->light; light){
                if(firstTime){
                    spectrum += light->evaluateEmission(intersection, -ray.direction);
                    firstTime = false;
                    break;
                }
                spectrum += weight * light->evaluateEmission(intersection, -ray.direction);
            }            
        }


        //���ϻ����⣨������ӣ�
        //...

        //����õ㱻��Դ������spectrum
        float pdfLight = .0f;
        auto light = scene.sampleLight(sampler->sampler1D(), pdfLight);

        if(light && pdfLight != 0){
            auto lightSampleResult = light->sample(intersection, sampler->sampler2D());
            Ray shadowRay(intersection.position, lightSampleResult.direction,
                          EPSILON, lightSampleResult.distance);
            //����㷨������ȱ�ݣ�������߱���һ����Դ��ס��Ҳ����Ϊû�й���(������Խ��)
            auto occlude = scene.rayIntersect(shadowRay);
            if(!occlude.has_value()){
                auto material = intersection.shape->material;
                auto bsdf = material->computeBSDF(intersection);
                auto f = bsdf->f(-ray.direction, shadowRay.direction);      //����ֱ��f=0�����εĲ����ǣ�ֱ�ӿ��Ƿ����Ľ��
                lightSampleResult.pdf *= pdfLight;
                convertPDF(lightSampleResult);
                auto pdf = lightSampleResult.pdf;
                spectrum += weight * lightSampleResult.energy * f / pdf;
            }
            else if (auto anotherLight = occlude.value().shape->light; anotherLight){     //�򵽵�����һ����Դ
                //��ʱ��Ϊ��һ����Դ���²���һ����󣬸õ���shading point��û���ϰ�
                lightSampleResult = anotherLight->sample(occlude.value(), sampler->sampler2D());
                shadowRay = Ray(intersection.position, lightSampleResult.direction,
                                EPSILON, lightSampleResult.distance);
                auto material = intersection.shape->material;
                auto bsdf = material->computeBSDF(intersection);
                auto f = bsdf->f(-ray.direction, shadowRay.direction);
                lightSampleResult.pdf *= pdfLight;
                convertPDF(lightSampleResult);
                auto pdf = lightSampleResult.pdf;
                spectrum += weight * lightSampleResult.energy * f / pdf;
            }
        }

        // ʹ��RR��������ƴ���
        if(depth > 3 && sampler->sampler1D() > 0.95f)
            break;
        weight /= 0.95f;        //RR�еı�֤�����Ĳ���

        //ȷ����һ�εĹ���
        auto bsdf = intersection.shape->material->computeBSDF(intersection);
        auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler->sampler2D());
        
        //����Ȩ��
        weight *= bsdfSampleResult.weight;

        //������һ�εĹ���
        ray = Ray(intersection.position, bsdfSampleResult.wi);

        //���²���״̬
        SpecularBounce = (bsdfSampleResult.type == BSDFType::Specular);
    }

    return spectrum;
}