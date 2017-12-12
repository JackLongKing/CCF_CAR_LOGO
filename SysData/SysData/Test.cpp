/***********************************************************************
@Author: Long Gu
@Date:   2017/11/20
***********************************************************************/
#include "Test.h"
#include "Common.h"

void test_get_int_range_rand()
{
	int res = get_int_range_rand(0, 10);
	printf("random int: %d\n", res);
	/*if (res >= 0 && res < 10)
		printf("Test get_int_range_rand() successfully!\n");
	else
		printf("Failed to test get_int_range_rand()!\n");*/
}

void test_get_double_range_rand()
{
	double res = get_double_range_rand(3, 8, 10);
	printf("random double: %f\n", res);
	/*if (res >= 0.2990000f && res < 0.8000000f)
		printf("Test get_double_range_rand() successfully!\n");
		else
		printf("Failed to test get_double_range_rand()!\n");*/
}

void test_get_position(cv::Mat& back_img, cv::Mat& fore_img, int border)
{
	std::vector<int> res = get_position(back_img, fore_img, border);
	if (res.size()==4)
		printf("Test get_start_position() successfully!\n");
	else
		printf("Failed to test get_start_position()!\n");
}

void test_img_fusion(cv::Mat& back_img, cv::Mat& fore_img)
{
	if (!back_img.data || !fore_img.data) return;

	if (back_img.rows > fore_img.rows || back_img.cols > fore_img.cols)
	{
		int back_start_x = get_int_range_rand(0, back_img.cols - fore_img.cols);
		int back_start_y = get_int_range_rand(0, back_img.rows - fore_img.rows);
		bool res = img_fusion(back_img, fore_img, back_start_x, back_start_y, 0);
		if (res)
			printf("Test img_fusion() successfully!\n");
		else
			printf("Failed to test img_fusion()!\n");
	}
	else
		printf("Failed to test img_fusion(), which caused by diff(back_img.rows-fore_img.rows,back_img.cols-fore_img.cols)\n \
			   		      is negative. As background, size of back_img should be greater than size of fore_img. Please check params!\n");
}

void test_fusion(cv::Mat& back_img, cv::Mat& fore_img, int border, int thresh)
{
	if (!back_img.data || !fore_img.data)
		return;
	
	std::vector<int> position = fusion(back_img, fore_img, border, thresh);
	if (position.size()==4)
		printf("Test fusion() successfully!\n");
	else
		printf("Failed to test fusion()!\n");
}

void test_preprocess(cv::Mat& img, int high_thresh)
{
	if (!img.data || high_thresh > 255)
		return;
	bool result = preprocess(img, high_thresh);
	if (result)
		printf("Test preprocess() successfully!\n");
	else
		printf("Failed to test preprocess()!\n");
}

void test_img_warp(cv::Mat& fore_img)
{
	if (!fore_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return;
	}
	cv::Mat dst;
	bool result = img_warp(fore_img,dst);
	if (result && dst.data)
		printf("Test img_warp() successfully!\n");
	else
		printf("Failed to test img_warp()!\n");
}

void test_img_perspect(cv::Mat& fore_img)
{
	if (!fore_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return;
	}
	cv::Mat dst;
	bool result = img_perspect(fore_img, dst);
	if (result && dst.data)
		printf("Test img_perspect() successfully!\n");
	else
		printf("Failed to test img_perspect()!\n");
}

void test_img_sature(cv::Mat& sys_img)
{
	if (!sys_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return;
	}
	bool result = img_sature(sys_img);
	if (result)
		printf("Test img_sature() successfully!\n");
	else
		printf("Failed to test img_sature()!\n");
}

void test_add_salt_noise(cv::Mat& sys_img)
{
	if (!sys_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return;
	}
	bool result = add_salt_noise(sys_img);
	if (result)
		printf("Test add_salt_noise() successfully!\n");
	else
		printf("Failed to test add_salt_noise()!\n");
}

void test_generate_gaussian_noise()
{
	double mu = get_double_range_rand(0, 5, 1);
	double sigma = get_double_range_rand(0, 10, 10);
	double result = generate_gaussian_noise(mu, sigma);
	if (result)
		printf("Test generate_gaussian_noise() successfully!\n");
	else
		printf("Failed to test generate_gaussian_noise()!\n");
}

void test_add_gaussian_noise(cv::Mat& sys_img)
{
	if (!sys_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return;
	}
	bool result = add_gaussian_noise(sys_img);
	if (result)
		printf("Test add_gaussian_noise() successfully!\n");
	else
		printf("Failed to test add_gaussian_noise()!\n");
}

void test_get_all_file_name(std::string& file_path,int num_file)
{
	if (!file_path.size())
	{
		printf("The length of txt_path is zero, please check file_path again!\n");
		return;
	}

	std::vector<std::string> all_file_names;
	get_all_file_name(file_path, all_file_names);
	if (all_file_names.size() == num_file)
		printf("Test get_all_file_name() successfully!\n");
	else
		printf("Failed to test get_all_file_name()!\n");
}

void test_write_txt(std::string& txt_file)
{
	if (!txt_file.size())
	{
		printf("The length of txt_path is zero, please check txt_path again!\n");
		return;
	}

	std::vector<std::string> all_file_names;
	all_file_names.push_back("test1.jpg");
	all_file_names.push_back("test2.xml");
	bool result = write_txt(txt_file, all_file_names);
	if (result)
		printf("Test write_txt() successfully!\n");
	else
		printf("Failed to test write_txt()!\n");
}

void test_write_xml(std::string& xml_file)
{
	if (!xml_file.size())
	{
		printf("The length of xml_file is zero, please check xml_file again!\n");
		return;
	}
	
	std::string jpg_file = "test_0001.jpg";
	std::string img_id = "0001";
	int img_width = 500;
	int img_height = 500;
	std::vector<int> position;
	position.push_back(5);
	position.push_back(10);
	position.push_back(100);
	position.push_back(200);

	bool result = write_xml(xml_file, jpg_file, img_id, img_width, img_height, position);
	if (result)
		printf("Test write_xml() successfully!\n");
	else
		printf("Failed to test write_xml()!\n");
}

void test_write_label_file(std::string& label_file)
{
	if (!label_file.size())
	{
		printf("The length of label_file is zero, please check label_file again!\n");
		return;
	}

	std::string jpg_file = "E:\\TestImages\\SysData\\img\\img_1.jpg";
	std::string xml_file = "E:\\TestImages\\SysData\\ann\\img_1.xml";
	int count = 0;

	bool result1 = write_label_file(label_file, jpg_file, xml_file, count);
	bool result2 = write_label_file(label_file, jpg_file, xml_file, count);
	if (result1 && result2)
		printf("Test write_label_file() successfully!\n");
	else
		printf("Failed to test write_label_file()!\n");
}

void test_split()
{
	std::string str = "E:\\TestImages\\SysData\\img\\img_1.jpg";
	std::vector<std::string> result;
	result = split(str, "\\");
	if (result.size() == 5)
	{
		if (result[0] == "E:" && result[1] == "TestImages" && result[2] == "SysData" && result[3] == "img" && result[4] == "img_1.jpg")
			printf("Test split() successfully!\n");
		else
			printf("Failed to test split()!\n");
	}
	else
		printf("Failed to test split()!\n");
}

void test_get_file_name_and_label()
{
	std::string back_img = "E:\\TestImages\\SysData\\back\\TVShow.jpg";
	std::string fore_img = "E:\\TestImages\\SysData\\fore\\0001.jpg";
	std::vector<std::string> result;
	result = get_file_name_and_label(back_img, fore_img);
	if (result.size() == 3)
	{
		if (result[0] == "TVShow_0001.jpg" && result[1] == "TVShow_0001.xml" && result[2] == "0001")
			printf("Test get_file_name_and_label() successfully!\n");
		else
			printf("Failed to test get_file_name_and_label()!\n");
	}
	else
		printf("Failed to test get_file_name_and_label()!\n");
}

void test_write_xml_jpg_label(std::string& label_file, std::string& xml_path, std::string& jpg_path)
{
	if (!label_file.size() || !xml_path.size() || !jpg_path.size())
	{
		printf("The length of one param is zero, please check all params again!\n");
		return;
	}

	std::vector<std::string> back_list, fore_list;
	//ensure all images exist in right path, E:\\TestImage\\SysData
	back_list.push_back("E:\\TestImage\\SysData\\Background1.jpg");
	back_list.push_back("E:\\TestImage\\SysData\\Background2.jpg");
	fore_list.push_back("E:\\TestImage\\SysData\\0001.jpg");
	fore_list.push_back("E:\\TestImage\\SysData\\0002.jpg");
	fore_list.push_back("E:\\TestImage\\SysData\\0003.jpg");
	fore_list.push_back("E:\\TestImage\\SysData\\0028.jpg");

	int border = 5;
	int thresh = 2;

	bool result = write_xml_jpg_label(label_file, xml_path, jpg_path, back_list, fore_list, border, thresh);
	if (result)
		printf("Test write_xml_jpg_label() successfully!\n");
	else
		printf("Failed to test write_xml_jpg_label()!\n");
}