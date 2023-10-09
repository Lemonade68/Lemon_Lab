#pragma once

#include<CoreLayer/Math/Math.h>

// ʹ��RGB��ͨ����ɫ�ռ�
class SpectrumRGB{
public:
    SpectrumRGB(float f = .0f) : rgb(f) {}
    SpectrumRGB(float r, float g, float b) : rgb(r,g,b) {}
    SpectrumRGB(Vector3f _rgb) : rgb(_rgb) {}

    SpectrumRGB operator+(const SpectrumRGB &rhs) const{
        return SpectrumRGB(rgb + rhs.rgb);
    }

    SpectrumRGB &operator+=(const SpectrumRGB &rhs) {
        rgb += rhs.rgb;
        return *this;
    }

    SpectrumRGB operator-(const SpectrumRGB &rhs) const {
        return SpectrumRGB(rgb - rhs.rgb);
    }

    SpectrumRGB &operator-=(const SpectrumRGB &rhs) {
        rgb -= rhs.rgb;
        return *this;
    }

    SpectrumRGB operator*(const SpectrumRGB &rhs) const {
        return SpectrumRGB(rgb * rhs.rgb);      //��λ���
    }

    SpectrumRGB &operator*=(const SpectrumRGB &rhs) {
        rgb *= rhs.rgb;
        return *this;
    }

    SpectrumRGB operator*(float f) const { return SpectrumRGB(rgb * f); }

    SpectrumRGB &operator*=(float f) {
        rgb *= f;
        return *this;
    }

    SpectrumRGB operator/(const SpectrumRGB &rhs) const {
        return SpectrumRGB(rgb / rhs.rgb);      //��λ���
    }

    SpectrumRGB &operator/=(const SpectrumRGB &rhs) {
        rgb /= rhs.rgb;
        return *this;
    }

    SpectrumRGB operator/(float f) const { return SpectrumRGB(rgb / f); }

    SpectrumRGB &operator/=(float f) {
        rgb /= f;
        return *this;
    }

    float operator[](int i) const { return rgb[i]; }

    float &operator[](int i) { return rgb[i]; }

    float x() const { return rgb.x(); }
    float y() const { return rgb.y(); }
    float z() const { return rgb.z(); }
    float &x() { return rgb.x(); }
    float &y() { return rgb.y(); }
    float &z() { return rgb.z(); }

    bool isZero() const { return rgb.isZero(); }

    void debugPrint() const {
        printf("[rgb](");
        for (int i = 0; i < 3; ++i) {
        std::cout << (i == 0 ? '\0' : ',') << rgb[i];
        }
        printf(")%c", '\n');
        fflush(stdout);
    }    

private:
    Vector3f rgb;
};

inline SpectrumRGB operator*(float f, const SpectrumRGB &spectrum) {
    return spectrum * f;
}

inline Vector3f s2v(const SpectrumRGB &spectrum) {
    return Vector3f{spectrum[0], spectrum[1], spectrum[2]};
}

inline SpectrumRGB v2s(const Vector3f &vec) {
    return SpectrumRGB(vec[0], vec[1], vec[2]);
}