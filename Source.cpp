#include"functions.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

int main(int, char**) {

	bool tryb = true;

	VideoCapture cap, capBackground;

	capBackground.open("Ziolkowski_Marcin/background.mts");
	if (!capBackground.isOpened()) {
		cout << "movie opening error";
		waitKey(0);
		return -1;
	}





	String text;
	String textWithPath;
	vector<String> vecText;

	char str2[255];
	char str[255];
	ifstream in("nazwy_sekwencji/nazwy_sekwencji.txt");
	if (!in) {
		cout << "file reading error";
		return 1;
	}

	while (in) {
		in.getline(str, 255);
		text = str;
		textWithPath = "sekwencje_wideo/" + text;
		vecText.push_back(textWithPath);
	}
	in.close();

	//for (int i = 0; i < vecText.size(); i++)
	//	cout << vecText[i] << endl;



	

	cap.open(vecText[0]);
	if (!cap.isOpened()) {
		cout << "movie opening error";
		waitKey(0);
		return -1;
	}



	Parameters p = Parameters();
	Line lineObj = Line();

	capBackground.read(p.backgroundModel);
	cvtColor(p.backgroundModel, p.backgroundModel, COLOR_BGR2GRAY);
	resize(p.backgroundModel, p.backgroundModel, Size(1000, 500));


	namedWindow("colorImage");
	createTrackbar("gaussianblur", "colorImage", &p.prog_slider_ksize, p.prog_slider_max_ksize, on_trackbar_blur, &p);
	createTrackbar("closing", "colorImage", &p.prog_slider_closing, p.prog_slider_max_closing, on_trackbar_closing, &p);
	createTrackbar("threshold", "colorImage", &p.prog_slider_threshold, p.prog_slider_max_threshold, on_trackbar_threshold, &p);

	while (true) {

		if (waitKey(10) == 50)
			for (unsigned int i = 0; i < 500; i++) {
				cap.read(p.frame);
				p.frameNumber = p.frameNumber + 1;
			}

		if (waitKey(10) == 49)
			for (unsigned int i = 0; i < 100; i++) {
				cap.read(p.frame);
				p.frameNumber = p.frameNumber + 1;
			}

		p.frameNumber++;
		cap.read(p.frame);
		resize(p.frame, p.frame, Size(1000, 500));
		Mat color;
		p.frame.copyTo(color);
		cvtColor(p.frame, p.frame, COLOR_BGR2GRAY);

		p.medianImage = p.przyblizonaMediana(p.frame, p.backgroundModel);

		on_trackbar_blur(p.prog_slider_ksize, &p);
		on_trackbar_closing(p.prog_slider_closing, &p);
		on_trackbar_threshold(p.prog_slider_threshold, &p);


		findContours(p.thresholdImage, p.contours, RETR_EXTERNAL, 1);
		vector<vector<Point>> convex(p.contours.size());
		p.rect.resize(p.contours.size());

		p.thresholdImage.copyTo(p.convexImage);
		p.thresholdImage.copyTo(p.contourImage);

		for (unsigned int i = 0; i< p.contours.size(); i++)
		{

			convexHull(p.contours[i], convex[i]);
			//drawContours(p.contourImage, p.contours, i, Scalar(255, 0, 0), 1, LINE_AA);
			//drawContours(p.convexImage, convex, i, Scalar(255, 0, 0), 1, LINE_AA);

			p.rect.at(i) = boundingRect(p.contours[i]);
			rectangle(color, p.rect.at(i), Scalar(255, 0, 0), 2, LINE_AA);
		}

		//imshow("contourImage", p.contourImage);
		//imshow("convexImage", p.convexImage);


		lineObj.checkCrossedLineLeft(p.rect);
		lineObj.checkCrossedLinePeople(p.rect);
		lineObj.checkCrossedLineRight(p.rect);
		lineObj.checkCrossedLineTrams(p.rect);

		//left
		if (lineObj.blockLeft == false)
			line(color, lineObj.p11, lineObj.p12, Scalar(0, 0, 255), 2);
		else if (lineObj.blockLeft == true)
			line(color, lineObj.p11, lineObj.p12, Scalar(0, 255, 0), 2);

		//people
		if (lineObj.blockPeople == false)
			line(color, lineObj.p21, lineObj.p22, Scalar(0, 0, 255), 2);
		else if (lineObj.blockPeople == true)
			line(color, lineObj.p21, lineObj.p22, Scalar(0, 255, 0), 2);

		//right
		if (lineObj.blockRight == false)
			line(color, lineObj.p31, lineObj.p32, Scalar(0, 0, 255), 2);
		else if (lineObj.blockRight == true)
			line(color, lineObj.p31, lineObj.p32, Scalar(0, 255, 0), 2);

		//trams left
		if (lineObj.blockTramsLeft == false)
			line(color, lineObj.p41, lineObj.p42, Scalar(0, 0, 255), 2);
		else if (lineObj.blockTramsLeft == true)
			line(color, lineObj.p41, lineObj.p42, Scalar(0, 255, 0), 2);

		//trams right
		if (lineObj.blockTramsRight == false)
			line(color, lineObj.p43, lineObj.p44, Scalar(0, 0, 255), 2);
		else if (lineObj.blockTramsRight == true)
			line(color, lineObj.p43, lineObj.p44, Scalar(0, 255, 0), 2);


		putText(color, "Cars Left", Point(10, 400), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, "Trucks Left", Point(10, 420), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, "People", Point(10, 440), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, "Cars Right", Point(150, 400), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, "Trucks Right", Point(150, 420), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, "Trams", Point(150, 440), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);

		putText(color, to_string(lineObj.carsLeft), Point(100, 400), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, to_string(lineObj.trucksLeft), Point(100, 420), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, to_string(lineObj.people), Point(100, 440), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, to_string(lineObj.carsRight), Point(250, 400), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, to_string(lineObj.trucksRight), Point(250, 420), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		putText(color, to_string(lineObj.trams), Point(250, 440), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);

		putText(color, to_string(p.frameNumber), Point(70, 480), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, LINE_AA, false);
		imshow("colorImage", color);


	}

	return 0;
}