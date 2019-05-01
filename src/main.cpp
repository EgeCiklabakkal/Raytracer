#include "utils.h"

int main(int argc, char* argv[])
{
	Scene scene;

	try	// load and render scene
	{
		scene.loadFromXML(argv[1]);
		scene.renderImages(DEFAULT_THREAD_COUNT, SHOW_PROGRESS_BAR);
	}

	catch(std::exception& e)
	{
		std::cerr << "Exception occured: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
