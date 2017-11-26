/// -----------------------------------------------
/// 作者：陳俋臣
/// 修改日期：2017/11/20
/// 內容：C++程序呼叫函式宣告
/// -----------------------------------------------


#ifdef PIMCPP_EXPORTS
#define PIMCPP_API __declspec(dllexport)  
#else
#define PIMCPP_API __declspec(dllimport) 
#endif


#pragma once
#include <wchar.h>
#include "PIMCppCore.h"


extern "C" PIMCPP_API bool matchFunc(char* fileName_c, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, bool enableRANSAC, OutputArray result);
