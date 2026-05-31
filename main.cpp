#define SDL_MAIN_HANDLED
#include "application/application.h"

int main(int argc, char** argv)
{
	Application::instance()->init(argc, argv);

	return Application::instance()->run(argc,argv);
}
