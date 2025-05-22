#include "Lib.h"

namespace Model_Interface {
	//기본 골격임 Default CRUD이며 
	//단순히 File이라는것에 초점을 맞춘 자료구조를 제공해줌
	bool CRUD_Struct::Create(const char* Path) {

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

	bool CRUD_Struct::Parsing() {

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