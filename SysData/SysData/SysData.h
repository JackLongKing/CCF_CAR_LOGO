/***********************************************************************
@Author: Long Gu
@Date:   2017/11/20
***********************************************************************/
#ifndef SYS_DATA_H_
#define SYS_DATA_H_

#include<vector>
#include<opencv2/core/core.hpp>

/*
 @params
     start: left border of a range
	 end:   right border of a range
 @result
     an int value located in [start,end)
 @function
	 get an int located in [start,end) generated by rand()
*/
int get_int_range_rand(int start, int end);

/*
 @params
	 start: left border of a range
	 end:   right border of a range
	 scale_size: scale ratio
 @result
	 a double value located in [start/scale_size,end/scale_size)
 @function
	 get a double located in [start/scale_size,end/scale_size) generated by rand()
*/
double get_double_range_rand(int start, int end, double scale_size = 10.0);

/*
 @params
	 back_img: an image used as background
     fore_img: an image used as object
     border: an int value defines border width
 @result
     a vector, [left,top,right,down], if valid
	 a vector, [], if invalid
 @function
     get start position and end position where fore_img located in back_img
*/
std::vector<int> get_position(cv::Mat& back_img, cv::Mat& fore_img,int border);

/*
 @params
     back_img: an image used as background
     fore_img: an image used as object
	 back_start_x: an int value defines start position(left) of fore_img in back_img
	 back_start_y: an int value defines start position(top) of fore_img in back_img
	 thresh: an int value define threshold, replace pixel of back_img with pixel of fore_img if 
	         pixel value of fore_img is greater than threshold; otherwise, do nothing
 @result
     a bool value indicates result of fusion, true: success; false: fail
 @function
     image fusion
*/
bool img_fusion(cv::Mat& back_img, cv::Mat& fore_img, int back_start_x, int back_start_y, int thresh=2);

/*
 @params
     back_img: an image used as background
     fore_img: an image used as object
     border: an int value defines border width
     back_start_y: an int value defines start position(top) of fore_img in back_img
	 thresh: an int value define threshold, replace pixel of back_img with pixel of fore_img if
	         pixel value of fore_img is greater than threshold; otherwise, do nothing
 @result
     a vector, [left,top,right,down], if valid
     a vector, [], if invalid
 @function
     image fusion (generally)
*/
std::vector<int> fusion(cv::Mat& back_img, cv::Mat& fore_img,int border,int thresh);

/*
 @params
     img: an image
	 high_thresh: threshold
 @result
     true if do this else false
 @function
     if a pixel in img whose value greater than high_thresh, change its value to 0, without change if not
*/
bool preprocess(cv::Mat& img, int high_thresh);

/*
 @params
     fore_img: an image used as object
	 dst_fore_img: an image got by fore_img with affine transform
	 note: this function apply certain probability into whether using affine transform or not
 @result
     true if operations are truly done or false if not
 @function
     apply certain probability into whether using affine transform or not, create image by affine transform in certain probability
*/
bool img_warp(cv::Mat& fore_img, cv::Mat& dst_fore_img);

/*
 @params
     fore_img: an image used as object
	 dst_fore_img: an image got by fore_img with perspective transform
	 note: this function apply certain probability into whether using perspective transform or not
 @result
     true if operations are truly done or false if not
 @function
     apply certain probability into whether using perspective transform or not, create image by perspective transform in certain probability
*/
bool img_perspect(cv::Mat& fore_img, cv::Mat& dst_fore_img);

/*
 @params
     sys_img: an image synthesized by fore_img and back_img
	 note: this function apply certain probability into whether using saturation transform or not
 @result
     true if operations are truly done or false if not
 @function
     apply certain probability into whether using saturation transform or not, adjust saturation of image
*/
bool img_sature(cv::Mat& sys_img);

/*
 @params
     sys_img: an image synthesized by fore_img and back_img
     note: this function apply certain probability into whether using salt and pepper noise or not
 @result
     true if operations are truly done or false if not
 @function
     apply certain probability into whether using salt and pepper noise or not
*/
bool add_salt_noise(cv::Mat& sys_img);

/*
 @params
     mu: mean value
	 sigma: standard deviation
	 mu,sigma meet with gaussian(normal) distribution: N(mu,sigma^2)
 @result
     a double value
 @function
     get parameters of gaussian noise
*/
double generate_gaussian_noise(double mu, double sigma);

/*
 @params
     sys_img: an image synthesized by fore_img and back_img
 @result
     true if add gaussian noise successfully or false if not
 @function
     apply certain probability into adding gaussian noise
*/
bool add_gaussian_noise(cv::Mat& sys_img);

#endif