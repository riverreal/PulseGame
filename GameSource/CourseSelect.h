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
		CourseData("Resource/test8.pld","WierdDream","Hitori Planet", "Resource/PlanetSkins/planet03.png")
	};

	struct MachineData
	{
		MachineData(std::string p, float bspeed, float cBonus, int layout)
			:path(p), baseSpeed(bspeed), comboBonus(cBonus), layoutType(layout)
		{};
		std::string path;
		float baseSpeed;
		float comboBonus;
		int layoutType;
	};

	MachineData MachineDataArray[4] =
	{
		MachineData("Packages/ships/ship01.pkg", 1.0f, 1.0f, 2),
		MachineData("Packages/ships/ship02.pkg", 1.0f, 1.0f, 1),
		MachineData("Packages/ships/ship03.pkg", 1.0f, 1.0f, 3),
		MachineData("Packages/ships/ship04.pkg", 1.0f, 1.0f, 3)
	};
}
