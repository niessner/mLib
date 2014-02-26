class TestUtility : public Test
{
public:
	void test0()
	{
		std::ofstream file("fileTest.txt");
		file << "01234" << std::endl << "56789" << std::endl;
		file.close();

		Utility::makeDirectory("testDir");
		Utility::copyFile("fileTest.txt", "testDir/output.txt");

		MLIB_ASSERT_STR(Utility::getFileData("testDir/output.txt").size() == 14, "File test0 failed");
		MLIB_ASSERT_STR(Utility::getFileLines("testDir/output.txt")[1] == "56789", "File test1 failed");
		MLIB_ASSERT_STR(Utility::getFileSize("testDir/output.txt") == Utility::getFileData("testDir/output.txt").size(), "File test1 failed");
		
		std::string data = "test data here";
		Utility::copyStringToClipboard(data);
		MLIB_ASSERT_STR(Utility::loadStringFromClipboard() == data, "Clipboard test failed");

		Directory dir("testDir");
		MLIB_ASSERT_STR(dir.files().contains("output.txt") && dir.filesWithSuffix(".txt").contains("output.txt"), "Directory failed");


		Console::log("Utility tests passed");
	}

	std::string name()
	{
		return "utility";
	}
};