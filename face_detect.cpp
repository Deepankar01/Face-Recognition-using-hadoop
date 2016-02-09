/*
 * 1.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: deep
 */
#include<opencv2/core/core.hpp>
#include<opencv2/contrib/contrib.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
#include<math.h>
#include<vector>
using namespace std;
using namespace cv;

/*steps left are:
 * detect the eyes
 * apply equalized histogram over the whole image
 * apply the smoothing effect
 * apply the ellipse of the exact size
 *allign all the face wrt to eyes so that face remains starit while comparing
 else eyes will be compared to nose
 * these steps are done to make the image of the same size and
 of same dimension
 * since recognizer needs the exact image
 */

//function to detect the eyes
vector<Rect> detect_eyes(Mat img, CascadeClassifier eyesDetector) {
	int flags = CASCADE_FIND_BIGGEST_OBJECT; // Search for many faces.
	// for searching the single face use CASCADE_FIND_BIGGEST_OBJECT
	Size minFeatureSize(10, 10); // Smallest face size if the image is close enough use 80,80.
	float searchScaleFactor = 1.1f; // How many sizes to search for best we use 1.1 for good use 1.2
	int minNeighbors = 7; // Reliability vs many faces A realiability Number greater the number more reliable but slower.
	// Detect faces in the small grayscale image.
	std::vector<Rect> eyes;
	eyesDetector.detectMultiScale(img, eyes, searchScaleFactor, minNeighbors,flags, minFeatureSize);
	cout << "eyes=" << eyes.size() << endl;

	for (int i = 0; i < (int) eyes.size(); i++) {
		if (eyes[i].x < 0)
			eyes[i].x = 0;
		if (eyes[i].y < 0)
			eyes[i].y = 0;
		if (eyes[i].x + eyes[i].width > img.cols)
			eyes[i].x = img.cols - eyes[i].width;
		if (eyes[i].y + eyes[i].height > img.rows)
			eyes[i].y = img.rows - eyes[i].height;
	}
	return eyes;

}

//function to draw the face
Mat drawrect(Mat img, vector<Rect> faces) {
	IplImage image;
	image = img;

	for (int i = 0; i < faces.size(); i++) {
		CvPoint pt1 = { pt1.x = faces[i].x, pt1.y = faces[i].y };
		CvPoint pt2 = { pt2.x = faces[i].x + faces[i].width, pt2.y = faces[i].y
				+ faces[i].height };
		cvRectangle(&image, pt1, pt2, CV_RGB(8, 108, 232), 8, 7, 0);
	}
	return &image;
}

//function to convert the image to grayscale
Mat img2GraySale(Mat img) {
	//converting image to grayscale
	Mat gray;
	if (img.channels() == 3) {
		cvtColor(img, gray, CV_BGR2GRAY);
	} else if (img.channels() == 4) {
		cvtColor(img, gray, CV_BGRA2GRAY);
	} else {
		// Access the grayscale input image directly.
		gray = img;
	}

	return gray;
}

//function to display the image
void disp_img(Mat img) {
	namedWindow("Display window", WINDOW_NORMAL);// Create a window for display.
	imshow("Display window", img);
}

//function to improve the dark images
Mat imgequalhist(Mat img) {
	// Standardize the brightness & contrast, such as
	// to improve dark images.
	Mat equalizedImg;
	equalizeHist(img, equalizedImg);
	return equalizedImg;
	//return img;
}

//function to call the remaining function
Mat img_color_histo(Mat source_img) {
	return (imgequalhist(img2GraySale(source_img)));
}

//function to detect the faces from the source grayscale image
vector<Rect> detect_faces(Mat img, CascadeClassifier faceDetector) {
	int flags = CASCADE_FIND_BIGGEST_OBJECT; // Search for many faces.
	// for searching the single face use CASCADE_FIND_BIGGEST_OBJECT
	Size minFeatureSize(30, 30); // Smallest face size if the image is close enough use 80,80.
	float searchScaleFactor = 1.1f; // How many sizes to search for best we use 1.1 for good use 1.2
	int minNeighbors = 5; // Reliability vs many faces A realiability Number greater the number more reliable but slower.
	// Detect faces in the small grayscale image.
	std::vector<Rect> faces;
	faceDetector.detectMultiScale(img, faces, searchScaleFactor, minNeighbors,flags, minFeatureSize);
	cout << "Faces=" << faces.size() << endl;


	for (int i = 0; i < (int) faces.size(); i++) {
		if (faces[i].x < 0)
			faces[i].x = 0;
		if (faces[i].y < 0)
			faces[i].y = 0;
		if (faces[i].x + faces[i].width > img.cols)
			faces[i].x = img.cols - faces[i].width;
		if (faces[i].y + faces[i].height > img.rows)
			faces[i].y = img.rows - faces[i].height;
	}
	return faces;
}

//read the image from directory
Mat read_image(char *location) {
	Mat source_image;
	source_image = imread(location, CV_LOAD_IMAGE_COLOR);
	return source_image;
}

//function to crop the face from the real image
Mat Crop_face(Mat img, Rect face) {
	//fetching a single face and then croping it for faster Processing

	IplImage crop;
	crop = img;

	CvPoint pt1 = { pt1.x = face.x, pt1.y = face.y };
	CvPoint pt2 = { pt2.x = face.x + face.width, pt2.y = face.y + face.height };
	cvSetImageROI(&crop, Rect(pt1, pt2));

	return &crop;
}

Mat geometricTransformface(Rect lefteye, Rect righteye, Mat gray) {
	// Get the center between the 2 eyes.
	Point2f eyesCenter;
	eyesCenter.x = (lefteye.x + righteye.x) * 0.5f;
	eyesCenter.y = (lefteye.y + righteye.y) * 0.5f;
	// Get the angle between the 2 eyes.
	double dy = (righteye.y - lefteye.y);
	double dx = (righteye.x - lefteye.x);
	double len = sqrt(dx * dx + dy * dy);
	// Convert Radians to Degrees.
	double angle = atan2(dy, dx) * 180.0 / CV_PI;
	cout<<angle<<endl;
	
	// Hand measurements shown that the left eye center should
	// ideally be roughly at (0.16, 0.14) of a scaled face image.
	const double DESIRED_LEFT_EYE_Y = 0.14;
	const double DESIRED_LEFT_EYE_X = 0.16;
	const double DESIRED_RIGHT_EYE_X = (1.0f - 0.16);

	// Get the amount we need to scale the image to be the desired
	// fixed size we want.
	const int DESIRED_FACE_WIDTH = 300;
	const int DESIRED_FACE_HEIGHT = 300;
	//double desiredLen = (DESIRED_RIGHT_EYE_X - 0.16);
	double desiredLen = 0.38;
	double scale = desiredLen * DESIRED_FACE_WIDTH / len;
	cout<<"scaled by"<<scale<<endl;

	// Get the transformation matrix for the desired angle & size.
	Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);
	// Shift the center of the eyes to be the desired center.
	double ex = DESIRED_FACE_WIDTH * 0.5f - eyesCenter.x;
	double ey = DESIRED_FACE_HEIGHT * DESIRED_LEFT_EYE_Y - eyesCenter.y;
	rot_mat.at<double>(0, 2) += ex;
	rot_mat.at<double>(1, 2) += ey;
	// Transform the face image to the desired angle & size &
	// position! Also clear the transformed image background to a
	// default grey.
	Mat warped = Mat(DESIRED_FACE_HEIGHT, DESIRED_FACE_WIDTH, CV_8U,Scalar(128));
	warpAffine(gray, warped, rot_mat, warped.size());
	cout<<"warped by"<<rot_mat.size();
	return warped;

}

//function to write to disk
void write_disk(Mat image, char filename[50]) {
	imwrite(filename, image);
}

//function to split the image in half
vector<Mat> splitimage(Mat image) {
	vector<Mat> img_splits;
	Mat temp;
	img_splits.push_back(image);
	img_splits.push_back(image);

	IplImage left_split, right_split;
	left_split = img_splits[0];
	right_split = img_splits[0];

	CvPoint rightpt1 = { rightpt1.x = 0, rightpt1.y = 0 };
	CvPoint rightpt2 = { rightpt2.x = image.cols / 2, rightpt2.y = image.rows };
	cvSetImageROI(&right_split, Rect(rightpt1, rightpt2));

	CvPoint leftpt1 = { leftpt1.x = image.cols / 2, leftpt1.y = 0 };
	CvPoint leftpt2 = { leftpt2.x = image.cols, leftpt2.y = image.rows };
	cvSetImageROI(&left_split, Rect(leftpt1, leftpt2));

	img_splits.clear();

	temp = &right_split;
	img_splits.push_back(temp);

	temp = &left_split;
	img_splits.push_back(temp);

	return img_splits;
}
//start of main
int main(int argc, char **argv) {

	//when not using eclipse use the image name as the first argument and
	//and the number of arguments have to be changed to 1
	string FaceCascadeFilename =
			"data/haarcascades/haarcascade_frontalface_default.xml";
	string LeftEyeCascadeFilename =
			"data/haarcascades/haarcascade_mcs_lefteye.xml";
	string RightEyeCascadeFilename =
			"data/haarcascades/haarcascade_mcs_righteye.xml";
	CascadeClassifier faceDetector;
	CascadeClassifier lefteyeDetector, righteyeDetector;
	Mat source_img, orig, detected_face,final;
	vector<Rect> faces, lefteye, righteye;
	vector<Mat> splits;

	//char* arg = argv[0];
	//check for the arguments
	if (argc != 2) {
		wcerr
				<< "Invalid Arguments Usage is: facedetect imagename --help for detecting the faces in the image";
		exit(1);
	}

	try {
		//check for the facedetector file_name
		faceDetector.load(FaceCascadeFilename);
		lefteyeDetector.load(LeftEyeCascadeFilename);
		righteyeDetector.load(RightEyeCascadeFilename);
	} catch (Exception e) {
	}
	if (faceDetector.empty()) {
		wcerr << "Could Not Load Face Detector file !!";
		exit(1);
	}
	if (lefteyeDetector.empty()) {
		wcerr << "Could Not Load Left Eye Detector file !!";
		exit(1);
	}
	if (righteyeDetector.empty()) {
		wcerr << "Could Not Load right Eye Detector file !!";
		exit(1);
	}

	//read the source image
	/*source_img = read_image(((char*)argv[1]));

	 if(! source_img.data)
	 {
	 wcerr << "Could not load the source image ";
	 exit(1);
	 }

	 //convert the image into the appropriate format
	 source_img=img_color_histo(source_img);
	 detect_faces(source_img,faceDetector);*/
	        int j=1;
		char location[50];
		source_img = read_image(((char*)argv[1]));
		orig = source_img;

		/*namedWindow("l",WINDOW_NORMAL);
		imshow("l",source_img);
		waitKey(0);*/

		if (!source_img.data) {
			wcerr << "Could not load the source image ";
			exit(1);
			cout << "here";
		}

		//convert the image into the appropriate format
		source_img = img_color_histo(source_img);
		cout << location << endl;
		faces = detect_faces(source_img, faceDetector);

		if(faces.size()==1)
		{
		detected_face = Crop_face(orig, faces[0]);

		/*imshow("LOla",drawrect(detected_face,faces));
		waitKey(0); */

		splits = splitimage(detected_face);

		//splits[0]=img_color_histo(splits[0]);
		//splits[1]=img_color_histo(splits[1]);

		lefteye = detect_eyes(splits[0], lefteyeDetector);
		righteye = detect_eyes(splits[1], righteyeDetector);
/*
		imshow("left image", drawrect(splits[0], lefteye));
		imshow("right image", drawrect(splits[1], righteye));
		waitKey(0);*/

		if (lefteye.size() == 1 && righteye.size() == 1)
		{
			righteye[0].x = righteye[0].x + splits[0].cols;
			//that means we got the face
			//trim the face to have eyes alligned
			final=geometricTransformface(lefteye[0], righteye[0],img2GraySale(detected_face));
			//sprintf(location, "%d.jpg", j);
			write_disk(final,((char*)argv[1]));
			j++;
		}
	}


	
	cout<<"Total images Written"<<j;
//		disp_img(source_img);

	return 0;
}
//end of main

