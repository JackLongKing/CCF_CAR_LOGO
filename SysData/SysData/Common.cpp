/***********************************************************************
@Author: Long Gu
@Date:   2017/11/20
***********************************************************************/
#include<io.h>
#include<fstream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "SysData.h"
#include "Common.h"


void get_all_file_name(std::string& file_path, std::vector<std::string>& all_files)
{
	//file handle
	//win10出现的问题：因为_findnext()返回类型为intptr_t而非long型，从“intptr_t”转换到“long”丢失了数据。
	//long hFile=0;
	intptr_t hFile = 0;
	//file information
	struct _finddata_t fileInfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(file_path).append("\\*").c_str(), &fileInfo)) != -1)
	{
		do 
		{
			//if directory, iter; otherwise, add it to list
			if ((fileInfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
					get_all_file_name(p.assign(file_path).append("\\").append(fileInfo.name), all_files);
			}
			else
				all_files.push_back(p.assign(file_path).append("\\").append(fileInfo.name));
		} while (_findnext(hFile,&fileInfo) == 0);
		_findclose(hFile);
	}
}

bool write_txt(std::string& txt_file, std::vector<std::string>& all_files)
{
	std::ofstream outFile;
	outFile.open(txt_file, std::ios::trunc);
	long length = all_files.size();
	if (length == 0)
	{
		outFile.close();
		printf("The length of file vector is 0, please check the directory!\n");
		return false;
	}
	for (long i = 0; i < length-1; ++i)
	{
		outFile << all_files[i] << "\n";
	}
	outFile << all_files[length - 1];

	outFile.close();
	return true;
}

bool write_xml(std::string& xml_file,std::string& jpg_file,std::string& img_id, 
	           int img_width,int img_height,std::vector<int>& position)
{
	if (xml_file.size() == 0 || jpg_file.size() == 0 || img_id.size() == 0
		|| img_width <= 0 || img_height <= 0 || position.size() != 4)
		return false;
	//first part
	std::ofstream outFile;
	outFile.open(xml_file, std::ios::trunc);
	outFile << "<annotation>\n";
	outFile << "	<folder>VOC2012</folder>\n";
	outFile << "	<filename>" << jpg_file << "</filename>\n";
	outFile << "	<size>\n";
	char temp_width[10],temp_height[10];
	_itoa(img_width, temp_width, 10);
	_itoa(img_height, temp_height, 10);
	outFile << "		<width>" << temp_width << "</width>\n";
	outFile << "		<height>" << temp_height << "</height>\n";
	outFile << "		<depth>3</depth>\n";
	outFile << "	</size>\n";
	//second part
	outFile << "	<object>\n";
	outFile << "		<name>" << img_id << "</name>\n";
	outFile << "		<pose>Unspecified</pose>\n";
	outFile << "		<truncated>0</truncated>\n";
	outFile << "		<difficult>0</difficult>\n";
	outFile << "		<bndbox>\n";
	char temp_xmin[10], temp_ymin[10], temp_xmax[10], temp_ymax[10];
	_itoa(position[0], temp_xmin,10);
	_itoa(position[1], temp_ymin, 10);
	_itoa(position[2], temp_xmax, 10);
	_itoa(position[3], temp_ymax, 10);
	outFile << "			<xmin>" << temp_xmin << "</xmin>\n";
	outFile << "			<ymin>" << temp_ymin << "</ymin>\n";
	outFile << "			<xmax>" << temp_xmax << "</xmax>\n";
	outFile << "			<ymax>" << temp_ymax << "</ymax>\n";
	outFile << "		</bndbox>\n";
	outFile << "	</object>\n";
	outFile << "</annotation>";

	outFile.close();
	return true;
}

bool write_label_file(std::string& label_file, std::string& jpg_file, std::string& xml_file, int& count)
{
	if (!jpg_file.size() || !xml_file.size())
		return false;
	
	std::ofstream outFile;
	if (!count)
		outFile.open(label_file, std::ios::trunc);
	else
		outFile.open(label_file, std::ios::app);

	outFile << jpg_file << "    " << xml_file << "\n";
	outFile.close();
	return true;
}

std::vector<std::string> split(const std::string& str, const std::string& pattern)
{
	//convert const char* to char*
	char* str_c = new char[strlen(str.c_str()) + 1];
	strcpy(str_c, str.c_str());
	std::vector<std::string> result;
	char* tempStr = strtok(str_c, pattern.c_str());
	while (tempStr != NULL)
	{
		result.push_back(std::string(tempStr));
		tempStr = strtok(NULL, pattern.c_str());
	}
	delete[] str_c;

	return result;
}

std::vector<std::string> get_file_name_and_label(std::string& back_img, std::string& fore_img)
{
	if (!back_img.size() || !fore_img.size())
		return std::vector<std::string>();

	std::vector<std::string> temp_back = split(back_img, "\\");
	std::vector<std::string> temp_fore = split(fore_img, "\\");

	if (!temp_back.size() || !temp_fore.size())
		return std::vector<std::string>();

	std::string back = temp_back[temp_back.size() - 1];
	std::string fore = temp_fore[temp_fore.size() - 1];

	temp_back.clear();
	temp_fore.clear();

	temp_back = split(back, ".");
	temp_fore = split(fore, ".");

	if (!temp_back.size() || !temp_fore.size())
		return std::vector<std::string>();

	back = temp_back[0];
	fore = temp_fore[0];
	std::string label = fore.substr(0,4);
	std::string img_format = temp_fore[temp_fore.size() - 1];
	std::string back_fore_jpg = back + "_" + fore + "." + img_format;
	std::string back_fore_xml = back + "_" + fore + ".xml";
	
	std::vector<std::string> result;
	result.push_back(back_fore_jpg);
	result.push_back(back_fore_xml);
	result.push_back(label);

	return result;
}

bool write_xml_jpg_label(std::string& label_file, std::string& xml_path, std::string& jpg_path, std::vector<std::string>& background_list,
	           std::vector<std::string>& fore_list,int border,int thresh)
{
	int count = 0;
	long back_length = background_list.size();
	long fore_length = fore_list.size();
	if (back_length == 0 || fore_length == 0)
	{
		printf("The length of file vector is 0, please check the directory!\n");
		return false;
	}
	for (long i = 0; i < back_length; ++i)
	{
		for (long j = 0; j < fore_length; ++j)
		{
			cv::Mat back_img = cv::imread(background_list[i],1);
			cv::Mat fore_img = cv::imread(fore_list[j]);
			if (!back_img.data || !fore_img.data)
			{
				printf("Error: fore_img or back_img failed to open, check again!\n");
				return false;
			}
			medianBlur(fore_img, fore_img, 7);
			int back_img_width = back_img.cols;
			int back_img_height = back_img.rows;
			
			cv::Mat dst;
			int trans_option = get_int_range_rand(0, 2);
			if (trans_option % 2 == 0)
				img_warp(fore_img, dst);
			else
				img_perspect(fore_img, dst);
			
			std::vector<int> position = fusion(back_img, dst, border, thresh);

			int post_process_option = get_int_range_rand(0, 3);
			if (post_process_option % 3 == 0)
				add_salt_noise(back_img);
			else if (post_process_option % 3 == 1)
				add_gaussian_noise(back_img);
			else
				img_sature(back_img);

			std::vector<std::string> file_name_and_label = get_file_name_and_label(background_list[i], fore_list[j]);

			if (!position.size() || !file_name_and_label.size())
			{
				printf("Failed to synthesis data, empty position or empty file name and label!\n");
				printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
				return false;
			}
			std::string jpg_file_no_path = file_name_and_label[0];
			std::string jpg_file = jpg_path + "\\" + file_name_and_label[0];
			std::string xml_file = xml_path + "\\" + file_name_and_label[1];
			std::string label = file_name_and_label[2];
			write_xml(xml_file, jpg_file_no_path, label, back_img_width, back_img_height, position);
			//opencv3.2
			/*std::vector<int> compression_params;
			compression_params.push_back(cv::IMWRITE_JPEG_OPTIMIZE);
			compression_params.push_back(3);*/
			/*cv::imshow(jpg_file, back_img);
			cv::waitKey(0);*/
			cv::imwrite(jpg_file, back_img);
			write_label_file(label_file, jpg_file, xml_file, count);
			++count;
			position.clear();
		}
	}

	return true;
}

