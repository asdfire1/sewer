
#include <iostream> //Includes
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv; //Namespaces
using namespace std;


void main()
{
	Mat img = imread("C:\\AAU Onedrive\\OneDrive - Aalborg Universitet\\P4 Group Folder\\screenshots\\displacement\\video 2 (ayr 0901)\\3(FSF2).png", IMREAD_GRAYSCALE); //input image

	Mat medianImg = Mat(img.size(), CV_8U);
	Mat rangeImg = Mat(img.size(), CV_8U); //Image for thresholding (same size as input)
	Mat morphImg = Mat(img.size(), CV_8U); //Image for 1st morphology (same size as input)
	Mat morphImg2 = Mat(img.size(), CV_8U); //Image for 2nd morphology (same size as input)
	Mat morphImg3 = Mat(img.size(), CV_8U);
	Mat morphImg4 = Mat(img.size(), CV_8U);
	Mat binImg = Mat(img.size(), CV_8U);
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat grad;


	medianBlur(img, medianImg, 15);

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Sobel(medianImg, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(medianImg, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	grad = grad * 2;


	threshold(grad, binImg, 50, 255, THRESH_BINARY);
	Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(3, 10));
	morphologyEx(binImg, morphImg, MORPH_CLOSE, elem); //Closes holes in objects

	elem = getStructuringElement(MORPH_ELLIPSE, Size(10, 3));
	morphologyEx(morphImg, morphImg3, MORPH_CLOSE, elem);
	elem = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));
	morphologyEx(morphImg3, morphImg4, MORPH_CLOSE, elem);
	//elem = getStructuringElement(MORPH_ELLIPSE, Size(25, 25)); //Optional different element size for opening
	morphologyEx(morphImg, morphImg2, MORPH_OPEN, elem); //Removes noise
	imshow("Input Image", img);
	imshow("Median", medianImg);
	imshow("window_name", grad);
	imshow("Binary edge", binImg);
	//imshow("Range", rangeImg);
	imshow("Morph", morphImg);
	imshow("Morph2", morphImg2);
	imshow("Morph3", morphImg3);
	imshow("Morph4", morphImg4);
	
	waitKey(0); //Needed in order for the image to not close instantly
}