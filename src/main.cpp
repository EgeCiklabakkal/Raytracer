#include "utils.h"

int main(int argc, char* argv[])
{
	Scene scene;
	scene.loadFromXML(argv[1]);
	scene.renderImages(DEFAULT_THREAD_COUNT, SHOW_PROGRESS_BAR);

	return 0;
}
