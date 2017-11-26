/// -----------------------------------------------
/// 作者：陳俋臣
/// 修改日期：2017/11/20
/// 內容：引用C++ DLL函式之宣告
/// -----------------------------------------------


using System;
using System.Runtime.InteropServices;

using Emgu.CV;


namespace PIMCore
{
    public static class PIMCppImport
    {
        [DllImport("PIMCpp.dll", CallingConvention = CvInvoke.CvCallingConvention)]
        public static extern bool matchFunc(String fileName, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, bool enableRANSAC, IntPtr result);
    }
}
