#include <stdio.h>
#include <Windows.h>
#include "FileBuf.h"

using namespace File_Handle;

namespace User_Interface {
	void UI_Main() {
		printf("Dom Tree ������ �Ľ����ִ� ���α׷��Դϴ�. \n");
		int cmd;
		const char* directory = "directory\\Dom_Tree";
		File_Handle::File_Main Root(directory, ".html");
		
		Root.Files_Print();
		Root.Select_File(1);
		Root.Select_File(10);
	}
}