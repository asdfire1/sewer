#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;


double median(vector<float> scores)
{
	size_t size = scores.size();

	if (size == 0)
	{
		return -1;  // Undefined, really.
	}
	else
	{
		sort(scores.begin(), scores.end());
		if (size % 2 == 0)
		{
			return (scores[size / 2 - 1] + scores[size / 2]) / 2;
		}
		else
		{
			return scores[size / 2];
		}
	}
}

float max(vector<float> inputvector) {
	float a = 0;
	for (int i = 0; i < inputvector.size(); i++) {
		if (inputvector[i] > a)
			a = inputvector[i];
	}
	return a;
}

ofstream outputfile;



void main()
{
	
	outputfile.open("C:\\Users\\nxtzo\\Desktop\\Pipes\\data.txt", ios::app);
	
	outputfile << "Seconds, GValue, Class" << endl;

	VideoCapture cap("C:\\Users\\nxtzo\\Desktop\\Pipes\\pipe01sc.mp4");

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
	int frames_per_second = 25;

	Mat bin = Mat(frame_size, CV_8U);
	Mat morph1 = Mat(frame_size, CV_8U);
	Mat morph2 = Mat(frame_size, CV_8U);
	Mat contourImg = Mat(frame_size, CV_8U);
	Mat Blur = Mat(frame_size, CV_8U);


	//Create and initialize the VideoWriter object 
	//VideoWriter oVideoWriter("C:\\Users\\nxtzo\\Desktop\\Pipes\\pipes.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true);


	Scalar color = Scalar(255, 100, 0);
	Point2f center;
	float radius;
	int countdown = 0;
	vector<float> gs;
	string objectClass = "";
	double frames = 0;
	float gValue = 0;

	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video 
		frames++;
		//Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}

		//medianBlur(frame, Blur, 5);
		inRange(frame, Scalar(130, 130, 130), Scalar(255, 255, 255), bin);

		Mat elem3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		morphologyEx(bin, bin, MORPH_OPEN, elem3);
		//Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		//morphologyEx(bin, bin, MORPH_DILATE, elem);
		Mat elem2 = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
		morphologyEx(bin, bin, MORPH_CLOSE, elem2);
		


		//Find objects:

		
		vector<vector<Point>> contours; // a vector of vectors holding points used to save the contours
		vector<Vec4i> hier; // a vector of vectors holding 4 intigers used to save hierarchy data

		findContours(bin, contours, hier, RETR_CCOMP, CHAIN_APPROX_NONE);
		

		//Loop through all external contours (hierarchy = -1)
		
		
		for (int i = 0; i < contours.size(); i++) {  

			if (hier[i][3] == -1 && contourArea(contours[i]) > 4000) {

				minEnclosingCircle(contours[i], center, radius);
				float diffx = abs(frame_width / 2 - center.x);
				float diffy = abs(frame_height / 2 - center.y);

				if (radius > 175 && diffx < 50 && diffy < 70) {

					//Rect rect = boundingRect(contours[i]);
					gValue = contourArea(contours[i]) / (radius * radius);

					//string gvalue = to_string(gValue);
					//string StrArea = to_string(contourArea(contours[i]));
					//string StrRadius = to_string(radius);
					//string StrAreaRect = to_string(rect.area());

					//putText(frame, StrArea, Point(20, 150), FONT_HERSHEY_PLAIN, 1, color, 2);
					//putText(frame, StrRadius, Point(20, 200), FONT_HERSHEY_PLAIN, 1, color, 2);
					//putText(frame, StrAreaRect, Point(20, 250), FONT_HERSHEY_PLAIN, 1, color, 2);
					//putText(frame, gvalue, Point(20, 300), FONT_HERSHEY_PLAIN, 1, color, 2);

					//circle(frame, center, radius, color, 2);
					//circle(frame, center, 1, color, 5);

					//circle(frame, Point(frame_width / 2, frame_height / 2), 1, Scalar(255, 0, 0), 5);

					//rectangle(frame, rect, Scalar(0, 255, 0), 1);
					//circle(frame, (rect.tl() + rect.br()) / 2, 1, Scalar(0, 255, 0), 5);
					if (gs.size() == 0) {
						cout << "new object" << endl;
						float seconds = frames / frames_per_second;
						outputfile << seconds << ", " ;
					}
					gs.push_back (gValue);
					cout << gs.size();
					
				}
								
				countdown = 5;
			}
		}

		if (countdown == 0 && gs.size() > 0) {
			cout << "max: " << max(gs) << endl;
			if (max(gs) > 0.22) {
				objectClass = "fs2";
			}
			else if (max(gs) > 0.06){
				objectClass = "fs1";
			}
			else {
				objectClass = "0";
			}
			outputfile << max(gs) << ", " << objectClass <<  endl;
			gs.clear();
		}

		if (countdown < -7) {
			objectClass = "";
		}
		
		countdown--;
		//putText(frame, objectClass, Point(20, 250), FONT_HERSHEY_PLAIN, 2, color, 2);

		//cvtColor(bin, bin, COLOR_GRAY2RGB);
		//oVideoWriter.write(bin);

		//show the frame in the created window
		imshow(window_name, frame);

		imshow("bin", bin);


		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}

	}

	//oVideoWriter.release();

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