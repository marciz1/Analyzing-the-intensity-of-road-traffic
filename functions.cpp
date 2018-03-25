#include "functions.h"

void on_trackbar_blur(int alpha_slider, void* param) {

	Parameters* p = (Parameters*)param;
	p->medianImage.copyTo(p->blurImage);
	alpha_slider = alpha_slider + 1;
	blur(p->blurImage, p->blurImage, Size(alpha_slider, alpha_slider));
	//imshow("blurImage", p->blurImage);
}

void on_trackbar_closing(int alpha_slider, void* param) {

	Parameters* p = (Parameters*)param;
	p->blurImage.copyTo(p->closingImage);
	dilate(p->closingImage, p->closingImage, Mat());
	for (int i = 0; i < alpha_slider; i++) {
		erode(p->closingImage, p->closingImage, Mat());
	}

	//imshow("closingImage", p->closingImage);

}

void on_trackbar_threshold(int alpha_slider, void* param) {

	Parameters* p = (Parameters*)param;
	p->closingImage.copyTo(p->thresholdImage);
	threshold(p->thresholdImage, p->thresholdImage, alpha_slider, p->prog_slider_max_threshold, THRESH_BINARY);
	//imshow("thresholdImage", p->thresholdImage);

}





Mat Parameters::przyblizonaMediana(Mat I, Mat& B) {
	Mat dst;
	int numberOfElemnts = I.rows * I.cols;
	uchar* i = I.data;
	uchar* b = B.data;

	for (int j = 0; j < numberOfElemnts; ++j)
	{
		if (i[j] > b[j])
			b[j] = b[j] + 1;
		if (i[j] < b[j])
			b[j] = b[j] - 1;
	}
	absdiff(I, B, dst);
	return dst;
}




void Line::checkCrossedLineLeft(vector<Rect> rect) {

	if (blockCounterLeft == blockCounterLeftMax) {
		blockCounterLeft = 0;
		blockLeft = false;
	}
	if (blockLeft == true) {
		blockCounterLeft++;
	}

	for (unsigned int i = 0; i < rect.size(); i++) {
		Point centre1(rect[i].x , rect[i].y + rect[i].height);

//car left
		if (rect[i].area() > areaCarLeft[0] && rect[i].area() < areaCarLeft[1]) {
			if (centre1.x < p11.x + activationSensity && centre1.x > p11.x - activationSensity) {
				if (centre1.y > p11.y && centre1.y < p12.y) {

					if (blockLeft == false) {
						cout << "Car Left: " << rect[i].area() << endl;
						carsLeft++;
						blockLeft = true;
					}

				}
			}
		}

//truck left
		if (rect[i].area() > areaTruckLeft[0] && rect[i].area() < areaTruckLeft[1]) {
			if (centre1.x < p11.x + activationSensity && centre1.x > p11.x - activationSensity) {
				if (centre1.y > p11.y && centre1.y < p12.y) {

					if (blockLeft == false) {
						cout << "Truck Left: " << rect[i].area() << endl;
						trucksLeft++;
						blockLeft = true;
					}
				}
			}
		}

	}
}




void Line::checkCrossedLineRight(vector<Rect> rect) {


	if (blockCounterRight == blockCounterRightMax) {
		blockRight = false;
		blockCounterRight = 0;
	}

	if (blockRight == true) {
		blockCounterRight++;
	}

	for (unsigned int i = 0; i < rect.size(); i++) {
		Point centre1(rect[i].x + rect[i].width, rect[i].y + rect[i].height);

//car right
		if (rect[i].area() > areaCarRight[0] && rect[i].area() <  areaCarRight[1]) {
			if (centre1.x < p31.x + activationSensity && centre1.x > p31.x - activationSensity) {
				if (centre1.y > p31.y && centre1.y < p32.y) {

					if (blockRight == false) {
						cout << "Car Right: " << rect[i].area() << endl;
						carsRight++;
						blockRight = true;
					}

				}
			}
		}

//truck right
		if (rect[i].area() > areaTruckRight[0] && rect[i].area() < areaTruckRight[1]) {
			if (centre1.x < p31.x + activationSensity && centre1.x > p31.x - activationSensity) {
				if (centre1.y > p31.y && centre1.y < p32.y) {

					if (blockRight == false) {
						cout << "Truck Right: " << rect[i].area() << endl;
						trucksRight++;
						blockRight = true;
					}
				}
			}
		}

	}
}

void Line::checkCrossedLinePeople(vector<Rect> rect) {


	if (blockCounterPeople == blockCounterPeopleMax) {
		blockPeople = false;
		blockCounterPeople = 0;
	}

	if (blockPeople == true) {
		blockCounterPeople++;
	}


	for (unsigned int i = 0; i < rect.size(); i++) {
		Point centre1(rect[i].x + rect[i].width, rect[i].y + rect[i].height / 2);

		if (rect[i].area() > areaPeople[0] && rect[i].area() < areaPeople[1]) {
			if (centre1.x < p21.x + activationSensity && centre1.x > p21.x - activationSensity) {
				if (centre1.y > p21.y && centre1.y < p22.y) {

					if (blockPeople == false) {
						people++;
						cout << "People: " << rect[i].area() << endl;
						blockPeople = true;
					}
				}
			}
		}
	}
}


void Line::checkCrossedLineTrams(vector<Rect> rect) {


	if (blockCounterTramsLeft == blockCounterTramsLeftMax) {
		blockCounterTramsLeft = 0;
		blockTramsLeft = false;
	}

	if (blockTramsLeft == true) {
		blockCounterTramsLeft++;
	}

	if (blockCounterTramsRight == blockCounterTramsRightMax) {
		blockCounterTramsRight = 0;
		blockTramsRight = false;
	}

	if (blockTramsRight == true) {
		blockCounterTramsRight++;
	}


	for (unsigned int i = 0; i < rect.size(); i++) {
		Point centre1(rect[i].x, rect[i].y + rect[i].height);

//trams left
		if (rect[i].area() > areaTramLeft[0] && rect[i].area() < areaTramLeft[1]) {
			if (centre1.x < p41.x + 50 && centre1.x > p42.x - 50) {
				if (centre1.y > p41.y && centre1.y < p42.y) {

					if (blockTramsLeft == false) {
						cout << "Trams: " << rect[i].area() << endl;
						trams++;
						blockTramsLeft = true;
					}
				}
			}
		}

//trams right
		Point centre2(rect[i].x + rect[i].width, rect[i].y + rect[i].height);

		if (rect[i].area() > areaTramRight[0] && rect[i].area() < areaTramRight[1]) {
			if (centre2.x < p43.x + 30 && centre2.x > p44.x - 30) {
				if (centre2.y > p43.y && centre2.y < p44.y) {

					if (blockTramsRight == false) {
						cout << "Trams: " << rect[i].area() << endl;
						trams++;
						blockTramsRight = true;
					}
				}
			}

		}
	}
	
}
