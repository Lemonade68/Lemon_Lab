#include"Texture.h"

//TODO£∫ÃÌº”Œ¢∑÷
TextureCoord UVMapping::map(const Intersection &intersection) const{
    return {intersection.texCoord};
}
