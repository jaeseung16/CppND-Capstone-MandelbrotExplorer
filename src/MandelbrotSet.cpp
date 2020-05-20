#include <complex>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <iostream>

#include <opencv2/core/utility.hpp>
#include <opencv2/core/types.hpp>

#include "MandelbrotSet.h"

MandelbrotSet::MandelbrotSet() {

}

MandelbrotSet::MandelbrotSet(std::vector<std::complex<float>> &&zs, int nIteration) {
    _zs = zs;
    _nIteration = nIteration;
    _values = std::vector<int>(_zs.size(), 0);;

    auto start = std::chrono::high_resolution_clock::now();
    parallel_for_(cv::Range(0, 800*800), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            uchar value = (uchar) mandelbrotFormula(this->_zs[r], this->_nIteration);
            _values[r] = value;
        }
    });

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "MandelbrotSet::MandelbrotSet took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

int MandelbrotSet::mandelbrot(const std::complex<float> &z0, const int max)
{
    std::complex<float> z = z0;
    for (int t = 0; t < max; t++)
    {
        if (z.real()*z.real() + z.imag()*z.imag() > 4.0f) return t;
        z = z*z + z0;
    }
    return max;
}

int MandelbrotSet::mandelbrotFormula(const std::complex<float> &z0, const int maxIter=50) {
    int value = mandelbrot(z0, maxIter);
    if(maxIter - value == 0)
    {
        return 0;
    }
    return cvRound(sqrt(value / (float) maxIter) * 255);
}