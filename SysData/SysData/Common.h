/***********************************************************************
@Author: Long Gu
@Date:   2017/11/20
***********************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_
#include<string>
#include<vector>

/*
 @params
     file_path: the path where files locate
     all_files: all file name of the path
 @result
     all file names of file_path
 @function
     get all file names of file_path
*/
void get_all_file_name(std::string& file_path, std::vector<std::string>& all_files);

/*
 @params
     txt_file: text file with path
     all_files: all file name of target path given by get_all_file_name()
 @result
     write all file names in text file
 @function
     write all file names 
*/
bool write_txt(std::string& txt_file,std::vector<std::string>& all_files);

/*
 @params
     xml_file: the xml file with path 
     jpg_file: the jpg file with path
	 img_id: the label of the foreground image
	 img_width: the width of background image
	 img_height: the height of foreground image
	 position: [xmin.ymin,xmax,ymax], marks where foreground locates in background
 @result
     true if write successfully or false if not
 @function
     write xml file, refer to PASCAL VOC 2012
*/
bool write_xml(std::string& xml_file, std::string& jpg_file,std::string& img_id,
	           int img_width,int img_height,std::vector<int>& position);

/*
 @params
    label_file: the text file with path
	xml_file: the xml file with path
    jpg_file: the jpg file with path
	count: a number used to apply different writing mode
 @result
	true if write successfully or false if not
 @function
	write label file, format: <jpg_file, xml_file>, refer to PASCAL VOC 2012
*/
bool write_label_file(std::string& label_file, std::string& jpg_file, std::string& xml_file, int& count);

/*
 @params
     str: a string will be parsed
     pattern: a string used to parse str
 @result
     a vector of string created by parsing 
 @function
     split a string into several parts to find a target string
*/
std::vector<std::string> split(const std::string& str, const std::string& pattern);

/*
 @params
    back_img: file name of background image
    fore_img: file name of foreground image
 @result
    a vector, [synthesis_jpg_file_name,synthesis_xml_file_name,label] if valid or [] if invalid
 @function
    get jpg file name, xml file name of synthesis image and label of foreground
*/
std::vector<std::string> get_file_name_and_label(std::string& back_img, std::string& fore_img);

/*
 @params
     label_file: the text file with path which stores labels, like <jpg_file,xml_file>
     xml_path: the path where xml files will be stored
     jpg_file: the path where jpg file will be stored 
     background_list: a list of image files used for background
     fore_list: a list of image files used for foreground
     border: an int value defines border width
	 thresh: an int value define threshold, replace pixel of back_img with pixel of fore_img if
	         pixel value of fore_img is greater than threshold; otherwise, do nothing
 @result
     true if write successfully or false if not
 @function
     write xml files and jpg files, refer to PASCAL VOC 2012
*/
bool write_xml_jpg_label(std::string& label_file, std::string& xml_path,std::string& jpg_path, std::vector<std::string>& background_list,
	           std::vector<std::string>& fore_list,int border,int thresh);



#endif