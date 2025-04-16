#include <stdio.h>
#include <Windows.h>
#include "FileBuf.h"

namespace File_Handle {
	class File_Main {
		//파일 리스트 출력
	public:
		File_Main(const char* directory) {
			swprintf(path, MAX_PATH, L"%S\\*", directory);
			file_path = directory;
			file_list_num = 0;
			select_file_index = -1;
			this->File_Connect();
			print = false;
			file_buffer = nullptr;
		}
		~File_Main() {}

		//디렉토리에 대한 파일 연결
		void File_Connect() {
			hFind = INVALID_HANDLE_VALUE;
			swprintf(path, MAX_PATH, L"%S\\*", file_path);

			hFind = FindFirstFile(path, &findFileData);
			if (hFind == INVALID_HANDLE_VALUE) {
				printf("디렉토리를 열 수 없습니다: %s\n", file_path);
				return;
			}
		}

		//파일 리스트
		bool ListFiles() {
			File_Connect();
			file_list_num = 0;
			bool index_ck = false;
			do {
				// "."과 ".." 디렉토리 항목 제외 .html, .txt만 읽어오기
				if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
					wchar_t* ext = wcsrchr(findFileData.cFileName, L'.');
					if (ext != NULL) {
						if (wcscmp(ext, L".html") == 0 || wcscmp(ext, L".txt") == 0) {
							if (select_file_index > 0) {
								if (select_file_index == file_list_num+1) {
									wchar_t full_path[MAX_PATH];
									swprintf(full_path, MAX_PATH, L"%S\\%s", file_path, findFileData.cFileName);

									char full_path_char[MAX_PATH];
									wcstombs(full_path_char, full_path, MAX_PATH); // wchar_t -> char 변환
									file_buffer = new nFile::ReadFile(full_path_char);

									printf("파일을 성공적으로 읽었습니다 : %s\n", full_path_char);
									index_ck = true;
								}
							}
							
							if (print) {
								wprintf(L"%d. %s\n", file_list_num + 1, findFileData.cFileName);
							}
							file_list_num++;
						}
					}
				}
			} while (FindNextFile(hFind, &findFileData) != 0);

			FindClose(hFind);
			return index_ck;
		}

		void Files_Print() {
			print = true;
			ListFiles();
			print = false;
		}

		void Select_File(int index) {
			select_file_index = index;
			if (!ListFiles()) {//파일 읽은거 실패
				printf("선택한 파일은 목록에 없습니다.\n");
			}
		}

	private:
		WIN32_FIND_DATA findFileData;
		HANDLE hFind;
		wchar_t path[MAX_PATH];
		const char *file_path;
		int file_list_num;
		int select_file_index;
		bool print;
		nFile::ReadFile* file_buffer;
	};
}

namespace User_Interface {
	void UI_Main() {
		printf("Dom Tree 구조를 파싱해주는 프로그램입니다. \n");
		int cmd;
		const char* directory = "directory\\Dom_Tree";
		File_Handle::File_Main Root(directory);
		
		Root.Files_Print();
		Root.Select_File(1);
		Root.Select_File(10);
	}
}