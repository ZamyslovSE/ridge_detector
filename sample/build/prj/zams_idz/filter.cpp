//#include "stdafx.h"
//#include<iostream>
//#include<cmath>
//#include<opencv2/imgproc/imgproc.hpp>
//#include<opencv2/highgui/highgui.hpp>
//
//using namespace std;
//using namespace cv;
//
//
//// Computes the x component of the gradient vector
//// at a given point in a image.
//// returns gradient in the x direction
//int xGradient(Mat image, int x, int y)
//{
//	return image.at<uchar>(y - 1, x - 1) +
//		2 * image.at<uchar>(y, x - 1) +
//		image.at<uchar>(y + 1, x - 1) -
//		image.at<uchar>(y - 1, x + 1) -
//		2 * image.at<uchar>(y, x + 1) -
//		image.at<uchar>(y + 1, x + 1);
//}
//
//// Computes the y component of the gradient vector
//// at a given point in a image
//// returns gradient in the y direction
//
//int yGradient(Mat image, int x, int y)
//{
//	return image.at<uchar>(y - 1, x - 1) +
//		2 * image.at<uchar>(y - 1, x) +
//		image.at<uchar>(y - 1, x + 1) -
//		image.at<uchar>(y + 1, x - 1) -
//		2 * image.at<uchar>(y + 1, x) -
//		image.at<uchar>(y + 1, x + 1);
//}
//
//int main()
//{
//
//	Mat src, dst;
//	int gx, gy, sum;
//
//	// Load an image
//	src = imread("C:\\Users\\zamys\\Desktop\\Pics\\leaf.jpg", IMREAD_GRAYSCALE);
//	dst = src.clone();
//	if (!src.data)
//	{
//		return -1;
//	}
//
//
//	for (int y = 0; y < src.rows; y++)
//		for (int x = 0; x < src.cols; x++)
//			dst.at<uchar>(y, x) = 0.0;
//
//	for (int y = 1; y < src.rows - 1; y++) {
//		for (int x = 1; x < src.cols - 1; x++) {
//			gx = xGradient(src, x, y);
//			gy = yGradient(src, x, y);
//			sum = abs(gx) + abs(gy);
//			sum = sum > 255 ? 255 : sum;
//			sum = sum < 0 ? 0 : sum;
//			dst.at<uchar>(y, x) = sum;
//		}
//	}
//
//	namedWindow("initial");
//	imshow("initial", src);
//
//	namedWindow("final");
//	imshow("final", dst);
//
//	Mat thresh;
//	adaptiveThreshold(dst, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 41, 3);
//
//	namedWindow("thresh");
//	imshow("thresh", thresh);
//
//	waitKey();
//
//
//	return 0;
//}