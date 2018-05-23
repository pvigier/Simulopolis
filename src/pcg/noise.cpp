#include "pcg/noise.h"

uint64_t integer_noise(uint64_t i)
{
    i += 0x62a9d9ed799705f5;
    i ^= (i >> 33);
    i *= 0x4be98134a5976fd3;
    i ^= (i >> 33);
    i *= 0x3bc0993a5ad19a13;
    i ^= (i >> 33);
    return i;
}

uint64_t integer_noise_2d(uint64_t i, uint64_t j)
{
    return integer_noise(i + integer_noise(j));
}

uint64_t integer_noise_3d(uint64_t i, uint64_t j, uint64_t k)
{
    return integer_noise(i + integer_noise(j + integer_noise(k)));
}

double noise(uint64_t i)
{
    return static_cast<double>(integer_noise(i)) / 18446744073709551615.0;
}

double noise_2d(uint64_t i, uint64_t j)
{
    return static_cast<double>(integer_noise_2d(i, j)) / 18446744073709551615.0;
}

double noise_3d(uint64_t i, uint64_t j, uint64_t k)
{
    return static_cast<double>(integer_noise_3d(i, j, k)) / 18446744073709551615.0;
}

double poly_interp(double t)
{
    return t * t * t * (10.0 + t * (-15.0 + t * 6.0));
}

double perlin_noise_2d(uint64_t seed, double x, double y)
{
    uint64_t i = static_cast<uint64_t>(x);
    uint64_t j = static_cast<uint64_t>(y);
    double u = x - i;
    double v = y - j;
    // Gradients
    double phi00 = 2 * PI * noise_3d(seed, i    , j);
    double phi10 = 2 * PI * noise_3d(seed, i + 1, j);
    double phi01 = 2 * PI * noise_3d(seed, i    , j + 1);
    double phi11 = 2 * PI * noise_3d(seed, i + 1, j + 1);
    // Ramps
    double n00 = std::cos(phi00) * u          + std::sin(phi00) * v;
    double n10 = std::cos(phi10) * (u - 1.0f) + std::sin(phi10) * v;
    double n01 = std::cos(phi01) * u          + std::sin(phi01) * (v - 1.0f);
    double n11 = std::cos(phi11) * (u - 1.0f) + std::sin(phi11) * (v - 1.0f);
    // Interpolation
    double t0 = poly_interp(u);
    double n0 = (1.0 - t0) * n00 + t0 * n10;
    double n1 = (1.0 - t0) * n01 + t0 * n11;
    double t1 = poly_interp(v);
    return SQRT_2 * ((1.0 - t1) * n0 + t1 * n1);
}

double fractal_noise_2d(uint64_t seed, double x, double y, unsigned int octaves, double lacunarity, double persistence)
{
    double n = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    for (unsigned int i = 0; i < octaves; ++i)
    {
        n += amplitude * perlin_noise_2d(seed, frequency * x, frequency * y);
        frequency *= lacunarity;
        amplitude *= persistence;
    }
    return n;
}
