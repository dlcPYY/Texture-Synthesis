#include "ImageQuilting.h"
ImageQuilting::ImageQuilting(string path, int the_blockx, int the_blocky,int tnum_x,int tnum_y){
	Mat img= imread(path,0);
	input_images = new Mat(img.rows, img.cols, 0);
	for (int i = 0; i < input_images->rows; i++) {
		for (int j = 0; j < input_images->cols; j++) {
			input_images->at<uchar>(i, j) = img.at<uchar>(i, j);
		}
	}
	select_x = the_blockx;
	select_y = the_blocky;
	num_x = tnum_x;
	num_y = tnum_y;
	block_x = input_images->cols;
	block_y = input_images->rows;
	quilting_results = new Mat(the_blocky * num_y, the_blockx * num_x, 0);
//	temporary_images = new Mat(the_blockx + 40, the_blocky + 40, 0);
	OverlapX = the_blockx / 6;//�ص�����֮һ
	OverlapY = the_blocky / 6;

	tem_rol = new Mat(OverlapY + 20, select_x + 20, 0);
	tem_coll = new Mat(select_y + 20, OverlapX + 20, 0);
	tem_colr = new Mat(select_y + 20, OverlapX + 20, 0);
	//����X ����Y

}

//sad�㷨�Ż�



//���
void ImageQuilting::Getdistance(Mat* coll,int X,int Y) {//ֻ������
	float dis = 999999999;
	Mat Kernel_L1(*coll, Rect(0, 0, OverlapX, select_y));
	for (int cy = 0; cy < block_y - select_y; cy++) {
		for (int cx = 0; cx < block_x - select_x - OverlapX; cx++) {
			float nowdissum = 0;
			Mat Kernel_R1(*input_images, Rect(cx, cy , OverlapX, select_y));
			Mat Dif;
			absdiff(Kernel_L1, Kernel_R1, Dif);
			//			printf("\n����11\n");
			Scalar ADD = sum(Dif);
			nowdissum += ADD[0];
			//��ȡ�ϱ�һ��
			if (nowdissum < dis) {
				Bestx = cx;
				Besty = cy;
				dis = nowdissum;
			}
		}
	}
	//�����ߣ������ͼ+�Ż��߽�
	//��һ���������ͼ
	for (int ty = Y; ty < Y + select_y; ty++) {
		for (int tx = X + OverlapX; tx < X + OverlapX + select_x; tx++) {
			quilting_results->at<uchar>(ty, tx) = input_images->at<uchar>(Besty + ty - Y, Bestx - X + tx);
		}
	}
	//�ڶ�����·���滮����������������Ȼ���ڷ�����ͼ
	memset(e1, 0, sizeof(e1));
	for (int ty = Y; ty < Y + select_y; ty++) {
		for (int tx = X; tx < X + OverlapX; tx++) {
			e1[ty - Y][tx - X] = sqrt(((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)coll->at<uchar>(ty - Y, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)coll->at<uchar>(ty - Y, tx - X)));
		}
	}
	for (int ty = 1; ty < select_y; ty++) {
		for (int tx = 0; tx < OverlapX; tx++) {
			float minn = e1[ty - 1][tx];
			if (tx != 0) minn = min(minn, e1[ty - 1][tx - 1]);
			if (tx != OverlapX - 1) minn = min(minn, e1[ty - 1][tx + 1]);
			e1[ty][tx] += minn;
		}
	}
	int tipsx;//��С������
	float nowmin = 1008611;
	for (int tx = 0; tx < OverlapX; tx++) {
		if (e1[select_y - 1][tx] < nowmin) {
			tipsx = tx;
			nowmin = e1[select_y - 1][tx];
		}
	}
	for (int ty = select_y - 1; ty > -1; ty--) {
		for (int tx = tipsx; tx < OverlapX; tx++) {
			quilting_results->at<uchar>(Y + ty, X + tx - tipsx) = input_images->at<uchar>(Besty + ty, Bestx + tx - tipsx);
		}
		if (ty == 0) break;
		if (tipsx == 0) {
			if (e1[ty - 1][tipsx] > e1[ty - 1][tipsx + 1]) {
				tipsx = tipsx + 1;
			}
			continue;
		}
		if (tipsx == OverlapX - 1) {
			if (e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx]) {
				tipsx = tipsx - 1;
			}
			continue;
		}
		if (e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx]) {
			tipsx = tipsx - 1;
		}
		if (e1[ty - 1][tipsx] > e1[ty - 1][tipsx + 1]) {
			tipsx = tipsx + 1;
		}
	}
}


//�����ϲ�
void ImageQuilting::Getdistance(Mat* col, Mat* rol, int X, int Y) {//�������ӣ��ϼ���
	float dis = 999999999;
	Mat Kernel_L1(*col, Rect(0, 0, OverlapX, select_y));
	Mat Kernel_L2(*rol, Rect(0, 0, select_x + OverlapX, OverlapY));
	clock_t start, finish;
	start = clock();
//	Mat MM(1, DSR, CV_32F, Scalar(0));
//	printf("\n����\n");
	for (int cy = 0; cy < block_y - select_y - OverlapY; cy++) {
		for (int cx = 0; cx < block_x - select_x - OverlapX; cx++) {
			float nowdissum = 0;
			Mat Kernel_R1(*input_images, Rect(cx, cy + OverlapY, OverlapX, select_y));
			Mat Dif;
			absdiff(Kernel_L1, Kernel_R1, Dif);
//			printf("\n����11\n");
			Scalar ADD = sum(Dif);
			nowdissum += ADD[0];
			//��ȡ�ϱ�һ��
			Mat Kernel_R2(*input_images, Rect(cx, cy, select_x + OverlapX, OverlapY));
			Mat Dif2;
			absdiff(Kernel_L2, Kernel_R2, Dif2);
			Scalar ADD2 = sum(Dif2);
			nowdissum += ADD2[0];

			if (nowdissum < dis) {
				Bestx = cx;
				Besty = cy;
				dis = nowdissum;
			}
		}
	}
	finish = clock();
	double Total_time = (double)(finish - start);
	printf("\nƥ�亯������ʱ�䣺%0.3f���� \n", Total_time);
	start = clock();
	//�����ߣ������ͼ+�Ż��߽�
	//��һ���������ͼ
	for (int ty = Y + OverlapY; ty < Y + select_y + OverlapY; ty++) {
		for (int tx = X + OverlapX; tx < X + OverlapX + select_x; tx++) {
			quilting_results->at<uchar>(ty, tx) = input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X);
		}
	}
	//�ڶ�����·���滮����������������Ȼ���ڷ�����ͼ
	memset(e1, 0, sizeof(e1));
	for (int ty = Y + OverlapY; ty < Y + select_y + OverlapY; ty++) {
		for (int tx = X; tx < X + OverlapX; tx++) {
			e1[ty - Y - OverlapY][tx - X] = sqrt(((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)col->at<uchar>(ty - Y - OverlapY, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)col->at<uchar>(ty - Y - OverlapY, tx - X)));
		}
	}
	for (int ty = 1; ty < select_y; ty++) {
		for (int tx = 0; tx < OverlapX; tx++) {
			float minn = e1[ty - 1][tx];
			if (tx != 0) minn = min(minn, e1[ty - 1][tx - 1]);
			if (tx != OverlapX - 1) minn = min(minn, e1[ty - 1][tx + 1]);
			e1[ty][tx] += minn;
		}
	}

	int tipsx;//��С������
	float nowmin = 1008611;
	for (int tx = 0; tx < OverlapX; tx++) {
		if (e1[select_y - 1][tx] < nowmin) {
			tipsx = tx;
			nowmin = e1[select_y - 1][tx];
		}
	}
	for (int ty = select_y - 1; ty > -1; ty--) {
		for (int tx = tipsx; tx < OverlapX; tx++) {
			quilting_results->at<uchar>(Y + ty + OverlapY, X + tx) = input_images->at<uchar>(Besty + ty + OverlapY, Bestx + tx);
		}
		if (ty == 0) break;
		if (tipsx == 0) {
			if (e1[ty - 1][tipsx] > e1[ty - 1][tipsx + 1]) {
				tipsx = tipsx + 1;
			}
			continue;
		}
		if (tipsx == OverlapX - 1) {
			if (e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx]) {
				tipsx = tipsx - 1;
			}
			continue;
		}
		if (e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx]&& e1[ty - 1][tipsx-1] < e1[ty - 1][tipsx+1]) {
			tipsx = tipsx - 1;
		}
		if (e1[ty - 1][tipsx + 1] < e1[ty - 1][tipsx] && e1[ty - 1][tipsx + 1] < e1[ty - 1][tipsx - 1]) {
			tipsx = tipsx + 1;
		}
	}




	memset(e2, 0, sizeof(e2));
	for (int ty = Y; ty < Y + OverlapY; ty++) {
		for (int tx = X; tx < X + OverlapX+select_x; tx++) {
			e2[ty - Y][tx - X] = sqrt(((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)rol->at<uchar>(ty - Y, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)rol->at<uchar>(ty - Y, tx - X)));
		}
	}
	
	for (int tx = 1; tx < OverlapX + select_x; tx++){
		for (int ty = 0; ty < OverlapY; ty++){
			float minn = e2[ty][tx - 1];
			if (ty != 0) minn = min(minn, e2[ty - 1][tx - 1]);
			if (ty != OverlapY - 1) minn = min(minn, e2[ty + 1][tx - 1]);
			e2[ty][tx] += minn;
		}
	}
	
	int tipsy;//��С������
	nowmin = 1008611;
	for (int ty = 0; ty < OverlapY; ty++) {
		if (e2[ty][select_x+OverlapX-1] < nowmin) {
			tipsy = ty;
			nowmin = e2[ty][select_x + OverlapX - 1];
		}
	}
	for (int tx = select_x + OverlapX - 1; tx > -1; tx--) {
		for (int ty = tipsy; ty < OverlapY; ty++) {
			quilting_results->at<uchar>(Y + ty , X +tx) = input_images->at<uchar>(Besty + ty, Bestx + tx );
		}
	//	cout << "test " << tx<<endl;
		if (tx == 0) break;
		if (tipsy == OverlapY - 1) {
			if (e2[tipsy - 1][tx-1] < e2[tipsy][tx - 1]) {
				tipsy = tipsy - 1;
			}
			continue;
		}
	//	cout << "pass1 " << endl;
		if (tipsy == 0) {
			if (e2[tipsy + 1][tx - 1] < e2[tipsy][tx - 1]) {
				tipsy = tipsy + 1;
			}
			continue;
		}
		if (e2[tipsy - 1][tx - 1] < e2[tipsy][tx -1]&& e2[tipsy - 1][tx - 1] < e2[tipsy+1][tx - 1]) {
			tipsy = tipsy - 1;
		}
		if (e2[tipsy + 1][tx - 1] < e2[tipsy][tx - 1] && e2[tipsy + 1][tx - 1] < e2[tipsy - 1][tx - 1]) {
			tipsy = tipsy + 1;
		}
	}
	finish = clock();
	Total_time = (double)(finish - start);
//	printf("\n·���滮��������ʱ�䣺%0.3f���� \n", Total_time);
}

//�ϲ�
void ImageQuilting::Getdistance(Mat* rol, int X, int Y, bool flag) {//��һ�У�ֻ��Ҫ���ϲ༴��
	float dis = 999999999;
	Mat Kernel_L1(*rol, Rect(0, 0, select_x, OverlapY));
	for (int cy = 0; cy < block_y - select_y - OverlapY; cy++) {
		for (int cx = 0; cx < block_x - select_x; cx++) {
			float nowdissum = 0;
			Mat Kernel_R1(*input_images, Rect(cx, cy, select_x, OverlapY));
			Mat Dif;
			absdiff(Kernel_L1, Kernel_R1, Dif);
			//			printf("\n����11\n");
			Scalar ADD = sum(Dif);
			nowdissum += ADD[0];

			if (nowdissum < dis) {
				Bestx = cx;
				Besty = cy;
				dis = nowdissum;
			}
		}
	}
	//�����¿�
	for (int ty = Y + OverlapY; ty < Y + select_y + OverlapY; ty++) {
		for (int tx = X; tx < X + select_x; tx++) {
			quilting_results->at<uchar>(ty, tx) = input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X);
		}
	}

	//�غϲ��֣�ȷ��·��
	memset(e1, 0, sizeof(e1));
	for (int ty = 0; ty < OverlapY; ty++) {
		for (int tx = 0; tx < select_x; tx++) {
			e1[ty][tx] = sqrt(((int)input_images->at<uchar>(Besty + ty, Bestx + tx) - (int)rol->at<uchar>(ty, tx)) * ((int)input_images->at<uchar>(Besty + ty, Bestx + tx) - (int)rol->at<uchar>(ty, tx)));
		}
	}

	//Ѱ��������С��·��
	for (int tx = 1; tx < select_x; tx++) {
		for (int ty = 0; ty < OverlapY; ty++) {
			float minn = e1[ty][tx - 1];
			if (ty != 0) minn = min(minn, e1[ty - 1][tx - 1]);
			if (ty != OverlapY - 1) minn = min(minn, e1[ty + 1][tx - 1]);
			e1[ty][tx] += minn;
		}
	}
	int tipsy;//��С������
	float nowmin = 1008611;
	for (int ty = 0; ty < OverlapY; ty++) {
		if (e1[select_x-1][ty] < nowmin) {
			tipsy = ty;
			nowmin = e1[select_x-1][ty];
		}
	}
	for (int tx = select_x - 1; tx > -1; tx--) {
		for (int ty = tipsy; ty < OverlapY; ty++) {
			quilting_results->at<uchar>(Y + ty, X + tx) = input_images->at<uchar>(Besty + ty, Bestx + tx);
		}
		if (tx == 0) break;
		if (tipsy == 0) {
			if (e1[tipsy][tx-1] > e1[tipsy+1][tx-1]) {
				tipsy = tipsy + 1;
			}
			continue;
		}
		if (tipsy == OverlapY-1) {
			if (e1[tipsy][tx - 1] > e1[tipsy - 1][tx - 1]) {
				tipsy = tipsy - 1;
			}
			continue;
		}
		if (e1[tipsy - 1][tx - 1] < e1[tipsy][tx-1]&& e1[tipsy - 1][tx - 1] < e1[tipsy+1][tx - 1]) {
			tipsy = tipsy - 1;
		}
		if (e1[tipsy + 1][tx - 1] < e1[tipsy][tx - 1] && e1[tipsy +1][tx - 1] < e1[tipsy - 1][tx - 1]) {
			tipsy = tipsy + 1;
		}
	}
}////PASS



//�����Ҳ�
void ImageQuilting::Getdistance(Mat* coll, Mat* colr, int X, int Y, bool flag) {//�����Ҳ�
	float dis = 999999999;
	Mat Kernel_L1(*coll, Rect(0, 0, OverlapX, select_y));
	Mat Kernel_L2(*colr, Rect(0, 0, OverlapX, select_y));
	for (int cy = 0; cy < block_y - select_y; cy++) {
		for (int cx = 0; cx < block_x - select_x - OverlapX - OverlapX; cx++) {
			float nowdissum = 0;
			Mat Kernel_R1(*input_images, Rect(cx, cy , OverlapX, select_y));
			Mat Dif;
			absdiff(Kernel_L1, Kernel_R1, Dif);
			//			printf("\n����11\n");
			Scalar ADD = sum(Dif);
			nowdissum += ADD[0];
			//��ȡ�ұ�һ��
			Mat Kernel_R2(*input_images, Rect(cx + select_x + OverlapX, cy, OverlapX, select_y));
			Mat Dif2;
			absdiff(Kernel_L2, Kernel_R2, Dif2);
			Scalar ADD2 = sum(Dif2);
			nowdissum += ADD2[0];
			if (nowdissum < dis) {
				Bestx = cx;
				Besty = cy;
				dis = nowdissum;
			}
		}
	}
	//��һ���������ͼ
	for (int ty = Y ; ty < Y + select_y; ty++) {
		for (int tx = X + OverlapX; tx < X + OverlapX + select_x; tx++) {
			quilting_results->at<uchar>(ty, tx) = input_images->at<uchar>(Besty + ty - Y, Bestx - X + tx);
		}
	}
	//return;
	//PASS
	memset(e1, 0, sizeof(e1));
	for (int ty = Y; ty < Y + select_y; ty++) {
		for (int tx = X; tx < X + OverlapX; tx++) {
			e1[ty - Y][tx - X] = sqrt(((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)coll->at<uchar>(ty - Y, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)coll->at<uchar>(ty - Y, tx - X)));
		}
	}
	//PASS�ϲ�δ�޸�




	for (int ty = 1; ty < select_y; ty++) {
		for (int tx = 0; tx < OverlapX; tx++) {
			float minn = e1[ty - 1][tx];
			if (tx != 0) minn = min(minn, e1[ty - 1][tx - 1]);
			if (tx != OverlapX - 1) minn = min(minn, e1[ty - 1][tx + 1]);
			e1[ty][tx] += minn;
		}
	}
	int tipsx;//��С������
	float nowmin = 1008611;
	for (int tx = 0; tx < OverlapX; tx++) {
		if (e1[select_y - 1][tx] < nowmin) {
			tipsx = tx;
			nowmin = e1[select_y - 1][tx];
		}
	}
	for (int ty = select_y - 1; ty > -1; ty--) {
		for (int tx = tipsx; tx < OverlapX; tx++) {
			quilting_results->at<uchar>(Y + ty, X +tx) = input_images->at<uchar>(Besty + ty, Bestx + tx);
		}
		if (ty == 0) break;
		if (tipsx == 0) {
			if (e1[ty - 1][tipsx] > e1[ty - 1][tipsx + 1]) {
				tipsx = tipsx + 1;
			}
			continue;
		}
		if (tipsx == OverlapX - 1) {
			if (e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx]) {
				tipsx = tipsx - 1;
			}
			continue;
		}
		if (e1[ty - 1][tipsx - 1] < e1[ty -1][tipsx] && e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx + 1]) {
			tipsx = tipsx - 1;
		}
		if (e1[ty - 1][tipsx + 1] < e1[ty -1][tipsx ] && e1[ty - 1][tipsx + 1] < e1[ty - 1][tipsx - 1]) {
			tipsx = tipsx + 1;
		}
	}

	//�ұߵĸ���

	memset(e2, 0, sizeof(e2));
	for (int ty = Y; ty < Y + select_y; ty++) {
		for (int tx = X  ; tx < X + OverlapX; tx++) {
			e2[ty - Y][tx - X] = sqrt(((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X + OverlapX + select_x) - (int)colr->at<uchar>(ty - Y, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X + OverlapX + select_x) - (int)colr->at<uchar>(ty - Y, tx - X)));
		}
	}
	for (int ty = 1; ty < select_y; ty++) {
		for (int tx = 0; tx < OverlapX; tx++) {
			float minn = e2[ty - 1][tx];
			if (tx != 0) minn = min(minn, e2[ty - 1][tx - 1]);
			if (tx != OverlapX - 1) minn = min(minn, e2[ty - 1][tx + 1]);
			e2[ty][tx] += minn;
		}
	}
	tipsx;//��С������
	nowmin = 1008611;
	for (int tx = 0; tx < OverlapX; tx++) {
		if (e2[select_y - 1][tx] < nowmin) {
			tipsx = tx;
			nowmin = e2[select_y - 1][tx];
		}
	}

	for (int ty = select_y - 1; ty > -1; ty--) {
		for (int tx = 0; tx < tipsx; tx++) {
			quilting_results->at<uchar>(Y + ty, tx) = input_images->at<uchar>(Besty + ty, Bestx + tx + OverlapX + select_x);
		}
		if (tipsx == 0) {
			if (e2[ty - 1][tipsx] > e2[ty - 1][tipsx + 1]) {
				tipsx = tipsx + 1;
			}
			continue;
		}
		if (tipsx == OverlapX - 1) {
			if (e2[ty - 1][tipsx - 1] < e2[ty - 1][tipsx]) {
				tipsx = tipsx - 1;
			}
			continue;
		}
		if (e2[ty - 1][tipsx - 1] < e2[ty - 1][tipsx] && e2[ty - 1][tipsx - 1] < e2[ty - 1][tipsx + 1]) {
			tipsx = tipsx - 1;
		}
		if (e2[ty - 1][tipsx + 1] < e2[ty - 1][tipsx] && e2[ty - 1][tipsx + 1] < e2[ty - 1][tipsx - 1]) {
			tipsx = tipsx + 1;
		}
	}
}



//����ϲ��Ҳ�
void ImageQuilting::Getdistance(Mat* coll, Mat* colr, Mat* rol, int X, int Y) {
	float dis = 999999999;
	Mat Kernel_L1(*coll, Rect(0, 0, OverlapX, select_y));
	Mat Kernel_L2(*rol, Rect(0, 0, select_x + OverlapX, OverlapY));
	Mat Kernel_L3(*colr, Rect(0, 0, OverlapX, select_y));
	for (int cy = 0; cy < block_y - select_y - OverlapY; cy++) {
	//	cout << cy + 1 << " is OK!" << endl;
		for (int cx = 0; cx < block_x - select_x - OverlapX - OverlapX; cx++) {
			float nowdissum = 0;
			Mat Kernel_R1(*input_images, Rect(cx, cy + OverlapY, OverlapX, select_y));
			Mat Dif;
			absdiff(Kernel_L1, Kernel_R1, Dif);
			//			printf("\n����11\n");
			Scalar ADD = sum(Dif);
			nowdissum += ADD[0];
			//��ȡ�ϱ�һ��
			Mat Kernel_R2(*input_images, Rect(cx, cy, select_x + OverlapX, OverlapY));
			Mat Dif2;
			absdiff(Kernel_L2, Kernel_R2, Dif2);
			Scalar ADD2 = sum(Dif2);
			nowdissum += ADD2[0];

			Mat Kernel_R3(*input_images, Rect(cx + OverlapX + select_x, cy + OverlapY, OverlapX, select_y));
			Mat Dif3;
			absdiff(Kernel_L3, Kernel_R3, Dif);
			//			printf("\n����11\n");
			Scalar ADD3 = sum(Dif);
			nowdissum += ADD3[0];


			if (nowdissum < dis) {
				Bestx = cx;
				Besty = cy;
				dis = nowdissum;
			}
		}
	}
	for (int ty = Y + OverlapY; ty < Y + select_y + OverlapY; ty++) {
		for (int tx = X + OverlapX; tx < X + OverlapX + select_x; tx++) {
			quilting_results->at<uchar>(ty, tx) = input_images->at<uchar>(Besty + ty - Y, Bestx - X + tx);
		}
	}
	//�ڶ�����·���滮����������������Ȼ���ڷ�����ͼ


	//���Ĳ���

	

	memset(e1, 0, sizeof(e1));
	for (int ty = Y + OverlapY; ty < Y + select_y + OverlapY; ty++) {
		for (int tx = X; tx < X + OverlapX; tx++) {
			e1[ty - Y - OverlapY][tx - X] = (((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)coll->at<uchar>(ty - Y - OverlapY, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)coll->at<uchar>(ty - Y - OverlapY, tx - X)));
		}
	}
	for (int ty = 1; ty < select_y; ty++) {
		for (int tx = 0; tx < OverlapX; tx++) {
			float minn = e1[ty - 1][tx];
			if (tx != 0) minn = min(minn, e1[ty - 1][tx - 1]);
			if (tx != OverlapX - 1) minn = min(minn, e1[ty - 1][tx + 1]);
			e1[ty][tx] += minn;
		}
	}
	int tipsx;//��С������
	float nowmin = 1008611;
	for (int tx = 0; tx < OverlapX; tx++) {
		if (e1[select_y - 1][tx] < nowmin) {
			tipsx = tx;
			nowmin = e1[select_y - 1][tx];
		}
	}

	for (int ty = select_y - 1; ty > -1; ty--) {
		for (int tx = tipsx; tx < OverlapX; tx++) {
			quilting_results->at<uchar>(Y + ty + OverlapY, X + tx) = input_images->at<uchar>(Besty + ty + OverlapY, Bestx + tx);
		}
		if (ty == 0) break;
		if (tipsx == 0) {
			if (e1[ty - 1][tipsx] > e1[ty - 1][tipsx + 1]) {
				tipsx = tipsx + 1;
			}
			continue;
		}
		if (tipsx == OverlapX - 1) {
			if (e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx]) {
				tipsx = tipsx - 1;
			}
			continue;
		}
		if (e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx]&& e1[ty - 1][tipsx - 1] < e1[ty - 1][tipsx+1]) {
			tipsx = tipsx - 1;
		}
		if (e1[ty - 1][tipsx] > e1[ty - 1][tipsx + 1]&& e1[ty - 1][tipsx - 1] > e1[ty - 1][tipsx+1]) {
			tipsx = tipsx + 1;
		}
	}
	

	
	//�ϲ�Ĳ���
	memset(e2, 0, sizeof(e2));
	for (int ty = Y; ty < Y + OverlapY; ty++) {
		for (int tx = X; tx < X + OverlapX + select_x ; tx++) {
			e2[ty - Y][tx - X] = sqrt(((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)rol->at<uchar>(ty - Y, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X) - (int)rol->at<uchar>(ty - Y, tx - X)));
		}
	}
	for (int tx = 1; tx < OverlapX + select_x ; tx++) {
		for (int ty = 0; ty < OverlapY; ty++) {
			float minn = e2[ty][tx - 1];
			if (ty != 0) minn = min(minn, e2[ty - 1][tx - 1]);
			if (ty != OverlapY - 1) minn = min(minn, e2[ty + 1][tx - 1]);
			e2[ty][tx] += minn;
		}
	}
	int tipsy;//��С������
	nowmin = 1008611;
//	cout << "FN1" << endl;
	for (int ty = 0; ty < OverlapY; ty++) {
		if (e2[ty][select_x + OverlapX-1] < nowmin) {
			tipsy = ty;
			nowmin = e2[ty][select_x + OverlapX-1];
		}
	}
//	cout << "FN0" << endl;
	for (int tx = select_x + OverlapX - 1; tx > -1; tx--) {
//		cout << "FIN " << "11" << endl;
		for (int ty = tipsy; ty < OverlapY; ty++) {
		//	cout << "TIPS1: " << Y + ty << "," << X + tx << endl;
		//	cout << "TIPS2: " << Besty + ty << "," << Bestx + tx << endl;
		//	cout << Besty << " " << ty << endl;
			quilting_results->at<uchar>(Y + ty, X + tx) = input_images->at<uchar>(Besty + ty, Bestx + tx);
		//	cout << "TIPS2: " << Besty + ty << "," << Bestx + tx << endl;
		}
	//	cout << "FIN " << tx << endl;
		if (tx == 0) break;
		if (tipsy == OverlapY - 1) {
			if (e2[tipsy - 1][tx - 1] < e2[tipsy][tx - 1]) {
				tipsy = tipsy - 1;
			}
			continue;
		}
		if (tipsy == 0) {
			if (e2[tipsy + 1][tx - 1] < e2[tipsy][tx - 1]) {
				tipsy = tipsy + 1;
			}
			continue;
		}
		if (e2[tipsy - 1][tx - 1] < e2[tipsy][tx - 1] && e2[tipsy - 1][tx - 1] < e2[tipsy + 1][tx - 1]) {
			tipsy = tipsy - 1;
		}
		if (e2[tipsy + 1][tx - 1] < e2[tipsy][tx - 1] && e2[tipsy + 1][tx - 1] < e2[tipsy - 1][tx - 1]) {
			tipsy = tipsy + 1;
		}
	}
	
//	cout << "FIN2" << endl;
	//�ұ�
	memset(e2, 0, sizeof(e2));
	for (int ty = Y + OverlapY; ty < Y + select_y + OverlapY; ty++) {
		for (int tx = X; tx < X + OverlapX; tx++) {
			e2[ty - Y][tx - X] = sqrt(((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X + OverlapX + select_x) - (int)colr->at<uchar>(ty - Y-OverlapY, tx - X)) * ((int)input_images->at<uchar>(Besty + ty - Y, Bestx + tx - X + OverlapX + select_x) - (int)colr->at<uchar>(ty - Y-OverlapY, tx - X)));
		}
	}
	for (int ty = 1; ty < select_y; ty++) {
		for (int tx = 0; tx < OverlapX; tx++) {
			float minn = e2[ty - 1][tx];
			if (tx != 0) minn = min(minn, e2[ty - 1][tx - 1]);
			if (tx != OverlapX - 1) minn = min(minn, e2[ty - 1][tx + 1]);
			e2[ty][tx] += minn;
		}
	}
	tipsx;//��С������
	nowmin = 1008611;
	for (int tx = 0; tx < OverlapX; tx++) {
		if (e2[select_y - 1][tx] < nowmin) {
			tipsx = tx;
			nowmin = e2[select_y - 1][tx];
		}
	}

	for (int ty = OverlapY + select_y - 1; ty > -1; ty--) {
		for (int tx = 0; tx < tipsx; tx++) {
			quilting_results->at<uchar>(Y + ty, tx) = input_images->at<uchar>(Besty + ty, Bestx + tx + OverlapX + select_x);
		}

		if (tipsx == 0) {
			if (e2[ty - 1][tipsx] > e2[ty - 1][tipsx + 1]) {
				tipsx = tipsx + 1;
			}
			continue;
		}
		if (tipsx == OverlapX - 1) {
			if (e2[ty - 1][tipsx - 1] < e2[ty - 1][tipsx]) {
				tipsx = tipsx - 1;
			}
			continue;
		}
		if (e2[ty - 1][tipsx - 1] < e2[ty - 1][tipsx] && e2[ty - 1][tipsx - 1] < e2[ty - 1][tipsx + 1]) {
			tipsx = tipsx - 1;
		}
		if (e2[ty - 1][tipsx + 1] < e2[ty - 1][tipsx] && e2[ty - 1][tipsx + 1] < e2[ty - 1][tipsx - 1]) {
			tipsx = tipsx + 1;
		}
	}
}




void ImageQuilting::ImageGenerate()
 {
	
	for (int YY = 0; YY < num_y; YY++) {
//		cout << "generate layer: " << YY + 1 << endl;
		int startY = YY * select_y - OverlapY;
		for (int XX = 0; XX < num_x; XX++) {
	//		cout << "\t generate block: " << XX + 1;
			int startX = XX * select_x - OverlapX;
			if (XX == 0 && YY == 0) {//��һ���������ѡ��
				//α�����ֱ��ѡȡ���Ͻ�
				for (int temy = 0; temy < select_y; temy++) {
					for (int temx = 0; temx < select_x; temx++) {
						quilting_results->at<uchar>(temy, temx) = input_images->at<uchar>(temy, temx);
					}
				}
				continue;
			}
			//ѡȡ���һ��
			if(XX>0)
				for (int temy = startY + OverlapY; temy < startY + select_y + OverlapY; temy++) {
					for (int temx = startX; temx < startX + OverlapX; temx++) {
						tem_coll->at<uchar>(temy - startY - OverlapY, temx - startX) = quilting_results->at<uchar>(temy, temx);
					}
				}
			//ѡȡ�ϲ�һ��
			if (YY > 0) {//����ѡȡ�е���Ϣ
				if (XX == 0) {//��һ�飬Ѱ�����Ϸ�
					for (int temy = startY; temy < startY + OverlapY; temy++) {
						for (int temx = startX+OverlapX; temx < startX + OverlapX + select_x; temx++) {
						//	cout << temy << " " << temx << endl;
							tem_rol->at<uchar>(temy - startY, temx - startX - OverlapX) = quilting_results->at<uchar>(temy, temx);
						}
					}
				}
				else {//��N��
					if (XX == num_x - 1) {//��N�����һ�飬��Ҫ���õ�һ�����Ϣ
						for (int temy = startY; temy < startY + OverlapY; temy++) {
							for (int temx = startX; temx < startX + OverlapX + select_x ; temx++) {
								tem_rol->at<uchar>(temy - startY, temx - startX) = quilting_results->at<uchar>(temy, temx);
							}
						}
					/*	for (int temy = startY; temy < startY + OverlapY; temy++) {
							for (int temx = 0; temx < OverlapX; temx++) {
								tem_rol->at<uchar>(temy - startY , temx + OverlapX + select_x) = quilting_results->at<uchar>(temy, temx);
							}
						}*/
					}
					else {//��N�������飬��Ҫǰ�����Ϣ��
						for (int temy = startY; temy < startY + OverlapY; temy++) {
							for (int temx = startX; temx < startX + OverlapX + select_x ; temx++) {
								tem_rol->at<uchar>(temy - startY , temx - startX) = quilting_results->at<uchar>(temy, temx);
							}
						}
					}
					
				}
			}
				
			//���һ���ˣ�ѡȡ���ʼ��һ��
			if (XX == num_x - 1) {
				for (int temy = startY + OverlapY; temy < startY + select_y + OverlapY; temy++) {
					for (int temx = 0; temx < OverlapX; temx++) {
						tem_colr->at<uchar>(temy - startY - OverlapY, temx) = quilting_results->at<uchar>(temy, temx);
					}
				}
			}
				
			
			if (YY == 0) {//��һ�У�ֻ������༴��
				if (XX == num_x - 1) {//��һ�е����һ����Ҫ�����Ҳ�
					Getdistance(tem_coll, tem_colr, startX, startY + OverlapY, true);
				}
				else {
					Getdistance(tem_coll, startX, startY + OverlapY);
				}
			}
			else {
				if (XX == 0) {//��һ�� ֻ��������һ��
					Getdistance(tem_rol, startX + OverlapX, startY, true);
				}
			if (XX == num_x - 1) {//���һ��
					Getdistance(tem_coll, tem_colr, tem_rol, startX, startY);
				}
				if (XX > 0 && XX < num_x - 1) {//����
				
					Getdistance(tem_coll, tem_rol, startX, startY);
				}
			}
	//		cout << " finish!" << endl;
		}
		//waitKey(1000);
	}
	
	imwrite("./result_Img/RES.jpg", *quilting_results);
}