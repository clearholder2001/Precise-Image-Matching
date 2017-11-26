/// -----------------------------------------------
/// 作者：陳俋臣
/// 修改日期：2017/11/20
/// 內容：C++程序呼叫函式
/// -----------------------------------------------


#include "PIMCppExport.h"


using namespace System;
using namespace System::IO;


#pragma region "other functions"
bool saveVectorOfMat(vector<Mat> matSeq, string tarDir)
{
	string ext = ".tif";

	if (matSeq.empty())
		return false;

	for (int i = 0; i < matSeq.size(); i++)
	{
		char c[3];
		snprintf(c, sizeof(c), "%02d", i + 1);
		imwrite(tarDir + "\\" + c + ext, matSeq[i]);
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

template<typename T>
inline void vecRelease(vector<T> & v) {
	vector<T> t;
	v.swap(t);
}
#pragma endregion


bool matchFunc(char* fileName_c, int minHessian, int octave, int octaveLayer, bool extended, bool upRight, double rmsThres, bool enableRANSAC, OutputArray result)
{
	string logName = ".\\PIMCore\\log.txt";
	string fileName = ".\\PIMCore\\" + string(fileName_c);


	string resDir = ".\\Matching Result";
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
	if (enableRANSAC)
		imgmatchrank = ful.homoallresult(imgSeq, keypointrank, descriptorrank, initialmultiple, homomatchrank, homoTranSeq, homoRmsSeq, rmsThres, multiplerank, homowarning, homoRMSwarning, homoallresultwarning);
	else
		imgmatchrank = ful.homoallresultnoRANSAC(imgSeq, keypointrank, descriptorrank, initialmultiple, homomatchrank, homoTranSeq, homoRmsSeq, rmsThres, multiplerank, homowarning, homoRMSwarning, homoallresultwarning);
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

	vecRelease(imgSeq);
	vecRelease(readwarning);
	vecRelease(keypointrank);
	vecRelease(descriptorrank);
	vecRelease(detectrank);
	vecRelease(detectwarning);
	vecRelease(imgmatchrank);
	vecRelease(homomatchrank);
	vecRelease(homoTranSeq);
	vecRelease(homoRmsSeq);
	vecRelease(multiplerank);
	vecRelease(homowarning);
	vecRelease(homoRMSwarning);
	vecRelease(mapperresultrank);
	vecRelease(mapperTranSeq);
	vecRelease(mapperRmsSeq);
	vecRelease(mapperwarning);
	vecRelease(mapperRMSwarning);
	vecRelease(imagewarning);
	vecRelease(tranwarning);
	vecRelease(vecOfAllimagejoint);

	return true;
}