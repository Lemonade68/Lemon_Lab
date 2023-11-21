#include"DirectIntegrator.h"

Spectrum DirectIntegrator_SampleBSDF::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f);
    Spectrum background(.1f, .2f, .4f);

    auto intersectionOpt = scene.rayIntersect(ray);    //与场景求交

    //如果没有打到：直接返回环境光(这里简化为直接返回背景颜色)
    if(!intersectionOpt.has_value()){
        //等待完善
        return background;
    }

    //下面是打到物体的情况：
    auto intersection = intersectionOpt.value();

    //判断物体是否自身发光
    if (auto light = intersection.shape->light; light){
        //传入的wo是从shading point朝外射的
        spectrum += light->evaluateEmission(intersection, -ray.direction);
    }

    //光源的计算(直接采样BSDF的话是采样随机一个方向，判断是否打到光源，打到：加上，没打到：不管)
    auto material = intersection.shape->material;
    auto bsdf = material->computeBSDF(intersection);
    auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler);     //考虑了材质

    //TODO：判断是否直接击中光源——击中则直接返回？

    //只采样一条光线 —— Path tracing，防止光线条数指数级爆炸
    Ray sampleRay{intersection.position, bsdfSampleResult.wi};      //采样得到的光线

    auto findLight = scene.rayIntersect(sampleRay);
    if(!findLight.has_value())
        spectrum += bsdfSampleResult.weight * background;           //计算环境光（等待完善）
    else{
        auto result = findLight.value();
        if (auto light = result.shape->light; light){               //击中的物体发光则视为光源
            //暂时不考虑光线衰减，考虑的话不好看
            spectrum += bsdfSampleResult.weight *
                        light->evaluateEmission(result, -sampleRay.direction);
        }
    }

    return spectrum;
}


//TODO：采样光源的方法
Spectrum DirectIntegrator_SampleLight::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f);
    Spectrum background(.1f, .2f, .4f);

    auto intersectionOpt = scene.rayIntersect(ray);    //与场景求交

    //如果没有打到：直接返回环境光(这里简化为直接返回背景颜色)
    if(!intersectionOpt.has_value()){
        //等待完善
        return background;
    }

    //下面是打到物体的情况：
    auto intersection = intersectionOpt.value();


    //判断物体是否自身发光(暂时直接当成光源)
    if (auto light = intersection.shape->light; light){
        //传入的wo是从shading point朝外射的
        spectrum += light->evaluateEmission(intersection, -ray.direction);
    }

    //计算除了场景光源外其他范围内所有无限远光源（环境光），TODO(这里的模拟有问题)
    // spectrum += background;              //加上之后可以看到光源照亮了哪些部分

    //下面开始可能会出现segmentation fault

    //均匀随机采样场景中的一个光源
    float pdfLight = .0f;

    // segmentation问题出在这里
    auto light = scene.sampleLight(sampler->sampler1D(), pdfLight);

    //如果有光源且可以被采样：
    if(light && pdfLight != 0){
        //在光源上采样一个点，并连接shading point和该点，生成一条光线
        auto lightSampleResult = light->sample(intersection, sampler->sampler2D());
        Ray shadowRay(intersection.position, lightSampleResult.direction, EPSILON, lightSampleResult.distance);
        //测试这条光线是否被阻挡
        auto occlude = scene.rayIntersect(shadowRay);
        if(!occlude.has_value()){
            auto material = intersection.shape->material;
            auto bsdf = material->computeBSDF(intersection);
            Spectrum f = bsdf->f(-ray.direction, shadowRay.direction);      //这里已经计算过cos了
            lightSampleResult.pdf *= pdfLight;                          //采样到该光源的概率 * 该光源上采样到该点的概率
            float pdf = convertPDF(lightSampleResult);                  //转换到对面光源积分
            spectrum += lightSampleResult.energy * f / pdf;
        }
    }
    return spectrum;
}