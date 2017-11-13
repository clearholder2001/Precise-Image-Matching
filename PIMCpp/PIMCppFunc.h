#ifdef PIMCPP_EXPORTS
#define PIMCPP_API __declspec(dllexport)  
#else
#define PIMCPP_API __declspec(dllimport) 
#endif

#include <wchar.h>

using namespace std;
using namespace cv;


#pragma once
extern "C" PIMCPP_API bool matchFunc(char* fileName_c, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, OutputArray result);