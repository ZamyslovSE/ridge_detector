// zams_idz.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"


class RidgeDetectionFilterImpl
{

public:
	int _ddepth, _dx, _dy, _ksize;
	double _scale, _delta;
	int _borderType;
	int _out_dtype;
	void getRidgeFilteredImage(InputArray _img, OutputArray out);
	RidgeDetectionFilterImpl(int ddepth = CV_32FC1, int dx = 1, int dy = 1, int ksize = 3, int out_dtype = CV_8UC1, double scale = 1, double delta = 0, int borderType = BORDER_DEFAULT)
	{
		CV_Assert((ksize == 1 || ksize == 3 || ksize == 5 || ksize == 7));
		CV_Assert((ddepth == CV_32FC1 || ddepth == CV_64FC1));
		_ddepth = ddepth;
		_dx = dx;
		_dy = dy;
		_ksize = ksize;
		_scale = scale;
		_delta = delta;
		_borderType = borderType;
		_out_dtype = out_dtype;
	}
};

void RidgeDetectionFilterImpl::getRidgeFilteredImage(InputArray _img, OutputArray out)
{
	Mat img = _img.getMat();
	CV_Assert(img.channels() == 1 || img.channels() == 3);

	if (img.channels() == 3)
		cvtColor(img, img, COLOR_BGR2GRAY);

	Mat sbx, sby;
	Sobel(img, sbx, _ddepth, _dx, 0, _ksize, _scale, _delta, _borderType);
	Sobel(img, sby, _ddepth, 0, _dy, _ksize, _scale, _delta, _borderType);

	Mat sbxx, sbyy, sbxy;
	Sobel(sbx, sbxx, _ddepth, _dx, 0, _ksize, _scale, _delta, _borderType);
	Sobel(sby, sbyy, _ddepth, 0, _dy, _ksize, _scale, _delta, _borderType);
	Sobel(sbx, sbxy, _ddepth, 0, _dy, _ksize, _scale, _delta, _borderType);

	Mat sb2xx, sb2yy, sb2xy;
	multiply(sbxx, sbxx, sb2xx);
	multiply(sbyy, sbyy, sb2yy);
	multiply(sbxy, sbxy, sb2xy);

	Mat sbxxyy;
	multiply(sbxx, sbyy, sbxxyy);

	Mat rootex;
	rootex = (sb2xx + (sb2xy + sb2xy + sb2xy + sb2xy) - (sbxxyy + sbxxyy) + sb2yy);
	Mat root;
	sqrt(rootex, root);
	Mat ridgexp;
	ridgexp = ((sbxx + sbyy) + root);
	ridgexp.convertTo(out, _out_dtype, 0.5);
}



Mat Img;
Mat Blur;
Mat Filter;
Mat Thresh;
Mat Overlay;

int threshold_border = 127;

int gauss_kernel_size = 9;

int sigma = 100;
const int sigmaMax = 1000;
double sigma_double = 1.0;

int delta = 0;
const int deltaMax = 1000;
double delta_double = 0;

int scale = 100;
const int scaleMax = 300;
double scale_double = 1.0;

int ddepth = CV_32FC1;
int dx = 1;
int dy = 1;
int ksize = 3;
int out_dtype = CV_8UC1;
//double scale = 1;
//double delta = 0;
int borderType = BORDER_DEFAULT;


void refresh()
{
	GaussianBlur(Img, Blur, Size(gauss_kernel_size, gauss_kernel_size), sigma_double, sigma_double, BORDER_DEFAULT);
	imshow("Blur", Blur);

	RidgeDetectionFilterImpl* filter = new RidgeDetectionFilterImpl(ddepth, dx, dy, ksize, out_dtype, scale_double, delta_double, borderType);
	filter->getRidgeFilteredImage(Blur, Filter);
	imshow("Ridge detection", Filter);

	threshold(Filter, Thresh, threshold_border, 255, THRESH_BINARY);
	imshow("Threshold", Thresh);

	//Mat thresh_rgb(Thresh.size(), CV_8UC3);
	//cvtColor(Thresh, thresh_rgb, COLOR_GRAY2RGB);
	////applyColorMap(thresh_rgb, thresh_rgb, COLORMAP_JET);
	//Mat img_rgb(Img.size(), CV_8UC4);
	//cvtColor(Img, img_rgb, COLOR_GRAY2RGB);


	//Mat src;
	//thresh_rgb.copyTo(src);
	//Mat dst;// (src.rows, src.cols, CV_8UC4);
	//Mat tmp, alpha;

	////cvtColor(src, tmp, COLOR_GRAY2RGB);
	//threshold(Thresh, alpha, 100, 255, THRESH_BINARY);

	//Mat rgb[3];
	//split(src, rgb);

	//Mat rgba[4] = { rgb[0],rgb[1],rgb[2],alpha };
	//merge(rgba, 4, dst);


	////imshow("Overlay", dst);
	//double alph = 0.3;
	//double beta = (1.0 - alph);
	//addWeighted(dst, alph, img_rgb, beta, 0.0, Overlay);

	//imshow("Overlay", Overlay);
}

static void on_trackbar_sigma(int, void*)
{
	sigma_double = (double)sigma / sigmaMax * 20;
	cout << "sigma=" << sigma_double << endl;
	refresh();
}

static void on_trackbar_scale(int, void*)
{
	scale_double = (double)scale / scaleMax * 3;
	cout << "scale=" << scale_double << endl;
	refresh();
}

static void on_trackbar_threshold(int, void*)
{
	refresh();
}

int main()
{
	string filename = "C:\\Users\\zamys\\Desktop\\pics\\grey\\fence.jpg";
	Img = imread(filename, IMREAD_GRAYSCALE);
	
	/*int y_size = 500;
	int x_size;
	if (Img.rows >= y_size) {
		x_size = (int)(Img.cols / ((double)Img.rows / y_size));
		resize(Img, Img, Size(x_size, y_size));
	}
	else
	{
		x_size = (int)(Img.cols * ((double)y_size / Img.rows));
		resize(Img, Img, Size(x_size, y_size));
	}*/

	// Вывод оригинала
	namedWindow("The Original Picture");
	//namedWindow("The Original Picture", 0);
	//resizeWindow("The Original Picture", 500, 500);
	imshow("The Original Picture", Img);

	// Вывод размытого изображения
	namedWindow("Blur");
	//namedWindow("Blur", 0);
	//resizeWindow("Blur", 500, 500);
	createTrackbar("Sigma", "Blur", &sigma, sigmaMax, on_trackbar_sigma);

	// Вывод отфильтрованного изображения
	namedWindow("Ridge detection");
	//namedWindow("Ridge detection", 0);
	//resizeWindow("Ridge detection", 500, 500);
	createTrackbar("Scale", "Ridge detection", &scale, scaleMax, on_trackbar_scale);

	namedWindow("Threshold");
	//namedWindow("Threshold", 0);
	//resizeWindow("Threshold", 500, 500);
	createTrackbar("Border", "Threshold", &threshold_border, 255, on_trackbar_threshold);

	//namedWindow("Overlay");

	refresh();
	waitKey();
}


