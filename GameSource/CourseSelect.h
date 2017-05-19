namespace
{
	enum CourseID
	{
		course03,
		test8,
		hitorigoto
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

	CourseData CourseDataArray[3] =
	{
		CourseData("Resource/course03.pld","LarsM-Lovers","course03"),
		CourseData("Resource/test8.pld","Dash","test8"),
		CourseData("Resource/test8.pld","hitorigoto","Hitori Planet")
	};
}
