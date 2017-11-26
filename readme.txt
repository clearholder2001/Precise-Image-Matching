//輸入影像檔，!!重要!!影像順序第一張輸入之影像，會在成果之最底層，同理類推
imagerank = ttt.readallfile(filename, readwarning);
vector<Mat> readallfile(vector<string>, vector<int> &);


//進行影像序列偵測特徵點
detectrank = ttt.findallfeatur(hessian, Octave, Octavelayer, extend, USURF, imagerank, keypointrank, descriptorrank, detectwarning);
vector<Mat> findallfeatur(int, int, int, bool, bool, vector<Mat>, vector<vector<KeyPoint>> &, vector<Mat> &, vector<int> &);


//進行相鄰影像轉換矩陣計算
imgmatchrank = ttt.homoallresult(imagerank, keypointrank, descriptorrank, initialmultiple, homomatchrank, homotranrank, homoRMSrank, RMSthreshold, multiplerank, homowarning, homoRMSwarning, homoallresultwarning);
vector<Mat> homoallresult(vector<Mat>, vector<vector<KeyPoint>>, vector<Mat>, double, vector<vector<DMatch>> &, vector<Mat> &, vector<double> &, double, vector<double> &, vector<int> &, vector<int> &, int &);


//進行相鄰影像mapper運算獲得二次轉換矩陣
mapperresultrank = ttt.mapperallresult(imagerank, homotranrank, mappertranrank, mapperRMSrank, multiplerank, mapperwarning, mapperRMSwarning);
vector<Mat> mapperallresult(vector<Mat>, vector<Mat>, vector<Mat> &, vector<double> &, vector<double>, vector<int> &, vector<int> &);


//輸入影像序列，獲得影像序列對位成果，先輸入之影像在下層 後輸入影像在上層
allimagejoint = ttt.jointallimage(imagerank, homotranrank, homoRMSrank, mappertranrank, mapperRMSrank, imagewarning, tranwarning, jointwarning);
Mat jointallimage(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &, vector<int> &, int &);


//輸出影像序列相鄰影像之融合圖，B色階與GR色階分別為兩張不同之影像
colorrank = ttt.allcolordifferent(imagerank, homotranrank, homoRMSrank, mappertranrank, mapperRMSrank, colorwarning);
vector<Mat> allcolordifferent(vector<Mat>, vector<Mat>, vector<double>, vector<Mat>, vector<double>, vector<int> &);


new func
1. vector<Mat> calFeatureAndMatch();
2. Mat projImageSequence(vector<Mat> imgSeq, vector<Mat> homoTranSeq, vector<double> homoRmsSeq, vector<Mat> mapperTranSeq, vector<double> mapperRmsSeq);


輸入參數：
vector<Mat> imagerank;
int hessian = 300;
int Octave = 5;
int Octavelayer = 5;
bool extend = true;
bool USURF = false;
double RMSthreshold = 2;


回傳參數：
vector<Mat> homotranrank;
vector<Mat> mappertranrank;
vector<double> mapperRMSrank;
vector<double> homoRMSrank;


固定參數：
ransacReprojThreshold = 3, 1;