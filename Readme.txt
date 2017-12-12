1.临时用于测试代码正确与否的文件组织格式
	E:\\TestImages\\sys\\
	                    back   #用于存放背景图像
			    fore   #用于存放前景图像(Logo)
			    jpg    #用于存放生成的合成图像
			    xml    #用于存放生成的xml文件
			    label.txt #用于存放类似  .jpg .xml 格式

2.最终进行生成时的文件组织格式
	E:\\TestImage\\SynData\\
	                        jpg_path #用于存放生成的合成图像
				xml_path #用于存放生成的xml文件
				label.txt #用于存放类似  .jpg .xml 格式
	E:\\TestImage\\SynData\\back_fore_img\\
	                                       back_img #用于存放背景图像,从官方提供的背景图像中选取495张
					       fore_img #用于存放前景图像(Logo，96张)，自己用PS处理生成了64张，原图32张

3.附注
	(1)选取的背景图像全部重新被命名为英文格式，并且选取的图像全部为24bit(RGB，每通道8bit)
	   官方提供的背景图像中存在8bit的图像，会导致生成程序出错。对此的策略为：先注释生成图
	   像和xml文件的代码，当程序读取图像发生错误时，剔除该文件，保留该图像之前通过程序的图
	   像。依次重复上述策略，也可写程序进行剔除(本程序未编写剔除程序)
	(2)Photoshop处理Logo的原因
	   进行完全的图像融合。官方提供的Logo存在的问题在于：
	   生成图像中Logo的背景为白色，不符合现实场景
	   官方提供的Logo除核心图像外，边缘占比过大，使得标记框过大，因此对其进行裁剪
	(3)Photoshop处理生成64张Logo的原因
	   第一部分:直接将Logo核心标志以外的部分处理为黑色，即像素值为0
	   第二部分:使用PS对第一部分的Logo进行仿射变换。前期用于测试仿射变换的作用
	(4)图像融合的策略
	   对前景图像中像素值为[0,thresh)范围内的像素替换为背景像素，thresh在函数synthesis_data()
	   中进行设置
	   在背景图像中，选取位于[border,back_img_size-border-fore_img_size]范围内的随机数作为前景
	   border在函数synthesis_data()中进行设置
	   图像在背景图像中的左上坐标(left,top)，右下坐标为[left+fore_img_width,top+fore_img_height]
	   当前景图像的width或height大于背景图像的1/2时，将前景图像缩小为原尺寸的1/2，保证前景图像
	   在背景图像中不会过大
	   其中，back_img为背景图像，fore_img为前景图像，size为尺寸，在上述计算值可替换为width或height
	   
	                         