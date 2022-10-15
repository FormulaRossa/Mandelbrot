#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "lodepng.h"
#include <cmath>

using namespace std;

int julia(long double real, long double imag, double a, double b, int limit) {
    long double zReal = real;
    long double zImag = imag;

    for (int i = 0; i < limit; ++i) {
        long double r2 = zReal * zReal;
        long double i2 = zImag * zImag;

        if (r2 + i2 > 4.0) return i;

        zImag = 2.0 * zReal * zImag + b;
        zReal = r2 - i2 + a;
    }
    return limit;
}

int mandelbrot(long double real, long double imag, int limit) {
    long double zReal = real;
    long double zImag = imag;

    for (int i = 0; i < limit; ++i) {
        long double r2 = zReal * zReal;
        long double i2 = zImag * zImag;

        if (r2 + i2 > 4.0) return i;

        zImag = 2.0 * zReal * zImag + imag;
        zReal = r2 - i2 + real;
    }
    return limit;
}

void encodeOneStep(const char *filename, std::vector<unsigned char> &image, unsigned width, unsigned height) {
    std::vector<unsigned char> png;

    unsigned error = lodepng::encode(png, image, width, height);
    for (char c: png) {
        cout << c;
    }
}

struct RGB {
    int r;
    int g;
    int b;
};

RGB HSVtoRGB(float H, float S, float V) {
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    } else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    } else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    } else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    } else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    } else {
        r = C, g = 0, b = X;
    }
    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;

    RGB rgb;
    rgb.r = R;
    rgb.g = G;
    rgb.b = B;
    return rgb;
}


int main() {

    char *path = std::getenv("PATH_INFO");

    char *z = strtok(path, "/");
    char *x = strtok(NULL, "/");
    char *y = strtok(NULL, ".");

    int tileZ;
    int tileX;
    int tileY;

    sscanf(z, "%d", &tileZ);
    sscanf(x, "%d", &tileX);
    sscanf(y, "%d", &tileY);

    int resolution;


    resolution = 256;


    long double zoomFactor = pow(2, tileZ);


    long double dTile = 4 / zoomFactor;


    long double dPixel = dTile / resolution;
    const char *filename = "./test.png";

    std::vector<unsigned char> image;
    image.resize(resolution * resolution * 4);

    int fractalX = floor((tileX) / zoomFactor);
    int fractalY = floor((tileY) / zoomFactor);


    long double x_start = -2.0 + dTile * tileX - 4 * fractalX;
    long double y_fin = 2.0 - dTile * tileY + 4 * fractalY;


    for (int y = 0; y < resolution; y++) {
        for (int x = 0; x < resolution; x++) {

            if (tileZ < 0) {
                fractalX = floor((tileX) / zoomFactor);
                fractalY = floor((tileY) / zoomFactor);

                fractalX += floor(x / zoomFactor / resolution);
                fractalY += floor(y / zoomFactor / resolution);

                x_start = -2.0 + dTile * tileX - 4 * fractalX;
                y_fin = 2.0 - dTile * tileY + 4 * fractalY;
            }

            long double mandel_x = x_start + x * dPixel;
            long double mandel_y = y_fin - y * dPixel;

            int colorValue;
            int limit = 512;
            if (fractalX == -1 && fractalY == -1) {
                colorValue = mandelbrot(mandel_x, mandel_y, limit);
            } else {
                colorValue = julia(mandel_x, mandel_y, fractalX * 0.01 + 0.2, fractalY * 0.01, limit);
            }
            float hue = 255 * colorValue / limit;
            float saturation = 255;
            float value;
            if (colorValue < 100) {
                value = 255;
            } else {
                value = 0;
            }
            RGB rgb = HSVtoRGB(hue, saturation, value);

            image[4 * resolution * y + 4 * x + 0] = rgb.r;
            image[4 * resolution * y + 4 * x + 1] = rgb.g;
            image[4 * resolution * y + 4 * x + 2] = rgb.b;
            image[4 * resolution * y + 4 * x + 3] = 255;
        }
    }

    cout << "Content-type:image/png\r\n\r\n";

    encodeOneStep(filename, image, resolution, resolution);
    return 0;
}