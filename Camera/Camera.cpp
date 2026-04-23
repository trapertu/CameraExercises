#include <opencv2/opencv.hpp>


int main()
{
	cv::Mat frame; //создание объекта класса Mat библиотеки opencv для хранения кадра 
	cv::VideoCapture video(1, cv::CAP_DSHOW); //создание объекта класса VideoCapture библиотеки opencv для получения видеопотока
	
	if (!video.isOpened()) return -1;

	while (true)
	{
		video >> frame;
		if (frame.empty()) return -2;
		imshow("Original Video", frame); //отображение кадра


		if (cv::waitKey(1) == 27) return 0;
	}
}

//коды ошибок: 
//-1: камера не открылась
//-2: камера не показывает ничего
//0: нормальное закрытие(escape)
