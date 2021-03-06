#ifndef _HELPERUTILS_H_
#define _HELPERUTILS_H_

// Configs
#define PROGRESS_BAR_WIDTH 80
#define PROGRESS_BAR_UPDATE_WAIT 10	// In miliseconds
#define SHOW_PROGRESS_BAR true

#include <chrono>
#include <iostream>
#include <thread>

#include "safeStack.h"

// util helper functions
namespace utils
{
	template<typename T>
	void displayProgressBar(SafeStack<T>& stack,
					const std::string& desc="",
					int bar_width=PROGRESS_BAR_WIDTH)
	{
		float progress = 0.0f;
		float size = stack.size;
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

			progress = float(stack.progress) / size;
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

	template<typename T>
	void printTimeDiff(std::ostream& out, T prior, T latter)
	{
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>
								(latter - prior).count();
		auto const msecs = diff % 1000;
		diff /= 1000;
		auto const secs = diff % 60;
		diff /= 60;
		auto const mins = diff % 60;
		diff /= 60;
		auto const hours = diff % 24;
		diff /= 24;
		auto const days = diff;
 
		bool printed_earlier = false;
		if(days >= 1) 
		{
			printed_earlier = true;
			out << days << "d";
		}
		if(printed_earlier || hours >= 1)
		{
			printed_earlier = true;
			out << hours << "h";
		}
		if(printed_earlier || mins >= 1)
		{
			printed_earlier = true;
			out << mins << "m";
		}

		// always print seconds
		out << secs << ",";
		out << msecs << "s";

		// adapted from https://stackoverflow.com/a/22064449
	}

	template<typename T>
	void printFancyTimeDiff(std::ostream& out, T prior, T latter)
	{
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>
								(latter - prior).count();
		auto const msecs = diff % 1000;
		diff /= 1000;
		auto const secs = diff % 60;
		diff /= 60;
		auto const mins = diff % 60;
		diff /= 60;
		auto const hours = diff % 24;
		diff /= 24;
		auto const days = diff;
 
		bool printed_earlier = false;
		if(days >= 1) 
		{
			printed_earlier = true;
			out << days << (1 != days ? " days" : " day") << ' ';
		}
		if(printed_earlier || hours >= 1)
		{
			printed_earlier = true;
			out << hours << (1 != hours ? " hours" : " hour") << ' ';
		}
		if(printed_earlier || mins >= 1)
		{
			printed_earlier = true;
			out << mins << (1 != mins ? " minutes" : " minute") << ' ';
		}
		if(printed_earlier || secs >= 1)
		{
			printed_earlier = true;
			out << secs << (1 != secs ? " seconds" : " second") << ' ';
		}
		if (printed_earlier || msecs >= 1)
		{
			printed_earlier = true;
			out << msecs << (1 != msecs ? " milliseconds" : " millisecond");
		}

		// from https://stackoverflow.com/a/22064449
	}

	template<typename T>
	void printTimingInfo(const std::string& image_name, T prior, T latter, bool fancy=false)
	{
		std::cout << image_name << " saved in ";
		if(fancy)
			printFancyTimeDiff(std::cout, prior, latter);
		else
			printTimeDiff(std::cout, prior, latter);
		std::cout << std::endl;
	}
}

#endif
