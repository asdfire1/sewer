#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;



void main()
{
	// Load colour image and create empty images for output:	

	String ImgPath = "C:\\Users\\Zoltán\\Desktop\\bin2.png"; 

	Mat OG = imread(ImgPath);
	Mat img = imread(ImgPath, IMREAD_GRAYSCALE); //Load the image in grayscale
	Mat bin = Mat(img.size(), CV_8U);
	Mat morph1 = Mat(img.size(), CV_8U);
	Mat morph2 = Mat(img.size(), CV_8U);
	Mat contourImg = Mat(img.size(), CV_8U);
	Mat Blur = Mat(img.size(), CV_8U);


	//Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
	//morphologyEx(img, morph1, MORPH_CLOSE, elem); //Closes holes in objects

	medianBlur(img, Blur, 5);
	//imshow("Blur", Blur);
	
	//Find objects:

	vector<Vec3f> circles;

	Vec3i match(0,0,-1);

	/// Apply the Hough Transform to find the circles
	int Filter = 100;
	do {
		HoughCircles(Blur, circles, HOUGH_GRADIENT, 1, 10, 50, Filter, 120, 350); //(140 for clay12)
		Filter--;
		cout << Filter;

		if (circles.size() >= 2) {
			for (size_t i = 0; i < circles.size()-1; i++) {

				int radius1 = cvRound(circles[i][2]);

				for (size_t j = i+1; j < circles.size(); j++) {

					//Point center1(cvRound(circles[i][0]), cvRound(circles[i][1]));

					//Point center2(cvRound(circles[j][0]), cvRound(circles[j][1]));
					int radius2 = cvRound(circles[j][2]);
					int diff = abs(radius1 - radius2);
					cout << diff;
					if (match[2] > diff || match[2] == -1) {
						match[0] = i;
						match[1] = j;
						match[2] = diff;
					}
					cout << match;
				}
			}
		}

	} while (circles.size() < 2 || match[2] > 15);
	

	/// Draw the circles detected
	
	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(OG, center, radius, Scalar(0, 255, 0), 2, 8, 0);
	}
	
	int a = match[0];
	int b = match[1];

	Point center(cvRound(circles[a][0]), cvRound(circles[a][1]));
	int radius = cvRound(circles[a][2]);
	circle(OG, center, radius, Scalar(0, 0, 255), 2, 8, 0);
	
	Point center2(cvRound(circles[b][0]), cvRound(circles[b][1]));
	int radius2 = cvRound(circles[b][2]);
	circle(OG, center2, radius2, Scalar(0, 0, 255), 2, 8, 0);
	
	
	// Show results:

	imshow("OG", OG);
	//imshow("Binary", bin);
	//imshow("Morph1", morph1);
	//imshow("Morph2", morph2);
	//imshow("Contours", contourImg);
	//imshow("Objects result", objects);
	imshow("out", Blur);
	//Save result as a new image file

	/*String newPath = ImgPath;
	newPath.resize(newPath.size() - 4);
	newPath = newPath + "_new.jpg";
	cout << newPath << '\n';
	imwrite(newPath, objects);
	*/
	waitKey(0);
}