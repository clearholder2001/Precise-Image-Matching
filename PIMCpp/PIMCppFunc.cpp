#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

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

//#include "rt_nonfinite.h"
//#include "SRFunc_Coder.h"
//#include "SRFunc_Coder_terminate.h"
//#include "SRFunc_Coder_emxAPI.h"
//#include "SRFunc_Coder_initialize.h"

#include "PIMCppFunc.h"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::reg;
using namespace std;
using namespace System;
using namespace System::IO;

#pragma region Yi-Cheng
static class imageproc
{
public:
	Mat readfile(wstring, double, double, int &);
	Mat findfeature(int, int, int, bool, bool, Mat, vector<KeyPoint> &, Mat &, int &);
	Mat homoresult(Mat, Mat, vector<KeyPoint>, vector<KeyPoint>, Mat, Mat, double, vector<DMatch> &, Mat &, int &);
	Mat mapperresult(Mat, Mat, Mat, Mat &, int &);
	Mat jointimage(Mat, Mat, Mat, Mat, int &, bool);
	Mat colordifferent(Mat, Mat, Mat, Mat, double, double, int &);
	vector<Point2f> mat2point(Mat, int &);
	vector<Point2f> tranpoint(vector<Point2f>, Mat, int &);
	void mixpoint(vector<vector<Point2f>> &, vector<Point2f>, vector<Mat> &, Mat, int &);
	Mat point2mat(vector<vector<Point2f>>, vector<Mat>, int &);
	double homoRMS(Mat, Mat, Mat, int &, double);
	double mapperRMS(Mat, Mat, Mat, Mat, int &, double);
	vector<Mat> readallfile(vector<wstring>, vector<int> &);
	vector<Mat> findallfeatur(int, int, int, bool, bool, vector<Mat>, vector<vector<KeyPoint>> &, vector<Mat> &, vector<int> &);
	vector<Mat> homoallresult(vector<Mat>, vector<vector<KeyPoint>>, vector<Mat>, double, vector<vector<DMatch>> &, vector<Mat> &, vector<double> &, double, vector<double> &, vector<int> &, vector<int> &, int &);
	vector<Mat> mapperallresult(vector<Mat>, vector<Mat>, vector<Mat> &, vector<double> &, vector<double>, vector<int> &, vector<int> &);
	Mat jointallimage(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &, vector<int> &, int &);
	vector<Mat> allcolordifferent(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &);
};


Mat imageproc::findfeature(int minhessian, int Octave, int Octavelayer, bool extend, bool USURF, Mat img, vector<KeyPoint> &keypoint, Mat &descriptor, int &warning)
{
	if (minhessian < 0 || minhessian>3000)
	{
		warning = 1;
		//cout << "minhessian需介於0~3000" << endl;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (Octave < 2 || Octave>10)
	{
		warning = 2;
		//cout << "Octave需介於2~10" << endl;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (Octavelayer < 2 || Octavelayer > 10)
	{
		warning = 3;
		//cout << "Octavelayer需介於2~10" << endl;

		Mat error(0, 0, CV_8U);
		return error;
	}

	keypoint.clear();

	warning = 0;



	Ptr<SURF> detector = SURF::create(minhessian, Octave, Octavelayer, extend, USURF);

	detector->detectAndCompute(img, Mat(), keypoint, descriptor, false);

	cout << keypoint.size() << endl;

	/*double des = 0;
	for (int i = 0; i < descriptor.rows; i++)
	{
	des=des+descriptor.data[i,0];
	}
	cout << des << endl;*/

	Mat detect;
	drawKeypoints(img, keypoint, detect, Scalar(100, 150, 0, 0));


	return detect;

}

Mat imageproc::homoresult(Mat img1, Mat img2, vector<KeyPoint> keypoint1, vector<KeyPoint> keypoint2, Mat descriptor1, Mat descriptor2,
	double multiple, vector<DMatch> &homomatch, Mat &homotran, int &warning)
{
	if (!img1.data || !img2.data)
	{
		//cout << "輸入影像失敗錯誤~！" << endl;
		warning = 1;

		Mat error(0, 0, CV_8U);
		return error;
	}


	if (keypoint1.size() == 0 || keypoint2.size() == 0 || !descriptor1.data || !descriptor2.data)
	{
		//cout << "輸入特徵點資料失敗！" << endl;
		warning = 2;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (multiple <= 1)
	{
		//cout << "門檻值倍率需大於1！" << endl;
		warning = 3;

		Mat error(0, 0, CV_8U);
		return error;
	}
	warning = 0;


	FlannBasedMatcher matcher/*(NORM_L1)*/;
	vector<vector<DMatch> > matches;
	vector<DMatch> match1;
	matches.clear();
	match1.clear();


	matcher.knnMatch(descriptor1, descriptor2, matches, 1);

	match1.resize(matches.size());
	for (int i = 0; i < matches.size(); i++)
	{
		match1[i] = matches[i][0];

	}

	double min_dist = 100;
	double max_dist = 0;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		double dist = match1[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	cout << match1.size() << endl;
	cout << max_dist - min_dist << endl;


	vector<DMatch> good_match;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		if (match1[i].distance < multiple * min_dist)
		{
			good_match.push_back(match1[i]);
		}
	}
	cout << "multipleSIZE:	" << good_match.size() << endl;


	vector<Point2f> point1, point2;
	for (unsigned int i = 0; i < good_match.size(); i++)
	{
		point1.push_back(keypoint1[good_match[i].queryIdx].pt);
		point2.push_back(keypoint2[good_match[i].trainIdx].pt);
	}

	Mat mask;
	double a = 3;

	homotran = findHomography(point2, point1, CV_RANSAC, a, mask, 2000, 0.9999);
	cout << homotran << endl;
	homomatch.clear();




	for (int i = 0; i < good_match.size(); i++)
	{
		if (mask.at<uchar>(i, 0) != 0)
		{
			homomatch.push_back(good_match[i]);
		}
	}
	if (!homotran.data)
	{
		//cout << "findhomography失敗" << endl;
		warning = 6;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (homomatch.size() < 9)
	{
		//cout << "RANSAC後匹配點組數過少" << endl;
		warning = 4;

		Mat error(0, 0, CV_8U);
		return error;
	}

	Mat imghomomatch;
	drawMatches(img1, keypoint1, img2, keypoint2, homomatch, imghomomatch, Scalar(100, 150, 0, 0), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);



	return imghomomatch;


}
Mat imageproc::jointimage(Mat left, Mat right, Mat transform, Mat mapperhomo, int &warning, bool oneup)
{
	if (!left.data || !right.data)
	{
		cout << "輸入影像失敗錯誤~！" << endl;
		warning = 1;
		system("pause");
		exit(0);
	}
	if (!transform.data)
	{
		cout << "輸入homography轉換矩陣失敗錯誤~！" << endl;
		warning = 2;
		system("pause");
		exit(0);
	}
	if (!mapperhomo.data)
	{
		cout << "輸入homography轉換矩陣失敗錯誤~！" << endl;
		warning = 3;
		system("pause");
		exit(0);
	}
	warning = 0;


	vector<Point2f> fourpoint;
	Point2f corner;
	corner.x = 0;
	corner.y = 0;
	fourpoint.push_back(corner);
	corner.x = right.cols;
	corner.y = 0;
	fourpoint.push_back(corner);
	corner.x = 0;
	corner.y = right.rows;
	fourpoint.push_back(corner);
	corner.x = right.cols;
	corner.y = right.rows;
	fourpoint.push_back(corner);

	vector<Point2f> trans;

	perspectiveTransform(fourpoint, trans, transform);

	int tranrow = 0, trancol = 0;
	for (int i = 0; i < 4; i++)
	{
		if (trans[i].x > trancol)
			trancol = trans[i].x;
		if (trans[i].y > tranrow)
			tranrow = trans[i].y;
	}
	if (trancol < left.cols)
		trancol = left.cols;
	if (tranrow < left.rows)
		tranrow = left.rows;

	Mat imgtran(tranrow, trancol, CV_8UC3);
	warpPerspective(right, imgtran, transform, imgtran.size(), INTER_AREA);

	warpPerspective(imgtran, imgtran, mapperhomo, imgtran.size(), INTER_AREA);


	for (int count = 0; count < 3; count++)
	{
		Mat temp = imgtran.clone();
		for (int i = 1; i < imgtran.rows - 1; i++)
			for (int j = 1; j < imgtran.cols - 1; j++)
			{
				if (imgtran.at<Vec3b>(i, j)[0] == 0 && imgtran.at<Vec3b>(i, j)[1] == 0 && imgtran.at<Vec3b>(i, j)[2] == 0)
				{
					for (int m = -1; m < 2; m++)
						for (int n = -1; n < 2; n++)
						{
							temp.at<Vec3b>(i + m, j + n)[0] = 0;
							temp.at<Vec3b>(i + m, j + n)[1] = 0;
							temp.at<Vec3b>(i + m, j + n)[2] = 0;
						}
				}

			}
		imgtran = temp.clone();
	}

	Mat imgjoint(tranrow, trancol, CV_8UC3);

	for (int i = 0; i < imgjoint.rows; i++)
		for (int j = 0; j < imgjoint.cols; j++)
		{
			imgjoint.at<Vec3b>(i, j)[0] = 0;
			imgjoint.at<Vec3b>(i, j)[1] = 0;
			imgjoint.at<Vec3b>(i, j)[2] = 0;
		}

	if (oneup == true)
	{
		for (int i = 0; i < imgtran.rows; i++)
			for (int j = 0; j < imgtran.cols; j++)
			{
				if (imgtran.at<Vec3b>(i, j)[0] != 0 && imgtran.at<Vec3b>(i, j)[1] != 0 && imgtran.at<Vec3b>(i, j)[2] != 0)
				{
					imgjoint.at<Vec3b>(i, j) = imgtran.at<Vec3b>(i, j);
				}
			}

		for (int i = 0; i < left.rows; i++)
			for (int j = 0; j < left.cols; j++)
			{
				if (left.at<Vec3b>(i, j)[0] != 0 && left.at<Vec3b>(i, j)[1] != 0 && left.at<Vec3b>(i, j)[2] != 0)
				{
					imgjoint.at<Vec3b>(i, j) = left.at<Vec3b>(i, j);
				}
			}
	}
	else
	{
		for (int i = 0; i < left.rows; i++)
			for (int j = 0; j < left.cols; j++)
			{
				if (left.at<Vec3b>(i, j)[0] != 0 && left.at<Vec3b>(i, j)[1] != 0 && left.at<Vec3b>(i, j)[2] != 0)
				{
					imgjoint.at<Vec3b>(i, j) = left.at<Vec3b>(i, j);
				}
			}
		for (int i = 0; i < imgtran.rows; i++)
			for (int j = 0; j < imgtran.cols; j++)
			{
				if (imgtran.at<Vec3b>(i, j)[0] != 0 && imgtran.at<Vec3b>(i, j)[1] != 0 && imgtran.at<Vec3b>(i, j)[2] != 0)
				{
					imgjoint.at<Vec3b>(i, j) = imgtran.at<Vec3b>(i, j);
				}
			}

	}
	return imgjoint;

}

Mat imageproc::mapperresult(Mat image1, Mat image2, Mat homotran, Mat &mapperhomo, int &warning)
{
	if (!image1.data || !image2.data)
	{
		//cout << "輸入影像失敗錯誤~！" << endl;
		warning = 1;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (!homotran.data)
	{
		//cout << "輸入homography轉換矩陣失敗錯誤~！" << endl;
		warning = 2;

		Mat error(0, 0, CV_8U);
		return error;
	}



	warning = 0;


	vector<Point2f> fourpoint1;
	Point2f corner;
	corner.x = 0;
	corner.y = 0;
	fourpoint1.push_back(corner);
	corner.x = image2.cols;
	corner.y = 0;
	fourpoint1.push_back(corner);
	corner.x = 0;
	corner.y = image2.rows;
	fourpoint1.push_back(corner);
	corner.x = image2.cols;
	corner.y = image2.rows;
	fourpoint1.push_back(corner);

	vector<Point2f> trans1;

	perspectiveTransform(fourpoint1, trans1, homotran);

	int tranrow = 0, trancol = 0;
	for (int i = 0; i < 4; i++)
	{
		if (trans1[i].x > trancol)
			trancol = trans1[i].x;
		if (trans1[i].y > tranrow)
			tranrow = trans1[i].y;
	}


	Mat imghomotran(tranrow, trancol, CV_8UC3);
	warpPerspective(image2, imghomotran, homotran, imghomotran.size(), INTER_AREA);


	vector<Point2f> fourpoint;
	//Point2f corner;
	corner.x = 0.2*image2.cols;
	corner.y = 0.2*image2.rows;
	fourpoint.push_back(corner);
	corner.x = 0.8*image2.cols;
	corner.y = 0.8*image2.rows;
	fourpoint.push_back(corner);
	corner.x = 0.2*image2.cols;
	corner.y = 0.8*image2.rows;
	fourpoint.push_back(corner);
	corner.x = 0.8*image2.cols;
	corner.y = 0.8*image2.rows;
	fourpoint.push_back(corner);

	vector<Point2f> trans;

	perspectiveTransform(fourpoint, trans, homotran);

	int minx = 10000000, maxx = -10000000, miny = 10000000, maxy = -10000000;
	for (int i = 0; i < 4; i++)
	{
		if (trans[i].x > maxx)
			maxx = trans[i].x;
		if (trans[i].x < minx)
			minx = trans[i].x;
		if (trans[i].y > maxy)
			maxy = trans[i].y;
		if (trans[i].y < miny)
			miny = trans[i].y;
	}

	if (minx <= image1.cols*0.1)
		minx = image1.cols*0.1;
	if (miny <= image1.rows*0.1)
		miny = image1.rows*0.1;
	if (maxx > image1.cols*0.9)
		maxx = image1.cols*0.9 - 1;
	if (maxy > image1.rows*0.9)
		maxy = image1.rows*0.9 - 1;
	if (maxx < minx)
		maxx = minx;
	if (maxy < miny)
		maxy = miny;

	Mat sp1(maxy - miny + 1, maxx - minx + 1, CV_8UC3);
	Mat	sp2(maxy - miny + 1, maxx - minx + 1, CV_8UC3);

	for (int i = 0; i < sp1.rows; i++)
		for (int j = 0; j < sp1.cols; j++)
		{
			sp1.at<Vec3b>(i, j) = image1.at<Vec3b>(miny + i, minx + j);
			sp2.at<Vec3b>(i, j) = imghomotran.at<Vec3b>(miny + i, minx + j);
		}

	sp1.convertTo(sp1, CV_64FC3);
	sp2.convertTo(sp2, CV_64FC3);

	Ptr<MapperGradProj> mapper = makePtr<MapperGradProj>();
	MapperPyramid mappPyr(mapper);
	Ptr<Map> mapPtr = mappPyr.calculate(sp1, sp2);

	Ptr<MapProjec> mapProj = MapTypeCaster::toProjec(mapPtr);

	mapProj->normalize();

	Mat(mapProj->getProjTr()).copyTo(mapperhomo);

	Mat dest(imghomotran.rows, imghomotran.cols, CV_8UC3);
	warpPerspective(imghomotran, dest, mapperhomo, dest.size(), INTER_AREA);


	return dest;
}

Mat imageproc::readfile(wstring image, double res1, double res2, int &warning)
{
	if (res1 <= 0 || res2 <= 0)
	{
		warning = 1;
		//cout << "resolution需大於0" << endl;

		Mat error(0, 0, CV_8U);
		return error;
	}

	//為了相容中文路徑
	FILE* fp = _wfopen(image.c_str(), L"rb");
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	char* buf = new char[sz];
	fseek(fp, 0, SEEK_SET);
	long n = fread(buf, 1, sz, fp);
	_InputArray arr(buf, sz);
	Mat img = imdecode(arr, CV_LOAD_IMAGE_ANYCOLOR);
	delete[] buf;
	fclose(fp);

	//Mat img = imread(image, 1);

	if (!img.data)
	{
		//cout << "讀取" << image << "錯誤~！" << endl;
		warning = 2;

		Mat error(0, 0, CV_8U);
		return error;
	}

	resize(img, img, Size(), res1 / res2, res1 / res2, INTER_LINEAR);
	return img;
}

Mat imageproc::colordifferent(Mat left, Mat right, Mat homotran, Mat mapperhomo, double homoRMS, double mapperRMS, int &warning)
{
	if (!left.data || !right.data)
	{
		cout << "輸入影像失敗錯誤~！" << endl;
		warning = 1;

		Mat error(0, 0, CV_8U);
		return error;

	}
	if (!homotran.data)
	{
		cout << "輸入homography轉換矩陣失敗錯誤~！" << endl;
		warning = 2;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (!mapperhomo.data)
	{
		cout << "輸入homography轉換矩陣失敗錯誤~！" << endl;
		warning = 3;

		Mat error(0, 0, CV_8U);
		return error;
	}
	warning = 0;


	vector<Point2f> fourpoint;
	Point2f corner;
	corner.x = 0;
	corner.y = 0;
	fourpoint.push_back(corner);
	corner.x = right.cols;
	corner.y = 0;
	fourpoint.push_back(corner);
	corner.x = 0;
	corner.y = right.rows;
	fourpoint.push_back(corner);
	corner.x = right.cols;
	corner.y = right.rows;
	fourpoint.push_back(corner);

	vector<Point2f> trans;

	perspectiveTransform(fourpoint, trans, homotran);

	int tranrow = 0, trancol = 0;
	for (int i = 0; i < 4; i++)
	{
		if (trans[i].x > trancol)
			trancol = trans[i].x;
		if (trans[i].y > tranrow)
			tranrow = trans[i].y;
	}
	if (trancol < left.cols)
		trancol = left.cols;
	if (tranrow < left.rows)
		tranrow = left.rows;

	Mat imgtran(tranrow, trancol, CV_8UC3);
	if (homoRMS > mapperRMS)
	{
		warpPerspective(right, imgtran, homotran, imgtran.size(), INTER_AREA);
		warpPerspective(imgtran, imgtran, mapperhomo, imgtran.size(), INTER_AREA);
	}
	else
	{
		warpPerspective(right, imgtran, homotran, imgtran.size(), INTER_AREA);
	}
	Mat colordifferent(tranrow, trancol, CV_8UC3);
	for (int i = 0; i < colordifferent.rows; i++)
		for (int j = 0; j < colordifferent.cols; j++)
		{
			colordifferent.at<Vec3b>(i, j)[0] = 0;
			colordifferent.at<Vec3b>(i, j)[1] = 0;
			colordifferent.at<Vec3b>(i, j)[2] = 0;
		}

	for (int i = 0; i < left.rows; i++)
		for (int j = 0; j < left.cols; j++)
		{
			colordifferent.at<Vec3b>(i, j)[0] = left.at<Vec3b>(i, j)[0];
		}
	for (int i = 0; i < imgtran.rows; i++)
		for (int j = 0; j < imgtran.cols; j++)
		{
			colordifferent.at<Vec3b>(i, j)[1] = imgtran.at<Vec3b>(i, j)[1];
			colordifferent.at<Vec3b>(i, j)[2] = imgtran.at<Vec3b>(i, j)[2];
		}

	return colordifferent;


}

double imageproc::homoRMS(Mat image1, Mat image2, Mat homotran, int &warning, double multiple)
{
	if (!image1.data || !image2.data)
	{
		//cout << "輸入影像失敗錯誤~！" << endl;
		warning = 1;


		return 100;
	}

	if (multiple <= 1)
	{
		//cout << "門檻值倍率需大於1！" << endl;
		warning = 2;


		return 100;
	}

	warning = 0;
	Mat firsttran;
	warpPerspective(image2, firsttran, homotran, image2.size(), INTER_AREA);

	Ptr<SURF> detector = SURF::create(600, 3, 3, true, false);

	vector<KeyPoint> key1;
	vector<KeyPoint> key2;
	Mat des1;
	Mat des2;
	detector->detectAndCompute(image1, Mat(), key1, des1, false);
	detector->detectAndCompute(firsttran, Mat(), key2, des2, false);

	FlannBasedMatcher matcher/*(NORM_L1)*/;
	vector<vector<DMatch> > matches;
	vector<DMatch> match1;
	matches.clear();
	match1.clear();
	if (key1.size() < 5 || key2.size() < 5)
	{
		//cout << "轉換後匹配失敗！" << endl;
		warning = 4;


		return 100;
	}

	matcher.knnMatch(des1, des2, matches, 1);

	match1.resize(matches.size());
	for (int i = 0; i < matches.size(); i++)
	{
		match1[i] = matches[i][0];
	}

	double min_dist = 100;
	double max_dist = 0;
	for (int i = 0; i < des1.rows; i++)
	{
		double dist = match1[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	vector<DMatch> good_match;
	for (int i = 0; i < des1.rows; i++)
	{
		if (match1[i].distance < multiple * min_dist)
		{
			good_match.push_back(match1[i]);
		}
	}



	vector<Point2f> point1, point2;
	for (unsigned int i = 0; i < good_match.size(); i++)
	{
		point1.push_back(key1[good_match[i].queryIdx].pt);
		point2.push_back(key2[good_match[i].trainIdx].pt);
	}

	Mat mask;
	Mat homo;

	homo = findHomography(point2, point1, CV_RANSAC, 1, mask, 2000, 0.9999);

	vector<DMatch> homomatch;
	for (int i = 0; i < good_match.size(); i++)
	{
		if (mask.at<uchar>(i, 0) != 0)
		{
			homomatch.push_back(good_match[i]);
		}
	}
	/*Mat imghomomatch;
	drawMatches(image1, key1, firsttran, key2, homomatch, imghomomatch, Scalar(100, 150, 0, 0), Scalar::all(-1),
	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//imshow("imghomomatch", imghomomatch);*/

	vector<Point2f> p1, p2;
	for (unsigned int i = 0; i < homomatch.size(); i++)
	{
		p1.push_back(key1[homomatch[i].queryIdx].pt);
		p2.push_back(key2[homomatch[i].trainIdx].pt);
	}

	double RMS = 0;
	for (int i = 0; i < p1.size(); i++)
	{
		double x, y;
		x = p1[i].x - p2[i].x;
		y = p1[i].y - p2[i].y;
		RMS = RMS + x*x + y*y;
	}
	RMS = RMS / p1.size();
	RMS = pow(RMS, 0.5);

	cout << "RMS:		" << RMS << endl;

	return RMS;
}

double imageproc::mapperRMS(Mat image1, Mat image2, Mat homotran, Mat mapperhomo, int &warning, double multiple)
{
	if (!image1.data || !image2.data)
	{
		//cout << "輸入影像失敗錯誤~！" << endl;
		warning = 1;


		return 100;
	}

	if (multiple <= 1)
	{
		//cout << "門檻值倍率需大於1！" << endl;
		warning = 2;


		return 100;
	}
	warning = 0;
	Mat firsttran, secondtran;
	warpPerspective(image2, firsttran, homotran, image2.size(), INTER_AREA);
	////imshow("firsttran", firsttran);
	warpPerspective(firsttran, secondtran, mapperhomo, firsttran.size(), INTER_AREA);
	////imshow("secondtran", secondtran);

	Ptr<SURF> detector = SURF::create(600, 3, 3, true, false);

	vector<KeyPoint> key1;
	vector<KeyPoint> key2;
	Mat des1;
	Mat des2;
	detector->detectAndCompute(image1, Mat(), key1, des1, false);
	detector->detectAndCompute(secondtran, Mat(), key2, des2, false);

	FlannBasedMatcher matcher/*(NORM_L1)*/;
	vector<vector<DMatch> > matches;
	vector<DMatch> match1;
	matches.clear();
	match1.clear();


	matcher.knnMatch(des1, des2, matches, 1);

	match1.resize(matches.size());
	for (int i = 0; i < matches.size(); i++)
	{
		match1[i] = matches[i][0];
	}

	double min_dist = 100;
	double max_dist = 0;
	for (int i = 0; i < des1.rows; i++)
	{
		double dist = match1[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	vector<DMatch> good_match;
	for (int i = 0; i < des1.rows; i++)
	{
		if (match1[i].distance < multiple * min_dist)
		{
			good_match.push_back(match1[i]);
		}
	}



	vector<Point2f> point1, point2;
	for (unsigned int i = 0; i < good_match.size(); i++)
	{
		point1.push_back(key1[good_match[i].queryIdx].pt);
		point2.push_back(key2[good_match[i].trainIdx].pt);
	}

	Mat mask;
	Mat homo;

	homo = findHomography(point2, point1, CV_RANSAC, 1, mask, 2000, 0.9999);

	vector<DMatch> homomatch;
	for (int i = 0; i < good_match.size(); i++)
	{
		if (mask.at<uchar>(i, 0) != 0)
		{
			homomatch.push_back(good_match[i]);
		}
	}
	/*Mat imghomomatch;
	drawMatches(image1, key1, secondtran, key2, homomatch, imghomomatch, Scalar(100, 150, 0, 0), Scalar::all(-1),
	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//imshow("imgmappermatch", imghomomatch);*/
	//waitKey(0);

	vector<Point2f> p1, p2;
	for (unsigned int i = 0; i < homomatch.size(); i++)
	{
		p1.push_back(key1[homomatch[i].queryIdx].pt);
		p2.push_back(key2[homomatch[i].trainIdx].pt);
	}

	double RMS = 0;
	for (int i = 0; i < p1.size(); i++)
	{
		double x, y;
		x = p1[i].x - p2[i].x;
		y = p1[i].y - p2[i].y;
		RMS = RMS + x*x + y*y;
	}
	RMS = RMS / p1.size();
	RMS = pow(RMS, 0.5);

	return RMS;
}

vector<Point2f> imageproc::mat2point(Mat image, int &warning)
{
	if (!image.data)
	{
		warning = 1;
		//cout << "影像資料錯誤" << endl;
		vector<Point2f> error;
		return error;

	}
	warning = 0;

	vector<Point2f> point;
	point.resize(image.rows*image.cols);

	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
		{
			point[i*image.cols + j].x = j;
			point[i*image.cols + j].y = i;
		}

	return point;
}

vector<Point2f> imageproc::tranpoint(vector<Point2f> point, Mat homotran, int &warning)
{
	if (point.size() == 0)
	{
		warning = 1;
		//cout << "輸入點資料資料量0" << endl;

		vector<Point2f> error;
		return error;
	}

	if (!homotran.data)
	{
		warning = 2;
		//cout << "輸入homography轉換矩陣錯誤" << endl;

		vector<Point2f> error;
		return error;
	}
	warning = 0;

	vector<Point2f> tran;
	perspectiveTransform(point, tran, homotran);

	return tran;

}

Mat imageproc::point2mat(vector<vector<Point2f>> pointlist, vector<Mat> matlist, int &warning)
{
	if (pointlist.size() == 0)
	{
		warning = 1;
		cout << "輸入點資料資料量0" << endl;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (matlist.size() == 0)
	{
		warning = 2;
		cout << "輸入圖資料資料量0" << endl;

		Mat error(0, 0, CV_8U);
		return error;
	}
	if (pointlist.size() != matlist.size())
	{
		warning = 3;
		cout << "輸入點層數與圖資料數量不同" << endl;

		Mat error(0, 0, CV_8U);
		return error;
	}
	warning = 0;

	double minleft, maxright, minup, maxdown;
	minleft = 1000000;
	maxright = -1000000;
	minup = 1000000;
	maxdown = -1000000;

	for (int layer = 0; layer < pointlist.size(); layer++)
		for (int i = 0; i < pointlist[layer].size(); i++)
		{
			if (pointlist[layer][i].x > maxright)
			{
				maxright = pointlist[layer][i].x;
			}
			if (pointlist[layer][i].x < minleft)
			{
				minleft = pointlist[layer][i].x;
			}
			if (pointlist[layer][i].y > maxdown)
			{
				maxdown = pointlist[layer][i].y;
			}
			if (pointlist[layer][i].y < minup)
			{
				minup = pointlist[layer][i].y;
			}
		}

	Mat joint(maxdown - minup + 10, maxright - minleft + 10, CV_8UC3);
	Mat distance = joint.clone();
	for (int i = 0; i < joint.rows; i++)
		for (int j = 0; j < joint.cols; j++)
		{
			joint.at<Vec3b>(i, j)[0] = 0;
			joint.at<Vec3b>(i, j)[1] = 0;
			joint.at<Vec3b>(i, j)[2] = 0;
		}

	for (int layer = 0; layer < pointlist.size(); layer++)
	{
		for (int i = 0; i < joint.rows; i++)
			for (int j = 0; j < joint.cols; j++)
			{
				distance.at<Vec3b>(i, j)[0] = 10000;
			}
		for (int k = 0; k < pointlist[layer].size(); k++)
		{
			int x = (int)(pointlist[layer][k].x + 0.5);
			int y = (int)(pointlist[layer][k].y + 0.5);

			for (int m = -1; m < 2; m++)
				for (int n = -1; n < 2; n++)
				{
					double pdistance = 0;
					pdistance = pow(pointlist[layer][k].x - (x + m), 2);
					pdistance = pdistance + pow(pointlist[layer][k].y - (y + n), 2);
					pdistance = pow(pdistance, 0.5);
					if (distance.at<Vec3b>(y - minup + n + 5, x - minleft + m + 5)[0] > pdistance)
					{
						joint.at<Vec3b>(y - minup + n + 5, x - minleft + m + 5)[0] = matlist[layer].at<Vec3b>(k / matlist[layer].cols, k % matlist[layer].cols)[0];
						joint.at<Vec3b>(y - minup + n + 5, x - minleft + m + 5)[1] = matlist[layer].at<Vec3b>(k / matlist[layer].cols, k % matlist[layer].cols)[1];
						joint.at<Vec3b>(y - minup + n + 5, x - minleft + m + 5)[2] = matlist[layer].at<Vec3b>(k / matlist[layer].cols, k % matlist[layer].cols)[2];
						distance.at<Vec3b>(y - minup + n + 5, x - minleft + m + 5)[0] = pdistance;
					}

				}


		}

	}


	for (int i = 1; i < joint.rows - 1; i++)
		for (int j = 1; j < joint.cols - 1; j++)
		{
			if (joint.at<Vec3b>(i, j)[0] == 0 && joint.at<Vec3b>(i, j)[1] == 0 && joint.at<Vec3b>(i, j)[2] == 0)
			{
				double blue, green, red;
				blue = 0;
				green = 0;
				red = 0;
				for (int m = -1; m < 2; m++)
					for (int n = -1; n < 2; n++)
					{
						blue = blue + joint.at<Vec3b>(i + m, j + n)[0];
						green = green + joint.at<Vec3b>(i + m, j + n)[1];
						red = red + joint.at<Vec3b>(i + m, j + n)[2];
					}
				red = red / 9;
				green = green / 9;
				blue = blue / 9;
				joint.at<Vec3b>(i, j)[0] = blue;
				joint.at<Vec3b>(i, j)[1] = green;
				joint.at<Vec3b>(i, j)[2] = red;
			}
		}
	return joint;



}

void imageproc::mixpoint(vector<vector<Point2f>> &pointlist, vector<Point2f> point, vector<Mat> &matlist, Mat image, int &warning)
{
	if (pointlist.size() != matlist.size())
	{
		warning = 1;
		cout << "輸入點層數與圖資料數量不同" << endl;
		system("pause");
		exit(0);
	}
	warning = 0;

	pointlist.push_back(point);
	matlist.push_back(image);

}

vector<Mat> imageproc::readallfile(vector<wstring> filename, vector<int> &warning)
{
	vector<Mat> imagerank;

	imagerank.resize(filename.size());
	warning.resize(filename.size());
	imageproc readrank;
	for (int i = 0; i < filename.size(); i++)
	{
		imagerank[i] = readrank.readfile(filename[i], 1, 1, warning[i]);
	}

	return imagerank;
}

vector<Mat> imageproc::findallfeatur(int hessian, int Octave, int Octavelayer, bool extend, bool USURF,
	vector<Mat> imagerank, vector<vector<KeyPoint>> &keypointrank, vector<Mat> &descriptorrank, vector<int> &warning)
{
	vector<Mat> detectrank;
	detectrank.resize(imagerank.size());

	warning.resize(imagerank.size());
	keypointrank.resize(imagerank.size());
	descriptorrank.resize(imagerank.size());

	imageproc allfeature;
	for (int i = 0; i < imagerank.size(); i++)
	{
		detectrank[i] = allfeature.findfeature(hessian, Octave, Octavelayer, extend, USURF, imagerank[i], keypointrank[i], descriptorrank[i], warning[i]);
	}

	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
	for (int i = 0; i < warning.size(); i++)
	{
		if (warning[i] != 0)
		{
			cout << "SURF偵測有誤，以下為錯誤編碼:" << endl;
			for (int j = 0; j < warning.size(); j++)
			{
				cout << warning[j] << endl;
			}
		}
	}


	return detectrank;
}

vector<Mat> imageproc::homoallresult(vector<Mat> imagerank, vector<vector<KeyPoint>>keypointrank, vector<Mat> descriptorrank,
	double initialmultiple, vector<vector<DMatch>> &homomatchrank, vector<Mat> &homotranrank, vector<double> &homoRMSrank,
	double RMSthreshold, vector<double> &multiplerank, vector<int> &homowarning, vector<int> &RMSwarning, int &warning)
{

	vector<Mat> imgmatchrank;
	imgmatchrank.resize(imagerank.size() - 1);
	homomatchrank.resize(imagerank.size() - 1);
	homotranrank.resize(imagerank.size() - 1);
	homoRMSrank.resize(imagerank.size() - 1);
	multiplerank.resize(imagerank.size() - 1);
	homowarning.resize(imagerank.size() - 1);
	RMSwarning.resize(imagerank.size() - 1);

	if (initialmultiple <= 1)
	{
		warning = 1;

		vector<Mat> error;
		return error;
	}
	warning = 0;
	imageproc allhomo;

	for (int i = 0; i < imgmatchrank.size(); i++)
	{
		int times = 0;
		homoRMSrank[i] = 10000;

		while (homoRMSrank[i] > RMSthreshold)
		{

			double m = pow(0.75, times);
			double multiple = m*initialmultiple;
			bool toomany = false;
			if (multiple < 1.5)
			{
				multiple = 1.5;
				toomany = true;
			}
			double RMStemp;
			vector<DMatch> homomatchtemp;
			Mat homotrantemp;
			Mat imgmatchtemp;
			int homowarningtemp;
			int RMSwarningtemp;
			imgmatchtemp = allhomo.homoresult(imagerank[i + 1], imagerank[i], keypointrank[i + 1], keypointrank[i], descriptorrank[i + 1], descriptorrank[i],
				multiple, homomatchtemp, homotrantemp, homowarningtemp);
			cout << "homomatchsize:	" << homomatchtemp.size() << endl;

			if (!homotrantemp.data)
			{
				homowarning[i] = homowarningtemp;
				RMSwarning[i] = 3;
				break;
			}
			RMStemp = allhomo.homoRMS(imagerank[i + 1], imagerank[i], homotrantemp, RMSwarningtemp, multiple);

			if (RMStemp < homoRMSrank[i])
			{
				homoRMSrank[i] = RMStemp;
				homomatchrank[i] = homomatchtemp;
				homotranrank[i] = homotrantemp.clone();
				imgmatchrank[i] = imgmatchtemp.clone();
				homowarning[i] = homowarningtemp;
				RMSwarning[i] = RMSwarningtemp;
				multiplerank[i] = multiple;
				if (homoRMSrank[i] <= RMSthreshold)
				{
					break;
				}
			}
			if (toomany == true)
			{
				homowarning[i] = 5;
				RMSwarning[i] = 3;
				homoRMSrank[i] = RMStemp;
				break;
			}
			times++;
		}

	}

	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
	if (warning != 0)
	{
		cout << "homoallresult設定有誤，以下為錯誤編碼:" << endl;
		cout << warning << endl;
	}
	for (int i = 0; i < homowarning.size(); i++)
	{
		if (homowarning[i] != 0)
		{
			cout << "homo轉換有誤，以下為錯誤編碼:" << endl;
			for (int j = 0; j < homowarning.size(); j++)
			{
				cout << homowarning[j] << endl;
			}
		}

	}
	for (int i = 0; i < RMSwarning.size(); i++)
	{
		if (RMSwarning[i] != 0)
		{
			cout << "homoRMS計算有誤，以下為錯誤編碼:" << endl;
			for (int j = 0; j < RMSwarning.size(); j++)
			{
				cout << RMSwarning[j] << endl;
			}
		}

	}


	return imgmatchrank;

}

vector<Mat> imageproc::mapperallresult(vector<Mat> imagerank, vector<Mat> homotranrank, vector<Mat> &mappertranrank, vector<double> &mapperRMS,
	vector<double> multiplerank, vector<int> &mapperwarning, vector<int> &RMSwarning)
{
	vector<Mat> mapperresultrank;
	mapperresultrank.resize(homotranrank.size());
	mappertranrank.resize(homotranrank.size());
	mapperRMS.resize(homotranrank.size());
	mapperwarning.resize(homotranrank.size());
	RMSwarning.resize(homotranrank.size());

	imageproc allmapper;

	for (int i = 0; i < homotranrank.size(); i++)
	{
		mapperresultrank[i] = allmapper.mapperresult(imagerank[i + 1], imagerank[i], homotranrank[i], mappertranrank[i], mapperwarning[i]);
		mapperRMS[i] = allmapper.mapperRMS(imagerank[i + 1], imagerank[i], homotranrank[i], mappertranrank[i], RMSwarning[i], multiplerank[i]);
		cout << i + 1 << i + 2 << "mapperRMS:		" << mapperRMS[i] << endl;
	}

	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
	for (int i = 0; i < mapperwarning.size(); i++)
	{
		if (mapperwarning[i] != 0)
		{
			cout << "mapper轉換有誤，以下為錯誤編碼:" << endl;
			for (int j = 0; j < mapperwarning.size(); j++)
			{
				cout << mapperwarning[j] << endl;
			}
		}

	}
	for (int i = 0; i < RMSwarning.size(); i++)
	{
		if (RMSwarning[i] != 0)
		{
			cout << "mapperRMS計算有誤，以下為錯誤編碼:" << endl;
			for (int j = 0; j < RMSwarning.size(); j++)
			{
				cout << RMSwarning[j] << endl;
			}
		}

	}

	return mapperresultrank;
}

Mat imageproc::jointallimage(vector<Mat> imagerank, vector<Mat> homotranrank, vector<double> homoRMS, vector<Mat> mappertranrank,
	vector<double> mapperRMS, vector<int> &imagewarning, vector<int> &tranwarning, int &jointwarning)
{
	vector<vector<Point2f>> pointrank;
	pointrank.resize(imagerank.size());

	imagewarning.resize(imagerank.size());
	tranwarning.resize(imagerank.size());

	imageproc allimage;

	for (int i = 0; i < imagerank.size(); i++)
	{
		pointrank[i] = allimage.mat2point(imagerank[i], imagewarning[i]);
	}



	for (int i = 0; i < imagerank.size(); i++)
	{
		int j = i;
		while (j < homotranrank.size())
		{
			int tempwarning = 0;
			bool change = false;
			if (tranwarning[i] != 0)
			{
				tempwarning = tranwarning[i];
				change = true;
			}
			if (homoRMS[j] <= mapperRMS[j])
			{
				pointrank[i] = allimage.tranpoint(pointrank[i], homotranrank[j], tranwarning[i]);
				if (change == true)
				{
					tranwarning[i] = tempwarning;
				}
			}
			else
			{
				pointrank[i] = allimage.tranpoint(pointrank[i], homotranrank[j], tranwarning[i]);
				pointrank[i] = allimage.tranpoint(pointrank[i], mappertranrank[j], tranwarning[i]);
				if (change == true)
				{
					tranwarning[i] = tempwarning;
				}
			}
			j++;
		}

	}

	Mat allimagejoint;


	allimagejoint = allimage.point2mat(pointrank, imagerank, jointwarning);

	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
	for (int i = 0; i < imagewarning.size(); i++)
	{
		if (imagewarning[i] != 0)
		{
			cout << "影像轉點資料有誤，以下為錯誤編碼:" << endl;
			for (int j = 0; j < imagewarning.size(); j++)
			{
				cout << imagewarning[j] << endl;
			}
		}

	}
	for (int i = 0; i < tranwarning.size(); i++)
	{
		if (tranwarning[i] != 0)
		{
			cout << "點資料進行homo轉換有誤，以下為錯誤編碼:" << endl;
			for (int j = 0; j < tranwarning.size(); j++)
			{
				cout << tranwarning[j] << endl;
			}
		}

	}

	if (jointwarning != 0)
	{
		cout << "jointimage有誤，以下為錯誤編碼:" << endl;
		cout << jointwarning << endl;
	}



	return allimagejoint;

}

vector<Mat> imageproc::allcolordifferent(vector<Mat> imagerank, vector<Mat> homotranrank, vector<double> homoRMSrank, vector<Mat> mappertranrank,
	vector<double> mapperRMSrank, vector<int> &warning)
{
	vector<Mat> colorrank;
	colorrank.resize(homotranrank.size());

	warning.resize(homotranrank.size());
	imageproc allcolor;


	for (int i = 0; i < colorrank.size(); i++)
	{
		colorrank[i] = allcolor.colordifferent(imagerank[i + 1], imagerank[i], homotranrank[i], mappertranrank[i], homoRMSrank[i], mapperRMSrank[i], warning[i]);
	}

	return colorrank;
}

//int main()
//{
//	vector<string> filename;
//
//	filename.push_back("../test data/G057_01.tif");
//	filename.push_back("../test data/G057_06.tif");
//	filename.push_back("../test data/G057_02.tif");
//	filename.push_back("../test data/G057_05.tif");
//	filename.push_back("../test data/G057_03.tif");
//	filename.push_back("../test data/G057_04.tif");
//
//
//
//	imageproc ttt;
//
//	vector<Mat> imagerank;
//	vector<int> readwarning;
//	//輸入影像檔，!!重要!!影像順序第一張輸入之影像，會在成果之最底層，同理類推
//	imagerank = ttt.readallfile(filename, readwarning);
//
//
//
//	for (int i = 0; i < imagerank.size(); i++)
//	{
//
//		//imshow("第"+to_string(i+1)+"張影像", imagerank[i]);
//	}
//
//
//	//waitKey(0);
//	vector<vector<KeyPoint>> keypointrank;
//	vector<Mat> descriptorrank;
//
//	int hessian = 300;
//	int Octave = 5;
//	int Octavelayer = 5;
//	bool extend = true;
//	bool USURF = false;
//
//	vector<Mat> detectrank;
//	vector<int> detectwarning;
//
//	//進行影像序列偵測特徵點
//	detectrank = ttt.findallfeatur(hessian, Octave, Octavelayer, extend, USURF, imagerank, keypointrank, descriptorrank, detectwarning);
//
//	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
//	for (int i = 0; i < detectwarning.size(); i++)
//	{
//		if (detectwarning[i] != 0)
//		{
//			cout << "SURF偵測有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < detectwarning.size(); j++)
//			{
//				cout << detectwarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//	}
//
//	for (int i = 0; i < detectrank.size(); i++)
//	{
//
//		//imshow("第" + to_string(i+1) + "張影像detect", detectrank[i]);
//	}
//
//
//	//waitKey(0);
//
//
//	vector<Mat> imgmatchrank;
//	double initialmultiple = 5;
//	vector<vector<DMatch>> homomatchrank;
//	vector<Mat> homotranrank;
//	vector<double> homoRMSrank;
//	vector<double> multiplerank;
//	vector<int> homowarning;
//	vector<int> homoRMSwarning;
//	int homoallresultwarning;
//	double RMSthreshold = 2;
//
//	//進行相鄰影像轉換矩陣計算
//	imgmatchrank = ttt.homoallresult(imagerank, keypointrank, descriptorrank, initialmultiple, homomatchrank, homotranrank, homoRMSrank, RMSthreshold,
//		multiplerank, homowarning, homoRMSwarning, homoallresultwarning);
//
//
//	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
//	if (homoallresultwarning != 0)
//	{
//		cout << "homoallresult設定有誤，以下為錯誤編碼:" << endl;
//		cout << homoallresultwarning << endl;
//		system("pause");
//		exit(0);
//	}
//	for (int i = 0; i < homowarning.size(); i++)
//	{
//		if (homowarning[i] != 0)
//		{
//			cout << "homo轉換有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < homowarning.size(); j++)
//			{
//				cout << homowarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//	for (int i = 0; i < homowarning.size(); i++)
//	{
//		if (homowarning[i] != 0)
//		{
//			cout << "homoRMS計算有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < homowarning.size(); j++)
//			{
//				cout << homowarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//
//	for (int i = 0; i < imgmatchrank.size(); i++)
//	{
//		//imshow("第" + to_string(i+1) + to_string(i + 2) + "張影像imgmatch", imgmatchrank[i]);
//	}
//
//	//waitKey(0);
//
//
//	vector<Mat> mapperresultrank;
//	vector<Mat> mappertranrank;
//	vector<double> mapperRMSrank;
//	vector<int> mapperwarning;
//	vector<int> mapperRMSwarning;
//
//	//進行相鄰影像mapper運算獲得二次轉換矩陣
//	mapperresultrank = ttt.mapperallresult(imagerank, homotranrank, mappertranrank, mapperRMSrank, multiplerank, mapperwarning, mapperRMSwarning);
//
//
//	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
//	for (int i = 0; i < mapperwarning.size(); i++)
//	{
//		if (mapperwarning[i] != 0)
//		{
//			cout << "mapper轉換有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < mapperwarning.size(); j++)
//			{
//				cout << mapperwarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//	for (int i = 0; i < mapperRMSwarning.size(); i++)
//	{
//		if (mapperRMSwarning[i] != 0)
//		{
//			cout << "mapperRMS計算有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < mapperRMSwarning.size(); j++)
//			{
//				cout << mapperRMSwarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//
//	for (int i = 0; i < mapperresultrank.size(); i++)
//	{
//
//		//imshow("第" + to_string(i+1) + to_string(i + 2) + "張影像mapper", mapperresultrank[i]);
//	}
//
//
//	//waitKey(0);
//	Mat allimagejoint;
//	vector<int> imagewarning;
//	vector<int> tranwarning;
//	int jointwarning;
//
//	//輸入影像序列，獲得影像序列對位成果，先輸入之影像在下層 後輸入影像在上層
//
//	allimagejoint = ttt.jointallimage(imagerank, homotranrank, homoRMSrank, mappertranrank, mapperRMSrank, imagewarning, tranwarning, jointwarning);
//
//
//	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
//	for (int i = 0; i < imagewarning.size(); i++)
//	{
//		if (imagewarning[i] != 0)
//		{
//			cout << "影像轉點資料有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < imagewarning.size(); j++)
//			{
//				cout << imagewarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//	for (int i = 0; i < tranwarning.size(); i++)
//	{
//		if (tranwarning[i] != 0)
//		{
//			cout << "點資料進行homo轉換有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < tranwarning.size(); j++)
//			{
//				cout << tranwarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//
//	if (jointwarning != 0)
//	{
//		cout << "jointimage有誤，以下為錯誤編碼:" << endl;
//		cout << jointwarning << endl;
//		system("pause");
//		exit(0);
//	}
//
//	//imshow("allimagejoint", allimagejoint);
//	waitKey(0);
//
//	imwrite("../test data/allimagejoint.tif", allimagejoint);
//
//	vector<Mat> colorrank;
//	vector<int> colorwarning;
//
//	//輸出影像序列相鄰影像之融合圖，B色階與GR色階分別為兩張不同之影像
//	colorrank = ttt.allcolordifferent(imagerank, homotranrank, homoRMSrank, mappertranrank, mapperRMSrank, colorwarning);
//
//
//	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
//	for (int i = 0; i < colorwarning.size(); i++)
//	{
//		if (colorwarning[i] != 0)
//		{
//			cout << "點資料進行homo轉換有誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < colorwarning.size(); j++)
//			{
//				cout << colorwarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//
//	for (int i = 0; i < colorrank.size(); i++)
//	{
//
//		//imshow("第" + to_string(i+1) + to_string(i + 2) + "張影像colordifferent", colorrank[i]);
//
//		imwrite("../test data/color" + to_string(i + 1) + to_string(i + 2) + ".tif", colorrank[i]);
//	}
//
//
//
//
//
//	waitKey(0);
//
//
//	return 0;
//}
#pragma endregion


bool matchFunc(char* fileName_c, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, OutputArray result)
{
	string logName = ".\\PIMCore\\log.txt";
	string fileName = ".\\PIMCore\\" + string(fileName_c);
	

	StreamReader^ sr = gcnew StreamReader(gcnew System::String(fileName.c_str()));
	System::String^ strLine;
	vector<wstring> fileSeq;
	while (strLine = sr->ReadLine())
	{
		fileSeq.push_back(msclr::interop::marshal_as<wstring>(strLine));
	}
	sr->Close();


	//Output cout to file
	ofstream out(logName);
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());


	imageproc ful;

	vector<Mat> imgSeq;
	vector<int> readwarning;

	//輸入影像檔，!!重要!!影像順序第一張輸入之影像，會在成果之最底層，同理類推
	imgSeq = ful.readallfile(fileSeq, readwarning);
	if (errorCheck(readwarning))
		return false;

	vector<vector<KeyPoint>> keypointrank;
	vector<Mat> descriptorrank;

	vector<Mat> detectrank;
	vector<int> detectwarning;

	//進行影像序列偵測特徵點
	detectrank = ful.findallfeatur(minHessian, octave, octaveLayer, extended, upRight, imgSeq, keypointrank, descriptorrank, detectwarning);
	if (errorCheck(detectwarning))
		return false;

	vector<Mat> imgmatchrank;
	double initialmultiple = 5;
	vector<vector<DMatch>> homomatchrank;
	vector<Mat> homoTranSeq;
	vector<double> homoRmsSeq;
	vector<double> multiplerank;
	vector<int> homowarning;
	vector<int> homoRMSwarning;
	int homoallresultwarning;

	//進行相鄰影像轉換矩陣計算
	imgmatchrank = ful.homoallresult(imgSeq, keypointrank, descriptorrank, initialmultiple, homomatchrank, homoTranSeq, homoRmsSeq, rmsThres, multiplerank, homowarning, homoRMSwarning, homoallresultwarning);
	if (errorCheck(homowarning) + errorCheck(homoRMSwarning) + homoallresultwarning)
		return false;

	vector<Mat> mapperresultrank;
	vector<Mat> mapperTranSeq;
	vector<double> mapperRmsSeq;
	vector<int> mapperwarning;
	vector<int> mapperRMSwarning;

	//進行相鄰影像mapper運算獲得二次轉換矩陣
	mapperresultrank = ful.mapperallresult(imgSeq, homoTranSeq, mapperTranSeq, mapperRmsSeq, multiplerank, mapperwarning, mapperRMSwarning);
	if (errorCheck(mapperwarning) + errorCheck(mapperRMSwarning))
		return false;

	Mat allimagejoint;
	vector<int> imagewarning;
	vector<int> tranwarning;
	int jointwarning;

	//輸入影像序列，獲得影像序列對位成果，先輸入之影像在下層 後輸入影像在上層
	allimagejoint = ful.jointallimage(imgSeq, homoTranSeq, homoRmsSeq, mapperTranSeq, mapperRmsSeq, imagewarning, tranwarning, jointwarning);
	if (errorCheck(imagewarning) + errorCheck(tranwarning) + jointwarning)
		return false;

	//imwrite(work_path + "result.tif", allimagejoint);

	allimagejoint.copyTo(result);

	return true;
	/*
	Mat homoRmsSeqMat = Mat(homoRmsSeq.size(), 1, CV_64F);
	Mat mapperRmsSeqMat = Mat(mapperRmsSeq.size(), 1, CV_64F);

	for (int i = 0; i < homoRmsSeq.size(); i++)
		homoRmsSeqMat.at<double>(i, 1) = homoRmsSeq[i];

	for (int i = 0; i < mapperRmsSeq.size(); i++)
		mapperRmsSeqMat.at<double>(i, 1) = mapperRmsSeq[i];


	vector<Mat> returnValue;
	returnValue.reserve(homoTranSeq.size() + mapperTranSeq.size() + 1 + 1);
	returnValue.insert(returnValue.end(), homoTranSeq.begin(), homoTranSeq.end());
	returnValue.insert(returnValue.end(), mapperTranSeq.begin(), mapperTranSeq.end());
	returnValue.push_back(homoRmsSeqMat);
	returnValue.push_back(mapperRmsSeqMat);
	*/
}

int errorCheck(vector<int> errorMsg)
{
	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
	for (int i = 0; i < errorMsg.size(); i++)
	{
		if (errorMsg[i] != 0)
			return errorMsg[i];
	}
	return 0;
}


/*
void srFunc(IplImage* LRIm, double Factor, double GuassianSigma, double GuassianKernelSize, double Lamda, OutputArray HRIm)
{
	// Initialize the application.
	// You do not need to do this more than one time.
	SRFunc_Coder_initialize();

	// Invoke the entry-point functions.
	// You can call entry-point functions multiple times.
	main_SRFunc_Coder(LRIm, Factor, GuassianSigma, GuassianKernelSize, Lamda, HRIm);

	// Terminate the application.
	// You do not need to do this more than one time.
	SRFunc_Coder_terminate();
}


//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
	return 0.0;
}

//
// Arguments    : void
// Return Type  : unsigned char
//
static unsigned char argInit_uint8_T()
{
	return 0;
}

//
// Arguments    : void
// Return Type  : emxArray_uint8_T *
//
static emxArray_uint8_T *c_argInit_UnboundedxUnboundedx3(IplImage* _LRIm)
{
	Mat LRMat = cvarrToMat(_LRIm);

	int nChannels = LRMat.channels();
	int nRows = LRMat.rows;
	int nCols = LRMat.cols;

	uchar* srcData = LRMat.data;

	emxArray_uint8_T *result;
	static int iv0[3] = { nRows, nCols, 3 };

	int idx0;
	int idx1;
	int idx2;

	// Set the size of the array.
	// Change this size to the value that the application requires.
	result = emxCreateND_uint8_T(3, *(int(*)[3])&iv0[0]);

	// Loop over the array to initialize each element.
	for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
		for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
			for (idx2 = 0; idx2 < 3; idx2++) {
				// Set the value of the array element.
				// Change this value to the value that the application requires.
				result->data[(idx0 + result->size[0] * idx1) + result->size[0] *
					result->size[1] * idx2] = *(srcData + idx0 * nRows + idx1 * nCols + idx2);
			}
		}
	}

	return result;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_SRFunc_Coder(IplImage* _LRIm, double Factor, double GuassianSigma, double GuassianKernelSize, double Lamda, OutputArray _HRIm)
{
	emxArray_uint8_T *HRIm;
	emxArray_uint8_T *LRIm;
	emxInitArray_uint8_T(&HRIm, 3);

	// Initialize function 'SRFunc_Coder' input arguments.
	// Initialize function input argument 'LRIm'.
	LRIm = c_argInit_UnboundedxUnboundedx3(_LRIm);

	// Call the entry-point 'SRFunc_Coder'.
	SRFunc_Coder(LRIm, Factor, GuassianSigma, GuassianKernelSize, Lamda, HRIm);
	emxDestroyArray_uint8_T(HRIm);
	emxDestroyArray_uint8_T(LRIm);
}
*/


//vector<Mat>* projImageSequence(vector<Mat>* imgSeq, vector<Mat>* homoTranSeq, vector<double>* homoRmsSeq, vector<Mat>* mapperTranSeq, vector<double>* mapperRmsSeq)
//{
//	imageproc ful;
//
//	Mat allimagejoint;
//	vector<int> imagewarning;
//	vector<int> tranwarning;
//	int jointwarning;
//
//	//輸入影像序列，獲得影像序列對位成果，先輸入之影像在下層 後輸入影像在上層
//	allimagejoint = ful.jointallimage(*imgSeq, *homoTranSeq, *homoRmsSeq, *mapperTranSeq, *mapperRmsSeq, imagewarning, tranwarning, jointwarning);
//
//
//	vector<Mat> returnValue;
//	returnValue.push_back(allimagejoint);
//
//	return &returnValue;
//}


