#include "Lib.h"

namespace Model_Interface {
	//기본 골격임 Default CRUD이며 
	//단순히 File이라는것에 초점을 맞춘 자료구조를 제공해줌
	bool CRUD_Struct::Create(const char* Path) {
		
		if (Ctrl->Excep_Path(Path)) {	//Path에 파일이 있음

		}
		else {

		}
		return 0;
	}

	bool CRUD_Struct::Read(const char* Path) {
		
		return 0;
	}

	bool CRUD_Struct::Update(const char* Path) {
		return 0;
	}

	bool CRUD_Struct::Delete(const char* Path) {
		return 0;
	}

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

	inline void CRUD_Struct::Ctrl_Box(Logic_Ctrl* Ctrl) {
		this->Ctrl = Ctrl;
	}

	//데이터를 처리해주는 약간의 로직들을 구분
	bool Logic_Ctrl::Excep_Path(const char* path) {
		DWORD attrib = GetFileAttributesA(path);
		if (attrib == INVALID_FILE_ATTRIBUTES) {
			//경로나 파일이 존재하지 않음
		}
		else if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
			//디렉토리 경로임(폴더)
		}
		else {
			//특정 파일대상임
		}
	}

	bool Logic_Ctrl::Excep_Data(nDynamic::DynamicStr* Data) {

		return 0;
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