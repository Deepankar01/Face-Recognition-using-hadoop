/*
 * recognize.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: deep
 */
#include<cv.h>
#include<highgui.h>
#include<opencv2/core/core.hpp>
#include<opencv2/contrib/contrib.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
#include<vector>
#include<math.h>
using namespace std;
using namespace cv;

//read the image from directory
Mat read_image(char *location)
{
	Mat source_image;
	source_image=imread(location, CV_LOAD_IMAGE_GRAYSCALE);
	if(!source_image.data)
	{
		cout<<location<<" image not found";
		exit(1);
	}
	return source_image;
}

//the main function
int main(int argc, char **argv)
{

	Mat testSample;
	char location[50];

	//testSample =read_image("src/predict/27.jpg");
	//cvtColor(testSample,testSample,COLOR_BGR2GRAY);

    Ptr<FaceRecognizer> model = createEigenFaceRecognizer(100);
    model->load(((char*)argv[2]));

    int predictedLabel = -1;
    double confidence;

    //sprintf(location,"src/faces/%d.jpg",);
    testSample =read_image(((char*)argv[1]));
    model->predict(testSample,predictedLabel,confidence);
    
    
    cout<<"Eigen Value for folder 1 "<<confidence<<endl;
   
    //cout<<(confidence[0]<confidence[1]?confidence[0]<confidence[2]?"1":"3":confidence[2]<confidence[1]?"3":"2");
    /*cout<<"image:"<<predictedLabel<<endl;*/
    /*double probability = 1.0f - sqrt( (confidence*confidence) / (float)(input.size() * 100) ) / 255.0f;
    cout<<"probability:"<<probability<<endl;*/


	return 0;
}



