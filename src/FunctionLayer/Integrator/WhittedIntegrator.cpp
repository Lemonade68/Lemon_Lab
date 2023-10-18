#include"WhittedIntegrator.h"

Spectrum WhittedIntegrator::li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const{
    Spectrum spectrum(.0f), weight(1.f);    //每次反射的权重，注意下面的每次计算都需要在前面乘上
    Spectrum background(.1f, .2f, .4f);

    int turn = 0;       // 限制次数

    while(1) {      //不停循环即可
        ++turn;
        if(turn > max_depth){
            // return Spectrum(1.f, .0f, .0f);     //找bug使用，使用时spp设置为1，凸显为红色
            return Spectrum(.0f);
        }

        auto intersectionOpt = scene.rayIntersect(ray);
        if(!intersectionOpt.has_value()){
            spectrum += weight * background;    //循环体中改成+=（在之前的基础上进行叠加）
            break;
        }

        auto intersection = intersectionOpt.value();
        if (auto light = intersection.shape->light; light)      //自发光
            spectrum += weight * light->evaluateEmission(intersection, -ray.direction);

        auto material = intersection.shape->material;
        auto bsdf = material->computeBSDF(intersection);
        auto bsdfSampleResult = bsdf->sampleShadingPoint(-ray.direction, sampler->sampler2D());

        //如果是镜面材质：直接重新生成反射光线，直接进入下一次循环
        if(bsdfSampleResult.type == BSDFType::Specular){
            ray = Ray(intersection.position, bsdfSampleResult.wi);
            weight *= bsdfSampleResult.weight;      //乘上权重以改变下一次光线的权重
            continue;
        }
        //不是镜面材质的话，采用sample_light的方法，取得该处漫反射的值，并退出循环
        else{
            //环境光部分，暂时不考虑，后续添加    暂时没有按照蒙特卡洛积分公式来直接写
            // spectrum += weight * background * .15f;

            //采样光源
            float pdfLight = .0f;
            auto light = scene.sampleLight(sampler->sampler1D(), pdfLight);
            if(light && pdfLight != .0f){
                auto lightSampleResult = light->sample(intersection, sampler->sampler2D());
                Ray shadowRay(intersection.position, lightSampleResult.direction,
                              EPSILON, lightSampleResult.distance);
                auto occlude = scene.rayIntersect(shadowRay);
                if(!occlude.has_value()){
                    auto f = bsdf->f(-ray.direction, shadowRay.direction);
                    lightSampleResult.pdf *= pdfLight;      //光源上采样到该点概率 * 采样到该光源的概率
                    float pdf = convertPDF(lightSampleResult);      //更改pdf值（变为对光源积分）
                    spectrum += weight * lightSampleResult.energy * f / pdf;
                }
            }
            break;      //直接退出循环
        }
    };

    return spectrum;
}