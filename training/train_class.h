/**
 * A class designed to implement a training program
 * for a model that can classify images of typeset numbers.
 * Written by Espen Peterson using examples from
 * https://learnopencv.com/handwritten-digits-classification-an-opencv-c-python-tutorial/
 * On 08/14/2022 
 **/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/ml.hpp>
#include <vector>
#include <string>

using namespace std;

class Trainer
{
public:
    /* Public Functions */
	Trainer();

    void Set_path(string path);

    void LoadTrainTestLabels(   vector<cv::Mat>& trainCells, 
                                vector<cv::Mat>& testCells, 
                                vector<int>& trainLabels, 
                                vector<int>& testLabels);

    void CreateDeskewedTrainTest(   vector<cv::Mat> &deskewedTrainCells, 
                                    vector<cv::Mat> &deskewedTestCells, 
                                    vector<cv::Mat> &trainCells, 
                                    vector<cv::Mat> &testCells);

    void CreateTrainTestHOG(vector<vector<float> >& trainHOG, 
                            vector<vector<float> >& testHOG, 
                            vector<cv::Mat>& deskewedtrainCells, 
                            vector<cv::Mat>& deskewedtestCells);


    void ConvertVectortoMatrix( vector<vector<float> > &trainHOG, 
                                vector<vector<float> > &testHOG, 
                                cv::Mat &trainMat, 
                                cv::Mat &testMat);

    void DisplaySVMParams();

    void svmInit(float C, float gamma);

    void svmTrain(cv::Mat& trainMat, vector<int>& trainLabels);

    void svmPredict(cv::Mat& testResponse, cv::Mat& testMat);

    void SVMevaluate(cv::Mat& testResponse, float& count, float& accuracy, vector<int>& testLabels);



private:
    /* Private Variables */
    cv::Ptr<cv::ml::SVM> model;

	int SZ;

    string data_path;

    string training_folder;

    string test_folder;

    int height;

    int width;

	float affineFlags;

    cv::HOGDescriptor hog;

    /* Private Functions */
    cv::Mat deskew(cv::Mat& img);

    void load_data(vector<cv::Mat>& images, vector<int>& labels, string path);
};

 
