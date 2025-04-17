#include <stdio.h>
#include <Windows.h>
#include "FileBuf.h"

using namespace File_Handle;

namespace User_Interface {
	void UI_Main() {
		printf("Dom Tree 구조를 파싱해주는 프로그램입니다. \n");
		int cmd;
		const char* directory = "directory\\Dom_Tree";
		File_Handle::File_Main Root(directory, ".html");
		
		Root.Files_Print();
		Root.Select_File(1);
		Root.Select_File(10);
	}
}