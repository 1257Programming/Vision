/*
 * vision.cpp
 *
 *  Created on: May 25, 2017
 *      Author: Aron Richman
 */
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

void grabFrame(Mat& outputMat)
{
//	TODO
	return;
}
void toBinary(Mat& input, Mat& output)
{
//	????
	cvtColor(input, output, CV_BGR2HSV);
	//TODO: Convert to binary and find contours

}

int main()
{
//	initialize variables
	Mat inputImage;
	Mat outputImage;
	grabFrame(inputImage);
	toBinary(inputImage, outputImage);
}
