/*
 * recognize.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: deep
 */
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
	vector<Mat> input1;
	vector<Mat> input2;
	vector<Mat> input3;
	
	vector<int> labels;

	Mat testSample;
	char location[50];

	//testSample =read_image("src/predict/27.jpg");
	//cvtColor(testSample,testSample,COLOR_BGR2GRAY);

	for(int i=1;i<101;i++)
		{

			
			labels.push_back(i);
			sprintf(location,"1/%d.jpg",i);
			input1.push_back(read_image(location));
			
			sprintf(location,"2/%d.jpg",i);
			input2.push_back(read_image(location));

			sprintf(location,"3/%d.jpg",i);
			input3.push_back(read_image(location));
			
			//cvtColor(input[0],input[0],COLOR_BGR2GRAY);
		}

    Ptr<FaceRecognizer> model1 = createEigenFaceRecognizer(100);
    Ptr<FaceRecognizer> model2 = createEigenFaceRecognizer(100);
    Ptr<FaceRecognizer> model3 = createEigenFaceRecognizer(100);

    model1->train(input1, labels);
    model2->train(input2, labels);
    model3->train(input3, labels);
    //model->save("trained_set.yml");

    int predictedLabel = -1;
    double confidence[3];

    //sprintf(location,"src/faces/%d.jpg",);
    testSample =read_image(((char*)argv[1]));
    
    
    model1->save("1.yml");
    model2->save("2.yml");
    model3->save("3.yml");

	
  
    


	return 0;
}



