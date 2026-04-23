#include <opencv2/opencv.hpp>
#include <iostream> 

int main()
{
	system("chcp 1251 > nul");

	cv::Mat frame, frameBGR, maskHSV, frameLAB; // создание объекта класса Mat библиотеки opencv для хранения кадра 
	cv::Mat maskA, maskB, maskLab, maskLabHSV; // создание двух объектов класса Mat библиотеки opencv для бинарных масок после поиска
	cv::VideoCapture video(0, cv::CAP_DSHOW); // создание объекта класса VideoCapture библиотеки opencv для получения видеопотока
	std::vector<cv::Mat> vectorFrameLab; // создание динамического массива для хранения каналов lab
	
	int Hmin = 10, Smin = 50, Vmin = 20, Hmax = 35;
	int aMin = 135, aMax = 170, bMin = 135, bMax = 180;

	if (!video.isOpened()) return -1;

	while (true)
	{
		video >> frameBGR;
		if (frameBGR.empty()) return -2;
		
		cv::cvtColor(frameBGR, maskHSV, cv::COLOR_BGR2HSV);
		cv::GaussianBlur(maskHSV, maskHSV, cv::Size(15, 15), 0);
		cv::inRange(maskHSV,
					cv::Scalar(Hmin, Smin, Vmin),
					cv::Scalar(Hmax, 255, 255),
					maskHSV);

		cv::cvtColor(frameBGR, frameLAB, cv::COLOR_BGR2Lab);
		cv::split(frameLAB, vectorFrameLab);
		cv::inRange(vectorFrameLab[1],
					aMin,
					aMax,
					maskA);
		cv::inRange(vectorFrameLab[2],
			bMin,
			bMax,
			maskB);

		maskLab = maskA & maskB;
		maskLabHSV = maskLab | maskHSV;

		imshow("Original Video", frameBGR); //отображение оригинального кадра
		imshow("HSV Video", maskHSV); //отображение HSV кадра
		imshow("Mask Video", maskLabHSV);
		imshow("Lab Video", maskLab);
		if (cv::waitKey(1) == 27) return 0;
	}
}

//коды ошибок: 
//-1: камера не открылась
//-2: камера не показывает ничего
//0: нормальное закрытие(escape)
