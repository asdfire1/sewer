#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

struct features
{
	int index;
	int area;
	int perimeter;
	float circularity;
	bool hasHole;
	float elongation;
};



void main()
{
	// Load colour image and create empty images for output:	

	String ImgPath = "C:\\Users\\Zoltán\\Desktop\\12(FSA2).png"; 

	Mat OG = imread(ImgPath);
	Mat img = imread(ImgPath, IMREAD_GRAYSCALE); //Load the image in grayscale
	Mat bin = Mat(img.size(), CV_8U);
	Mat morph1 = Mat(img.size(), CV_8U);
	Mat morph2 = Mat(img.size(), CV_8U);
	Mat contourImg = Mat(img.size(), CV_8U);
	Mat Blur = Mat(img.size(), CV_8U);

	GaussianBlur(img, Blur, Size(7, 7), 2, 2);

	//imshow("Blur", Blur);

	/*Mat kernel_X = getGaussianKernel(img.cols, 300);
	Mat kernel_Y = getGaussianKernel(img.rows, 300);
	Mat kernel_X_transpose;
	transpose(kernel_X, kernel_X_transpose);
	Mat kernel = kernel_Y * kernel_X_transpose;

	Mat mask, processed_image;
	normalize(kernel, mask, 0, 1, NORM_MINMAX);
	Blur.convertTo(processed_image, CV_64F);
	multiply(mask, processed_image, processed_image);
	convertScaleAbs(processed_image, processed_image);

	imshow("Vignette", processed_image);
	*/
	
	//Find objects:

	vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	int Filter = 80;
	while (circles.size() < 2) {
		HoughCircles(Blur, circles, HOUGH_GRADIENT, 1, 10, 140, Filter, 130, 350); //(140 for clay12)
		Filter--;
		cout << Filter;
	}
	

	/// Draw the circles detected
		
	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(OG, center, radius, Scalar(0, 0, 255), 2, 8, 0);
	}


	// Show results:

	//imshow("Input greyscale", img);
	//imshow("Binary", bin);
	//imshow("Morph1", morph1);
	//imshow("Morph2", morph2);
	//imshow("Contours", contourImg);
	//imshow("Objects result", objects);
	imshow("out", OG);
	//Save result as a new image file

	/*String newPath = ImgPath;
	newPath.resize(newPath.size() - 4);
	newPath = newPath + "_new.jpg";
	cout << newPath << '\n';
	imwrite(newPath, objects);
	*/
	waitKey(0);
}