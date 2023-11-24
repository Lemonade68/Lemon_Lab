#include "PathIntegrator.h"

//pathIntegrator两种写法：递归或者循环（循环应该是利用了 L=E+KE+K方E+... 的结论）
//此处使用循环的写法

//多重重要性采样方法：  见https://howl144.github.io/2023/09/30/00014.%20Games101%20FinalProject/#brdf-and-bsdf
float PowerHeurisetic(float nf, float fPdf, float ng, float gPdf){
    float f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
}

Spectrum PathIntegrator::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f), weight(1.f);
    Spectrum background(.0f, .0f, .0f);         //没有hdr，只能这样来限制下
    int depth = 0;

    bool SpecularBounce = false;

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

        //加上自身的光
        //这里是防止过于亮，若第二次及以上弹射，漫反射物体的自发光暂不考虑，只有镜面照射出的光源才考虑
        if(depth == 1 || SpecularBounce){       
            if (auto light = intersection.shape->light; light){
                spectrum += weight * light->evaluateEmission(intersection, -ray.direction);
            }            
        }

        //加上环境光（后需添加）
        //...

        //MIS的准备工作(brdf采样与light采样)
        auto material = intersection.shape->material;
        auto bsdf = material->computeBSDF(intersection);
        auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler);     //考虑了材质
        auto pdf_brdf = bsdfSampleResult.pdf;

        float pdfLight = .0f;
        auto light = scene.sampleLight(sampler->sampler1D(), pdfLight);
        //暂时默认light一定存在
        auto lightSampleResult = light->sample(intersection, sampler->sampler2D());
        lightSampleResult.pdf *= pdfLight;
        convertPDF(lightSampleResult);      //这里先不考虑问题，看结果(对同一个东西积分)
        auto pdf_light = lightSampleResult.pdf;

        auto weight_brdf = PowerHeurisetic(1.f, pdf_brdf, 1.f, pdf_light);
        auto weight_light = PowerHeurisetic(1.f, pdf_light, 1.f, pdf_brdf);

        //MIS方法：使用直接采样光源和直接采样bsdf两种方法的结合（只在计算直接光照时使用，间接光照还是只有brdf采样）
        //采样bsdf
        Ray sampleRay{intersection.position, bsdfSampleResult.wi};      //采样得到的光线
        auto findLight = scene.rayIntersect(sampleRay);
        if(!findLight.has_value())
            spectrum += weight_brdf * bsdfSampleResult.weight * background;           //计算环境光（等待完善）
        else{
            auto result = findLight.value();
            auto f = bsdf->f(-ray.direction, bsdfSampleResult.wi);
            if (auto light = result.shape->light; light){               //击中的物体发光则视为光源
                //暂时不考虑光线衰减，考虑的话不好看
                spectrum += weight_brdf * weight * light->evaluateEmission(result, -sampleRay.direction) * f / pdf_brdf;
            }
        }
        //采样光源
        if(light && pdfLight != 0){
            Ray shadowRay(intersection.position, lightSampleResult.direction, EPSILON, lightSampleResult.distance);
            //这个算法还是有缺陷：如果光线被另一个光源挡住，也会认为没有贡献(下面粗略解决)
            auto occlude = scene.rayIntersect(shadowRay);
            if(!occlude.has_value()){
                auto f = bsdf->f(-ray.direction, shadowRay.direction);      //cos项在这里计算过了
                spectrum += weight_light * weight * lightSampleResult.energy * f / pdf_light;
            }
            // else if (auto anotherLight = occlude.value().shape->light; anotherLight){     //打到的是另一个光源
            //     //暂时认为另一个光源重新采样一个点后，该点与shading point间没有障碍
            //     lightSampleResult = anotherLight->sample(occlude.value(), sampler->sampler2D());
            //     shadowRay = Ray(intersection.position, lightSampleResult.direction,
            //                     EPSILON, lightSampleResult.distance);
            //     auto material = intersection.shape->material;
            //     auto bsdf = material->computeBSDF(intersection);
            //     auto f = bsdf->f(-ray.direction, shadowRay.direction);
            //     lightSampleResult.pdf *= pdfLight;
            //     convertPDF(lightSampleResult);
            //     auto pdf = lightSampleResult.pdf;
            //     spectrum += weight * lightSampleResult.energy * f / pdf;
            // }
        }

        // 使用RR来随机限制次数
        // if(depth > 3 && sampler->sampler1D() > 0.95f)
            // break;
        // weight /= 0.95f;        //RR中的保证期望的操作

        //确定下一次的光线
        // auto bsdf = intersection.shape->material->computeBSDF(intersection);
        // auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler);
        
        //更新权重
        weight *= bsdfSampleResult.weight;
        if(weight[0] < EPSILON && weight[1] < EPSILON && weight[2] < EPSILON)       //权重过小直接退出(舍弃掉pdf太小的光线)
            break;

        //更新下一次的光线
        ray = Ray(intersection.position, bsdfSampleResult.wi);

        //更新材质状态
        SpecularBounce = (bsdfSampleResult.type == BSDFType::Specular);
    }

    return spectrum;
}