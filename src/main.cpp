#include "rgb.h"

int main(int argc, char* argv[])
{
	rgb c1(0.5, 0.2, 0.1);
	rgb c2(0.1, 0.5, 0.2);

	std::cout << c1*c2 << std::endl;
	return 0;
}
