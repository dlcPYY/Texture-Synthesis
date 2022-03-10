#pragma once
#include<opencv2\opencv.hpp>   
#include<opencv2\highgui\highgui.hpp>
#include<iostream>
#include<cstring>
#include<queue>
#include<vector>
#include"time.h"
using namespace std;
using namespace cv;
class ImageQuilting
{
	private:
		Mat *input_images;//�����ԭʼͼ��
		Mat *quilting_results;//���ɵ�ͼ��
		Mat *tem_rol;//���ȵ�ʱ���õľ������,�����һ��
		Mat *tem_coll;//���ȵ�ʱ���õľ������,��������
		Mat *tem_colr;//���ȵ�ʱ���õľ������,������Ҳ�
		int OverlapX;//�ص����������Ŀ��
		int OverlapY;//�ص����ֺ����ĸ߶�
		int block_x;//����ͼ��ĳ���
		int block_y;//����ͼ��ĸ߶�
		int select_x;//ѡȡ��ĳ���
		int select_y;//ѡȡ��Ŀ��
		int num_x;//������������ɶ��ٿ�
		int num_y;//������������ɶ��ٿ�
		int Bestx, Besty;//���ѡ���ĵط�
		float e1[100][100];//·���滮��
		float e2[100][100];//·���滮��
		float e3[100][100];//·���滮��
	public:
		ImageQuilting(string path, int the_blockx, int the_blocky, int tnum_x, int tnum_y);//ÿһ��Ĵ�С����
		void ImageGenerate();
		void Getdistance(Mat* coll, int X, int Y);//��һ�У�ֻ��Ҫ����༴��
		void Getdistance(Mat* rol, int X, int Y, bool flag);//��һ�У�ֻ��Ҫ���ϲ༴��
		void Getdistance(Mat* col, Mat* rol, int X, int Y);//�����ϲ�����
		void Getdistance(Mat* coll, Mat* colr, int X, int Y, bool flag);//��һ�����һ���������Ҳ� 
		void Getdistance(Mat* coll, Mat* rol, Mat* colr, int X, int Y);//�����е����һ���������Ҳ���ϲ�
};

