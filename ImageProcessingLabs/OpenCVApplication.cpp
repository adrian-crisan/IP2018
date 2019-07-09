// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <math.h>
#include <stdio.h>
#include <queue>
#include <random>
#include <ctime>
#include <cmath>

#define _USE_MATH_DEFINES

void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testNegativeImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]
		
		Mat src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height,width,CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				uchar val = src.at<uchar>(i,j);
				uchar neg = 255-val;
				dst.at<uchar>(i,j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}

void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the “diblook style”
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		int w = (int) src.step; // no dword alignment is done !!!
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}

void testColor2Gray()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src = imread(fname);

		int height = src.rows;
		int width = src.cols;

		Mat dst = Mat(height,width,CV_8UC1);

		// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i,j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst.at<uchar>(i,j) = (r+g+b)/3;
			}
		}
		
		imshow("input image",src);
		imshow("gray image",dst);
		waitKey();
	}
}

void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				int hi = i*width * 3 + j * 3;
				int gi = i*width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}

void testResize()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1,dst2;
		//without interpolation
		resizeImg(src,dst1,320,false);
		//with interpolation
		resizeImg(src,dst2,320,true);
		imshow("input image",src);
		imshow("resized image (without interpolation)",dst1);
		imshow("resized image (with interpolation)",dst2);
		waitKey();
	}
}

void testCanny()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src,dst,gauss;
		src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		double k = 0.4;
		int pH = 50;
		int pL = (int) k*pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss,dst,pL,pH,3);
		imshow("input image",src);
		imshow("canny",dst);
		waitKey();
	}
}

void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
	//VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey(0);
		return;
	}
		
	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame,edges,40,100,3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey(0);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n"); 
			break;  //ESC pressed
		};
	}
}


void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];
	
	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;
		
		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115){ //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess) 
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
		{
			printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
				x, y,
				(int)(*src).at<Vec3b>(y, x)[2],
				(int)(*src).at<Vec3b>(y, x)[1],
				(int)(*src).at<Vec3b>(y, x)[0]);
		}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

void testAdditive()
{
	Mat img = imread("Images/kids.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int height = img.rows;
	int width = img.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			uchar val = img.at<uchar>(i, j);
			uchar add = val + 30;
			if (val > 255) {
				val = 255;
			}
			dst.at<uchar>(i, j) = add;
		}
	}
	imshow("input image", img);
	imshow("negative image", dst);
	waitKey();

}

void testMultiplicative()
{
	Mat img = imread("Images/kids.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int height = img.rows;
	int width = img.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			uchar val = img.at<uchar>(i, j);
			uchar add = val * 2;
			if (val > 255) {
				val = 255;
			}
			if (val < 0) {
				val = 0;
			}
			dst.at<uchar>(i, j) = add;
		}
	}
	imwrite("Images/kids_mult.bmp", dst);
	imshow("input image", img);
	imshow("negative image", dst);
	waitKey();

}

void createColorImage() {
	Mat img(256, 256, CV_8UC3);
	for (int i = 0; i < 255; i++) {
		for (int j = 0; j < 255; j++) {
			if (i < 127 && j < 127) {
				img.at<Vec3b>(i, j)[0] = 255;
				img.at<Vec3b>(i, j)[1] = 255;
				img.at<Vec3b>(i, j)[2] = 255;
			}
			if (i < 127 && j > 127) {
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 0;
				img.at<Vec3b>(i, j)[2] = 255;
			}
			if (i > 127 && j < 127) {
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 255;
				img.at<Vec3b>(i, j)[2] = 0;
			}
			if (i > 127 && j > 127) {
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 255;
				img.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}
	imshow("color image", img);
	waitKey();
}

void matrixInverse() {
	float vals[9] = { 3, 0, 0, 0, 3, 0, 0, 0, 3 };
	Mat M(3, 3, CV_32FC1, vals);
	Mat invM(3, 3, CV_32FC1);
	invM = M.inv();
	std::cout << invM << std::endl;
	waitKey(0);
}

void line2sin() {
	float T = 30.0f;
	float A = 10.0f;
	Mat img = imread("Images/cameraman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int height = img.rows;
	int width = img.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			float x = (float)j / T * 360;
			float radx = x * PI / 180;
			if ((float)i + A*sin(radx) < 255 && (float)i + A*sin(radx) > 0) {
				 dst.at<uchar>(i, j) = img.at<uchar>((float)i + A*sin(radx), (float)j);
			}
			else {
				dst.at<uchar>(i, j) = img.at<uchar>(i, j);
			}
		
		}
	}
	imshow("input image", img);
	imshow("sin image", dst);
	waitKey();
}

void rgb3mat() {
	Mat src = imread("Images/kids.bmp", CV_LOAD_IMAGE_UNCHANGED);
	int height = src.rows;
	int width = src.cols;
	Mat dst1 = Mat(height, width, CV_8UC1);
	Mat dst2 = Mat(height, width, CV_8UC1);
	Mat dst3 = Mat(height, width, CV_8UC1);
	for (int i = 0; i<height; i++) {
		for (int j = 0; j<width; j++) {
			dst1.at<uchar>(i, j) = src.at<Vec3b>(i, j)[0];
			dst2.at<uchar>(i, j) = src.at<Vec3b>(i, j)[1];
			dst3.at<uchar>(i, j) = src.at<Vec3b>(i, j)[2];
		}
	}
	imshow("R", dst3);
	imshow("G", dst2);
	imshow("B", dst1);
	waitKey(0);
}

void rgb2gray() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;
		int r, g, b, avg;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r = src.at<Vec3b>(i, j)[2];
				g = src.at<Vec3b>(i, j)[1];
				b = src.at<Vec3b>(i, j)[0];
				avg = (int)(r + g + b) / 3;
				dst.at<uchar>(i, j) = avg;
			}
		}
		imshow("color", src);
		imshow("gray", dst);
		waitKey(0);
	}
}

void gray2binary() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int threshold = 0;
		printf("Threshold: ");
		scanf("%d", &threshold);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (src.at<uchar>(i, j) <= threshold) {
					dst.at<uchar>(i, j) = 0;
				}
				else {
					dst.at<uchar>(i, j) = 255;
				}
			}
		}
		imshow("gray", src);
		imshow("binary", dst);
		waitKey(0);
	}
}

void rgb2hsv() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;
		Mat dst1 = Mat(height, width, CV_8UC1);
		Mat dst2 = Mat(height, width, CV_8UC1);
		Mat dst3 = Mat(height, width, CV_8UC1);
		float r, g, b, maxim, minim, c, h, s, v;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r = (float)src.at<Vec3b>(i, j)[2] / 255.0;
				g = (float)src.at<Vec3b>(i, j)[1] / 255.0;
				b = (float)src.at<Vec3b>(i, j)[0] / 255.0;
				float maxim1 = max(r, g);
				maxim = max(maxim1, b);
				float minim1 = min(r, g);
				minim = min(minim1, b);
				c = maxim - minim;
				v = maxim;
				dst3.at<uchar>(i, j) = v * 255;
				if (c) {
					s = c / v;
				}
				else {
					s = 0;
				}
				dst2.at<uchar>(i, j) = s * 255;
				if (c) {
					if (maxim == r) {
						h = 60 * (g - b) / c;
					}
					if (maxim == g) {
						h = 120 + 60 * (b - r) / c;
					}
					if (maxim == b) {
						h = 240 + 60 * (r - g) / c;
					}
				}
				else {
					h = 0;
				}
				if (h < 0) {
					h = h + 360;
				}
				dst1.at<uchar>(i, j) = (h * 255) / 360;
			}
		}
		imshow("color", src);
		imshow("H", dst1);
		imshow("S", dst2);
		imshow("V", dst3);
		waitKey(0);
	}
}

void hsv2binary() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;
		Mat dst1 = Mat(height, width, CV_8UC1);
		//Mat dst2 = Mat(height, width, CV_8UC1);
		//Mat dst3 = Mat(height, width, CV_8UC1);
		float r, g, b, maxim, minim, c, h, s, v;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r = (float)src.at<Vec3b>(i, j)[2] / 255.0;
				g = (float)src.at<Vec3b>(i, j)[1] / 255.0;
				b = (float)src.at<Vec3b>(i, j)[0] / 255.0;
				float maxim1 = max(r, g);
				maxim = max(maxim1, b);
				float minim1 = min(r, g);
				minim = min(minim1, b);
				c = maxim - minim;
				v = maxim;
				//dst3.at<uchar>(i, j) = v * 255;
				if (c) {
					s = c / v;
				}
				else {
					s = 0;
				}
				//dst2.at<uchar>(i, j) = s * 255;
				if (c) {
					if (maxim == r) {
						h = 60 * (g - b) / c;
					}
					if (maxim == g) {
						h = 120 + 60 * (b - r) / c;
					}
					if (maxim == b) {
						h = 240 + 60 * (r - g) / c;
					}
				}
				else {
					h = 0;
				}
				if (h < 0) {
					h = h + 360;
				}
				if ((h > 340 && h < 360) || (h > 0 && h < 10) && s > 0.1) {
					dst1.at<uchar>(i, j) = 255;
				}
				else {
					dst1.at<uchar>(i, j) = 0;
				}
				//dst1.at<uchar>(i, j) = (h * 255) / 360;
			}
		}
		imshow("color", src);
		imshow("binary", dst1);
		waitKey(0);
	}
}

void mouseCallbackF(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		int r = (int)(*src).at<Vec3b>(y, x)[2];
		int g = (int)(*src).at<Vec3b>(y, x)[1];
		int b = (int)(*src).at<Vec3b>(y, x)[0];
		//area
		int height = (int)(*src).rows;
		int width = (int)(*src).cols;
		int row = 0, col = 0;
		int area = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (r == (*src).at<Vec3b>(i, j)[2] && g == (*src).at<Vec3b>(i, j)[1] && b == (*src).at<Vec3b>(i, j)[0]) {
					area += 1;
					row += i;
					col += j;
				}
			}
		}
		printf("Area = %d\n", area);
		//center of mass
		int rMass = row / area;
		int cMass = col / area;
		printf("Center of Mass = %d %d\n", rMass, cMass);
		//axis of elongation
		int res = 0, resr = 0, resc = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (r == (*src).at<Vec3b>(i, j)[2] && g == (*src).at<Vec3b>(i, j)[1] && b == (*src).at<Vec3b>(i, j)[0]) {
					res += (i - rMass)*(j - cMass);
					resr += (i - rMass)*(i - rMass);
					resc += (j - cMass)*(j - cMass);
				}
			}
		}
		float tan2 = atan2((2 * res), (resc - resr));
		printf("Axis of Elongation = %f\n", (tan2/3.1415)*180);
		
		//perimeter
		int perim = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (r == (*src).at<Vec3b>(i, j)[2] && g == (*src).at<Vec3b>(i, j)[1] && b == (*src).at<Vec3b>(i, j)[0]) {
					if ((*src).at<Vec3b>(i - 1, j)[0] == 255 || 
						(*src).at<Vec3b>(i + 1, j)[0] == 255 || 
						(*src).at<Vec3b>(i, j + 1)[0] == 255 || 
						(*src).at<Vec3b>(i, j - 1)[0] == 255) {
						perim += 1;
					}
				}
			}
		}
		printf("Perimeter = %d\n", perim);
		//thinness ratio
		float thinness = 4 * 3.14 * area / (perim * perim);
		printf("Thinness ratio = %f\n", thinness);
		
		//aspect ratio
		int cmax = 0, cmin = 50000, rmax = 0, rmin = 50000;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (r == (*src).at<Vec3b>(i, j)[2] && g == (*src).at<Vec3b>(i, j)[1] && b == (*src).at<Vec3b>(i, j)[0]) {
					if (i < rmin) {
						rmin = i;
					}
					if (i > rmax) {
						rmax = i;
					}
					if (j > cmax) {
						cmax = j;
					}
					if (j < cmin) {
						cmin = j;
					}
				}
			}
		}
		float aspect = (float)(cmax - cmin + 1) / (float)(rmax - rmin + 1);
		printf("Aspect ratio = %f\n", aspect);
		
		Mat* dst = src;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (r == (*dst).at<Vec3b>(i, j)[2] && g == (*dst).at<Vec3b>(i, j)[1] && b == (*dst).at<Vec3b>(i, j)[0]) {
					if ((*dst).at<Vec3b>(i - 1, j)[0] == 255 ||
						(*dst).at<Vec3b>(i + 1, j)[0] == 255 ||
						(*dst).at<Vec3b>(i, j + 1)[0] == 255 ||
						(*dst).at<Vec3b>(i, j - 1)[0] == 255) {
						(*dst).at<Vec3b>(i, j)[0] = 0;
						(*dst).at<Vec3b>(i, j)[1] = 0;
						(*dst).at<Vec3b>(i, j)[2] = 0;
					}
				}
			}
		}
		(*dst).at<Vec3b>(rMass, cMass)[0] = 0;
		(*dst).at<Vec3b>(rMass, cMass)[1] = 0;
		(*dst).at<Vec3b>(rMass, cMass)[2] = 0;

		Point p1, p2;
		p1.x = rMass;
		p1.y = cMass;
		p2.x = 100 * cos(tan2/2);
		p2.y = 100 * sin(tan2/2);
		line(*dst, p1, p2, 0, 1);
		imshow("My Window2", *dst);

		int vec1[1000], vec2[1000];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (r == (*src).at<Vec3b>(i, j)[2] && g == (*src).at<Vec3b>(i, j)[1] && b == (*src).at<Vec3b>(i, j)[0]) {
					vec1[i] += 1;
					vec1[j] += 1;
				}
			}
		}
		
	}
}

void onMouse()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", mouseCallbackF, &src);

		//show the image
		imshow("My Window", src);
		
		// Wait until user press some key
		waitKey(0);
	}
}

void bfsLabel() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		int di[8] = { -1,-1,-1,0,0,1,1,1 };
		int dj[8] = { -1,0,1,-1,1,-1,0,1 };
		uchar neighbors[8];
		int label = 0;
		Mat labels = Mat::zeros(height, width, CV_8UC1);
		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++) {
				if (src.at<uchar>(i, j) == 0 && labels.at<uchar>(i, j) == 0) {
					label++;
					std::queue <Point2i> Q;
					labels.at<uchar>(i, j) = label;
					Q.push(Point2i(i, j));
					while (!Q.empty()) {
						Point2i q = Q.front();
						Q.pop();
						for (int k = 0; k < 8; k++) {
							if (q.x + di[k] < 0 || q.y + dj[k] < 0 || q.x + di[k] > height || q.y + dj[k] > width) {
								continue;
							}
							if (src.at<uchar>(q.x + di[k], q.y + dj[k]) == 0 && labels.at<uchar>(q.x + di[k], q.y + dj[k]) == 0) {
								labels.at<uchar>(q.x + di[k], q.y + dj[k]) = label;
								Q.push(Point2i(q.x + di[k], q.y + dj[k]));
							}
						}
					}
				}
			}
		}
		imshow("labels", labels);
	}
}

int gen() {
	int x = rand() % 256;
	return x;
}

void bfsLabelColors() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		srand(time(NULL));
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		int nr = 0;
		//i,j-1 i,j+1, i+1,j i+1,j-1 i+1,j+1, i-1,j i-1,j-1 i-1,j+1
		int di[8] = { -1,-1,-1,0,0,1,1,1 };
		int dj[8] = { -1,0,1,-1,1,-1,0,1 };
		uchar neighbors[8];
		int label = 0;
		Mat labels = Mat::zeros(height, width, CV_8UC3);
		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++) {
				if (src.at<uchar>(i, j) == 0 && labels.at<Vec3b>(i, j) == Vec3b(0, 0, 0)) {
				
					uchar x = gen();
					uchar y = gen();
					uchar z = gen();
					labels.at<Vec3b>(i, j) = Vec3b(x, y, z);
					std::queue <Point2i> Q;
					Q.push(Point2i(i, j));
					while (!Q.empty()) {
						Point2i q = Q.front();
						Q.pop();
						for (int k = 0; k < 8; k++) {
							if (q.x + di[k] < 0 || q.y < dj[k] < 0 || q.x + di[k] > height || q.y + dj[k] > width) {
								continue;
							}
							if (src.at<uchar>(q.x + di[k], q.y + dj[k]) == 0 && labels.at<Vec3b>(q.x + di[k], q.y + dj[k]) == Vec3b(0,0,0)) {
								nr++;
								labels.at<Vec3b>(q.x + di[k], q.y + dj[k]) = Vec3b(x, y, z);
								Q.push(Point2i(q.x + di[k], q.y + dj[k]));
								if (nr > 50) {
									imshow("labels color", labels);
									waitKey();
									nr = 0;
								}
							}
						}
					}
				}
			}
		}
		imshow("labels colors", labels);
	}
}

void borderTracing() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		int di[8] = { 0,-1,-1,-1,0,1,1,1 };
		int dj[8] = { 1,1,0,-1,-1,-1,0,1 };
		int starti, startj;
		int dir = 7;
		Mat border = Mat::zeros(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (src.at<uchar>(i, j) != 255) {
					starti = i;
					startj = j;
					break;
				}
			}
		}
		int currenti = starti + 1;
		int currentj = startj + 1;
		int first = 0;
		int chain[10000];
		int index = 0;
		while (currenti != starti || currentj != startj) {
			if (first == 0) {
				currenti--;
				currentj--;
				first = 1;
			}
			if (dir % 2 != 0) {
				dir = (dir + 6) % 8;
			}
			else {
				dir = (dir + 7) % 8;
			}
			int newdir = dir;
			chain[index] = dir;
			index++;
			for (int k = newdir; k < newdir + 8; k++) {
				if (src.at<uchar>(currenti + di[k%8], currentj + dj[k%8]) != 255) {
					border.at<uchar>(currenti + di[k % 8], currentj + dj[k % 8]) = 255;
					dir = k % 8;
					currenti = currenti + di[k % 8];
					currentj = currentj + dj[k % 8];
					break;
				}
			}
		}
		for (int i = 0; i < index; i++) {
			printf("%d ", chain[i]);
		}
		border.at<uchar>(starti, startj) = 255;
		imshow("border", border);
	}

}

void dilation() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 1; i < height-1; i++) {
			for (int j = 1; j < width-1; j++) {
				if (src.at<uchar>(i, j) == 0) {
					dst.at<uchar>(i, j) = 0;
					dst.at<uchar>(i - 1, j) = 0;
					dst.at<uchar>(i, j - 1) = 0;
					dst.at<uchar>(i + 1, j) = 0;
					dst.at<uchar>(i, j + 1) = 0;
				}
			}
		}
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

void ndilation(int n) {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat original = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				original.at<uchar>(i, j) = src.at<uchar>(i,j);
			}
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = 255;
			}
		}
		for (int k = 0; k < n; k++) {
			for (int i = 1; i < height - 1; i++) {
				for (int j = 1; j < width - 1; j++) {
					if (src.at<uchar>(i, j) == 0) {
						dst.at<uchar>(i, j) = 0;
						dst.at<uchar>(i - 1, j) = 0;
						dst.at<uchar>(i, j - 1) = 0;
						dst.at<uchar>(i + 1, j) = 0;
						dst.at<uchar>(i, j + 1) = 0;
					}
				}
			}
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					src.at<uchar>(i, j) = dst.at<uchar>(i,j);
				}
			}
		}
		imshow("src", original);
		imshow("dst", dst);
		waitKey(0);
	}
}
void erosion() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 1; i < height - 1; i++) {
			for (int j = 1; j < width - 1; j++) {
				if (src.at<uchar>(i, j) == 0) {
					if (src.at<uchar>(i - 1, j) == 0 &&
						src.at<uchar>(i, j - 1) == 0 &&
						src.at<uchar>(i + 1, j) == 0 &&
						src.at<uchar>(i, j + 1) == 0) {
						dst.at<uchar>(i, j) = 0;
					}
				}
			}
		}
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

void opening() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat dst2 = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst2.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 1; i < height - 1; i++) {
			for (int j = 1; j < width - 1; j++) {
				if (src.at<uchar>(i, j) == 0) {
					if (src.at<uchar>(i - 1, j) == 0 &&
						src.at<uchar>(i, j - 1) == 0 &&
						src.at<uchar>(i + 1, j) == 0 &&
						src.at<uchar>(i, j + 1) == 0) {
						dst.at<uchar>(i, j) = 0;
					}
				}
			}
		}
		for (int i = 1; i < height - 1; i++) {
			for (int j = 1; j < width - 1; j++) {
				if (dst.at<uchar>(i, j) == 0) {
					dst2.at<uchar>(i, j) = 0;
					dst2.at<uchar>(i - 1, j) = 0;
					dst2.at<uchar>(i, j - 1) = 0;
					dst2.at<uchar>(i + 1, j) = 0;
					dst2.at<uchar>(i, j + 1) = 0;
				}
			}
		}
		imshow("src", src);
		imshow("dst", dst2);
		waitKey(0);
	}
}

void closing() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat dst2 = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst2.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 1; i < height - 1; i++) {
			for (int j = 1; j < width - 1; j++) {
				if (src.at<uchar>(i, j) == 0) {
					dst.at<uchar>(i, j) = 0;
					dst.at<uchar>(i - 1, j) = 0;
					dst.at<uchar>(i, j - 1) = 0;
					dst.at<uchar>(i + 1, j) = 0;
					dst.at<uchar>(i, j + 1) = 0;
				}
			}
		}
		for (int i = 1; i < height - 1; i++) {
			for (int j = 1; j < width - 1; j++) {
				if (dst.at<uchar>(i, j) == 0) {
					if (dst.at<uchar>(i - 1, j) == 0 &&
						dst.at<uchar>(i, j - 1) == 0 &&
						dst.at<uchar>(i + 1, j) == 0 &&
						dst.at<uchar>(i, j + 1) == 0) {
						dst2.at<uchar>(i, j) = 0;
					}
				}
			}
		}
		imshow("src", src);
		imshow("dst", dst2);
		waitKey(0);
	}
}

void boundaryExtraction() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat dst2 = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst2.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++) {
				if (src.at<uchar>(i, j) == 0) {
					if (src.at<uchar>(i - 1, j) == 0 &&
						src.at<uchar>(i, j - 1) == 0 &&
						src.at<uchar>(i + 1, j) == 0 &&
						src.at<uchar>(i, j + 1) == 0 &&
						src.at<uchar>(i - 1, j - 1) == 0 &&
						src.at<uchar>(i - 1, j + 1) == 0 &&
						src.at<uchar>(i + 1, j - 1) == 0 &&
						src.at<uchar>(i + 1, j + 1) == 0) {
						dst.at<uchar>(i, j) = 0;
					}
				}
			}
		}
		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++) {
				if (src.at<uchar>(i, j) == 0 && dst.at<uchar>(i, j) != 0) {
					dst2.at<uchar>(i, j) = 0;
				}
			}
		}
		imshow("src", src);
		imshow("dst", dst2);
		waitKey(0);
	}
}

Mat dilationnew(Mat img, Mat init) {
	Mat dst = Mat(img.rows, img.cols, CV_8UC1);
	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			dst.at<uchar>(i, j) = 255;
		}
	}
	img.copyTo(dst);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (i == 0 || i == img.rows - 1 || j == 0 || j == img.cols - 1) {
				continue;
			}
			if (dst.at<uchar>(i, j) == 0) {
				dst.at<uchar>(i, j) = 0;
				if (init.at<uchar>(i - 1, j - 1) != 0) {
					dst.at<uchar>(i - 1, j - 1) = 0;
				}
				if (init.at<uchar>(i - 1, j) != 0) {
					dst.at<uchar>(i - 1, j) = 0;
				}
				if (init.at<uchar>(i - 1, j + 1) != 0) {
					dst.at<uchar>(i - 1, j + 1) = 0;
				}
				if (init.at<uchar>(i, j - 1) != 0) {
					dst.at<uchar>(i, j - 1) = 0;
				}
				if (init.at<uchar>(i, j + 1) != 0) {
					dst.at<uchar>(i, j + 1) = 0;
				}
				if (init.at<uchar>(i + 1, j - 1) != 0) {
					dst.at<uchar>(i + 1, j - 1) = 0;
				}
				if (init.at<uchar>(i + 1, j) != 0) {
					dst.at<uchar>(i + 1, j) = 0;
				}
				if (init.at<uchar>(i + 1, j + 1) != 0) {
					dst.at<uchar>(i + 1, j + 1) = 0;
				}

			}
		}
	}
	return dst;
}

int equal(Mat a, Mat b) {
	int e = 1;
	for (int i = 0; i < a.rows; i++) {
		for (int j = 0; j < a.cols; j++) {
			if (a.at<uchar>(i, j) != b.at<uchar>(i, j)) {
				e = 0;
			}
		}
	}
	return e;
}

void regionfilling() {
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat src = imread(fname, CV_8UC1);
		Mat dst = Mat(src.rows, src.cols, CV_8UC1);
		for (int i = 0; i < dst.rows; i++) {
			for (int j = 0; j < dst.cols; j++) {
				dst.at<uchar>(i, j) = 255;
			}
		}
		int f = 1;
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (f == 1) {
					if (i == 0 || i == src.rows - 1 || j == 0 || j == src.cols - 1) {
						continue;
					}
					if (src.at<uchar>(i - 1, j) == 0 && src.at<uchar>(i, j - 1) == 0 && src.at<uchar>(i, j) != 0) {
						dst.at<uchar>(i, j) = 0;
						f = 0;
					}
				}
			}
		}
		boolean pos = true;
		Mat dst1;
		while (pos == true) {
			for (int i = 0; i < dst.rows; i++) {
				for (int j = 0; j < dst.cols; j++) {
					if (dst.at<uchar>(i, j) == 0) {
						dst.copyTo(dst1);
						dst = dilationnew(dst, src);
						if (equal(dst, dst1) == 1) {
							pos = false;
							break;
						}
					}
				}
			}
		}
		for (int i = 0; i < dst.rows; i++) {
			for (int j = 0; j < dst.cols; j++) {
				if (src.at<uchar>(i, j) == 0 || dst.at<uchar>(i, j) == 0) {
					dst.at<uchar>(i, j) = 0;
				}
			}
		}

		imshow("img", src);
		imshow("dst", dst);
		waitKey();
	}
}


void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

																		 //computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i<hist_cols; i++)
		if (hist[i] > max_hist)
			max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(0, 0, 0)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}

void computeHistogram() {
	Mat src;
	int h[255] = { 0 };
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
	}
	showHistogram("hist", h, 255, 300);
	waitKey(0);
}

void meanIntensity() {
	Mat src;
	int h[255] = { 0 };
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows-1; i++) {
				for (int j = 0; j < src.cols-1; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		int sum = 0;
		for (int i = 0; i < src.rows - 1; i++) {
			for (int j = 0; j < src.cols - 1; j++) {
				sum += (int)src.at<uchar>(i, j);
			}
		}
		int m = height * width;
		float mean = sum / m;
		printf("Mean Intensity Value: %f", mean);
		showHistogram("hist", h, 255, 300);
		waitKey(0);
	}
}

void stdDeviation() {
	Mat src;
	int h[255] = { 0 };
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows - 1; i++) {
				for (int j = 0; j < src.cols - 1; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		int sum = 0;
		for (int i = 0; i < src.rows - 1; i++) {
			for (int j = 0; j < src.cols - 1; j++) {
				sum += (int)src.at<uchar>(i, j);
			}
		}
		int m = height * width;
		float mean = sum / m;
		float std = 0.0f;
		for (int i = 0; i < src.rows - 1; i++) {
			for (int j = 0; j < src.cols - 1; j++) {
				std += ((int)src.at<uchar>(i, j) - mean)*((int)src.at<uchar>(i, j) - mean);
			}
		}
		float dev = sqrt(std / m);
		printf("Standard deviation: %f", dev);
		showHistogram("hist", h, 255, 300);
		waitKey(0);
	}
}

void computeCumulativeHistogram() {
	Mat src;
	int h[255] = { 0 };
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		int sum = 0;
		for (int k = 0; k <= 25; k++) {
			sum += h[k];
		}
		printf("Cumulative histogram: %d", sum);
	}
	showHistogram("hist", h, 255, 300);
	waitKey(0);
}

void globalThresholding() {
	Mat src;
	int h[255] = { 0 };
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		int imax, imin, threshold;
		int n1 = 0, n2 = 0;
		float ug1 = 0, ug2 = 0, sum1 = 0, sum2 = 0;
		int error = 3;

		for (int k = 0; k <= 255; k++) {
			if (h[k] != 0) {
				imin = k;
				break;
			}
		}
		for (int k = 255; k >= 0; k--) {
			if (h[k] != 0) {
				imax = k;
				break;
			}
		}
		threshold = (imax + imin) / 2;
		int t1 = threshold;
		int delta = 0;
		while (delta < error) {
			for (int k = 0; k <= 255; k++) {
				if (h[k] != 0) {
					imin = k;
					break;
				}
			}
			for (int k = 255; k >= 0; k--) {
				if (h[k] != 0) {
					imax = k;
					break;
				}
			}
			threshold = (imax + imin) / 2;
			for (int k = imin; k < threshold; k++) {
				n1 += h[k];
			}
			for (int k = threshold + 1; k <= imax; k++) {
				n2 += h[k];
			}
			for (int k = imin; k < threshold; k++) {
				sum1 += (k * h[k]);
			}
			for (int k = threshold + 1; k < imax; k++) {
				sum2 += (k * h[k]);
			}
			ug1 = sum1 / n1;
			ug2 = sum2 / n2;
			threshold = (ug1 + ug2) / 2;
			delta = abs(t1 - threshold);
			t1 = threshold;
		}
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (src.at<uchar>(i, j) <= threshold) {
					dst.at<uchar>(i, j) = 0;
				}
				else {
					dst.at<uchar>(i, j) = 255;
				}
			}
		}
		imshow("src", src);
		imshow("threshold", dst);
		showHistogram("hist", h, 255, 300);
		waitKey(0);
	}
}

void negativeFunc() {
	Mat src;
	int h[255] = { 0 };
	int newh[255] = { 0 };
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				dst.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);
			}
		}
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (dst.at<uchar>(i, j) == k) {
						newh[k]++;
					}
				}
			}
		}
		showHistogram("hist", newh, 255, 300);
		showHistogram("hist", h, 255, 300);
		imshow("src", src);
		imshow("neg", dst);
		waitKey(0);
	}
}

void histogramS() {
	Mat src;
	int h[255] = { 0 };
	int newh[255] = { 0 };
	float gmax, gmin;
	printf("Give min G: ");
	scanf("%f", &gmin);
	printf("\n");
	printf("Give max G: ");
	scanf("%f", &gmax);
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		Mat dst = Mat(height, width, CV_8UC1);
		float gout = 0;
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				gout = gmin + (float)(src.at<uchar>(i, j)) * ((gmax - gmin) / 255.0f);
				if (gout < 0) {
					dst.at<uchar>(i, j) = 0;
				}
				if (gout > 255) {
					dst.at<uchar>(i, j) = 255;
				}
				if (gout >= 0 && gout <= 255) {
					dst.at<uchar>(i, j) = (int)gout;
				}
			}
		}
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (dst.at<uchar>(i, j) == k) {
						newh[k]++;
					}
				}
			}
		}
		//imshow("src", src);
		//imshow("dst", dst);
		showHistogram("newHist", newh, 255, 300);
		showHistogram("hist", h, 255, 300);
		waitKey(0);
	}
}

void gamma() {
	Mat src;
	int h[255] = { 0 };
	int newh[255] = { 0 };
	float gamma;
	printf("Give gamma: ");
	scanf("%f", &gamma);
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				dst.at<uchar>(i, j) = (int)(255.0f * (pow(((float)src.at<uchar>(i, j) / 255.0f), gamma)));
			}
		}
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (dst.at<uchar>(i, j) == k) {
						newh[k]++;
					}
				}
			}
		}
		imshow("src", src);
		imshow("gamma", dst);
		showHistogram("newHist", newh, 255, 300);
		showHistogram("hist", h, 255, 300);
		waitKey(0);
	}
}

void brightness() {
	Mat src;
	int h[255] = { 0 };
	int newh[255] = { 0 };
	int bright, newBright;
	printf("Give brightness offset: ");
	scanf("%d", &bright);
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) {
						h[k]++;
					}
				}
			}
		}
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				newBright = src.at<uchar>(i, j) + bright;
				if (newBright < 0) {
					dst.at<uchar>(i, j) = 0;
				}
				if (newBright > 255) {
					dst.at<uchar>(i, j) = 255;
				}
				if (newBright >= 0 && newBright <= 255) {
					dst.at<uchar>(i, j) = newBright;
				}
			}
		}
		for (int k = 0; k <= 255; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (dst.at<uchar>(i, j) == k) {
						newh[k]++;
					}
				}
			}
		}
		imshow("src", src);
		imshow("bright", dst);
		showHistogram("newHist", newh, 255, 300);
		showHistogram("hist", h, 255, 300);
		waitKey(0);
	}
}

void generalFilter() {
	Mat src;
	int n = 0;
	int h[10][10];
	printf("Give kernel size: ");
	scanf("%d", &n);
	printf("\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &h[i][j]);
			printf("\n");
		}
	}
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		int di[9] = { 0,0,-1,-1,-1,0,1,1,1 };
		int dj[9] = { 0,1,1,0,-1,-1,-1,0,1 };
		int max = -10000, min = 10000;
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (i == 0 || i == src.rows || j == 0 || j == src.cols) {
					continue;
				}
				int sum = 0;
				int k = 0;
				for (int u = 0; u < 3; u++) {
					for (int w = 0; w < 3; w++) {
						sum += src.at<uchar>(i + di[k], j + dj[k]) * h[w][u];
						k++;
					}
				}
				if (sum > 255) {
					dst.at<uchar>(i, j) = 255;
				}
				if (sum < 0) {
					dst.at<uchar>(i, j) = 0;
				}
				if (sum >= 0 && sum <= 255) {
					dst.at<uchar>(i, j) = sum;
				}
			}
		}

		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

void centering_transform(Mat img) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
		}
	}
}

void fourier() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);

		Mat mag, phi;
		magnitude(channels[0], channels[1], mag);
		phase(channels[0], channels[1], phi);


		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dstf);

		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);

		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

void fourierLog() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);

		Mat mag, phi, magLog;
		magnitude(channels[0], channels[1], mag);
		phase(channels[0], channels[1], phi);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				magLog.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}

		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dstf);

		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);

		imshow("src", src);
		imshow("magLog", magLog);
		imshow("dst", dst);
		waitKey(0);
	}
}

int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

void medianFilter() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	int w;
	printf("w=");
	scanf("%d", &w);
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		int *arr = (int*)malloc(w*w * sizeof(int));
		Mat dst = Mat(height, width, CV_8UC1);
		double t = (double)getTickCount();
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i < w - 2 && j < w - 2) {
					continue;
				}
				int index = 0;
				for (int k = -w / 2; k <= w / 2; k++) {
					for (int m = -w / 2; m <= w / 2; m++) {
						arr[index] = src.at<uchar>(i + k, j + m);
						index++;
					}
				}
				std::qsort(arr, w*w, sizeof(int), compare);
				dst.at<uchar>(i, j) = arr[w*w / 2];
			}
		}
		t = ((double)getTickCount() - t) / getTickFrequency();
		printf("Time = %.3f [ms]\n", t * 1000);
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

void gaussianFilter() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	float w;
	const double pi = 3.14159265358979323846;
	printf("w=");
	scanf("%f", &w);
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		float kernel[10][10];
		float sigma;
		sigma = w / 6;
		printf("%f\n", w);
		printf("%f\n", sigma);
		double t = (double)getTickCount();
		for (int k = 0; k < w; k++) {
			for (int m = 0; m < w; m++) {
				kernel[k][m] = (1.0f / (2.0f * pi*(sigma*sigma))) * exp(-((((k-w/2)*(k-w/2)) + (m-w/2)*(m-w/2)) / (2.0f*(sigma*sigma))));
			}
		}
		for (int k = 0; k < w; k++) {
			printf("\n");
			for (int l = 0; l < w; l++) {
				printf("%f ", kernel[k][l]);
			}
		}
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i < w - 2 && j < w - 2) {
					continue;
				}
				float sum = 0;
				int o = 0, p = 0;
				for (int u = -w/2; u <= w/2; u++) {
					p = 0;
					for (int z = -w/2; z <= w/2; z++) {
						sum += src.at<uchar>(i + u, j + z) * kernel[o][p];
						p++;
					}
					o++;
				}
				if (sum > 255) {
					dst.at<uchar>(i, j) = 255;
				}
				if (sum < 0) {
					dst.at<uchar>(i, j) = 0;
				}
				if (sum >= 0 && sum <= 255) {
					dst.at<uchar>(i, j) = sum;
				}
			}
		}
		t = ((double)getTickCount() - t) / getTickFrequency();
		printf("Time = %.3f [ms]\n", t * 1000);
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

void gaussianFilter2() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	float w;
	const double pi = 3.14159265358979323846;
	printf("w=");
	scanf("%f", &w);
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		float kernelx[10], kernely[10];
		float sigma;
		sigma = w / 6;
		double t = (double)getTickCount();
		for (int k = 0; k < w; k++) {
			kernelx[k] = (1.0f / (2.0f * pi*(sigma*sigma))) * exp((-(k - w / 2)*(k - w / 2)) / (2.0f * (sigma *sigma)));
			kernely[k] = (1.0f / (2.0f * pi*(sigma*sigma))) * exp((-(k - w / 2)*(k - w / 2)) / (2.0f * (sigma *sigma)));

		}
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i < w - 2 && j < w - 2) {
					continue;
				}
				float sum = 0;
				int o = 0, p = 0;
				for (int u = -w / 2; u <= w / 2; u++) {
					p = 0;
					for (int z = -w / 2; z <= w / 2; z++) {
						sum += src.at<uchar>(i + u, j + z) * kernely[p];
						p++;
					}
					o++;
				}
				if (sum > 255) {
					dst.at<uchar>(i, j) = 255;
				}
				if (sum < 0) {
					dst.at<uchar>(i, j) = 0;
				}
				if (sum >= 0 && sum <= 255) {
					dst.at<uchar>(i, j) = sum;
				}
			}
		}
		t = ((double)getTickCount() - t) / getTickFrequency();
		printf("Time = %.3f [ms]\n", t * 1000);
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

void kernelGradient() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	int sobelx[3][3] = { -1,0,1,-2,0,2,-1,0,1 };
	int sobely[3][3] = { 1,2,1,0,0,0,-1,-2,-1 };
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat gradx = Mat(height, width, CV_32FC1);
		Mat grady = Mat(height, width, CV_32FC1);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i < 2 && j < 2) {
					continue;
				}
				float sum = 0, sum2 = 0;
				int o = 0, p = 0;
				for (int u = -1; u <= 1; u++) {
					p = 0;
					for (int z = -1; z <= 1; z++) {
						sum += src.at<uchar>(i + u, j + z) * sobelx[o][p];
						sum2 += src.at<uchar>(i + u, j + z) * sobely[o][p];
						p++;
					}
					o++;
				}
				gradx.at<float>(i,j) = sum;
				grady.at<float>(i,j) = sum2;
			}
		}
		imshow("gradx", gradx);
		imshow("grady", grady);
		waitKey(0);
	}
}

Mat gradientMagnitude() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	int sobelx[3][3] = { -1,0,1,-2,0,2,-1,0,1 };
	int sobely[3][3] = { 1,2,1,0,0,0,-1,-2,-1 };
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_8UC1);
		int height = src.rows;
		int width = src.cols;
		Mat gradx = Mat(height, width, CV_32FC1);
		Mat grady = Mat(height, width, CV_32FC1);
		Mat magnitude = Mat(height, width, CV_8UC1);
		Mat magnitudeMax = Mat(height, width, CV_8UC1);
		Mat magnitudeTh = Mat(height, width, CV_8UC1);
		Mat magnitudeTh2 = Mat(height, width, CV_8UC1);
		Mat direction = Mat(height, width, CV_32FC1);
		Mat quantized = Mat(height, width, CV_8UC1);
		for (int i = 1; i < height - 1; i++) {
			for (int j = 1; j < width - 1; j++) {
				if (i < 2 && j < 2) {
					continue;
				}
				float sum = 0, sum2 = 0;
				int o = 0, p = 0;
				for (int u = -1; u <= 1; u++) {
					p = 0;
					for (int z = -1; z <= 1; z++) {
						sum += src.at<uchar>(i + u, j + z) * sobelx[o][p];
						sum2 += src.at<uchar>(i + u, j + z) * sobely[o][p];
						p++;
					}
					o++;
				}
				gradx.at<float>(i, j) = sum;
				grady.at<float>(i, j) = sum2;
				magnitude.at<uchar>(i, j) = sqrt((gradx.at<float>(i,j)*gradx.at<float>(i, j))+(grady.at<float>(i, j)*grady.at<float>(i, j))) / (float)(4.0f * sqrt(2.0f));
				direction.at<float>(i, j) = atan2(grady.at<float>(i, j), gradx.at<float>(i, j));
				if ((direction.at<float>(i, j) > (-PI / 8) && direction.at<float>(i, j) <= (PI / 8)) || (direction.at<float>(i, j) >= (7 * PI / 8) || direction.at<float>(i, j) <= (-7 * PI / 8))) {
					quantized.at<uchar>(i, j) = 2;
				}
				if ((direction.at<float>(i, j) > (PI / 8) && direction.at<float>(i, j) <= (3 * PI / 8)) || (direction.at<float>(i, j) <= (-5 * PI / 8) && direction.at<float>(i, j) >= (-7 * PI / 8))) {
					quantized.at<uchar>(i, j) = 1;
				}
				if ((direction.at<float>(i, j) > (3 * PI / 8) && direction.at<float>(i, j) <= (5 * PI / 8)) || (direction.at<float>(i, j) <= (-3 * PI / 8) && direction.at<float>(i, j) >= (-5 * PI / 8))) {
					quantized.at<uchar>(i, j) = 0;
				}
				if ((direction.at<float>(i, j) > (5 * PI / 8) && direction.at<float>(i, j) <= (7 * PI / 8)) || (direction.at<float>(i, j) <= (PI / 8) && direction.at<float>(i, j) >= (-3 * PI / 8))) {
					quantized.at<uchar>(i, j) = 3;
				}


				if (quantized.at<uchar>(i, j) == 0) {
					if ((magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i-1, j)) && (magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i+1, j))) {
						magnitudeMax.at<uchar>(i, j) = magnitude.at<uchar>(i, j);
					}
					else {
						magnitudeMax.at<uchar>(i, j) = 0;
					}
				}
				if (quantized.at<uchar>(i, j) == 1) {
					if ((magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i - 1, j+1)) && (magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i + 1, j-1))) {
						magnitudeMax.at<uchar>(i, j) = magnitude.at<uchar>(i, j);
					}
					else {
						magnitudeMax.at<uchar>(i, j) = 0;
					}
				}
				if (quantized.at<uchar>(i, j) == 2) {
					if ((magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i, j-1)) && (magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i, j+1))) {
						magnitudeMax.at<uchar>(i, j) = magnitude.at<uchar>(i, j);
					}
					else {
						magnitudeMax.at<uchar>(i, j) = 0;
					}
				}
				if (quantized.at<uchar>(i, j) == 3) {
					if ((magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i - 1, j-1)) && (magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i + 1, j+1))) {
						magnitudeMax.at<uchar>(i, j) = magnitude.at<uchar>(i, j);
					}
					else {
						magnitudeMax.at<uchar>(i, j) = 0;
					}
				}

				if (magnitudeMax.at<uchar>(i, j) < 15) {
					magnitudeTh.at<uchar>(i, j) = 0;
				}
				if (magnitudeMax.at<uchar>(i, j) >= 15) {
					magnitudeTh.at<uchar>(i, j) = 255;
				}

				if (magnitude.at<uchar>(i, j) < 15) {
					magnitudeTh2.at<uchar>(i, j) = 0;
				}
				if (magnitude.at<uchar>(i, j) >= 15) {
					magnitudeTh2.at<uchar>(i, j) = 255;
				}

			}
		}
		//printf("%f", magnitude.at<float>(100, 100));
		//imshow("gradx", gradx);
		//imshow("grady", grady);
		//imshow("mag", magnitude);
		//imshow("magMax", magnitudeMax);
		//imshow("magTh", magnitudeTh);
		//imshow("magTh2", magnitudeTh2);
		return magnitudeMax;
		//imshow("dir", direction);
		imshow("initial", src);

		waitKey(0);
	}
}

void adaptive() {
	Mat src;
	int h[255] = { 0 };
	// Read image from file 
	Mat mag = gradientMagnitude();

	int height = mag.rows;
	int width = mag.cols;

	for (int i = 1; i < mag.rows - 1; i++) {
		for (int j = 1; j < mag.cols - 1; j++) {
				h[mag.at<uchar>(i,j)]++;
		}
	}
	for (int k = 0; k < 255; k++) {
		printf("%d %d\n", k, h[k]);
	}

	float noNonEdge = 0.9f * (height * width - (float)h[0]);
	printf("%f\n", noNonEdge);

	int s = 0;
	int thresh = 0;
	for (int i = 1; i < 255; i++) {
		s += h[i];
		thresh = i;
		if (s > noNonEdge)
			break;
	}

	int th = thresh;
	int tl = 0.4 * th;

	printf("%d %d\n", tl, th);

	Mat res = Mat(height, width, CV_8UC1);

	for (int i = 1; i < mag.rows - 1; i++) {
		for (int j = 1; j < mag.cols - 1; j++) {
			if (mag.at<uchar>(i, j) > th) {
				res.at<uchar>(i, j) = 255;
			}
			if (mag.at<uchar>(i, j) <= th && mag.at<uchar>(i, j) > tl) {
				res.at<uchar>(i, j) = 128;
			}
			if (mag.at<uchar>(i, j) <= tl) {
				res.at<uchar>(i, j) = 0;
			}
		}
	}

	Mat res2 = Mat::zeros(res.rows, res.cols, CV_8UC1);

	for (int i = 1; i < res.rows - 1; i++) {
		for (int j = 1; j < res.cols - 1; j++) {
			if (res.at<uchar>(i, j) == 255 && res2.at<uchar>(i, j) == 0) {

				res2.at<uchar>(i, j) = 255;
				std::queue < Point2i > Q;
				Q.push(Point2i(i, j));

				while (!Q.empty()) {

					Point2i q = Q.front();
					Q.pop();

					int di[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
					int dj[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

					for (int k = 0; k < 8; k++) {
						if (q.x + di[k] < 0 || q.y + dj[k] < 0 || q.x + di[k] > res.rows - 1 || q.y + dj[k] > res.cols - 1) {
							continue;
						}
						if ((res.at<uchar>(q.x + di[k], q.y + dj[k]) == 255 && res2.at<uchar>(q.x + di[k], q.y + dj[k]) == 0) || (res.at<uchar>(q.x + di[k], q.y + dj[k]) == 128 && res2.at<uchar>(q.x + di[k], q.y + dj[k]) == 0)) {
							res2.at<uchar>(q.x + di[k], q.y + dj[k]) = 255;
							Q.push(Point2i(q.x + di[k], q.y + dj[k]));
						}
					}
				}
			}
		}
	}

	for (int i = 1; i < res2.rows - 1; i++) {
		for (int j = 1; j < res2.cols - 1; j++) {
			if (res2.at<uchar>(i, j) == 128) {
				res2.at<uchar>(i, j) = 0;
			}
		}
	}
	printf("%d ", thresh);
	imshow("mag", mag);
	imshow("res", res);
	imshow("final", res2);
	waitKey(0);
}

int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf(" 10 - test Additive\n");
		printf(" 11 - test Multiplicative\n");
		printf(" 12 - Create Color Image\n");
		printf(" 13 - Matrix Inverse\n");
		printf(" 14 - Line to Sin\n");
		printf(" 15 - RGB Matrix\n");
		printf(" 16 - RGB to Gray\n");
		printf(" 17 - Gray to Binary\n");
		printf(" 18 - RGB to HSV\n");
		printf(" 19 - HSV to Binary\n");
		printf(" 20 - On Mouse\n");
		printf(" 21 - BFS Labelling\n");
		printf(" 22 - BFS Color Labelling\n");
		printf(" 23 - Border Tracing\n");
		printf(" 24 - Dilation\n");
		printf(" 25 - Erosion\n");
		printf(" 26 - Opening\n");
		printf(" 27 - Closing\n");
		printf(" 28 - Boundary Extraction\n");
		printf(" 29 - N times Dilation\n");
		printf(" 30 - Region Filling\n");
		printf(" 31 - Compute Histogram\n");
		printf(" 32 - Compute Mean Intensity\n");
		printf(" 33 - Compute Standard Deviation\n");
		printf(" 34 - Compute Cumulative Histogram\n");
		printf(" 35 - Global Thresholding\n");
		printf(" 36 - Image Negative\n");
		printf(" 37 - Stretch/Shrink Histogram\n");
		printf(" 38 - Gamma Correction\n");
		printf(" 39 - Brightness Change\n");
		printf(" 40 - Apply general filter\n");
		printf(" 41 - Fourier\n");
		printf(" 42 - Log Fourier\n");
		printf(" 43 - Apply Median Filter\n");
		printf(" 44 - Apply Gaussian Filter\n");
		printf(" 45 - Apply Gaussian Filter 2\n");
		printf(" 46 - Compute kernel gradient\n");
		printf(" 47 - Compute gradient magnitude\n");
		printf(" 48 - Detect edges\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testParcurgereSimplaDiblookStyle(); //diblook style
				break;
			case 4:
				//testColor2Gray();
				testBGR2HSV();
				break;
			case 5:
				testResize();
				break;
			case 6:
				testCanny();
				break;
			case 7:
				testVideoSequence();
				break;
			case 8:
				testSnap();
				break;
			case 9:
				testMouseClick();
				break;
			case 10:
				testAdditive();
				break;
			case 11:
				testMultiplicative();
				break;
			case 12:
				createColorImage();
				break;
			case 13:
				matrixInverse();
				break;
			case 14:
				line2sin();
				break;
			case 15:
				rgb3mat();
				break;
			case 16:
				rgb2gray();
				break;
			case 17:
				gray2binary();
				break;
			case 18:
				rgb2hsv();
				break;
			case 19:
				hsv2binary();
				break;
			case 20:
				onMouse();
				break;
			case 21:
				bfsLabel();
				break;
			case 22:
				bfsLabelColors();
				break;
			case 23:
				borderTracing();
				break;
			case 24:
				dilation();
				break;
			case 25:
				erosion();
				break;
			case 26:
				opening();
				break;
			case 27:
				closing();
				break;
			case 28:
				boundaryExtraction();
				break;
			case 29:
				int n;
				printf("n=");
				scanf("%d", &n);
				ndilation(n);
				break;
			case 30:
				regionfilling();
				break;
			case 31:
				computeHistogram();
				break;
			case 32:
				meanIntensity();
				break;
			case 33:
				stdDeviation();
				break;
			case 34:
				computeCumulativeHistogram();
				break;
			case 35:
				globalThresholding();
				break;
			case 36:
				negativeFunc();
				break;
			case 37:
				histogramS();
				break;
			case 38:
				gamma();
				break;
			case 39:
				brightness();
				break;
			case 40:
				generalFilter();
				break;
			case 41:
				fourier();
				break;
			case 42:
				fourierLog();
				break;
			case 43:
				medianFilter();
				break;
			case 44:
				gaussianFilter();
				break;
			case 45:
				gaussianFilter2();
				break;
			case 46:
				kernelGradient();
				break;
			case 47:
				gradientMagnitude();
				break;
			case 48:
				adaptive();
				break;
		}
	}
	while (op!=0);
	return 0;
}