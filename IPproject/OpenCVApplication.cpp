
#include "stdafx.h"
#include "common.h"
#include <math.h>
#include <stdio.h>
#include <queue>
#include <random>
#include <ctime>
#include <cmath>

#define _USE_MATH_DEFINES

Mat openImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname, CV_8UC1);
		return src;
	}
}


Mat thresholding(Mat image) {
	int threshold = 135;
	Mat thresholded = Mat(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) < threshold) {
				thresholded.at<uchar>(i, j) = 0;
			}
			else {
				thresholded.at<uchar>(i, j) = 255;
			}
		}
	}
	return thresholded;
}

Mat connectedComponents(Mat image) {
	int di[4] = { -1,0,1,0 };
	int dj[4] = { 0,-1,0,1 };
	int label = 0;
	Mat labels = Mat::zeros(image.rows, image.cols, CV_32FC1);
	for (int i = 0; i < image.rows - 1; i++) {
		for (int j = 0; j < image.cols - 1; j++) {
			if (image.at<uchar>(i, j) == 0 && labels.at<float>(i, j) == 0) {
				std::queue <Point2i> Q;
				label++;
				labels.at<float>(i, j) = label;
				Q.push(Point2i(i, j));
				while (!Q.empty()) {
					Point2i q = Q.front();
					Q.pop();
					for (int k = 0; k < 4; k++) {
						if (q.x + di[k] < 0 || q.y + dj[k] < 0 || q.x + di[k] > image.rows || q.y + dj[k] > image.cols) {
							continue;
						}
						if (image.at<uchar>(q.x + di[k], q.y + dj[k]) == 0 && labels.at<float>(q.x + di[k], q.y + dj[k]) == 0) {
							labels.at<float>(q.x + di[k], q.y + dj[k]) = label;
							Q.push(Point2i(q.x + di[k], q.y + dj[k]));
							
						}
					}
				}
			}
		}
	}
	return labels;
}
	
Mat cleanConnectedComponents(Mat image) {
	int arr[30000] = { 0 };
	Mat result = Mat(image.rows, image.cols, CV_32FC1);
	for (int i = 0; i < image.rows - 1; i++) {
		for (int j = 0; j < image.cols - 1; j++) {
			result.at<float>(i, j) = image.at<float>(i, j);
		}
	}
	for (int i = 0; i < image.rows - 1; i++) {
		for (int j = 0; j < image.cols - 1; j++) {
			arr[(int)image.at<float>(i, j)] += 1;
		}
	}
	for (int i = 1; i < 3000; i++) {
		if (arr[i] > 1000) {
			for (int m = 0; m < image.rows - 1; m++) {
				for (int n = 0; n < image.cols - 1; n++) {
					if (image.at<float>(m, n) == i) {
						result.at<float>(m, n) = 0;
					}
				}
			}
		}
	}
	return result;
}

Mat convertToUchar(Mat image) {
	Mat result = Mat(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows - 1; i++) {
		for (int j = 0; j < image.cols - 1; j++) {
			if (image.at<float>(i, j) == 0.0f) {
				result.at<uchar>(i, j) = 0;
			}
			else {
				result.at<uchar>(i, j) = 200;
			}
		}
	}
	return result;
}

Mat ndilation(int n, Mat src) {

	int height = src.rows;
	int width = src.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	Mat original = Mat(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			original.at<uchar>(i, j) = src.at<uchar>(i, j);
		}
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst.at<uchar>(i, j) = src.at<uchar>(i,j);
		}
	}
	for (int k = 0; k < n; k++) {
		for (int i = 1; i < height - 1; i++) {
			for (int j = 1; j < width - 1; j++) {
				if (src.at<uchar>(i, j) != 0) {
					dst.at<uchar>(i, j) = src.at<uchar>(i,j);
					dst.at<uchar>(i - 1, j) = src.at<uchar>(i, j);
					dst.at<uchar>(i, j - 1) = src.at<uchar>(i, j);
					dst.at<uchar>(i + 1, j) = src.at<uchar>(i, j);
					dst.at<uchar>(i, j + 1) = src.at<uchar>(i, j);
				}
			}
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				src.at<uchar>(i, j) = dst.at<uchar>(i, j);
			}
		}
	}
	return dst;
}

Mat getTextArea(Mat image, Mat initial) {
	Mat result = Mat(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			result.at<uchar>(i, j) = initial.at<uchar>(i, j);
		}
	}
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (initial.at<uchar>(i, j) != 0 && image.at<uchar>(i, j) != 0) {
				result.at<uchar>(i, j) = 200;
			}
		}
	}
	return result;
}

int main()
{
	Mat image = openImage();
	Mat thresholded = thresholding(image);
	Mat labeled = Mat(image.rows, image.cols, CV_32FC1);
	Mat dilated = Mat(image.rows, image.cols, CV_8UC1);
	Mat final = Mat(image.rows, image.cols, CV_8UC1);
	Mat cleaned = Mat(image.rows, image.cols, CV_32FC1);
	Mat converted = Mat(image.rows, image.cols, CV_8UC1);

	labeled = connectedComponents(thresholded);
	cleaned = cleanConnectedComponents(labeled);
	converted = convertToUchar(cleaned);
	dilated = ndilation(20, converted);
	final = getTextArea(dilated, thresholded);

	imshow("initial", image);
	//imshow("thresh", thresholded);
	//imshow("labeled", labeled);
	//imshow("cleaned", cleaned);
	//imshow("converted", converted);
	//imshow("dilated", dilated);

	imwrite("final.bmp", final);
	imshow("final", final);

	waitKey();
	return 0;
}