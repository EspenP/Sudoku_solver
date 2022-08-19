/**
 * train.cpp
 * Used to train an SVM to recognize
 * digits in a Sudoku puzzle. 
 * Written by Espen Peterson
 * Using examples from https://learnopencv.com/handwritten-digits-classification-an-opencv-c-python-tutorial/
 * 
 * 08/14/2022
 **/


#include "train_class.h"
#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
    string path = argc >= 2 ? argv[1] : "/home/espen/projects/Sudoku_solver/training/"; 
	Trainer trn;
    trn.Set_path(path);

    vector<cv::Mat> train_cells, test_cells;
    vector<int> train_labels, test_labels; 
    trn.LoadTrainTestLabels(train_cells, test_cells, train_labels, test_labels);

    vector<cv::Mat> deskewed_train_cells, deskewed_test_cells;
    trn.CreateDeskewedTrainTest(deskewed_train_cells, 
                                deskewed_test_cells,
                                train_cells,
                                test_cells);


    vector<vector<float>> train_HOG;
    vector<vector<float>> test_HOG;
    trn.CreateTrainTestHOG( train_HOG, 
                            test_HOG, 
                            deskewed_train_cells, 
                            deskewed_test_cells);


    int descriptor_size = train_HOG[0].size();
    cout << "Descriptor Size: " << descriptor_size << endl;

    cv::Mat train_mat(train_HOG.size(), descriptor_size, CV_32FC1);
    cv::Mat test_mat(test_HOG.size(), descriptor_size, CV_32FC1);

    trn.ConvertVectortoMatrix(train_HOG, test_HOG, train_mat, test_mat);

    float C = 12.5, gamma = 0.5;

    cv::Mat test_response;
    trn.svmInit(C, gamma);

    /* SVM Training */
    trn.svmTrain(train_mat, train_labels);

    /* SVM Testing */
    trn.svmPredict(test_response, test_mat);

    /* Evaluate SVM Performance */
    float count = 0;
    float accuracy = 0;
    trn.DisplaySVMParams();

    trn.SVMevaluate(test_response, count, accuracy, test_labels);

	return 0;
}