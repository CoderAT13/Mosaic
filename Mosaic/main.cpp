//
//  main.cpp
//  Mosaic
//
//  Created by 张涵健 on 2019/3/2.
//  Copyright © 2019 张涵健Coder.@. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#define PIX_SIZE 50
#define RATE 0.618
#define OUTPUT_WIDTH 5000

using namespace std;
using namespace cv;

int main()
{
    int index = 0;
    for (int i = 0; i < 400; i++){
        Mat a = imread(to_string(i)+".jpg");
        if (a.empty() || (a.cols-a.rows)/a.rows > 1.5) continue;
        resize(a, a, Size(PIX_SIZE,PIX_SIZE));
        imwrite(to_string(index++)+".jpg", a);
    }

    Mat input = imread("test.jpg");
    resize(input, input, Size(OUTPUT_WIDTH,OUTPUT_WIDTH*input.rows/input.cols));
    Mat output;

    //原图rows、cols
    int rows = input.rows;
    int cols = input.cols;
    //将原图rows、cols除以50
    int ColRows = (int)rows/PIX_SIZE;
    int ColCols = (int)cols/PIX_SIZE;
    //是否扩展
    ColRows = (ColRows*PIX_SIZE < rows)?ColRows+1:ColRows;
    ColCols = (ColCols*PIX_SIZE < cols)?ColCols+1:ColCols;
    output = Mat(ColRows*PIX_SIZE, ColCols*PIX_SIZE, CV_8UC3, Scalar(255, 255, 255));

    string filename;
    char tmpname[20];
    int colorN = 0;
    Mat picColor;
    Mat tmpS;
    for(int i = 0; i < ColRows; ++i)
    {
        for(int j = 0; j < ColCols; ++j)
        {
            filename = "";
            //素材照片
            colorN = (colorN+1)%index+1;
            sprintf(tmpname, "%d.jpg", colorN);
            filename += tmpname;
            picColor = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
            //将素材照片缩放到50*50
            //resize(picColor, picColor, Size(50, 50));
            picColor.convertTo(picColor, CV_8UC3);
            tmpS = Mat(output, Rect(j*PIX_SIZE, i*PIX_SIZE, PIX_SIZE, PIX_SIZE));
            picColor.copyTo(tmpS);
        }
    }

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            //像素RGB值修改
            output.at<Vec3b>(i, j)[0] = (1-RATE)*output.at<Vec3b>(i, j)[0]+RATE*input.at<Vec3b>(i, j)[0];
            output.at<Vec3b>(i, j)[1] = (1-RATE)*output.at<Vec3b>(i, j)[1]+RATE*input.at<Vec3b>(i, j)[1];
            output.at<Vec3b>(i, j)[2] = (1-RATE)*output.at<Vec3b>(i, j)[2]+RATE*input.at<Vec3b>(i, j)[2];
        }
    }
    //输出照片保存
    output = Mat(output, Rect(0, 0, cols, rows));
    imwrite(to_string(RATE)+"pic100.jpg", output);
    output.release();
    return 0;
}
