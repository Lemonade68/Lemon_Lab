#pragma once
#include"ConstantTexture.h"
#include"CoreLayer/ColorSpace/Spectrum.h"

class CheckerTexture : public Texture<Spectrum>{
public:
    CheckerTexture(std::shared_ptr<Texture<Spectrum>> _even = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.5f)),
                   std::shared_ptr<Texture<Spectrum>> _odd = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.1f)))
        : even(_even), odd(_odd) {}

    virtual Spectrum evaluate(const TextureCoord &texCoord, const Point3f &position) const override{
        auto sines = sin(10 * position[0]) * sin(10 * position[1]) * sin(10 * position[2]);
        if(sines < 0)
            return even->evaluate(texCoord,position);
        else
            return odd->evaluate(texCoord,position);
    }

    virtual Spectrum evaluate(const Intersection &intersection) const override{
        auto texCoord = mapping->map(intersection);
        return evaluate(texCoord, intersection.position);
    }

private:
    std::shared_ptr<Texture<Spectrum>> even;
    std::shared_ptr<Texture<Spectrum>> odd;
};