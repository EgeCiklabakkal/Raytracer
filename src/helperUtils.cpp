#include "helperUtils.h"

void utils::displayProgressBar(SafeStack<std::pair<float, float>>& pixels,
				const std::string& desc, int bar_width)
{
	float progress = 0.0f;
	float size = pixels.size;
	int pos;
	std::string info = (desc != "") ? (desc + " : ") : "";

	while(progress < 1.0f)
	{
		std::cout << info << "[";
		pos = bar_width * progress;
		for(int i = 0; i < bar_width; i++)
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
		std::this_thread::sleep_for(std::chrono::milliseconds(PROGRESS_BAR_UPDATE_WAIT));

		progress = float(pixels.progress) / size;
	}
	// %100
	std::cout << info << "[";
	for(int i = 0; i < bar_width; i++)
	{
			std::cout << "=";
	}
	std::cout << "] " << 100 << "%" << std::endl;

	// adapted from https://stackoverflow.com/a/14539953
}
