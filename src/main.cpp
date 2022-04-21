#include <omniax/Runtime.hpp>
#include "TestApp.hpp"

constexpr int32_t Width = 64 * 16;
constexpr int32_t Height = 64 * 9;

int main(int argc, char** argv)
{
	TestApp app;
	app.create(Width, Height, "OmniaX - Test", "res/icon64.png");
	/** Game Loop **/
	while (app.isRunning())
	{
		app.nextFrame();
	}
	return 0;
}