//--------------------------------------------------------------------
//				Bingyang Liu  2015/07/14
//				Meanshift Algorithm
//				OpenCV 3.0.0
//--------------------------------------------------------------------

//---------------- Head  File ---------------------------------------
#include <iostream>
#include "MeanShift.h"

int main() {
	// Load image
	Mat Img = imread("C:/Users/julno/Desktop/iV1qavzzp_4.jpg");
	resize(Img, Img, Size(685, 395), 0, 0, 1);
	// Show that image
	namedWindow("The Original Picture");
	imshow("The Original Picture", Img);

	// Convert color from RGB to Lab
	cvtColor(Img, Img, COLOR_RGB2Lab);

	// Initilize Mean Shift with spatial bandwith and color bandwith
	MeanShift MSProc(16, 8);
	// Filtering Process
	MSProc.MSFiltering(Img);
	// Segmentation Process include Filtering Process (Region Growing)
//	MSProc.MSSegmentation(Img);

	// Print the bandwith
	cout << "the Spatial Bandwith is " << MSProc.hs << endl;
	cout << "the Color Bandwith is " << MSProc.hr << endl;

	// Convert color from Lab to RGB
	cvtColor(Img, Img, COLOR_Lab2RGB);

	// Show the result image
	namedWindow("MS Picture");
	imshow("MS Picture", Img);

	Mat dst = Img;

	float kernel[9];
	kernel[0] = 0;
	kernel[1] = 0;
	kernel[2] = 0;

	kernel[3] = 0;
	kernel[4] = 1;
	kernel[5] = 0;

	kernel[6] = 0;
	kernel[7] = 0;
	kernel[8] = 0;

	Mat kernel_matrix = Mat (3, 3, CV_32FC1, kernel);
	filter2D(Img, dst, 4, kernel_matrix, Point(-1, -1));

	namedWindow("Picture");
	imshow("Picture", dst);

	waitKey();
	return 1;
}