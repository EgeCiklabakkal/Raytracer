#include "utils.h"

void utils::displayProgressBar(SafeStack<std::pair<float, float>>& pixels)
{
	float progress = 0.0f;
	float size = pixels.size;
	int pos;

	while(progress < 1.0f)
	{
		std::cout << "[";
		pos = PROGRESS_BAR_WIDTH * progress;
		for(int i = 0; i < PROGRESS_BAR_WIDTH; i++)
		{
			if(i < pos) 
				std::cout << "=";
			else if(i == pos) 
				std::cout << ">";
			else 
				std::cout << " ";
		}
		std::cout << "] " << int(progress * 100.0) << "%\r";
		std::cout.flush();

		// we don't want this thread to hog the CPU, sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		progress = float(pixels.progress) / size;
	}
	// %100
	std::cout << "[";
	for(int i = 0; i < PROGRESS_BAR_WIDTH; i++)
	{
			std::cout << "=";
	}
	std::cout << "] " << 100 << "%" << std::endl;

	// adapted from https://stackoverflow.com/a/14539953
}
