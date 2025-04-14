#include <stdio.h>
#include <Windows.h>

namespace User_Interface {
	void ListFiles(const char* directory) {
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		wchar_t path[MAX_PATH];
		swprintf(path, MAX_PATH, L"%S\\*", directory);

		hFind = FindFirstFile(path, &findFileData);

		if (hFind == INVALID_HANDLE_VALUE) {
			printf("디렉토리를 열 수 없습니다: %s\n", directory);
			return;
		}

		do {
			// "."과 ".." 디렉토리 항목을 제외
			if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
				wprintf(L"%s\n", findFileData.cFileName);
			}
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}




	void UI_Main() {
		printf("Dom Tree 구조를 파싱해주는 프로그램입니다. \n");
		printf("읽어서 파싱하고자 하는 파일을 선택하여 주세요 \n\n");

		const char* directory = "directory\\Dom_Tree";
		ListFiles(directory);
	}
}