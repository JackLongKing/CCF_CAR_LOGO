/***********************************************************************
@Author: Long Gu
@Date:   2017/11/20
***********************************************************************/
#include<ctime>
#include<cstdlib>
#include<opencv2/imgproc/imgproc.hpp>
#include "SysData.h"
using namespace cv;


int get_int_range_rand(int start, int end)
{
	int res = (rand() % (end - start)) + start;
	return res;
}

double get_double_range_rand(int start, int end, double scale_size)
{
	double res;
	int temp_res;
	temp_res = (rand() % (end - start)) + start;
	res = double(temp_res) / scale_size;
	return res;
}

std::vector<int> get_position(Mat& back_img, Mat& fore_img,int border)
{
	if (!back_img.data || !fore_img.data)
	{
		printf("Error: back_img or fore_img failed to open, check again!\n");
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return std::vector<int>();
	}
	double scale_ratio = get_double_range_rand(2, 8);
	resize(fore_img, fore_img, Size(fore_img.cols*scale_ratio, fore_img.rows*scale_ratio), 0, 0, INTER_LINEAR);
	
	int width = back_img.cols;
	int height = back_img.rows;
	int scaled_fore_img_width = fore_img.cols;
	int scaled_fore_img_height = fore_img.rows;
	if (width<=scaled_fore_img_width || height<=scaled_fore_img_height)
		return std::vector<int>();

	int start_x = get_int_range_rand(border, width - scaled_fore_img_width - border);
	int start_y = get_int_range_rand(border, height - scaled_fore_img_height - border);
	std::vector<int> res;
	res.push_back(start_x);
	res.push_back(start_y);
	res.push_back(start_x + scaled_fore_img_width);
	res.push_back(start_y + scaled_fore_img_height);

	return res;
}

bool img_fusion(Mat& back_img, Mat& fore_img, int back_start_x, int back_start_y, int thresh)
{
	if (!back_img.data || !fore_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}
	if (back_start_x >= back_img.cols || back_start_y >= back_img.rows) 
		return false;
	int rows = fore_img.rows;
	int cols = fore_img.cols;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (fore_img.at<Vec3b>(i, j)[0] >= thresh && fore_img.at<Vec3b>(i, j)[1] >= thresh
				&& fore_img.at<Vec3b>(i, j)[2] >= thresh)
			{
				back_img.at<Vec3b>(back_start_y + i, back_start_x + j)[0] = fore_img.at<Vec3b>(i, j)[0];
				back_img.at<Vec3b>(back_start_y + i, back_start_x + j)[1] = fore_img.at<Vec3b>(i, j)[1];
				back_img.at<Vec3b>(back_start_y + i, back_start_x + j)[2] = fore_img.at<Vec3b>(i, j)[2];
			}
		}
	}
	return true;
}

std::vector<int> fusion(cv::Mat& back_img, cv::Mat& fore_img, int border, int thresh)
{
	if (!back_img.data || !fore_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return std::vector<int>();
	}

	while (back_img.rows <= (fore_img.rows + border)*2 || back_img.cols <= (fore_img.cols + border)*2)
	{
		resize(fore_img, fore_img, Size(fore_img.cols / 2, fore_img.rows / 2), 0, 0, INTER_LINEAR);
	}
	
	std::vector<int> position = get_position(back_img, fore_img, border);

	if (position.size() == 0)
		return std::vector<int>();

	if (img_fusion(back_img, fore_img, position[0], position[1], thresh))
		return position;
	else
		return std::vector<int>();
}

bool preprocess(cv::Mat& img, int high_thresh)
{
	if (!img.data || high_thresh > 255)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}

	int rows = img.rows;
	int cols = img.cols;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (img.at<Vec3b>(i, j)[0] >= high_thresh && img.at<Vec3b>(i,j)[1] >= high_thresh && img.at<Vec3b>(i,j)[2] >= high_thresh)
			{
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 0;
				img.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	return true;
}

bool img_warp(cv::Mat& fore_img,cv::Mat& dst_fore_img)
{
	if (!fore_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}
	int option = get_int_range_rand(0, 2);
	if (option % 2 == 0)
	{
		dst_fore_img = fore_img;
		return true;
	}
	int rows = fore_img.rows;
	int cols = fore_img.cols;
	Point2f srcTri[3];
	Point2f dstTri[3];
	Mat rot_mat(2, 3, CV_32FC1);
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_rotate_dst;
	warp_dst = Mat::zeros(rows, cols, fore_img.type());
	//affine warp  
	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(cols - 1, 0);
	srcTri[2] = Point2f(0, rows - 1);

	double scale_x1 = get_double_range_rand(-1, 6, 10);
	double scale_y1 = get_double_range_rand(-2, 3, 10);
	double scale_x2 = get_double_range_rand(6, 9, 10);
	double scale_y2 = get_double_range_rand(-1, 5, 10);
	double scale_x3 = get_double_range_rand(-2, 4, 10);
	double scale_y3 = get_double_range_rand(6, 9, 10);

	dstTri[0] = Point2f(cols*scale_x1, rows*scale_y1);
	dstTri[1] = Point2f(cols*scale_x2, rows*scale_y2);
	dstTri[2] = Point2f(cols*scale_x3, rows*scale_y3);

	option = get_int_range_rand(0, 2);
	warp_mat = getAffineTransform(srcTri, dstTri);
	warpAffine(fore_img, warp_dst, warp_mat, warp_dst.size());
	if (option % 2 == 0)
	{
		dst_fore_img = warp_dst;
		return true;
	}
	else
	{
		//rotate matrix
		Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
		double angle = get_double_range_rand(-60, 60, 1);
		double scale = get_double_range_rand(5, 10, 10);
		rot_mat = getRotationMatrix2D(center, angle, scale);
		warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());
		dst_fore_img = warp_rotate_dst;
		return true;
	}
}

bool img_perspect(cv::Mat& fore_img, cv::Mat& dst_fore_img)
{
	if (!fore_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}
	int option = get_int_range_rand(0, 2);
	if (option % 2 == 0)
	{
		dst_fore_img = fore_img;
		return true;
	}

	dst_fore_img = Mat::zeros(fore_img.size(), fore_img.type());
	int rows = fore_img.rows;
	int cols = fore_img.cols;

	/*std::vector<Point2f> corners(4);
	corners[0] = Point2f(0, 0);
	corners[1] = Point2f(cols - 1, 0);
	corners[2] = Point2f(0, rows - 1);
	corners[3] = Point2f(cols - 1, rows - 1);*/

	//opencv3.2
	Point2f corners[] = { Point2f(0, 0), Point2f(cols - 1, 0), Point2f(0, rows - 1), Point2f(cols - 1, rows - 1) };
	
	double scale_x1 = get_double_range_rand(-1, 5, 10);
	double scale_y1 = get_double_range_rand(-3, 3, 10);
	double scale_x2 = get_double_range_rand(8, 10, 10);
	double scale_y2 = get_double_range_rand(0, 5, 10);
	double scale_x3 = get_double_range_rand(-2, 3, 10);
	double scale_y3 = get_double_range_rand(5, 10, 10);
	double scale_x4 = get_double_range_rand(7, 10, 10);
	double scale_y4 = get_double_range_rand(7, 10, 10);
	
	/*std::vector<Point2f> corners_trans(4);
	corners_trans[0] = Point2f(cols*scale_x1, rows*scale_y1);
	corners_trans[1] = Point2f(cols*scale_x2, rows*scale_y2);
	corners_trans[2] = Point2f(cols*scale_x3, rows*scale_y3);
	corners_trans[3] = Point2f(cols*scale_x4, rows*scale_y4);*/

	//opencv3.2
	Point2f corners_trans[] = { Point2f(cols*scale_x1, rows*scale_y1), Point2f(cols*scale_x2, rows*scale_y2),
		Point2f(cols*scale_x3, rows*scale_y3), Point2f(cols*scale_x4, rows*scale_y4) };

	Mat transform = getPerspectiveTransform(corners, corners_trans);
	warpPerspective(fore_img, dst_fore_img, transform, fore_img.size());

	return true;
}

bool img_sature(cv::Mat& sys_img)
{
	if (!sys_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}
	int option = get_int_range_rand(0, 2);
	if (option % 2 == 0)
		return true;

	double alpha = get_double_range_rand(1, 3, 1);
	double beta = get_double_range_rand(40, 60, 1);

	for (int i = 0; i < sys_img.rows; ++i)
		for (int j = 0; j < sys_img.cols; ++j)
			for (int k = 0; k < 3; ++k)
				sys_img.at<Vec3b>(i, j)[k] = saturate_cast<uchar>(sys_img.at<Vec3b>(i, j)[k] * alpha + beta);
	return true;
}

bool add_salt_noise(cv::Mat& sys_img)
{
	if (!sys_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}
	int option = get_int_range_rand(0, 2);
	if (option % 2 == 0)  //do nothing
		return true;
	
	int n = get_int_range_rand(10, 3000);
	//add salt, white points
	for (int k = 0; k < n; ++k)
	{
		int j = rand() % sys_img.rows;
		int i = rand() % sys_img.cols;
		if (sys_img.channels() == 3)
		{
			sys_img.at<Vec3b>(j, i)[0] = 255;
			sys_img.at<Vec3b>(j, i)[1] = 255;
			sys_img.at<Vec3b>(j, i)[0] = 255;
		}
	}
	//add another noise, black points
	for (int k = 0; k < n; ++k)
	{
		int j = rand() % sys_img.rows;
		int i = rand() % sys_img.cols;
		if (sys_img.channels() == 3)
		{
			sys_img.at<Vec3b>(j, i)[0] = 0;
			sys_img.at<Vec3b>(j, i)[1] = 0;
			sys_img.at<Vec3b>(j, i)[2] = 0;
		}
	}
	return true;
}

double generate_gaussian_noise(double mu, double sigma)
{
	const double epsilon = std::numeric_limits<double>::min();
	static double z0, z1;
	static bool flag = false;
	flag = !flag;
	//flag: fake gaussian random number x
	if (!flag)
		return z1*sigma + mu;

	double u1, u2;
	do
	{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand()*(1.0 / RAND_MAX);
	} while (u1 <= epsilon);
	//flag: true gaussian random number x
	z0 = sqrt(-2.0*log(u1))*cos(2 * CV_PI*u2);
	z1 = sqrt(-2.0*log(u1))*sin(2 * CV_PI*u2);
	return z0*sigma + mu;
}

bool add_gaussian_noise(cv::Mat& sys_img)
{
	if (!sys_img.data)
	{
		printf("Failed in line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}
	int option = get_int_range_rand(0, 2);
	if (option % 2 == 0)
		return true;

	int rows = sys_img.rows;
	int cols = sys_img.cols*sys_img.channels();
	//image is continuous or not
	if (sys_img.isContinuous())
	{
		cols *= rows;
		rows = 1;
	}
	
	double mu = get_double_range_rand(0, 5, 1);
	double sigma = get_double_range_rand(0, 10, 10);
	for (int j = 0; j < rows; ++j)
	{
		for (int i = 0; i < cols; ++i)
		{
			int val = sys_img.ptr<uchar>(j)[i] + generate_gaussian_noise(mu, sigma) * 32;
			if (val < 0)
				val = 0;
			if (val > 255)
				val = 255;
			sys_img.ptr<uchar>(j)[i] = (uchar)val;
		}
	}
	return true;
}
