#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main() {

	Mat src = imread("C:/Users/julno/Desktop/sample/data/Jenna.jpg");
	if (src.data == nullptr) return -1;

	imshow("src", src);

	// Create binary image from source image (EN)
	// ������� �������� ����������� �� ��������� (RU)
	Mat bw;
	cvtColor(src, bw, COLOR_BGR2GRAY);
	threshold(bw, bw, 40, 255, THRESH_BINARY);
	imshow("bw", bw);

	// Perform the distance transform algorithm (EN)
	// ��������� �������� distance transform (RU)
	Mat dist;
	distanceTransform(bw, dist, DIST_L2, 3);

	// Normalize the distance image for range = {0.0, 1.0} (EN)
	// ����������� ����������� � ��������� {0.0 1.0} (RU)
	normalize(dist, dist, 0, 1., NORM_MINMAX);
	imshow("dist", dist);

	// Threshold to obtain the peaks
	// This will be the markers for the foreground objects (EN)
	// ��������� Threshold ��� ����������� �����
	// ��� ����� ������� ��� �������� �� �������� ����� (RU)
	threshold(dist, dist, .5, 1., THRESH_BINARY);
	imshow("dist2", dist);

	// Create the CV_8U version of the distance image
	// It is needed for cv::findContours() (EN)
	// ������� CV_8U ������ distance �����������
	// ��� ����� ��� ������� cv::findContours() (RU)
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);

	// Find total markers (EN)
	// ������� ��� ������� (RU)
	vector<vector<Point> > contours;
	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	auto ncomp = static_cast<int>(contours.size());

	// Create the marker image for the watershed algorithm (EN)
	// ������� ��������� ����������� ��� ��������� watershed (RU)
	Mat markers = Mat::zeros(dist.size(), CV_32SC1);

	// Draw the foreground markers (EN)
	// ������ ������� ��������� ����� (RU)
	for (int i = 0; i < ncomp; i++)
		drawContours(markers, contours, i, Scalar::all(i + 1), -1);


	// Draw background marker
	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
	imshow("markers", markers * 10000);

	// Perform the watershed algorithm (EN)
	// ��������� �������� watershed (RU)
	watershed(src, markers);

	// Generate random colors (EN)
	// ���������� ��������� ����� (RU)
	vector<Vec3b> colors;

	for (int i = 0; i < ncomp; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);

		colors.emplace_back(static_cast<uchar>(b), static_cast<uchar>(g), static_cast<uchar>(r));
	}

	// Create the result image (EN)
	// ������� �������������� ����������� (RU)
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);

	// Fill labeled objects with random colors (EN)
	// ��������� ���������� ������� ��������� ������ (RU)
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			dst.at<Vec3b>(i, j) = index > 0 && index <= ncomp ? colors[index - 1] : Vec3b(0, 0, 0);
		}
	}

	imshow("dst", dst);

	waitKey(0);

	return 0;
}