#include"Texture.h"

//TODO�����΢��
TextureCoord UVMapping::map(const Intersection &intersection) const{
    return {intersection.texCoord};
}
