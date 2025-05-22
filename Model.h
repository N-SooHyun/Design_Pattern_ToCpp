#pragma once
#ifndef MODEL
#define MODEL
#include "Lib.h"

//CRUD의 역할만 하면 되는 영역
//데이터 HDD, SDD에서 읽어와서 자료구조의 형태로 파싱만 하는 역할
//파싱된 데이터를 CRUD의 형태에 맞게 API를 제공만 해주는 역할


namespace Model_Interface {
	//반드시 필요한 인터페이스 구조

	//자료구조를 넘겨주면 해당 자료구조에 맞는 파일을 만들어줌
	class Create_File {
	public:
		Create_File() {}
		~Create_File() {}

		//자료구조 Data를 기반으로 파일을 생성
		virtual bool Create(const char* Path) = 0;
	};

	class Read_File {
	public:
		Read_File() {}
		~Read_File() {}

		//자료구조 Data를 기반으로 파일을 읽어오기(파싱이 포함됨)
		virtual bool Read(const char* Path) = 0;
	};

	class Update_File {
	public:
		Update_File() {}
		~Update_File() {}

		//자료구조 Data를 기반으로 기존파일을 덮어씌워줌
		virtual bool Update(const char* Path) = 0;
	};

	class Delete_File {
	public:
		Delete_File() {}
		~Delete_File() {}

		//자료구조와 관계없이 파일을 지워줌
		virtual bool Delete(const char* Path) = 0;
	};

	class Data_Login {
	public:
		Data_Login() {}
		~Data_Login() {}

		virtual bool Parsing() = 0;
	};

	class CRUD_Struct : public Create_File, public Read_File, public Update_File, public Delete_File, public Data_Login {
	public:
		CRUD_Struct() : Data(1024){}
		~CRUD_Struct() {}

		virtual bool Create(const char* Path);	//필수 : Path, Data
		virtual bool Read(const char* Path);	//필수 : Path, Data
		virtual bool Update(const char* Path);	//필수 : Path, Data
		virtual bool Delete(const char* Path);	//필수 : Path, Data
		virtual bool Parsing();		//필수 : Data!=null(Read할때 반드시 필요)


	private:
		nDynamic::DynamicStr Data;
	};
}

using namespace Model_Interface;

namespace Json_Struct {
	class Data_Json : public CRUD_Struct{
	public:
		Data_Json() {}
		~Data_Json() {}

		bool Create(const char* Path);
	};
}

namespace Xml_Struct {
	class Data_Xml : public CRUD_Struct{

	};
}

namespace Ini_Struct {
	class Data_Ini {

	};
}

#endif