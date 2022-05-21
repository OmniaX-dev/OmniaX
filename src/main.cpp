
#include <omniax/Runtime.hpp>
#include "TestApp.hpp"

constexpr int32_t Width = 32 * 40;
constexpr int32_t Height = 32 * 30;
// constexpr int32_t Width = 64 * 16;
// constexpr int32_t Height = 64 * 9;

class TestDataFile : public ox::DataFile
{
	public:
		void onDataBlockRead(ox::String instr, DFD_LIST data, int32_t paramCount) override
		{
			std::cout << instr << " (" << (int)data.size() << "):\n";
			for (auto& param : data)
			{
				std::cout << "\t" << param.name << " = " << param.asString() << " -> (type: " << (int)param.type << ")\n";
			}
			std::cout << std::endl;
		}
};

#include <omniax/utils/Serial.hpp>

int main(int argc, char** argv)
{
	// TestDataFile tdf;
	// tdf.load("res/icons.dat");
	TestApp app;
	app.create(Width, Height, "OmniaX - Test", "", 4, 0);
	/** Game Loop **/
	while (app.isRunning())
	{
		app.nextFrame();
		//break;
	}
	return 0;
}