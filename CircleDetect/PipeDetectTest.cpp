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
	//Mat contourImg = Mat(frame_size, CV_8U);
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




	while (true)
	{

		Mat contourImg = Mat(frame_size, CV_8UC1, Scalar(0));
		Mat lines = Mat(frame_size, CV_8UC1, Scalar(0));
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
		inRange(frame, Scalar(135, 135, 135), Scalar(255, 255, 255), bin);

		Mat elem3 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
		morphologyEx(bin, bin, MORPH_OPEN, elem3);
		//Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		//morphologyEx(bin, bin, MORPH_DILATE, elem);
		Mat elem2 = getStructuringElement(MORPH_ELLIPSE, Size(13, 13));
		morphologyEx(bin, bin, MORPH_CLOSE, elem2);
		


		//Find objects:

		
		vector<vector<Point>> contours; // a vector of vectors holding points used to save the contours
		//vector<Vec4i> hier; // a vector of vectors holding 4 intigers used to save hierarchy data

		findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		

		//Loop through all external contours (hierarchy = -1)
		
		float maxLenght = 0;
		float longBoii = 0;

		for (int i = 0; i < contours.size(); i++) {  

			if (contourArea(contours[i]) > 4000) {

				drawContours(contourImg, contours, i, Scalar(255), FILLED, 8);
				minEnclosingCircle(contours[i], center, radius);
				float diffx = abs(frame_width / 2 - center.x);
				float diffy = abs(frame_height / 2 - center.y);

				if (radius > 180 && diffx < 50 && diffy < 70) {

					for (int yRate = -2; yRate <= 0; yRate++) {
						cout << "yrate" << yRate << endl;
						for (int xRate = -2; xRate <= 2; xRate++) {
							cout << "xrate" << xRate << endl;
							Point Pixel = Point(center.x, center.y);
								
							Point firstPixel = Point(0, 0);
							Point lastPixel = Point(0, 0);
							
							if (xRate != 0 || yRate != 0) {

								while (Pixel.x < frame_width && Pixel.x > 0 && Pixel.y < frame_height && Pixel.y > 0) {

									int value = contourImg.at<uchar>(Pixel.y, Pixel.x);



									for (int x = abs(xRate); x > 0; x--) {
										
										if (value == 255) {
											if (firstPixel.x == 0 && firstPixel.y == 0) {
												firstPixel = Pixel;
											}
											lastPixel = Pixel;
											lines.at<uchar>(Point(Pixel.x, Pixel.y)) = 255;
										}
										if (xRate > 0) {
											Pixel.x++;
										}
										else {
											Pixel.x--;
										}
										
									}
									for (int y = abs(yRate); y > 0; y--) {
										
										if (value == 255) {
											if (firstPixel.x == 0 && firstPixel.y == 0) {
												firstPixel = Pixel;
											}
											lastPixel = Pixel;
											lines.at<uchar>(Point(Pixel.x, Pixel.y)) = 255;
										}

										if (yRate > 0) {
											Pixel.y++;
										}
										else {
											Pixel.y--;
										}
									}
									
									//imshow("contour", contourImg);
								}
								//cout << Pixel.x << "   " << Pixel.y << "   " << value << endl;


							float Lenght = norm(firstPixel - lastPixel);
								if (Lenght > maxLenght) {
									maxLenght = Lenght;
								}

							longBoii = maxLenght / radius;

							cout << maxLenght << endl;
							
							}
						}
						
						

					}
					
					cout << "max lenghthth. The long boii: " << longBoii << endl;

					if (gs.size() == 0) {
						cout << "new object" << endl;
						float seconds = frames / frames_per_second;
						outputfile << seconds << ", ";
					}
					gs.push_back(longBoii);
					cout << gs.size();

					//circle(frame, center, radius, color, 2);
					//circle(frame, center, 1, color, 5);

					//circle(frame, Point(frame_width / 2, frame_height / 2), 1, Scalar(255, 0, 0), 5);

					//rectangle(frame, rect, Scalar(0, 255, 0), 1);
					//circle(frame, (rect.tl() + rect.br()) / 2, 1, Scalar(0, 255, 0), 5);
					
					
				}
								
				countdown = 10;
			}
		}

		if (countdown == 0 && gs.size() > 0) {
			cout << "max: " << max(gs) << endl;
			if (max(gs) > 0.1) {
				objectClass = "fs2";
			}
			else if (max(gs) > 0.01) {
				objectClass = "fs1";
			}
			else {
				objectClass = "0";
			}
			outputfile << max(gs) << ", " << objectClass << endl;
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

		imshow("contour", contourImg);
		imshow("lines", lines);


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