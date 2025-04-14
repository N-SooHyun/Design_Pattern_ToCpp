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
			printf("���丮�� �� �� �����ϴ�: %s\n", directory);
			return;
		}

		do {
			// "."�� ".." ���丮 �׸��� ����
			if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
				wprintf(L"%s\n", findFileData.cFileName);
			}
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}




	void UI_Main() {
		printf("Dom Tree ������ �Ľ����ִ� ���α׷��Դϴ�. \n");
		printf("�о �Ľ��ϰ��� �ϴ� ������ �����Ͽ� �ּ��� \n\n");

		const char* directory = "directory\\Dom_Tree";
		ListFiles(directory);
	}
}