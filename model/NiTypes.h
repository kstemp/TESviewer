#pragma once

#include <string>
#include <array>
#include <qvector3d.h>

#include "..\io\BinaryIO.h"

typedef uint64_t uint64;
typedef uint32_t ulittle32;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef int16_t BlockTypeIndex;

typedef uint BSLightingShaderType;

typedef uint64 BSVertexDesc;

typedef int32_t ref;

struct NiVersion {

    std::array<int, 4> V;

    void parse(BinaryStreamReader& bsr) {
        V[0] = std::stoi(bsr.readString(2));

        for (int i = 1; i <= 3; ++i) {
            bsr.skip(1); // DOT separator
            V[i] = std::stoi(bsr.readString(1));
        }
    }

};

/* A vector in 3D space(x, y, z). */
template<typename T>
struct TVector3 {
    T x;
    T y;
    T z;

    void parse(BinaryStreamReader& bsr) {
        bsr >> x >> y >> z;
    }

    TVector3<T> operator + (const TVector3<T>& rhs) const {
        return TVector3<T>({ x + rhs.x, y + rhs.y, z + rhs.z });
    }

    TVector3<T> operator - (const TVector3<T>& rhs) {
        return TVector3<T>({ x - rhs.x, y - rhs.y, z - rhs.z });
    }

    TVector3<T>& operator += (const TVector3<T>& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    T length() {
        return std::sqrt(length2());
    }
    
    T length2() {
        return x * x + y * y + z * z;
    }

    const QVector3D toQVector3D() const {
        return { x, y, z };
    }

};

using Vector3 = TVector3<float>;
using ByteVector3 = TVector3<signed char>;

struct Matrix33 {
    float m[9];

    void parse(BinaryStreamReader& bsr) {
        bsr.readIntoVar(m[0], sizeof(float) * 9);
    }
};

struct TexCoord {
    float u, v;

    void parse(BinaryStreamReader& bsr) {
        bsr >> u >> v;
    }
};

std::string readSizedString(BinaryStreamReader& bsr);