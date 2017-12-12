/***********************************************************************
@Author: Long Gu
@Date:   2017/11/20
***********************************************************************/
#include<io.h>
#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "Common.h"
#include "SysData.h"
#include "Test.h"

using namespace std;
using namespace cv;

void test_all_functions()
{
	/*
	Mat back_img = imread("E:\\TestImages\\sys\\back\\back1.jpg");
	Mat fore_img = imread("E:\\TestImages\\sys\\fore\\0002.jpg");
	
	test_get_int_range_rand();
	test_get_double_range_rand();
	test_get_position(back_img,fore_img,0);
	test_img_fusion(back_img, fore_img);
	test_fusion(back_img, fore_img, 0, 0);
	test_preprocess(fore_img, 250);
	medianBlur(fore_img, fore_img, 5);
	int struct_size = 1;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * struct_size + 1, 2 * struct_size + 1));
	//erode(fore_img, fore_img, element);
	imwrite("E:\\TestImage\\SysData\\00031_2.jpg", fore_img);
	test_img_warp(fore_img);
	test_img_perspect(fore_img);
	test_img_sature(fore_img);
	test_add_salt_noise(fore_img);
	test_generate_gaussian_noise();
	test_add_gaussian_noise(fore_img);*/

	//second part common
	/*test_get_all_file_name(string("E:\\TestImage\\OpenCVTest"), 29);
	test_write_txt(string("E:\\TestImage\\SysData\\result.txt"));
	test_write_xml(string("E:\\TestImage\\SysData\\result.xml"));
	test_write_label_file(string("E:\\TestImage\\SysData\\label.txt"));
	test_split();
	test_get_file_name_and_label();*/
	//test_write_xml_jpg_label(string("E:\\TestImage\\SynData\\label.txt"), string("E:\\TestImage\\SynData\\xml_path"), string("E:\\TestImage\\SynData\\jpg_path"));
}

void show_synthesis_result()
{
	Mat back_img = imread("E:\\TestImages\\sys\\back\\back1.jpg");
	Mat fore_img = imread("E:\\TestImages\\sys\\fore\\0001.jpg");
	vector<int> position = fusion(back_img, fore_img, 5, 2);
	printf("x1: %d, y1: %d, x2: %d, y2: %d", position[0], position[1], position[2], position[3]);
	//GaussianBlur(back_img, back_img, Size(3, 3), 0, 0);
	namedWindow("result");
	imshow("result", back_img);
	waitKey(0);
	destroyAllWindows();
}
void synthesis_data()
{
	string label_file = "E:\\TestImage\\SynData\\label.txt";
	string xml_path = "E:\\TestImage\\SynData\\xml_path";
	string jpg_path = "E:\\TestImage\\SynData\\jpg_path";
	string background_path = "E:\\TestImage\\back_fore_img\\back_img";
	string foreground_path = "E:\\TestImage\\back_fore_img\\fore_img";
	int border = 5;
	int thresh = 3;
	vector<string> background_list, foreground_list;
	get_all_file_name(background_path, background_list);
	get_all_file_name(foreground_path, foreground_list);
	printf("size of background_list: %d\n", background_list.size());
	printf("size of foreround_list: %d\n", foreground_list.size());
	write_xml_jpg_label(label_file, xml_path, jpg_path, background_list, foreground_list, border, thresh);
}

int main(int argc, char** argv)
{
	//Recommend to use opencv3.0

	//test_all_functions();
	//show_synthesis_result();
	synthesis_data();

	system("pause");
	return 0;
}