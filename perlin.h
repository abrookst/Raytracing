#ifndef PERLIN_H
#define PERLIN_H

#include "common.h"

class Perlin
{
public:
    Perlin()
    {
        for (int i = 0; i < pointCount; i++)
        {
            randVec[i] = unit_vector(Vector3::random(-1,1));
        }

        perlin_generate_perm(permX);
        perlin_generate_perm(permY);
        perlin_generate_perm(permZ);
    }

    float noise(const Point3 &p) const
    {
        float u = p.x() - std::floor(p.x());
        float v = p.y() - std::floor(p.y());
        float w = p.z() - std::floor(p.z());

        int i = int(std::floor(p.x()));
        int j = int(std::floor(p.y()));
        int k = int(std::floor(p.z()));
        Vector3 c[2][2][2];

        for (int di=0; di < 2; di++){
            for (int dj=0; dj < 2; dj++){
                for (int dk=0; dk < 2; dk++){
                    c[di][dj][dk] = randVec[
                        permX[(i+di) & 255] ^
                        permY[(j+dj) & 255] ^
                        permZ[(k+dk) & 255]
                    ];
                }  
            }
        }
            

        return perlin_interp(c, u, v, w);
    }

        double turb(const Point3& p, int depth) const {
        float accum = 0.0;
        Point3 temp = p;
        float weight = 1.0;

        for (int i = 0; i < depth; i++) {
            accum += weight * noise(temp);
            weight *= 0.5;
            temp *= 2;
        }

        return std::fabs(accum);
    }

private:
    static const int pointCount = 256;
    Vector3 randVec[pointCount];
    int permX[pointCount];
    int permY[pointCount];
    int permZ[pointCount];

    static void perlin_generate_perm(int *p)
    {
        for (int i = 0; i < pointCount; i++)
            p[i] = i;

        permute(p, pointCount);
    }

    static void permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static float perlin_interp(const Vector3 c[2][2][2], float u, float v, float w) {
        float uu = u*u*(3-2*u);
        float vv = v*v*(3-2*v);
        float ww = w*w*(3-2*w);
        float accum = 0.0;

        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++) {
                    Vector3 weight_v(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))
                           * (j*vv + (1-j)*(1-vv))
                           * (k*ww + (1-k)*(1-ww))
                           * dot(c[i][j][k], weight_v);
                }

        return accum;
    }
};

#endif