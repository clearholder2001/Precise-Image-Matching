#ifdef PIMCPP_EXPORTS
#define PIMCPP_API __declspec(dllexport)  
#else
#define PIMCPP_API __declspec(dllimport) 
#endif

//#include "rt_nonfinite.h"
//#include "rtwtypes.h"
//#include "omp.h"
//#include "SRFunc_Coder_types.h"

#include <wchar.h>

using namespace std;
using namespace cv;

#pragma once
extern "C" PIMCPP_API bool matchFunc(char* fileName_c, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, OutputArray result);
//extern "C" PIMCPP_API void srFunc(IplImage* LRIm, double Factor, double GuassianSigma, double GuassianKernelSize, double Lamda, OutputArray HRIm);
//extern "C" PIMCPP_API vector<Mat>* projImageSequence(vector<Mat>* imgSeq, vector<Mat>* homoTranSeq, vector<double>* homoRmsSeq, vector<Mat>* mapperTranSeq, vector<double>* mapperRmsSeq);

int errorCheck(vector<int> errorMsg);


//static double argInit_real_T();
//static unsigned char argInit_uint8_T();
//static emxArray_uint8_T *c_argInit_UnboundedxUnboundedx3(IplImage* _LRIm);
//static void main_SRFunc_Coder(IplImage* _LRIm, double Factor, double GuassianSigma, double GuassianKernelSize, double Lamda, OutputArray _HRIm);



