/// -----------------------------------------------
/// 作者：蔡易澄、陳俋臣
/// 修改日期：2017/11/20
/// 內容：影像匹配核心與操作函式(使用OpenCV)
/// -----------------------------------------------


#include "PIMCppCore.h"


template<typename T>
inline void vecRelease(vector<T> & v) {
	vector<T> t;
	v.swap(t);
}

#pragma region "蔡易澄's codes using OpenCV"
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


	/*log修正*/
	/*cout << "----------find the feature point-----------" << endl;
	cout << ""<<keypoint.size() << endl;*/

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

#pragma omp parallel for
	for (int i = 0; i < matches.size(); i++)
	{
		match1[i] = matches[i][0];

	}

	double min_dist = 100;
	double max_dist = 0;

#pragma omp parallel for
	for (int i = 0; i < descriptor1.rows; i++)
	{
		double dist = match1[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	/*log修正*/
	//cout << match1.size() << endl;
	//cout << max_dist - min_dist << endl;


	vector<DMatch> good_match;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		if (match1[i].distance <= multiple * min_dist)
		{
			good_match.push_back(match1[i]);
		}
	}

	/*log修正*/
	//cout << "multipleSIZE:	" << good_match.size() << endl;


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

	/*log修正*/
	cout << "Homography Matrix:" << endl;
	cout << scientific;
	cout.precision(3);
	for (int m = 0; m < 3; m++)
	{
		for (int n = 0; n < 3; n++)
		{
			cout << scientific << homotran.at<double>(m, n);
			if (n < 2)
			{
				cout << ", ";
			}
		}
		cout << endl;
	}
	cout << defaultfloat;
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
	//cout << match1.size() << endl;
	//cout << max_dist - min_dist << endl;


	vector<DMatch> good_match;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		if (match1[i].distance <= multiple * min_dist)
		{
			good_match.push_back(match1[i]);
		}
	}
	//cout << "multipleSIZE:	" << good_match.size() << endl;
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
	
	/*log修正*/
	cout << "Homography Matrix:" << endl;
	cout << scientific;
	cout.precision(3);
	for (int m = 0; m < 3; m++)
	{
		for (int n = 0; n < 3; n++)
		{
			cout << scientific << homotran.at<double>(m, n);
			if (n < 2)
			{
				cout << ", ";
			}
		}
		cout << endl;
	}
	cout << defaultfloat;
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

	//imshow("image1", image1);
	//imshow("imghomo", imghomotran);

	//waitKey(0);

	for (int i = 0; i < sp1.rows; i++)
		for (int j = 0; j < sp1.cols; j++)
		{
			sp1.at<Vec3b>(i, j) = image1.at<Vec3b>(miny + i, minx + j);
			sp2.at<Vec3b>(i, j) = imghomotran.at<Vec3b>(miny + i, minx + j);
		}

	//imshow("sp1", sp1);
	//imshow("sp2", sp2);

	//waitKey(0);

	//sp1.convertTo(sp1, CV_64FC3);
	//sp2.convertTo(sp2, CV_64FC3);

	Ptr<MapperGradProj> mapper = makePtr<MapperGradProj>();
	Ptr<Map> mapPtr;
	mapPtr = mapper->calculate(sp1, sp2, mapPtr);
	
	//原始寫法
	//MapperPyramid mappPyr(mapper);
	//Ptr<Map> mapPtr = mappPyr.calculate(sp1, sp2);

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

	cout << "RMS: " << RMS << endl;

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


	/*log修正*/
	cout << "---SURF Matching Start---" << endl;

	for (int i = 0; i < keypointrank.size(); i++)
	{
		cout << "Image #" << i + 1 << " : " << keypointrank[i].size() << " keypoints" << endl;
	}




	return detectrank;
}

vector<Mat> imageproc::homoallresult(vector<Mat> imagerank, vector<vector<KeyPoint>>keypointrank, vector<Mat> descriptorrank, double initialmultiple, vector<vector<DMatch>> &homomatchrank, vector<Mat> &homotranrank, vector<double> &homoRMSrank,	double RMSthreshold, vector<double> &multiplerank, vector<int> &homowarning, vector<int> &RMSwarning, int &warning)
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

	/*log修正*/



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

			/*log修正*/
			cout << "Matched points of Image #" << i + 1 << " and Image #" << i + 2 << " : " << homomatchtemp.size() << endl;

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
					/*log修正*/
					cout << "RMS <= Threshold" << endl;
					cout << "---SURF Matching End---" << endl;
					cout << endl << endl;

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

			/*log修正*/
			cout << "RMS > Threshold" << endl;
			cout << "---SURF Matching Again---" << endl;
			cout << endl << endl;


		}

	}
	return imgmatchrank;

}

vector<Mat> imageproc::homoallresultnoRANSAC(vector<Mat> imagerank, vector<vector<KeyPoint>>keypointrank, vector<Mat> descriptorrank, double initialmultiple, vector<vector<DMatch>> &homomatchrank, vector<Mat> &homotranrank, vector<double> &homoRMSrank,	double RMSthreshold, vector<double> &multiplerank, vector<int> &homowarning, vector<int> &RMSwarning, int &warning)
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

		/*log修正*/
		cout << "Matched points of Image #" << i + 1 << " and Image #" << i + 2 << " : " << homomatchtemp.size() << endl;
		cout << "---SURF Matching End---" << endl;
		cout << endl << endl;

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

	cout << "Mapper RMS: " << endl;
	for (int i = 0; i < homotranrank.size(); i++)
	{
		mapperresultrank[i] = allmapper.mapperresult(imagerank[i + 1], imagerank[i], homotranrank[i], mappertranrank[i], mapperwarning[i]);
		mapperRMS[i] = allmapper.mapperRMS(imagerank[i + 1], imagerank[i], homotranrank[i], mappertranrank[i], RMSwarning[i], multiplerank[i]);

		cout << "Image #" << i + 1 << " and Image #" << i + 2 << " : " << mapperRMS[i] << endl;
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
	vector<vector<vector<byte>>> pointvaluerank;
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

#pragma omp parallel for
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

#pragma omp parallel for
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

					vecRelease(value);
					vecRelease(valueup);
					vecRelease(valuedown);
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

	vecRelease(inverserank);
	vecRelease(fourpointrank);

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

	vecRelease(limitnumber);

	Mat allimagejoint(down - up + 1, right - left + 1, CV_8UC3);
	vector<vector<vector<byte>>> imagejoint;
	imagejoint.resize((right - left + 1)*(down - up + 1));
	int imgNum = imagerank.size();
#pragma omp parallel for
	for (int i = 0; i < imagejoint.size(); i++)
	{
		imagejoint[i].resize(3);
		imagejoint[i][0].reserve(imgNum);
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

	vecRelease(pointvaluerank);
	vecRelease(pointrank);

	for (int i = 0; i < imagerank[imagerank.size() - 1].rows; i++)
	{
		for (int j = 0; j < imagerank[imagerank.size() - 1].cols; j++)
		{
			imagejoint[j - left + (i - up)*(right - left + 1)][0].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[0]);
			imagejoint[j - left + (i - up)*(right - left + 1)][1].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[1]);
			imagejoint[j - left + (i - up)*(right - left + 1)][2].push_back(imagerank[imagerank.size() - 1].at<Vec3b>(i, j)[2]);
		}
	}

	vecRelease(imagerank);

#pragma omp parallel for
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

			vecRelease(value);
		}

	vecRelease(imagejoint);

	return allimagejoint;
}
#pragma endregion