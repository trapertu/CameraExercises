#include <opencv2/opencv.hpp>
#include <iostream> 

int main()
{
	system("chcp 1251 > nul"); // установка кодировки для нормального вывода русского языка в консоль

	cv::Mat frame, frameBGR, maskHSV, frameLAB; // создание объекта класса Mat библиотеки opencv для хранения кадра 
	cv::Mat maskA, maskB, maskLab, maskLabHSV; // создание двух объектов класса Mat библиотеки opencv для бинарных масок после поиска
	cv::VideoCapture video(0, cv::CAP_DSHOW); // создание объекта класса VideoCapture библиотеки opencv для получения видеопотока
	std::vector<cv::Mat> vectorFrameLab; // создание динамического массива для хранения каналов lab
	
	int Hmin = 10, Smin = 50, Vmin = 20, Hmax = 35; // создание переменных для хранения значений, используемых для проверки вхождения в HSV
	int aMin = 135, aMax = 170, bMin = 135, bMax = 180; // создание переменных для хранения значений, используемых для проверки вхождения в Lab

	if (!video.isOpened()) return -1; // проверка на нахождение камеры

	while (true)
	{
		video >> frameBGR; // передача изображение из видеопотока в хранение
		if (frameBGR.empty()) return -2; // проверка на выполнение передачи
		
		cv::cvtColor(frameBGR, maskHSV, cv::COLOR_BGR2HSV); // преобразование оригинального кадра в Hsv
		cv::GaussianBlur(maskHSV, maskHSV, cv::Size(15, 15), 0); // размыливание картинки за ненужностью идеального качества
		cv::inRange(maskHSV,
					cv::Scalar(Hmin, Smin, Vmin),
					cv::Scalar(Hmax, 255, 255),
					maskHSV); // проверка вхождения нужного цвета в HSV кадр

		cv::cvtColor(frameBGR, frameLAB, cv::COLOR_BGR2Lab); // преобразование оригинального кадра в Lab
		cv::split(frameLAB, vectorFrameLab); // разделение Lab кадра на потоки
		cv::inRange(vectorFrameLab[1],
					aMin,
					aMax,
					maskA); // проверка вхождения нужного цвета в поток a из Lab
		cv::inRange(vectorFrameLab[2],
			bMin,
			bMax,
			maskB); // проверка вхождения нужного цвета в поток b из Lab

		maskLab = maskA & maskB; // объединение найденных потоков a и b
		maskLabHSV = maskLab | maskHSV; // объединение масок HSV и Lab

		imshow("Original Video", frameBGR); // отображение оригинального кадра
		imshow("HSV Video", maskHSV); // отображение HSV кадра
		imshow("Mask Video", maskLabHSV); // отображение итоговой маски
		imshow("Lab Video", maskLab); // отображение Lab кадра
		if (cv::waitKey(1) == 27) return 0; // таймер на 1 мс и проверка на нажатие escape для выхода
	}
}

//коды ошибок: 
//-1: камера не открылась
//-2: камера не показывает ничего
//0: нормальное закрытие(escape)
