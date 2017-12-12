/***********************************************************************
@Author: Long Gu
@Date:   2017/11/20
***********************************************************************/
#ifndef SYS_TEST_H_
#define SYS_TEST_H_

#include "SysData.h"

//test sysdata
void test_get_int_range_rand();

void test_get_double_range_rand();

void test_get_position(cv::Mat& back_img, cv::Mat& fore_img, int border);

void test_img_fusion(cv::Mat& back_img, cv::Mat& fore_img);

void test_fusion(cv::Mat& back_img, cv::Mat& fore_img, int border, int thresh);

void test_preprocess(cv::Mat& img, int high_thresh);

void test_img_warp(cv::Mat& fore_img);

void test_img_perspect(cv::Mat& fore_img);

void test_img_sature(cv::Mat& sys_img);

void test_add_salt_noise(cv::Mat& sys_img);

void test_generate_gaussian_noise();

void test_add_gaussian_noise(cv::Mat& sys_img);


//test common
void test_get_all_file_name(std::string& file_path,int num_file);

void test_write_txt(std::string& txt_file);

void test_write_xml(std::string& xml_file);

void test_write_label_file(std::string& label_file);

void test_split();

void test_get_file_name_and_label();

void test_write_xml_jpg_label(std::string& label_file, std::string& xml_path, std::string& jpg_path);

#endif