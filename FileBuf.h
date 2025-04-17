#pragma

#ifndef FILE_BUF
#define FILE_BUF
#include <Windows.h>
#include <stdio.h>

namespace nFile {
	class ReadFile {
	public:
		FILE* pFile = NULL;
		char* pXml_Content;
		int capacity;
		int current_size_str;

		ReadFile(char* path) {
			//fopen_s(&pFile, path, "r");
			fopen_s(&pFile, path, "r");
			capacity = 1024;
			pXml_Content = new char[capacity];
			current_size_str = 0;
			CopyFile();
		}
		~ReadFile() {
			delete pXml_Content;
			fclose(pFile);
		}
		void CopyFile() {
			char c;
			char* temp;
			for (current_size_str = 0; ; current_size_str++) {
				c = fgetc(pFile);
				if (c == EOF) break;
				else if (current_size_str == capacity - 1) {
					capacity *= 2;
					temp = pXml_Content;
					pXml_Content = new char[capacity];
					for (int i = 0; i < current_size_str; i++) {
						pXml_Content[i] = temp[i];
					}
					delete temp;
				}
				pXml_Content[current_size_str] = c;
			}
			pXml_Content[current_size_str] = '\0';
			temp = new char[current_size_str + 1];
			for (int i = 0; i < current_size_str; i++) {
				temp[i] = pXml_Content[i];
			}
			temp[current_size_str] = '\0';
			capacity = current_size_str;
			delete pXml_Content;
			pXml_Content = temp;
			fclose(pFile);
		}
	};

}

namespace File_Handle {
	class File_Main {
		//파일 리스트 출력
	public:
		File_Main(const char* directory, const char* file_extension) {
			swprintf(path, MAX_PATH, L"%S\\*", directory);
			file_path = directory;
			file_list_num = 0;
			select_file_index = -1;
			this->File_Connect();
			print = false;
			file_buffer = nullptr;
			this->file_extension = file_extension;
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
			wchar_t wfile_extension[MAX_PATH];
			mbstowcs(wfile_extension, file_extension, MAX_PATH);
			do {
				// "."과 ".." 디렉토리 항목 제외 .html, .txt만 읽어오기
				if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
					wchar_t* ext = wcsrchr(findFileData.cFileName, L'.');
					if (ext != NULL) {
						//if (wcscmp(ext, L".html") == 0 || wcscmp(ext, L".txt") == 0) {
						if (wcsicmp(ext, wfile_extension) == 0) {
							if (select_file_index > 0) {
								if (select_file_index == file_list_num + 1) {
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

		nFile::ReadFile* file_buffer;
		const char* file_path;
	private:
		WIN32_FIND_DATA findFileData;
		HANDLE hFind;
		wchar_t path[MAX_PATH];
		int file_list_num;
		int select_file_index;
		bool print;
		const char* file_extension;
	};
}

#endif