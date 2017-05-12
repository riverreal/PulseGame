#include "../Source/System/IElixirGame.h"
#include "../Source/Helper/TypeHelper.h"


enum CourseID
{
	Name0,
	Name1,
};
struct CourseData
{
	std::string path;
	std::string music;
	std::string name;

	CourseData(std::string p, std::string m, std::string n)
		:path(p), music(m), name(n)
	{};
};

CourseData CourseDataArray[2]
{
	CourseData("Path1","Music1","Name1"),
	CourseData("Path2","Music2","Name2")
};