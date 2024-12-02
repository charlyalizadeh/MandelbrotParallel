#include "../include/MandelbrotSingleThread.hpp"


#include <cstdio>
void computeMandelbrotSingleThread(const sf::Vector2i& origin,
                                   const sf::Vector2i& dimension,
                                   const sf::Vector2<double>& pixelDimensions,
                                   int precision,
                                   unsigned char *pixelColors)
{
    int mandelbrotValue;
    float ratio;

    for(int x = 0; x < dimension.x; x++)
    {
        for(int y = 0; y < dimension.y; y++)
        {
            mandelbrotValue = computeMandelbrotPixel(x, y, origin.x, origin.y, pixelDimensions.x, pixelDimensions.y, precision);
            ratio = (float)mandelbrotValue / precision * 5;
            if(mandelbrotValue == precision)
                colorPixel(pixelColors, (y * dimension.x + x) * 4, 255, 255, 255, 255);
            else
                colorPixel(pixelColors, (y * dimension.x + x) * 4, 0, 0, 0, 255);
        }
    }
}


#include <cstdio>
void computeMandelbrotSingleThreadIntrinsic(const sf::Vector2i& origin,
                                            const sf::Vector2i& dimension,
                                            const sf::Vector2<double>& pixelDimensions,
                                            int precision,
                                            unsigned char *pixelColors)
{
    double xScale = -origin.x * pixelDimensions.x;
    double yScale = -origin.y * pixelDimensions.y;

    __m256d _cR, _cI, _zR, _zI, _zR2, _zI2;
    __m256d _newzR, _newzI;
    __m256d _two;
    __m256d _xStart, _xDimension, _xOffsets, _xJumps;

    __m256i _i;
    __m256i _one;
    __m256i _precision;

    __m256i _maskPrecision;
    __m256d _maskCriterion, _zR2addzI2;
    __m256d _four;
    __m256i _addOne;

    _one = _mm256_set1_epi64x(1);
    _two = _mm256_set1_pd(2.0);
    _four = _mm256_set1_pd(4.0);
    _precision = _mm256_set1_epi64x(precision);

    _xStart = _mm256_set1_pd(xScale);
    _xDimension = _mm256_set1_pd(pixelDimensions.x);
    _xOffsets = _mm256_set_pd(0, 1, 2, 3);
    _xOffsets = _mm256_mul_pd(_xOffsets, _xDimension);
    _xStart = _mm256_add_pd(_xStart, _xOffsets);
    _xJumps = _mm256_set1_pd(pixelDimensions.x * 4);
    for(int y = 0; y < dimension.y; y++)
    {
        _cI = _mm256_set1_pd(yScale);
        _cR = _xStart;
        for(int x = 0; x < dimension.x; x+=4)
        {
			_i = _mm256_setzero_si256();
			_zR = _mm256_setzero_pd();
			_zI = _mm256_setzero_pd();

        repeat:
            _zR2 = _mm256_mul_pd(_zR, _zR);
            _zI2 = _mm256_mul_pd(_zI, _zI);

            _newzR = _mm256_sub_pd(_zR2, _zI2);
            _newzR = _mm256_add_pd(_newzR, _cR);

            _newzI = _mm256_mul_pd(_zR, _zI);
            _newzI = _mm256_fmadd_pd(_newzI, _two, _cI);

            _zR = _newzR;
            _zI = _newzI;

            _maskPrecision = _mm256_cmpgt_epi64(_precision, _i);
            _zR2addzI2 = _mm256_add_pd(_zR2, _zI2);
            _maskCriterion = _mm256_cmp_pd(_zR2addzI2, _four, _CMP_LT_OQ);
            _maskPrecision = _mm256_and_si256(_maskPrecision, _mm256_castpd_si256(_maskCriterion));

            _addOne = _mm256_and_si256(_one, _maskPrecision);

            _i = _mm256_add_epi64(_i, _addOne);

            if(_mm256_movemask_pd(_mm256_castsi256_pd(_maskPrecision)) > 0)
                goto repeat;

            // Coloring
            for(int i = 0; i < 4; i++)
            {
                if(int(_i[i]) == precision)
                    colorPixel(pixelColors, (y * dimension.x + x + 3 - i) * 4, 255, 255, 255, 255);
                else
                    colorPixel(pixelColors, (y * dimension.x + x + 3 - i) * 4, 0, 0, 0, 255);
            }
            _cR = _mm256_add_pd(_cR, _xJumps);
        }
        yScale += pixelDimensions.y;
    }
}
