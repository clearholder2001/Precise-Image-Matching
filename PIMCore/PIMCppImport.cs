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
    }
}
