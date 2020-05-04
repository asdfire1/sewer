#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

struct features //crate data structure for the features
{
	int index;
	int area;
};


void main()
{
	// Load colour image and create empty images for output:	

	String ImgPath = "C:\\Users\\Zoltán\\Desktop\\nothing.png"; 

	Mat OG = imread(ImgPath);
	Mat img = imread(ImgPath, IMREAD_GRAYSCALE); //Load the image in grayscale
	Mat bin = Mat(img.size(), CV_8U);
	Mat morph1 = Mat(img.size(), CV_8U);
	Mat morph2 = Mat(img.size(), CV_8U);
	Mat contourImg = Mat(img.size(), CV_8U);
	Mat Blur = Mat(img.size(), CV_8U);

	VideoCapture cap("C:\\Users\\Zoltán\\Desktop\\pipe.mp4");

	//Uncomment the following line if you want to start the video in the middle
	//cap.set(CAP_PROP_POS_MSEC, 300); 

	//get the frames rate of the video
	double fps = cap.get(CAP_PROP_FPS);
	cout << "Frames per seconds : " << fps << endl;

	String window_name = "Pipe video";

	namedWindow(window_name, WINDOW_NORMAL); //create a window

	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
	int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

	Size frame_size(frame_width, frame_height);
	int frames_per_second = 10;

	//Create and initialize the VideoWriter object 
	VideoWriter oVideoWriter("C:\\Users\\Zoltán\\Desktop\\pipe.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true);


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
		inRange(frame, Scalar(170, 170, 170), Scalar(255, 255, 255), bin);

		Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
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
			if (hier[i][3] == -1 && contourArea(contours[i]) > 6500) {
				Point2f center;
				float radius;
				minEnclosingCircle(contours[i], center, radius);
				Scalar color = Scalar(0, 0, 255);
				cout << radius;
				if (radius > 220) {
					circle(frame, center, radius, color, 2);
				}
			}
		}


		//Loop through contours saved in vector. Sort based on feature values manually defined.

		

		oVideoWriter.write(frame);

		//show the frame in the created window
		imshow(window_name, frame);


		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
		
	}

	oVideoWriter.release();

	/// Draw the circles detected
	



	
	// Show results:

	//imshow("OG", OG);
	//imshow("Binary", bin);
	//imshow("Morph1", morph1);
	//imshow("Morph2", morph2);
	//imshow("Contours", contourImg);
	//imshow("Objects result", objects);
	//imshow("out", Blur);
	//Save result as a new image file

	/*String newPath = ImgPath;
	newPath.resize(newPath.size() - 4);
	newPath = newPath + "_new.jpg";
	cout << newPath << '\n';
	imwrite(newPath, objects);
	*/
	waitKey(0);
}