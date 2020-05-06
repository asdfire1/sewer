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

<<<<<<< Updated upstream
	String ImgPath = "C:\\Users\\Zoltán\\Desktop\\12(FSA2).png"; 
=======
	String ImgPath = "C:\\AAU Onedrive\\OneDrive - Aalborg Universitet\\P4 Group Folder\\screenshots\\displacement\\video 1 (ayr0603)\\1(fsf4).jpg"; 
>>>>>>> Stashed changes

	Mat OG = imread(ImgPath);
	Mat img = imread(ImgPath, IMREAD_GRAYSCALE); //Load the image in grayscale
	Mat bin = Mat(img.size(), CV_8U);
	Mat morph1 = Mat(img.size(), CV_8U);
	Mat morph2 = Mat(img.size(), CV_8U);
	Mat contourImg = Mat(img.size(), CV_8U);
	Mat Blur = Mat(img.size(), CV_8U);

<<<<<<< Updated upstream
	GaussianBlur(img, Blur, Size(7, 7), 2, 2);
=======
	VideoCapture cap("C:\\AAU Onedrive\\OneDrive - Aalborg Universitet\\P4 Group Folder\\screenshots\\pipes.mp4");
>>>>>>> Stashed changes

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

<<<<<<< Updated upstream
	imshow("Vignette", processed_image);
	*/
	
	//Find objects:
=======
	namedWindow(window_name, WINDOW_NORMAL); //create a window

	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
	int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

	Size frame_size(frame_width, frame_height);
	int frames_per_second = 10;

	//Create and initialize the VideoWriter object 
	VideoWriter oVideoWriter("C:\\AAU Onedrive\\OneDrive - Aalborg Universitet\\P4 Group Folder\\screenshots\\opcvvideo\\pipes.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true);


	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video 

		//Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}




		//medianBlur(frame, Blur, 3);
		inRange(frame, Scalar(150, 150, 150), Scalar(255, 255, 255), bin);

		Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
		morphologyEx(bin, morph1, MORPH_CLOSE, elem); //Closes holes in objects
				
		//medianBlur(img, Blur, 5);
		//imshow("Blur", Blur);

		//Find objects:

		vector<vector<Point>> contours; // a vector of vectors holding points used to save the contours
		vector<Vec4i> hier; // a vector of vectors holding 4 intigers used to save hierarchy data

		findContours(morph1, contours, hier, RETR_CCOMP, CHAIN_APPROX_NONE);
		//drawContours(contourImg, contours, -1, Scalar(0, 0, 255), 3, 16, hier, 1); //draw the contours on a seperate image


		vector<features> featVec; // vector to hold every feature of every object

		//Loop through all external contours (hierarchy = -1)
		//Save contour index and features in vector featVec

		for (int i = 0; i < contours.size(); i++) {		//loop through the objects and save all features
			if (hier[i][3] == -1 && contourArea(contours[i]) > 6800) {
				
				minEnclosingCircle(contours[i], center, radius);
				float diff = abs(frame_width / 2 - center.x);
				if (radius > 220 && diff<30) {
					string gvalue = to_string(contourArea(contours[i]) /( radius * radius));
					Scalar color = Scalar(255, 128, 187);
					string StrArea = to_string(contourArea(contours[i]));
					string StrRadius = to_string(radius);
					Rect rect = boundingRect(contours[i]);
					string StrAreaRect = to_string(rect.area());
					putText(frame, StrArea, Point(20,150), FONT_HERSHEY_PLAIN, 1, color, 2);
					putText(frame, StrRadius, Point(20, 200), FONT_HERSHEY_PLAIN, 1, color, 2);
					putText(frame, StrAreaRect, Point(20, 250), FONT_HERSHEY_PLAIN, 1, color, 2);
					putText(frame, gvalue, Point(20, 300), FONT_HERSHEY_PLAIN, 1, color, 2);
					circle(frame, center, radius, color, 2);
					circle(frame, center, 1, color, 5);
					circle(frame, Point(frame_width / 2, frame_height / 2), 1, Scalar(255, 0, 0), 5);
					
					rectangle(frame, rect, Scalar(0, 255, 0), 1);
					circle(frame, (rect.tl() + rect.br())/2, 1, Scalar(0, 255, 0), 5);
				}
			}
		}

		

		oVideoWriter.write(frame);

		//show the frame in the created window
		imshow(window_name, frame);

		imshow("Morph1", morph1);
>>>>>>> Stashed changes

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