 #pragma once
 #include "Screen.h"
 #include "Button.h"
 #include <vector>
 #include <string>
 #include <filesystem>
 
 struct SaveEntry {
	 std::string name;      // file name without extension
	 std::string path;      // full path
	 std::filesystem::file_time_type time; // last write time
 };
 
 class SaveScreen : public Screen {
 private:
	 std::vector<SaveEntry> saves;
	 std::vector<Button> saveButtons;
	 Button backButton;
	 int scrollOffset = 0; // not yet used; simple paging possible later
	 int selectedIndex = -1;
 
	 void ScanSaves();
 public:
	 SaveScreen(ScreenController* controller);
	 void Update() override;
	 void Draw() override;
 };
 