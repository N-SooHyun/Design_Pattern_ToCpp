#include "Lib.h"

namespace Model_Interface {
	//기본 골격임 Default CRUD이며 
	//단순히 File이라는것에 초점을 맞춘 자료구조를 제공해줌
	bool CRUD_Struct::Create(const char* Path) {
		//1. 생성은 곧 파일이 없어야함(폴더경로여야하는거임)
		//2. 생성을 할려면 Data에 값이 존재해야함
		if (Ctrl->Excep_Path(Path) == Ctrl->IsDirectory) {
			if (Ctrl->Excep_Data(&Data) == Ctrl->YesData) {
				//비로소 생성이 가능
			}
			printf("저장하고자 하는 데이터가 없습니다.\n");
			return Ctrl->Fail;
		}
		else{
			printf("생성하고자 하는 폴더의 경로를 올바르게 작성해주세요\n");
			return Ctrl->Fail;
		}
	}

	bool CRUD_Struct::Read(const char* Path) {
		//읽는것은 곧 파일이어야함
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {

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

	//File이 아닌 자료구조 Data에 초점을 맞춘 메소드들
	bool CRUD_Struct::Parsing() {

		return 0;
	}

	bool CRUD_Struct::SetData() {

		return 0;
	}

	bool CRUD_Struct::GetData() {

		return 0;
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
		//path가 "C:\Users\NSH\Documents" 이런식으로 있으면 에러나니까 "R(C:\Users\NSH\Documents)" 이렇게 하도록

		DWORD attrib = GetFileAttributesA(path);

		if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
			//디렉토리 경로임(폴더)
			return PathStatus::IsDirectory;
		}
		else if (attrib == INVALID_FILE_ATTRIBUTES) {
			//경로나 파일이 존재하지 않음
			return PathStatus::NotFound;
		}
		else {
			//특정 파일대상임
			return PathStatus::IsFile;
		}
	}

	Logic_Ctrl::DataStatus Logic_Ctrl::Excep_Data(nDynamic::DynamicStr* Data) {
		//데이터가 있는지 판단
		if (Data->p_d_str[0] == '\0') {
			return DataStatus::NoData;
		}
		else
			return DataStatus::YesData;
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