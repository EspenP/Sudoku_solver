
#include "train_class.h"
#include <string>


Trainer::Trainer():
	hog(    
	cv::Size(20,20), //winSize
    cv::Size(8,8), //blocksize
    cv::Size(4,4), //blockStride,
    cv::Size(8,8), //cellSize,
    9,   //nbins,
    1,   //derivAper,
    -1,  //winSigma,
    cv::HOGDescriptor::HistogramNormType::L2Hys, //histogramNormType,
    0.2, //L2HysThresh,
    0,   //gammal correction,
    64,  //nlevels=64
    1),
    data_path("/home/espen/projects/Sudoku_solver/training/"),
    SZ(20),
    affineFlags(cv::WARP_INVERSE_MAP|cv::INTER_LINEAR),
    height(30),
    width(30),
    training_folder("training_test_data/training_data/"),
    test_folder("training_test_data/testing_data/"),
    model()

{
	// No op
}

void Trainer::Set_path(string path)
{
	data_path = path;
}

cv::Mat Trainer::deskew(cv::Mat& img)
{
	cv::Moments m = cv::moments(img);
	if(abs(m.mu02) < 1e-2)
	{
		return img.clone();
	}
	float skew = m.mu11/m.mu02;
	cv::Mat warpMat = (cv::Mat_<float>(2,3) << 1, skew, -0.5*SZ*skew, 0, 1, 0);
	cv::Mat imgOut = cv::Mat::zeros(img.rows, img.cols, img.type());
	cv::warpAffine(img, imgOut, warpMat, imgOut.size(),affineFlags);
	return imgOut;
}

void Trainer::load_data(vector<cv::Mat>& images, vector<int>& labels, string folder)
{
	for(int i = 0; i <= 9; i++)
	{
		vector<string> fns;
		cv::glob(data_path + folder + to_string(i) + "/*.png", fns, false);
		for(auto f : fns)
		{
			cv::Mat im = cv::imread(f, cv::IMREAD_GRAYSCALE);
			cv::Mat im_rsz;
			cv::resize(im, im_rsz, cv::Size(width, height), cv::INTER_LINEAR);
			images.push_back(im_rsz);
			labels.push_back(i);
		}
	}
}


void Trainer::LoadTrainTestLabels(	vector<cv::Mat>& train_cells, 
                                	vector<cv::Mat>& test_cells, 
                                	vector<int>& train_labels, 
                                	vector<int>& test_labels)
{
	load_data(train_cells, train_labels, training_folder);
	cout << "Successfully loaded training data of " << 
			train_cells.size() << " images" << endl;
	load_data(test_cells, test_labels, test_folder);
	cout << "Successfully loaded testing data of " << 
			test_cells.size() << " images" << endl;
}



void Trainer::CreateDeskewedTrainTest(	vector<cv::Mat>& deskewedTrainCells,
										vector<cv::Mat>& deskewedTestCells, 
										vector<cv::Mat>& trainCells, 
										vector<cv::Mat>& testCells){

    for(int i=0;i<trainCells.size();i++)
    {

        cv::Mat deskewedImg = deskew(trainCells[i]);
        deskewedTrainCells.push_back(deskewedImg);
    }

    for(int i=0;i<testCells.size();i++)
    {
        cv::Mat deskewedImg = deskew(testCells[i]);
        deskewedTestCells.push_back(deskewedImg);
    }

    cout << "Deskewed cells created" << endl;
}



void Trainer::CreateTrainTestHOG(vector<vector<float> > &trainHOG, vector<vector<float> > &testHOG, vector<cv::Mat> &deskewedtrainCells, vector<cv::Mat> &deskewedtestCells)
{
    for(int y=0;y<deskewedtrainCells.size();y++)
    {
        vector<float> descriptors;
        hog.compute(deskewedtrainCells[y],descriptors);
        trainHOG.push_back(descriptors);
    }

    for(int y=0;y<deskewedtestCells.size();y++)
    {

        vector<float> descriptors;
        hog.compute(deskewedtestCells[y],descriptors);
        testHOG.push_back(descriptors);
    }

    cout << "TrainTestHOG Created" << endl;
}

void Trainer::ConvertVectortoMatrix(vector<vector<float> > &trainHOG, vector<vector<float> > &testHOG, cv::Mat &trainMat, cv::Mat &testMat)
{
    int descriptor_size = trainHOG[0].size();

    for(int i = 0;i<trainHOG.size();i++)
    {
        for(int j = 0;j<descriptor_size;j++)
        {
           trainMat.at<float>(i,j) = trainHOG[i][j];
        }
    }

    for(int i = 0;i<testHOG.size();i++)
    {
        for(int j = 0;j<descriptor_size;j++)
        {
            testMat.at<float>(i,j) = testHOG[i][j];
        }
    }

    cout << "Vectors Converted to Matrices" << endl;
}

void Trainer::DisplaySVMParams()
{
    cout << "Kernel type     : " << model->getKernelType() << endl;
    cout << "Type            : " << model->getType() << endl;
    cout << "C               : " << model->getC() << endl;
    cout << "Degree          : " << model->getDegree() << endl;
    cout << "Nu              : " << model->getNu() << endl;
    cout << "Gamma           : " << model->getGamma() << endl;
}

void Trainer::svmInit(float C, float gamma)
{
  cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
  svm->setGamma(gamma);
  svm->setC(C);
  svm->setKernel(cv::ml::SVM::RBF);
  svm->setType(cv::ml::SVM::C_SVC);

  model = svm;
}

void Trainer::svmTrain(cv::Mat &trainMat, vector<int> &trainLabels)
{
  cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(trainMat, cv::ml::ROW_SAMPLE, trainLabels);
  model->train(td);
  model->save(data_path + "models/DigitClassifierSVM.yml"); // Edit this
}

void Trainer::svmPredict(cv::Mat &testResponse, cv::Mat &testMat )
{
  model->predict(testMat, testResponse);
}

void Trainer::SVMevaluate(cv::Mat &testResponse, float &count, float &accuracy, vector<int> &testLabels)
{
  for(int i = 0; i < testResponse.rows; i++)
  {
    // cout << testResponse.at<float>(i,0) << " " << testLabels[i] << endl;
    if(testResponse.at<float>(i,0) == testLabels[i])
      count++;
  }
  accuracy = (count/testResponse.rows)*100;

  cout << endl << "RESULTS: " << endl;
  cout << "Number of guesses: " << testResponse.rows << endl;
  cout << "Correct guesses: " << count << endl;
  cout << "Accuracy: " << accuracy << endl;
}



