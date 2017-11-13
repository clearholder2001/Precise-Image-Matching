#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
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

#include "PIMCppFunc.h"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::reg;
using namespace std;
using namespace System;
using namespace System::IO;

#pragma region "蔡易澄's codes using OpenCV"
static class imageproc
{
public:
	Mat readfile(wstring, double, double, int &);
	Mat findfeature(int, int, int, bool, bool, Mat, vector<KeyPoint> &, Mat &, int &);
	Mat homoresult(Mat, Mat, vector<KeyPoint>, vector<KeyPoint>, Mat, Mat, double, vector<DMatch> &, Mat &, int &);
	Mat homoresultnoRANSAC(Mat, Mat, vector<KeyPoint>, vector<KeyPoint>, Mat, Mat, double, vector<DMatch> &, Mat &, int &);
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
	vector<Mat> findallfeature(int, int, int, bool, bool, vector<Mat>, vector<vector<KeyPoint>> &, vector<Mat> &, vector<int> &);
	vector<Mat> homoallresult(vector<Mat>, vector<vector<KeyPoint>>, vector<Mat>, double, vector<vector<DMatch>> &, vector<Mat> &, vector<double> &, double, vector<double> &, vector<int> &, vector<int> &, int &);
	vector<Mat> homoallresultnoRANSAC(vector<Mat>, vector<vector<KeyPoint>>, vector<Mat>, double, vector<vector<DMatch>> &, vector<Mat> &, vector<double> &, double, vector<double> &, vector<int> &, vector<int> &, int &);
	vector<Mat> mapperallresult(vector<Mat>, vector<Mat>, vector<Mat> &, vector<double> &, vector<double>, vector<int> &, vector<int> &);
	//會有錯位問題，勿用
	Mat jointallimage(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &, vector<int> &, int &);
	Mat jointallimageback(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &, vector<int> &, int &);
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

Mat imageproc::homoresult(Mat img1, Mat img2, vector<KeyPoint> keypoint1, vector<KeyPoint> keypoint2, Mat descriptor1, Mat descriptor2, double multiple, vector<DMatch> &homomatch, Mat &homotran, int &warning)
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
		if (match1[i].distance <= multiple * min_dist)
		{
			good_match.push_back(match1[i]);
		}
	}
	cout << "multipleSIZE:	" << good_match.size() << endl;
	if (good_match.size() < 8)
	{
		warning = 7;
		Mat error(0, 0, CV_8U);
		return error;
	}

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

Mat imageproc::homoresultnoRANSAC(Mat img1, Mat img2, vector<KeyPoint> keypoint1, vector<KeyPoint> keypoint2, Mat descriptor1, Mat descriptor2, double multiple, vector<DMatch> &homomatch, Mat &homotran, int &warning)
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
		if (match1[i].distance <= multiple * min_dist)
		{
			good_match.push_back(match1[i]);
		}
	}
	cout << "multipleSIZE:	" << good_match.size() << endl;
	if (good_match.size() < 8)
	{
		warning = 7;
		Mat error(0, 0, CV_8U);
		return error;
	}

	Mat imghomomatch;
	drawMatches(img1, keypoint1, img2, keypoint2, good_match, imghomomatch, Scalar(100, 150, 0, 0), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	vector<Point2f> point1, point2;
	for (unsigned int i = 0; i < good_match.size(); i++)
	{
		point1.push_back(keypoint1[good_match[i].queryIdx].pt);
		point2.push_back(keypoint2[good_match[i].trainIdx].pt);
	}

	Mat mask;
	double a = 3;

	homotran = findHomography(point2, point1, 0, a, mask, 2000, 0.9999);
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
	Mat img = imdecode(arr, 1);
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
		if (match1[i].distance <= multiple * min_dist)
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

	imshow("imghomomatch", imghomomatch);*/

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
	//imshow("firsttran", firsttran);
	warpPerspective(firsttran, secondtran, mapperhomo, firsttran.size(), INTER_AREA);
	//imshow("secondtran", secondtran);

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
		if (match1[i].distance <= multiple * min_dist)
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

	imshow("imgmappermatch", imghomomatch);*/
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

	vector<vector<Point2f>> linear;
	vector<vector<Point>> linearnumber;
	linear.resize(joint.rows*joint.cols);
	linearnumber.resize(joint.rows*joint.cols);


	for (int layer = 0; layer < pointlist.size(); layer++)
	{
		for (int k = 0; k < pointlist[layer].size(); k++)
		{
			int x = (int)(pointlist[layer][k].x);
			int y = (int)(pointlist[layer][k].y);

			Point number;
			number.x = layer;
			number.y = k;

			linearnumber[(y - minup + 5)*joint.cols + x - minleft + 5].push_back(number);
			linearnumber[(y - minup + 5)*joint.cols + x - minleft + 6].push_back(number);
			linearnumber[(y - minup + 6)*joint.cols + x - minleft + 5].push_back(number);
			linearnumber[(y - minup + 6)*joint.cols + x - minleft + 6].push_back(number);
			linear[(y - minup + 5)*joint.cols + x - minleft + 5].push_back(pointlist[layer][k]);
			linear[(y - minup + 5)*joint.cols + x - minleft + 6].push_back(pointlist[layer][k]);
			linear[(y - minup + 6)*joint.cols + x - minleft + 5].push_back(pointlist[layer][k]);
			linear[(y - minup + 6)*joint.cols + x - minleft + 6].push_back(pointlist[layer][k]);
		}


		/*for (int i = 0; i<joint.rows; i++)
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


		}*/

	}

	for (int i = 0; i < linear.size(); i++)
	{
		vector<float> distance;
		distance.resize(linear[i].size());
		float totaldistance = 0;
		for (int j = 0; j < linear[i].size(); j++)
		{
			linear[i][j].x = linear[i][j].x + 5 - minleft;
			linear[i][j].y = linear[i][j].y + 5 - minup;
			distance[j] = 1 / pow(pow(linear[i][j].x - (float)(i%joint.cols), 2) + pow(linear[i][j].x - (float)(i / joint.cols), 2), 0.5);
			totaldistance = totaldistance + distance[j];
		}

		joint.at<Vec3b>(i / joint.cols, i%joint.cols)[0] = 0;
		joint.at<Vec3b>(i / joint.cols, i%joint.cols)[1] = 0;
		joint.at<Vec3b>(i / joint.cols, i%joint.cols)[2] = 0;
		for (int j = 0; j < linear[i].size(); j++)
		{
			joint.at<Vec3b>(i / joint.cols, i%joint.cols)[0] = joint.at<Vec3b>(i / joint.cols, i%joint.cols)[0]
				+ (distance[j] / totaldistance)*matlist[linearnumber[i][j].x].at<Vec3b>(linearnumber[i][j].y / matlist[linearnumber[i][j].x].cols, linearnumber[i][j].y % matlist[linearnumber[i][j].x].cols)[0];
			joint.at<Vec3b>(i / joint.cols, i%joint.cols)[1] = joint.at<Vec3b>(i / joint.cols, i%joint.cols)[1]
				+ (distance[j] / totaldistance)*matlist[linearnumber[i][j].x].at<Vec3b>(linearnumber[i][j].y / matlist[linearnumber[i][j].x].cols, linearnumber[i][j].y % matlist[linearnumber[i][j].x].cols)[1];
			joint.at<Vec3b>(i / joint.cols, i%joint.cols)[2] = joint.at<Vec3b>(i / joint.cols, i%joint.cols)[2]
				+ (distance[j] / totaldistance)*matlist[linearnumber[i][j].x].at<Vec3b>(linearnumber[i][j].y / matlist[linearnumber[i][j].x].cols, linearnumber[i][j].y % matlist[linearnumber[i][j].x].cols)[2];
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

vector<Mat> imageproc::findallfeature(int hessian, int Octave, int Octavelayer, bool extend, bool USURF, vector<Mat> imagerank, vector<vector<KeyPoint>> &keypointrank, vector<Mat> &descriptorrank, vector<int> &warning)
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

	return detectrank;
}

vector<Mat> imageproc::homoallresult(vector<Mat> imagerank, vector<vector<KeyPoint>>keypointrank, vector<Mat> descriptorrank, double initialmultiple, vector<vector<DMatch>> &homomatchrank, vector<Mat> &homotranrank, vector<double> &homoRMSrank, double RMSthreshold, vector<double> &multiplerank, vector<int> &homowarning, vector<int> &RMSwarning, int &warning)
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
	return imgmatchrank;

}

vector<Mat> imageproc::homoallresultnoRANSAC(vector<Mat> imagerank, vector<vector<KeyPoint>>keypointrank, vector<Mat> descriptorrank, double initialmultiple, vector<vector<DMatch>> &homomatchrank, vector<Mat> &homotranrank, vector<double> &homoRMSrank, double RMSthreshold, vector<double> &multiplerank, vector<int> &homowarning, vector<int> &RMSwarning, int &warning)
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
		imgmatchtemp = allhomo.homoresultnoRANSAC(imagerank[i + 1], imagerank[i], keypointrank[i + 1], keypointrank[i], descriptorrank[i + 1], descriptorrank[i],
			multiple, homomatchtemp, homotrantemp, homowarningtemp);


		homomatchrank[i] = homomatchtemp;
		homotranrank[i] = homotrantemp.clone();
		imgmatchrank[i] = imgmatchtemp.clone();
		homowarning[i] = homowarningtemp;
		multiplerank[i] = multiple;

	}
	return imgmatchrank;

}

vector<Mat> imageproc::mapperallresult(vector<Mat> imagerank, vector<Mat> homotranrank, vector<Mat> &mappertranrank, vector<double> &mapperRMS, vector<double> multiplerank, vector<int> &mapperwarning, vector<int> &RMSwarning)
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

	return mapperresultrank;
}

Mat imageproc::jointallimage(vector<Mat> imagerank, vector<Mat> homotranrank, vector<double> homoRMS, vector<Mat> mappertranrank, vector<double> mapperRMS, vector<int> &imagewarning, vector<int> &tranwarning, int &jointwarning)
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

	return allimagejoint;

}

vector<Mat> imageproc::allcolordifferent(vector<Mat> imagerank, vector<Mat> homotranrank, vector<double> homoRMSrank, vector<Mat> mappertranrank, vector<double> mapperRMSrank, vector<int> &warning)
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

Mat imageproc::jointallimageback(vector<Mat> imagerank, vector<Mat> homotranrank, vector<double> homoRMS, vector<Mat> mappertranrank, vector<double> mapperRMS, vector<int> &imagewarning, vector<int> &tranwarning, int &jointwarning)
{
	vector<vector<Point2d>> pointrank;
	pointrank.resize(imagerank.size());
	vector<vector<vector<int>>> pointvaluerank;
	pointvaluerank.resize(imagerank.size());
	vector<vector<Point2d>> fourpointrank;
	fourpointrank.resize(imagerank.size());
	vector<vector<int>> limitnumber;
	limitnumber.resize(imagerank.size());

	imagewarning.resize(imagerank.size());
	tranwarning.resize(imagerank.size());

	for (int i = 0; i < imagerank.size(); i++)
	{
		imagewarning[i] = 0;
		tranwarning[i] = 0;
	}
	jointwarning = 0;

	//計算homo之反矩陣
	vector<Mat> inverserank;
	inverserank.resize(homotranrank.size());
	for (int i = 0; i < homotranrank.size(); i++)
	{
		invert(homotranrank[i], inverserank[i], DECOMP_LU);
	}

	for (int layer = 0; layer < imagerank.size(); layer++)
	{

		Point2d pp;
		pp.x = imagerank[layer].cols; pp.y = 0;
		fourpointrank[layer].push_back(pp);
		pp.x = imagerank[layer].cols; pp.y = 0;
		fourpointrank[layer].push_back(pp);
		pp.x = 0; pp.y = imagerank[layer].rows;
		fourpointrank[layer].push_back(pp);
		pp.x = imagerank[layer].cols; pp.y = imagerank[layer].rows;
		fourpointrank[layer].push_back(pp);

		for (int i = layer; i < homotranrank.size(); i++)
		{
			perspectiveTransform(fourpointrank[layer], fourpointrank[layer], homotranrank[i]);
		}

		int left = (int)fourpointrank[layer][0].x, right = (int)(fourpointrank[layer][0].x + 0.5), up = (int)fourpointrank[layer][0].y, down = (int)(fourpointrank[layer][0].y + 0.5);
		for (int i = 1; i < 4; i++)
		{
			if (fourpointrank[layer][i].x < left)
				left = (int)fourpointrank[layer][i].x;
			if (fourpointrank[layer][i].x > right)
				right = (int)(fourpointrank[layer][i].x + 0.5);
			if (fourpointrank[layer][i].y < up)
				up = (int)fourpointrank[layer][i].y;
			if (fourpointrank[layer][i].y > down)
				down = (int)(fourpointrank[layer][i].y + 0.5);
		}

		limitnumber[layer].push_back(left);
		limitnumber[layer].push_back(right);
		limitnumber[layer].push_back(up);
		limitnumber[layer].push_back(down);

		if (layer < imagerank.size() - 1)
		{

			pointrank[layer].resize((down - up + 1)*(right - left + 1));
			pointvaluerank[layer].resize(pointrank[layer].size());

			vector<Point2d> invertran;
			invertran.resize(pointrank[layer].size());

			for (int i = 0; i < pointrank[layer].size(); i++)
			{
				pointrank[layer][i].x = i % (right - left + 1) + left;
				pointrank[layer][i].y = i / (right - left + 1) + up;
				invertran[i].x = i % (right - left + 1) + left;
				invertran[i].y = i / (right - left + 1) + up;
			}

			for (int i = 0; i < inverserank.size() - layer; i++)
			{
				perspectiveTransform(invertran, invertran, inverserank[inverserank.size() - i - 1]);
			}

			for (int i = 0; i < pointvaluerank[layer].size(); i++)
			{
				pointvaluerank[layer][i].resize(3);
				if (invertran[i].x > 5 && invertran[i].x < imagerank[layer].cols - 5 && invertran[i].y>5 && invertran[i].y < imagerank[layer].rows - 5)
				{
					vector<double> valueup, valuedown;
					valueup.resize(3);
					valuedown.resize(3);
					double dis1, dis2, totaldis;
					dis1 = 1 / (invertran[i].x - (int)invertran[i].x);
					dis2 = 1 / ((int)invertran[i].x + 1 - invertran[i].x);
					totaldis = dis1 + dis2;
					for (int k = 0; k < 3; k++)
					{
						valueup[k] = (dis1 / totaldis)*imagerank[layer].at<Vec3b>((int)invertran[i].y, (int)invertran[i].x)[k] +
							(dis2 / totaldis)*imagerank[layer].at<Vec3b>((int)invertran[i].y, (int)invertran[i].x + 1)[k];
						valuedown[k] = (dis1 / totaldis)*imagerank[layer].at<Vec3b>((int)invertran[i].y + 1, (int)invertran[i].x)[k] +
							(dis2 / totaldis)*imagerank[layer].at<Vec3b>((int)invertran[i].y + 1, (int)invertran[i].x + 1)[k];
					}

					dis1 = 1 / (invertran[i].y - (int)invertran[i].y);
					dis2 = 1 / ((int)invertran[i].y + 1 - invertran[i].y);
					totaldis = dis1 + dis2;
					vector<double> value;
					value.resize(3);

					for (int k = 0; k < 3; k++)
					{
						pointvaluerank[layer][i][k] = (dis1 / totaldis)*valueup[k] + (dis2 / totaldis)*valuedown[k];
					}



				}
				else
				{
					pointvaluerank[layer][i][0] = 0;
					pointvaluerank[layer][i][1] = 0;
					pointvaluerank[layer][i][2] = 0;
				}
			}
		}

	}

	int left = limitnumber[0][0];
	int right = limitnumber[0][0];
	int up = limitnumber[0][2];
	int down = limitnumber[0][2];

	for (int i = 0; i < limitnumber.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (limitnumber[i][0] < left)
				left = limitnumber[i][0];
			if (limitnumber[i][1] > right)
				right = limitnumber[i][1];
			if (limitnumber[i][2] < up)
				up = limitnumber[i][2];
			if (limitnumber[i][3] > down)
				down = limitnumber[i][3];
		}
	}

	Mat allimagejoint(down - up + 1, right - left + 1, CV_8UC3);
	vector<vector<vector<int>>> imagejoint;
	imagejoint.resize((right - left + 1)*(down - up + 1));
	for (int i = 0; i < imagejoint.size(); i++)
	{
		imagejoint[i].resize(3);
	}


	for (int i = 0; i < pointrank.size() - 1; i++)
	{

		for (int j = 0; j < pointrank[i].size(); j++)
		{
			if (pointvaluerank[i][j][0] != 0)
				imagejoint[pointrank[i][j].x - left + (pointrank[i][j].y - up)*(right - left + 1)][0].push_back(pointvaluerank[i][j][0]);
			if (pointvaluerank[i][j][1] != 0)
				imagejoint[pointrank[i][j].x - left + (pointrank[i][j].y - up)*(right - left + 1)][1].push_back(pointvaluerank[i][j][1]);
			if (pointvaluerank[i][j][2] != 0)
				imagejoint[pointrank[i][j].x - left + (pointrank[i][j].y - up)*(right - left + 1)][2].push_back(pointvaluerank[i][j][2]);
		}
	}
	for (int i = 0; i < imagerank[imagerank.size() - 1].rows; i++)
	{
		for (int j = 0; j < imagerank[imagerank.size() - 1].cols; j++)
		{
			imagejoint[j - left + (i - up)*(right - left + 1)][0].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[0]);
			imagejoint[j - left + (i - up)*(right - left + 1)][1].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[1]);
			imagejoint[j - left + (i - up)*(right - left + 1)][2].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[2]);
		}

	}


	for (int i = 0; i < allimagejoint.rows; i++)
		for (int j = 0; j < allimagejoint.cols; j++)
		{
			int count = 0;
			vector<double> value;
			value.resize(3);
			value[0] = 0;
			value[1] = 0;
			value[2] = 0;

			for (int k = 0; k < imagejoint[i*allimagejoint.cols + j][0].size(); k++)
			{
				value[0] = value[0] + imagejoint[i*allimagejoint.cols + j][0][k];
				count++;
			}
			value[0] = value[0] / count;
			count = 0;
			for (int k = 0; k < imagejoint[i*allimagejoint.cols + j][1].size(); k++)
			{
				value[1] = value[1] + imagejoint[i*allimagejoint.cols + j][1][k];
				count++;
			}
			value[1] = value[1] / count;
			count = 0;
			for (int k = 0; k < imagejoint[i*allimagejoint.cols + j][2].size(); k++)
			{
				value[2] = value[2] + imagejoint[i*allimagejoint.cols + j][2][k];
				count++;
			}
			value[2] = value[2] / count;
			allimagejoint.at<Vec3b>(i, j)[0] = value[0];
			allimagejoint.at<Vec3b>(i, j)[1] = value[1];
			allimagejoint.at<Vec3b>(i, j)[2] = value[2];
		}

	/*vector<vector<vector<int>>> imagejointt;
	imagejointt.resize((right - left + 1)*(down - up + 1));
	for (int i = 0; i < imagejointt.size(); i++)
	{
	imagejointt[i].resize(3);
	}


	for (int i = 0; i < imagerank[imagerank.size() - 1].rows; i++)
	{
	for (int j = 0; j < imagerank[imagerank.size() - 1].cols; j++)
	{
	imagejointt[j - left + (i - up)*(right - left + 1)][0].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[0]);
	imagejointt[j - left + (i - up)*(right - left + 1)][1].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[1]);
	imagejointt[j - left + (i - up)*(right - left + 1)][2].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[2]);
	}

	}
	Mat allimagejointt(down - up + 1, right - left + 1, CV_8UC3);
	for (int i = 0; i<allimagejointt.rows; i++)
	for (int j = 0; j < allimagejointt.cols; j++)
	{
	int count = 0;
	vector<double> value;
	value.resize(3);
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;

	for (int k = 0; k < imagejointt[i*allimagejointt.cols + j][0].size(); k++)
	{
	value[0] = value[0] + imagejointt[i*allimagejointt.cols + j][0][k];
	count++;
	}
	value[0] = value[0] / count;
	count = 0;
	for (int k = 0; k < imagejointt[i*allimagejointt.cols + j][1].size(); k++)
	{
	value[1] = value[1] + imagejointt[i*allimagejointt.cols + j][1][k];
	count++;
	}
	value[1] = value[1] / count;
	count = 0;
	for (int k = 0; k < imagejointt[i*allimagejointt.cols + j][2].size(); k++)
	{
	value[2] = value[2] + imagejointt[i*allimagejointt.cols + j][2][k];
	count++;
	}
	value[2] = value[2] / count;
	allimagejointt.at<Vec3b>(i, j)[0] = value[0];
	allimagejointt.at<Vec3b>(i, j)[1] = value[1];
	allimagejointt.at<Vec3b>(i, j)[2] = value[2];
	}
	for (int i = 0; i < 50000; i++)
	{
	imshow("ttt", allimagejoint);
	waitKey(200);
	imshow("ttt", allimagejointt);
	waitKey(200);

	}*/


	return allimagejoint;


}


//int main()
//{
//	vector<string> filename;
//
//	//*****編輯此處，修改輸入資料路徑******
//	/*filename.push_back(".\\test data\\bigimage1.tif");
//	filename.push_back(".\\test data\\bigimage2.tif");
//	filename.push_back(".\\test data\\bigimage3.tif");*/
//	/*filename.push_back(".\\test data\\split4.tif");
//	filename.push_back(".\\test data\\split5.tif");*/
//
//	filename.push_back(".\\test data\\orisplitimage1.tif");
//	filename.push_back(".\\test data\\orisplitimage2.tif");
//	filename.push_back(".\\test data\\orisplitimage3.tif");
//	/*filename.push_back(".\\test data\\orisplitimage4.tif");
//	filename.push_back(".\\test data\\orisplitimage5.tif");*/
//
//
//
//	//*****編輯此處，修改輸入資料路徑******
//
//	imageproc ttt;
//
//	vector<Mat> imagerank;
//	vector<int> readwarning;
//	//輸入影像檔，!!重要!!影像順序第一張輸入之影像，會在成果之最底層，同理類推
//	imagerank = ttt.readallfile(filename, readwarning);
//
//	//非關鍵程式碼，在C++目的為因程式回傳錯誤資訊，用以中斷程式
//	for (int i = 0; i < readwarning.size(); i++)
//	{
//		if (readwarning[i] != 0)
//		{
//			cout << "讀取影像發生錯誤，以下為錯誤編碼:" << endl;
//			for (int j = 0; j < readwarning.size(); j++)
//			{
//				cout << readwarning[j] << endl;
//			}
//			system("pause");
//			exit(0);
//		}
//
//	}
//
//	for (int i = 0; i < imagerank.size(); i++)
//	{
//
//		imshow("第" + to_string(i + 1) + "張影像", imagerank[i]);
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
//
//	}
//
//	for (int i = 0; i < detectrank.size(); i++)
//	{
//
//		imshow("第" + to_string(i + 1) + "張影像detect", detectrank[i]);
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
//	double RMSthreshold = 3;
//
//	//進行相鄰影像轉換矩陣計算
//	/*imgmatchrank = ttt.homoallresult(imagerank, keypointrank, descriptorrank, initialmultiple, homomatchrank, homotranrank, homoRMSrank, RMSthreshold,
//	multiplerank, homowarning, homoRMSwarning, homoallresultwarning);*/
//
//	imgmatchrank = ttt.homoallresult(imagerank, keypointrank, descriptorrank, initialmultiple, homomatchrank, homotranrank, homoRMSrank, RMSthreshold,
//		multiplerank, homowarning, homoRMSwarning, homoallresultwarning);
//
//	/*for (int i = 0; i < imgmatchrank.size(); i++)
//	{
//	imshow("第" + to_string(i + 1) + to_string(i + 2) + "張影像imgmatch", imgmatchrank[i]);
//	}
//
//	waitKey(0);*/
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
//		imshow("第" + to_string(i + 1) + to_string(i + 2) + "張影像imgmatch", imgmatchrank[i]);
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
//		imshow("第" + to_string(i + 1) + to_string(i + 2) + "張影像mapper", mapperresultrank[i]);
//	}
//
//	//waitKey(0);
//
//	/*fstream input1;
//	input1.open("./test data/matchpoint23.txt");
//
//	vector<Point2d> check1;
//	vector<Point2d> check2;
//
//
//
//	for (int i = 0; i < 35; i++)
//	{
//	string x;
//	Point2d ttt;
//	getline(input1, x, ',');
//	ttt.x = atof(x.c_str());
//	getline(input1, x, '	');
//	ttt.y = atof(x.c_str());
//	check1.push_back(ttt);
//
//	getline(input1, x, ',');
//	ttt.x = atof(x.c_str());
//	getline(input1, x, '\n');
//	ttt.y = atof(x.c_str());
//	check2.push_back(ttt);
//	}*/
//
//	/*vector<Point2d> backcheck1;
//	vector<Point2d> backcheck2;
//
//	backcheck1.resize(check1.size());
//	backcheck2.resize(check1.size());
//	double m = 2;
//	double pi = 3.1415926;
//	double degree1 = 3.00152 * pi / 180;
//	double degree2 = 3.00152 * pi / 180;
//
//
//
//	for (int i = 0; i < check1.size(); i++)
//	{
//	backcheck1[i].x = 2 * cos(-degree1)*(check1[i].x + 2500 * cos(degree1) + 2500 * sin(degree1)) + 2 * sin(-degree1)*(check1[i].y + 2500 * cos(degree1) - 2500 * sin(degree1));
//	backcheck1[i].y = -2 * sin(-degree1)*(check1[i].x + 2500 * cos(degree1) + 2500 * sin(degree1)) + 2 * cos(-degree1)*(check1[i].y + 2500 * cos(degree1) - 2500 * sin(degree1));
//
//	backcheck2[i].x = 2 * cos(-degree2)*(check2[i].x + 2900 * cos(degree2) + 2500 * sin(degree2)) + 2 * sin(-degree2)*(check2[i].y + 2500 * cos(degree2) - 2900 * sin(degree2));
//	backcheck2[i].y = -2 * sin(-degree2)*(check2[i].x + 2900 * cos(degree2) + 2500 * sin(degree2)) + 2 * cos(-degree2)*(check2[i].y + 2500 * cos(degree2) - 2900 * sin(degree2));
//
//
//
//	}
//	*/
//	//fstream file;
//	//file.open("./test data/doublechange.txt", ios::out);
//
//	///*for (int i = 0; i < backcheck1.size(); i++)
//	//{
//	//	file << setw(15) << setprecision(15) << backcheck1[i].x << "," << setw(15) << setprecision(15) << backcheck1[i].y << "	" << setw(15) << setprecision(15) << backcheck2[i].x << "," << setw(15) << setprecision(15) << backcheck2[i].y << endl;
//	//}*/
//
//	//Mat inverse;
//	//invert(homotranrank[0], inverse, DECOMP_LU);
//
//	//perspectiveTransform(check1, check1, homotranrank[0]);
//	///*perspectiveTransform(check1, check1, homotranrank[1]);
//	//perspectiveTransform(check1, check1, homotranrank[2]);
//	//perspectiveTransform(check1, check1, homotranrank[3]);
//
//	//perspectiveTransform(check2, check2, homotranrank[1]);
//	//perspectiveTransform(check2, check2, homotranrank[2]);
//	//perspectiveTransform(check2, check2, homotranrank[3]);*/
//
//	///*vector<KeyPoint> testcheck1;
//	//vector<KeyPoint> testcheck2;
//
//	//for (int i = 0; i < check1.size(); i++)
//	//{
//	//	KeyPoint ttt;
//	//	ttt.pt.x = check1[i].x;
//	//	ttt.pt.y = check1[i].y;
//	//	testcheck1.push_back(ttt);
//
//	//	ttt.pt.x = check2[i].x;
//	//	ttt.pt.y = check2[i].y;
//	//	testcheck2.push_back(ttt);
//	//}
//
//	//Mat detect1;
//	//Mat detect2;
//	//drawKeypoints(imagerank[1], testcheck1, detect1, Scalar(100, 150, 0, 0));
//	//drawKeypoints(imagerank[1], testcheck2, detect2, Scalar(100, 150, 0, 0));
//
//	//warpPerspective(imagerank[0], detect1, homotranrank[0], detect1.size());
//	//warpPerspective(detect1, detect1, homotranrank[0], detect1.size());
//	//warpPerspective(imagerank[1], imagerank[1], homotranrank[0], detect1.size());
//
//	//for (int i = 0; i < 50000; i++)
//	//{
//	//	imshow("001", detect1);
//	//	waitKey(200);
//	//	imshow("001", imagerank[1]);
//	//	waitKey(200);
//	//}*/
//
//
//
//
//	//vector<int> globalornot;
//	//globalornot.resize(check1.size());
//
//	//vector<double> errornumber;
//	//errornumber.resize(check1.size());
//	//vector<double> onetime;
//	//onetime.resize(check1.size());
//
//
//	//double averageerror = 0;
//	//for (int i = 0; i < check1.size(); i++)
//	//{
//	//	double oncetime = 0;
//	//	onetime[i] = pow(pow(check1[i].x - check2[i].x, 2) + pow(check1[i].y - check2[i].y, 2), 0.5);
//	//	averageerror = averageerror + onetime[i];
//	//	if (check1[i].x - check2[i].x > 0)
//	//	{
//	//		if (check1[i].y - check2[i].y > 0)
//	//		{
//	//			globalornot[i] = 1;
//	//		}
//	//		else
//	//		{
//	//			globalornot[i] = 2;
//	//		}
//	//	}
//	//	else
//	//	{
//	//		if (check1[i].y - check2[i].y > 0)
//	//		{
//	//			globalornot[i] = 3;
//	//		}
//	//		else
//	//		{
//	//			globalornot[i] = 4;
//	//		}
//	//	}
//
//
//
//	//}
//
//	//averageerror = averageerror / check1.size();
//
//
//	//cout << averageerror << endl;
//
//
//
//	//waitKey(0);
//
//	//Mat jointimage;
//	//vector<Mat> tranimage;
//	//tranimage.resize(imagerank.size());
//
//	//for (int i = 0; i < imagerank.size()-1; i++)
//	//{
//	//	tranimage[i] = imagerank[i].clone();
//	//	for (int j = i; j < imagerank.size()-1; j++)
//	//	{
//	//		warpPerspective(tranimage[i], tranimage[i], homotranrank[j], imagerank[imagerank.size()-1].size(),INTER_LINEAR+CV_WARP_FILL_OUTLIERS,BORDER_CONSTANT);
//	//	}
//	//	Mat transition;
//	//	transition = tranimage[i].clone();
//	//	for (int m = 0; m < tranimage[i].rows; m++)
//	//		for (int n = 0; n < tranimage[i].cols; n++)
//	//		{
//	//			if (tranimage[i].at<Vec3b>(m, n)[0] == 0 || tranimage[i].at<Vec3b>(m, n)[1] == 0 || tranimage[i].at<Vec3b>(m, n)[2] == 0)
//	//			{
//	//				if (m < tranimage[i].rows - 1 && m>0 && n < tranimage[i].cols - 1 && n>0)
//	//				{
//	//					for (int g = -1; g < 2; g++)for (int h = -1; h < 2; h++)
//	//					{
//	//						transition.at<Vec3b>(m + g, n + h)[0] = 0;
//	//						transition.at<Vec3b>(m + g, n + h)[1] = 0;
//	//						transition.at<Vec3b>(m + g, n + h)[2] = 0;
//	//					}
//	//				}
//	//			}
//	//		}
//	//	tranimage[i] = transition.clone();
//
//
//	//}
//	//tranimage[imagerank.size() - 1] = imagerank[imagerank.size() - 1].clone();
//
//	//for (int i = 0; i < tranimage.size(); i++)
//	//{
//
//	//	imshow("第" + to_string(i + 1) + to_string(i + 2) + "張影像", tranimage[i]);
//	//}
//
//	//waitKey(0);
//
//
//	//jointimage = tranimage[tranimage.size() - 1].clone();
//
//	//for (int i = 0; i < tranimage[tranimage.size()-1].rows; i++)
//	//	for (int j = 0; j < tranimage[tranimage.size() - 1].cols; j++)
//	//	{
//	//		int count = 0;
//	//		vector<double> valuerank;
//	//		valuerank.resize(3);
//	//		valuerank[0] = 0;
//	//		valuerank[1] = 0;
//	//		valuerank[2] = 0;
//	//		for (int m = 0; m < tranimage.size(); m++)
//	//		{
//	//			if (tranimage[m].at<Vec3b>(i, j)[0] == 0 || tranimage[m].at<Vec3b>(i, j)[1] == 0 || tranimage[m].at<Vec3b>(i, j)[2] == 0 )
//	//			{
//	//				continue;
//	//			}
//	//			//if(tranimage[m].rows<i&&tranimage[m].cols)
//	//			count++;
//	//			valuerank[0] = valuerank[0] + tranimage[m].at<Vec3b>(i, j)[0];
//	//			valuerank[1] = valuerank[1] + tranimage[m].at<Vec3b>(i, j)[1];
//	//			valuerank[2] = valuerank[2] + tranimage[m].at<Vec3b>(i, j)[2];
//	//		}
//	//		if (count > 0)
//	//		{
//	//			jointimage.at<Vec3b>(i, j)[0] = (int)(valuerank[0] / count + 0.5);
//	//			jointimage.at<Vec3b>(i, j)[1] = (int)(valuerank[1] / count + 0.5);
//	//			jointimage.at<Vec3b>(i, j)[2] = (int)(valuerank[2] / count + 0.5);
//	//		}
//
//	//	}
//	//
//	//	
//	//imshow("jointimage", jointimage);
//	//imwrite("jointimage.tif", jointimage);
//
//	//waitKey(0);
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	Mat allimagejoint;
//	vector<int> imagewarning;
//	vector<int> tranwarning;
//	int jointwarning;
//
//
//	//輸入影像序列，獲得影像序列對位成果，先輸入之影像在下層 後輸入影像在上層
//
//	allimagejoint = ttt.jointallimageback(imagerank, homotranrank, homoRMSrank, mappertranrank, mapperRMSrank, imagewarning, tranwarning, jointwarning);
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
//	imshow("allimagejoint", allimagejoint);
//	//waitKey(0);
//
//	imwrite("./test data/allimagejoint.tif", allimagejoint);
//
//	vector<Mat> colorrank;
//	vector<int> colorwarning;
//
//	//輸出影像序列相鄰影像之融合圖，B色階與GR色階分別為兩張不同之影像
//	//colorrank = ttt.allcolordifferent(imagerank, homotranrank, homoRMSrank, mappertranrank, mapperRMSrank, colorwarning);
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
//		imshow("第" + to_string(i + 1) + to_string(i + 2) + "張影像colordifferent", colorrank[i]);
//
//		imwrite("./test data/color" + to_string(i + 1) + to_string(i + 2) + ".tif", colorrank[i]);
//	}
//
//
//	waitKey(0);
//	return 0;
//}

#pragma endregion

#pragma region "other functions"
bool saveVectorOfMat(vector<Mat> matSeq, string tarDir)
{
	string ext = ".tif";

	if (matSeq.empty())
		return false;

	for (int i = 0; i < matSeq.size(); i++)
	{
		imwrite(tarDir + "\\" + to_string(i + 1) + ext, matSeq[i]);
	}
	return true;
}

System::String^ str2Str(string str)
{
	return gcnew System::String(str.c_str());
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
#pragma endregion


bool matchFunc(char* fileName_c, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, OutputArray result)
{
	string logName = ".\\PIMCore\\log.txt";
	string fileName = ".\\PIMCore\\" + string(fileName_c);


	string resDir = ".\\Result";
	string resSubDir1 = resDir + "\\1. Input Image";
	string resSubDir2 = resDir + "\\2. Detected Feature";
	string resSubDir3 = resDir + "\\3. Homography";
	string resSubDir4 = resDir + "\\4. Mapper";
	string resSubDir5 = resDir + "\\5. Joint Image";

	if (Directory::Exists(str2Str(resDir)))
		Directory::Delete(str2Str(resDir), true);

	Directory::CreateDirectory(str2Str(resDir));
	Directory::CreateDirectory(str2Str(resSubDir1));
	Directory::CreateDirectory(str2Str(resSubDir2));
	Directory::CreateDirectory(str2Str(resSubDir3));
	Directory::CreateDirectory(str2Str(resSubDir4));
	Directory::CreateDirectory(str2Str(resSubDir5));


	StreamReader^ sr = gcnew StreamReader(str2Str(fileName));
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

	//---1---
	//輸入影像檔，!!重要!!影像順序第一張輸入之影像，會在成果之最底層，同理類推
	imgSeq = ful.readallfile(fileSeq, readwarning);
	if (errorCheck(readwarning))
		return false;
	else
		saveVectorOfMat(imgSeq, resSubDir1);

	vector<vector<KeyPoint>> keypointrank;
	vector<Mat> descriptorrank;

	vector<Mat> detectrank;
	vector<int> detectwarning;

	//---2---
	//進行影像序列偵測特徵點
	detectrank = ful.findallfeature(minHessian, octave, octaveLayer, extended, upRight, imgSeq, keypointrank, descriptorrank, detectwarning);
	if (errorCheck(detectwarning))
		return false;
	else
		saveVectorOfMat(detectrank, resSubDir2);

	vector<Mat> imgmatchrank;
	double initialmultiple = 5;
	vector<vector<DMatch>> homomatchrank;
	vector<Mat> homoTranSeq;
	vector<double> homoRmsSeq;
	vector<double> multiplerank;
	vector<int> homowarning;
	vector<int> homoRMSwarning;
	int homoallresultwarning;

	//---3---
	//進行相鄰影像轉換矩陣計算
	imgmatchrank = ful.homoallresult(imgSeq, keypointrank, descriptorrank, initialmultiple, homomatchrank, homoTranSeq, homoRmsSeq, rmsThres, multiplerank, homowarning, homoRMSwarning, homoallresultwarning);
	if (errorCheck(homowarning) + errorCheck(homoRMSwarning) + homoallresultwarning)
		return false;
	else
		saveVectorOfMat(imgmatchrank, resSubDir3);

	vector<Mat> mapperresultrank;
	vector<Mat> mapperTranSeq;
	vector<double> mapperRmsSeq;
	vector<int> mapperwarning;
	vector<int> mapperRMSwarning;

	//---4---
	//進行相鄰影像mapper運算獲得二次轉換矩陣
	mapperresultrank = ful.mapperallresult(imgSeq, homoTranSeq, mapperTranSeq, mapperRmsSeq, multiplerank, mapperwarning, mapperRMSwarning);
	if (errorCheck(mapperwarning) + errorCheck(mapperRMSwarning))
		return false;
	else
		saveVectorOfMat(mapperresultrank, resSubDir4);

	Mat allimagejoint;
	vector<int> imagewarning;
	vector<int> tranwarning;
	int jointwarning;
	vector<Mat> vecOfAllimagejoint;

	//輸入影像序列，獲得影像序列對位成果，先輸入之影像在下層 後輸入影像在上層
	allimagejoint = ful.jointallimageback(imgSeq, homoTranSeq, homoRmsSeq, mapperTranSeq, mapperRmsSeq, imagewarning, tranwarning, jointwarning);
	vecOfAllimagejoint.push_back(allimagejoint);
	if (errorCheck(imagewarning) + errorCheck(tranwarning) + jointwarning)
		return false;
	else
		saveVectorOfMat(vecOfAllimagejoint, resSubDir5);

	allimagejoint.copyTo(result);

	return true;
}