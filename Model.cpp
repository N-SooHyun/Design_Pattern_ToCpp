#include "Lib.h"

namespace Model_Interface {

	

	//기본 골격임 Default CRUD이며 
	//단순히 File이라는것에 초점을 맞춘 자료구조를 제공해줌
	//기본골격이기 때문에 파일 확장자는 자유롭게 만들어줄 예정임
	bool CRUD_Struct::Create(const char* Path) {
		//1. 생성은 곧 파일이 없어야함(폴더경로여야하는거임)
		//2. 생성을 할려면 Data에 값이 존재해야함
		int FileStatus = Ctrl->Excep_Path(Path);
		if (FileStatus == Ctrl->IsDirectory) {
			if (Ctrl->Excep_Data(&Data) == Ctrl->YesData) {
				//비로소 생성이 가능
				//Data를 기반으로 생성해야함
				size_t len = std::strlen(Path);
				nDynamic::DynamicStr fullPath(1024);
				fullPath.OperStr(Path);
				
				if (Path[len - 1] != '\\') {
					fullPath.AddWord("\\");
				}

				char* Extension = GetExtension();

				if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NoData) {
					printf("기존 데이터의 Name이 존재하지 않습니다.\n");
					return Ctrl->Fail;
				}

				fullPath.AddStr(Data_Name.p_d_str);
				fullPath.AddStr(Extension);

				printf("%s\n", fullPath.p_d_str);

				if (!Ctrl->WinFileCreate(fullPath.p_d_str, &Data)) {
					//파일생성 실패
					return Ctrl->Fail;
				}


				return Ctrl->Success;
			}
			printf("저장하고자 하는 데이터가 없습니다.\n");
			return Ctrl->Fail;
		}
		else if (FileStatus == Ctrl->IsFile) {
			//파일인경우 파일이 이미 존재한다는거임
			char s;
			printf("이미 해당 파일이 존재합니다.\n");
			printf("해당 파일을 덮어생성할까요?(y,Y or n,N) : ");
			scanf_s("%c", &s);

			switch (s) {
			case 'n': case 'N':
				printf("파일 생성을 취소합니다.\n");
				return Ctrl->Fail;
			case 'y': case 'Y':
				break;
			default:
				printf("잘못 선택하셨습니다.\n");
				printf("파일 생성을 취소합니다.\n");
				return Ctrl->Fail;
			}


			//파일 덮어쓰기
			printf("덮어씌울게요~~");
			if (!Ctrl->WinFileCreate(Path, &Data)) {
				//파일생성 실패
				return Ctrl->Fail;
			}

			

		}
		else if (FileStatus == Ctrl->NewFile) {
			//파일인경우인데 파일이 존재하지 않는경우
			printf("파일이 존재하지 않는 경로입니다.\n");
			printf("해당 경로로 파일을 만들어드릴게요\n");
			if (!Ctrl->WinFileCreate(Path, &Data)) {
				//파일생성 실패
				return Ctrl->Fail;
			}

			

		}
		else{
			printf("생성하고자 하는 폴더의 경로를 올바르게 작성해주세요\n");
			return Ctrl->Fail;
		}
	}

	bool CRUD_Struct::Read(const char* Path) {
		//읽는것은 무조건 파일이어야 함
		//1. 기존데이터에 무조건 덮어쓰기? 혹은 Append?
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			if (Ctrl->WinFileRead(Path, &Data, &Data_Name) == Ctrl->Fail) {
				printf("읽기 실패\n");
				return Ctrl->Fail;
			}



		}
		else
			return Ctrl->Fail;
	}

	bool CRUD_Struct::Update(const char* Path) {
		//수정해주는것은 곧 파일이어야함(덮어쓰기)
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			//
		}
		else
			return Ctrl->Fail;

	}

	bool CRUD_Struct::Delete(const char* Path) {
		//삭제해주는것은 곧 파일이어야함
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			if (DeleteFileA(Path)) {
				printf("파일 삭제 성공\n");
			}
			else {
				printf("파일 삭제 실패 : %d\n", GetLastError());
			}
		}
		else {
			printf("삭제하고자 하는 파일의 경로를 올바르게 작성해주세요\n");
			return Ctrl->Fail;
		}
	}

	



//---------------------------------------------Data Logic----------------------------------------------------
	//Data의 이름영역
	char* CRUD_Struct::GetName() {
		if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NoData) {
			printf("이름이 없습니다.\n");
			return Data_Name.p_d_str;
		}
		else if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NullData) {
			printf("문자열이 Null값입니다.\n");
			return nullptr;
		}
		return Data_Name.p_d_str;
	}

	bool CRUD_Struct::SetName(char* new_Name) {
		if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NullData) {
			printf("문자열이 Null값입니다.\n");
			return 0;
		}

		if (new_Name == nullptr) {
			printf("매개변수가 잘못된 값입니다.\n");
			return 0;
		}

		Data_Name.DeleteStr();

		Data_Name.OperStr(new_Name);

		return 1;
	}

	bool CRUD_Struct::AppendName(char* new_Name) {
		if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NullData) {
			printf("문자열이 Null값입니다.\n");
			return 0;
		}
		
		if (new_Name == nullptr) {
			printf("매개변수가 Null 값입니다.\n");
			return 0;
		}
		Data_Name.UpdateStr(new_Name);
		return 1;
	}

	//File이 아닌 자료구조 Data에 초점을 맞춘 메소드들
	bool CRUD_Struct::Parsing() {

		return 0;
	}

	bool CRUD_Struct::SetData(char* new_Data) {
		//덮어쓰기
		if (Ctrl->Excep_Data(&Data) == Ctrl->NullData) {
			printf("문자열이 Null값입니다.\n");
			return 0;
		}
		
		if (new_Data == nullptr) {
			printf("매개변수가 Null 값입니다.\n");
			return 0;
		}



		//Set이라함은 곧 덮어쓰기
		Data.DeleteStr();

		Data.OperStr(new_Data);

		return 1;
	}

	bool CRUD_Struct::AppendData(char* new_Data) {
		//이어쓰기
		if (Ctrl->Excep_Data(&Data) == Ctrl->NullData) {
			printf("문자열이 Null값입니다.\n");
			return 0;
		}

		if (new_Data == nullptr) {
			printf("매개변수가 Null 값입니다.\n");
			return 0;
		}
		
		Data.UpdateStr(new_Data);
		return 1;
	}

	char* CRUD_Struct::GetData() {
		if (Ctrl->Excep_Data(&Data) == Ctrl->NoData) {
			printf("이름이 없습니다.\n");
			return Data_Name.p_d_str;
		}
		else if (Ctrl->Excep_Data(&Data) == Ctrl->NullData) {
			printf("문자열이 Null값입니다.\n");
			return nullptr;
		}

		return Data.p_d_str;
	}

	void CRUD_Struct::Ctrl_Box(Logic_Ctrl* Ctrl) {
		this->Ctrl = Ctrl;
	}

//---------------------------------------------Logic Ctrl----------------------------------------------------

	//데이터를 처리해주는 약간의 로직들을 구분
	Logic_Ctrl::PathStatus Logic_Ctrl::Excep_Path(const char* path) {
		//path 작성 예시
		//상대경로  "..\\Access_File_Security_Git\\Xml\\test.txt"
		//절대경로 "C:\\Users\\NSH\\Documents\\Visual_studio_pro\\Design_Pattern_ToCpp"
		//path가 "C:\Users\NSH\Documents" 이런식으로 있으면 에러나니까 R"(C:\Users\NSH\Documents)" 이렇게 하도록

		DWORD attrib = GetFileAttributesA(path);

		if (attrib == INVALID_FILE_ATTRIBUTES) {
			//경로나 파일이 존재하지 않음
			//파일 경로일때는 이러한 파일을 만들고 싶다는 의미가 될수도 있음
			size_t len = std::strlen(path);
			nDynamic::DynamicStr fullPath(1024);
			fullPath.OperStr(path);
			len--;
			for (; ;) {
				if (fullPath.p_d_str[len] == '\\') {
					break;
				}
				fullPath.DelWord();
				len--;
			}
			
			attrib = GetFileAttributesA(fullPath.p_d_str);

			if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
				//상위 폴더 존재함
				//하위 존재가 파일인지 폴더인지 확장자를 통해서 찾아야함
				len = std::strlen(path);
				for (;; len--) {
					if (path[len] == '.') {
						//하위 존재가 파일임 
						return PathStatus::NewFile;
					}
					else if (path[len] == '\\') {
						//하위 존재가 폴더임
						return PathStatus::NotFound;
					}
				}

			}
			else {
				//상위 폴더 자체가 없음 완전히 경로가
				return PathStatus::NotFound;
			}

			//폴더의 경로일때는 NotFound하는게 맞음
			return PathStatus::NotFound;

		}
		else {
			//경로가 존재하긴 함(파일인지 폴더인지 모름)
			if (attrib & FILE_ATTRIBUTE_DIRECTORY)
				//폴더임
				return PathStatus::IsDirectory;
			else
				//파일임
				return PathStatus::IsFile;
		}
	}

	Logic_Ctrl::DataStatus Logic_Ctrl::Excep_Data(nDynamic::DynamicStr* Data) {
		//데이터가 있는지 판단
		if (Data->p_d_str[0] == '\0') {
			return DataStatus::NoData;
		}
		else if (Data->p_d_str == nullptr) {
			return DataStatus::NullData;
		}
		else
			return DataStatus::YesData;
	}

	Logic_Ctrl::ExcepStatus Logic_Ctrl::WinFileCreate(const char* path, const nDynamic::DynamicStr* Data) {
		HANDLE hFile = CreateFileA(
			path,			//파일경로
			GENERIC_WRITE,				//쓰기권한
			0,							//공유 안함
			NULL,						//기본 보안 속성
			CREATE_ALWAYS,				//항상 덮어쓰기
			FILE_ATTRIBUTE_NORMAL,		//일반 파일 속성
			NULL						//템플릿 없음
		);


		if (hFile == INVALID_HANDLE_VALUE) {
			printf("파일 생성 실패: ");
			std::cerr << GetLastError() << std::endl;
			return Fail;
		}

		DWORD written;

		if (!WriteFile(hFile, Data->p_d_str, Data->capacity_str, &written, NULL)) {
			DWORD err = GetLastError();
			std::cerr << "파일 Write 실패 에러 코드 : " << err << std::endl;
			return Fail;
		}

		CloseHandle(hFile);

		return Success;
	}


	Logic_Ctrl::ExcepStatus Logic_Ctrl::WinFileRead(const char* path, const nDynamic::DynamicStr* Data, const nDynamic::DynamicStr* Data_Name) {
		HANDLE hFile = CreateFileA(
			path,			//파일경로
			GENERIC_READ,				//쓰기권한
			0,							//공유 안함
			NULL,						//기본 보안 속성
			OPEN_EXISTING,				//읽기 전용으로 열기
			FILE_ATTRIBUTE_NORMAL,		//일반 파일 속성
			NULL						//템플릿 없음
		);


		if (hFile == INVALID_HANDLE_VALUE) {
			printf("파일 읽기 실패: ");
			std::cerr << GetLastError() << std::endl;
			return Fail;
		}

		DWORD read;

		if (!ReadFile(hFile, Data->p_d_str, Data->capacity_str, &read, NULL)) {
			DWORD err = GetLastError();
			std::cerr << "파일 오버로딩 실패 에러 코드 : " << err << std::endl;
			return Fail; 
		}

		Data->p_d_str[read] = '\0';

		CloseHandle(hFile);

		return Success;
	}

}

namespace Json_Struct {
	bool Data_Json::Create(const char* Path) {
		
		return 0;
	}
}

namespace Xml_Struct {

}

namespace Ini_Struct {

}