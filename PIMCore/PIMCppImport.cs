using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using Emgu.CV;
using Emgu.CV.Util;

namespace PIMCore
{
    public static class PIMCppImport
    {
        [DllImport("PIMCpp.dll", EntryPoint = "matchFunc", CallingConvention = CvInvoke.CvCallingConvention)]
        public static extern bool matchFunc(String fileName, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, IntPtr result);

//        [DllImport("PIMCpp.dll", EntryPoint = "srFunc", CallingConvention = CvInvoke.CvCallingConvention)]
//        public static extern void srFunc(IntPtr LRIm, double Factor, double GuassianSigma, double GuassianKernelSize, double Lamda, IntPtr HRIm);


//        [DllImport("PIMCpp.dll", EntryPoint = "projImageSequence", CallingConvention = CvInvoke.CvCallingConvention)]
//        public static extern IntPtr projImageSequence(IntPtr imgSeq, IntPtr homoTranSeq, IntPtr homoRmsSeq, IntPtr mapperTranSeq, IntPtr mapperRmsSeq);
    }
}
