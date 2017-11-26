/// -----------------------------------------------
/// 作者：蔡易澄、陳俋臣
/// 修改日期：2017/11/20
/// 內容：核心函式宣告
/// -----------------------------------------------


#pragma once
#include <omp.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/reg/mapaffine.hpp"
#include "opencv2/reg/mapshift.hpp"
#include "opencv2/reg/mapprojec.hpp"
#include "opencv2/reg/mappergradshift.hpp"
#include "opencv2/reg/mappergradeuclid.hpp"
#include "opencv2/reg/mappergradsimilar.hpp"
#include "opencv2/reg/mappergradaffine.hpp"
#include "opencv2/reg/mappergradproj.hpp"
#include "opencv2/reg/mapperpyramid.hpp"


using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::reg;


class imageproc
{
public:
	static Mat readfile(wstring, double, double, int &);
	static Mat findfeature(int, int, int, bool, bool, Mat, vector<KeyPoint> &, Mat &, int &);
	static Mat homoresult(Mat, Mat, vector<KeyPoint>, vector<KeyPoint>, Mat, Mat, double, vector<DMatch> &, Mat &, int &);
	static Mat homoresultnoRANSAC(Mat, Mat, vector<KeyPoint>, vector<KeyPoint>, Mat, Mat, double, vector<DMatch> &, Mat &, int &);
	static Mat mapperresult(Mat, Mat, Mat, Mat &, int &);
	static Mat jointimage(Mat, Mat, Mat, Mat, int &, bool);
	static Mat colordifferent(Mat, Mat, Mat, Mat, double, double, int &);
	static vector<Point2f> mat2point(Mat, int &);
	static vector<Point2f> tranpoint(vector<Point2f>, Mat, int &);
	static void mixpoint(vector<vector<Point2f>> &, vector<Point2f>, vector<Mat> &, Mat, int &);
	static Mat point2mat(vector<vector<Point2f>>, vector<Mat>, int &);
	static double homoRMS(Mat, Mat, Mat, int &, double);
	static double mapperRMS(Mat, Mat, Mat, Mat, int &, double);
	static vector<Mat> readallfile(vector<wstring>, vector<int> &);
	static vector<Mat> findallfeature(int, int, int, bool, bool, vector<Mat>, vector<vector<KeyPoint>> &, vector<Mat> &, vector<int> &);
	static vector<Mat> homoallresult(vector<Mat>, vector<vector<KeyPoint>>, vector<Mat>, double, vector<vector<DMatch>> &, vector<Mat> &, vector<double> &, double, vector<double> &, vector<int> &, vector<int> &, int &);
	static vector<Mat> homoallresultnoRANSAC(vector<Mat>, vector<vector<KeyPoint>>, vector<Mat>, double, vector<vector<DMatch>> &, vector<Mat> &, vector<double> &, double, vector<double> &, vector<int> &, vector<int> &, int &);
	static vector<Mat> mapperallresult(vector<Mat>, vector<Mat>, vector<Mat> &, vector<double> &, vector<double>, vector<int> &, vector<int> &);
	static Mat jointallimage(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &, vector<int> &, int &);
	static Mat jointallimageback(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &, vector<int> &, int &);
	static vector<Mat> allcolordifferent(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &);
};