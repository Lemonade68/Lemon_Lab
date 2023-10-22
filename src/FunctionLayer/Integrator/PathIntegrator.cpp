#include "PathIntegrator.h"

//pathIntegrator两种写法：递归或者循环（循环应该是利用了 L=E+KE+K方E+... 的结论）
//此处使用循环的写法

Spectrum PathIntegrator::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f), weight(1.f);
    Spectrum background(.0f, .0f, .0f);         //没有hdr，只能这样来限制下
    int depth = 0;

    bool firstTime = true, SpecularBounce = false;

    //进入循环，直到光线打到背景、弹射次数达到上限或者达到RR的条件时退出循环
    while(1){
        ++depth;
        if(depth >= max_depth)
            break;

        auto intersectionOpt = scene.rayIntersect(ray);

        if(!intersectionOpt.has_value()){
            //直接返回权重*背景环境光
            spectrum += weight * background;
            break;
        }

        auto intersection = intersectionOpt.value();

        //加上自身的光(如果是第一次打到光源，直接返回光源的energy)
        //这里是防止过于亮，若第二次及以上弹射，漫反射物体的自发光暂不考虑，只有镜面照射出的光源才考虑
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


        //加上环境光（后需添加）
        //...

        //计算该点被光源照亮的spectrum
        float pdfLight = .0f;
        auto light = scene.sampleLight(sampler->sampler1D(), pdfLight);

        if(light && pdfLight != 0){
            auto lightSampleResult = light->sample(intersection, sampler->sampler2D());
            Ray shadowRay(intersection.position, lightSampleResult.direction,
                          EPSILON, lightSampleResult.distance);
            //这个算法还是有缺陷：如果光线被另一个光源挡住，也会认为没有贡献(下面粗略解决)
            auto occlude = scene.rayIntersect(shadowRay);
            if(!occlude.has_value()){
                auto material = intersection.shape->material;
                auto bsdf = material->computeBSDF(intersection);
                auto f = bsdf->f(-ray.direction, shadowRay.direction);      //镜面直接f=0，当次的不考虑，直接考虑反射后的结果
                lightSampleResult.pdf *= pdfLight;
                convertPDF(lightSampleResult);
                auto pdf = lightSampleResult.pdf;
                spectrum += weight * lightSampleResult.energy * f / pdf;
            }
            else if (auto anotherLight = occlude.value().shape->light; anotherLight){     //打到的是另一个光源
                //暂时认为另一个光源重新采样一个点后，该点与shading point间没有障碍
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

        // 使用RR来随机限制次数
        if(depth > 3 && sampler->sampler1D() > 0.95f)
            break;
        weight /= 0.95f;        //RR中的保证期望的操作

        //确定下一次的光线
        auto bsdf = intersection.shape->material->computeBSDF(intersection);
        auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler->sampler2D());
        
        //更新权重
        weight *= bsdfSampleResult.weight;

        //更新下一次的光线
        ray = Ray(intersection.position, bsdfSampleResult.wi);

        //更新材质状态
        SpecularBounce = (bsdfSampleResult.type == BSDFType::Specular);
    }

    return spectrum;
}