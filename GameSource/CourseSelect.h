#pragma once

namespace
{
	enum CourseID
	{
		lars,
		dash,
		hitorigoto
	};

	struct CourseData
	{
		std::string path;
		std::string music;
		std::string name;
		std::string planetSkinPath;

		CourseData(std::string p, std::string m, std::string n, std::string pl)
			:path(p), music(m), name(n), planetSkinPath(pl)
		{};
	};

	CourseData CourseDataArray[3] =
	{
		CourseData("Resource/course03.pld","LarsM-Lovers","course03", "Resource/PlanetSkins/planet01.png"),
		CourseData("Resource/test8.pld","Dash","test8", "Resource/PlanetSkins/planet02.png"),
		CourseData("Resource/test8.pld","hitorigoto","Hitori Planet", "Resource/PlanetSkins/planet03.png")
	};
}
