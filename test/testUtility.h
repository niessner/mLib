class TestUtility : public Test
{
public:
	void test0()
	{
		std::ofstream file("fileTest.txt");
		file << "01234" << std::endl << "56789" << std::endl;
		file.close();

		util::makeDirectory("testDir");
		util::copyFile("fileTest.txt", "testDir/output.txt");

		MLIB_ASSERT_STR(util::getFileData("testDir/output.txt").size() == 14, "File test0 failed");
		MLIB_ASSERT_STR(util::getFileLines("testDir/output.txt")[1] == "56789", "File test1 failed");
		MLIB_ASSERT_STR(util::getFileSize("testDir/output.txt") == util::getFileData("testDir/output.txt").size(), "File test1 failed");
		
		std::string data = "test data here";
		util::copyStringToClipboard(data);
		MLIB_ASSERT_STR(util::loadStringFromClipboard() == data, "Clipboard test failed");

		Directory dir("testDir");
		auto filesWithSuffic = dir.filesWithSuffix(".txt");
		MLIB_ASSERT_STR(
			std::find(dir.files().begin(), dir.files().end(), "output.txt") != dir.files().end() &&
			std::find(filesWithSuffic.begin(), filesWithSuffic.end(), "output.txt") != filesWithSuffic.end(), 
			"Directory failed");


		Console::log("util tests passed");
	}

	std::string name()
	{
		return "utility";
	}
};