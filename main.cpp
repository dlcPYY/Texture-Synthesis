#include<opencv2\opencv.hpp>   
#include<opencv2\highgui\highgui.hpp>
#include"ImageQuilting.h" 
#include"time.h"
using namespace std;
using namespace cv;

int main()
{
	clock_t start, finish;
	start = clock();
	ImageQuilting thepic("D:/PROJECT/Handcrafted 3d printing/TestImg/20.png",20 ,20,20,20);
	thepic.ImageGenerate();
	finish = clock();
	double Total_time = (double)(finish - start);
	printf("\n��������ʱ�䣺%0.3f���� \n", Total_time);

}