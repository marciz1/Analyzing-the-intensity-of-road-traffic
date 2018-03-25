#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

void on_trackbar_threshold(int alpha_slider, void* param);
void on_trackbar_closing(int alpha_slider, void* param);
void on_trackbar_blur(int alpha_slider, void* param);

class Parameters
{
public: 
	Mat backgroundModel;
	Mat frame;
	Mat medianImage;
	Mat blurImage;
	Mat closingImage;
	Mat thresholdImage;

	Mat contourImage;
	Mat convexImage;

	int prog_slider_threshold;
	int prog_slider_closing;
	int prog_slider_max_threshold;
	int prog_slider_max_closing;
	int prog_slider_ksize;
	int prog_slider_max_ksize;

	int frameNumber;

	vector<vector<Point>>contours;
	vector<Rect> rect;

	Parameters() {
		prog_slider_threshold = 25;
		prog_slider_closing = 1;
		prog_slider_max_closing = 20;
		prog_slider_max_threshold = 150;
		prog_slider_ksize = 15;
		prog_slider_max_ksize = 30;

		frameNumber = 0;

	}

	Mat przyblizonaMediana(Mat I, Mat& B);


};

class Line {
public:
	Point p11, p12;
	Point p21, p22;
	Point p31, p32;
	Point p41, p42 ,p43, p44;

	bool blockLeft;
	bool blockRight;
	bool blockPeople;
	bool blockTramsLeft;
	bool blockTramsRight;
	bool blockTramsRightSpecial;

	int blockCounterLeft;
	int blockCounterRight;
	int blockCounterPeople;
	int blockCounterTramsLeft;
	int blockCounterTramsRight;


	int blockCounterLeftMax;
	int blockCounterRightMax;
	int blockCounterPeopleMax;
	int blockCounterTramsLeftMax;
	int blockCounterTramsRightMax;


	int areaCarLeft[2] = { 2000, 10000 };
	int areaTruckLeft[2] = { 10000, 35000 };

	int areaCarRight[2] = { 4000, 35000 };
	int areaTruckRight[2] = { 35000, 100000 };

	int areaTramLeft[2] = { 40000, 200000 };
	int areaTramRight[2] = { 1500, 200000 };

	int areaPeople[2] = { 1000, 5000 };
	int areaCyclist[2];

	int carsLeft;
	int trucksLeft;
	int carsRight;
	int trucksRight;
	int people;
	int trams;

	int activationSensity;


	Line() {
		//left
		p11 = Point(70, 40);
		p12 = Point(70, 160);
		//people
		p21 = Point(800, 320);
		p22 = Point(800, 500);
		//right
		p31 = Point(420, 220);
		p32 = Point(420, 330);
		//tram left
		p41 = Point(20, 170);
		p42 = Point(20, 220);
		//tram right
		p43 = Point(970, 170);
		p44 = Point(970, 220);

		blockLeft = false;
		blockRight = false;
		blockPeople = false;
		blockTramsLeft = false;
		blockTramsRight = false;


		blockCounterLeft = 0;
		blockCounterRight = 0;
		blockCounterPeople = 0;
		blockCounterTramsLeft = 0;
		blockCounterTramsRight = 0;


		blockCounterLeftMax = 7;
		blockCounterRightMax = 7;
		blockCounterPeopleMax = 15;
		blockCounterTramsLeftMax = 300;
		blockCounterTramsRightMax = 300;


		people = 0;
		carsLeft = 0;
		trucksLeft = 0;
		carsRight = 0;
		trucksRight = 0;
		trams = 0;

		activationSensity = 20;

	}

	void checkCrossedLineLeft(vector<Rect> rect);
	void checkCrossedLinePeople(vector<Rect> rect);
	void checkCrossedLineRight(vector<Rect> rect);
	void checkCrossedLineTrams(vector<Rect> rect);

};




#endif
