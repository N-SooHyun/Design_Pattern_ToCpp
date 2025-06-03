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
		virtual ~Create_File() {}

		//자료구조 Data를 기반으로 파일을 생성
		virtual bool Create(const char* Path) = 0;
		virtual bool CreateWithExtension(const char* Path, const char* extension) {
			return 0;
		}
	};

	class Read_File {
	public:
		Read_File() {}
		virtual ~Read_File() {}

		//자료구조 Data를 기반으로 파일을 읽어오기(파싱이 포함됨)
		virtual bool Read(const char* Path) = 0;
	};

	class Update_File {
	public:
		Update_File() {}
		virtual ~Update_File() {}

		//자료구조 Data를 기반으로 기존파일을 덮어씌워줌
		virtual bool Update(const char* Path) = 0;
	};

	class Delete_File {
	public:
		Delete_File() {}
		virtual ~Delete_File() {}

		//자료구조와 관계없이 파일을 지워줌
		virtual bool Delete(const char* Path) = 0;
	};
//-----------------------------------------------------------------------------
	class Data_Login {
	public:
		Data_Login() {}
		virtual ~Data_Login() {}

		//Data의 이름
		virtual char* GetName() = 0;
		virtual bool SetName(char *) = 0;
		virtual bool AppendName(char *) = 0;

		//Data를 자료구조화 만들기(Default는 문자열)
		virtual bool Parsing() = 0;
		virtual bool SetData(char*) = 0;
		virtual char* GetData() = 0;
		virtual bool AppendData(char*) = 0;
	};
//-----------------------------------------------------------------------------
	//예외 처리등 약간의 처리 로직에 대한 클래스 의존성 인터페이스들
	class Logic_Ctrl {
	public:
		Logic_Ctrl(bool is_dynamic) {
			this->is_dynamic = is_dynamic;
		}
		virtual ~Logic_Ctrl() {}

		inline bool get_isdynamic() { return this->is_dynamic; }
		inline void set_isdynamic() {
			//필요할지는 모르겠음
		}

		enum PathStatus {
			NotFound = 0,	//INVALID_FILE_ATTRIBUTES
			IsDirectory,	//FILE_ATTRIBUTE_DIRECTORY
			IsFile			//정상파일
		};

		enum ExcepStatus {
			Fail = 0,
			Success
		};

		enum DataStatus {
			NoData=0,
			YesData,
			NullData
		};

		virtual PathStatus Excep_Path(const char* path);
		virtual DataStatus Excep_Data(nDynamic::DynamicStr* Data);
	private:
		bool is_dynamic;
	};
//-----------------------------------------------------------------------------
	class CRUD_Struct : public Create_File, public Read_File, public Update_File, public Delete_File, public Data_Login {
	public:
		CRUD_Struct() :Data_Name(1024), Data(1024), Ctrl(nullptr) {}
		virtual ~CRUD_Struct() { 
			if (Ctrl != nullptr) {
				if (Ctrl->get_isdynamic()) {
					delete Ctrl;
				}
			}
		}

		//File 단위
		virtual bool Create(const char* Path);	//필수 : Path, Data
		virtual bool Read(const char* Path);	//필수 : Path, Data
		virtual bool Update(const char* Path);	//필수 : Path, Data
		virtual bool Delete(const char* Path);	//필수 : Path, Data
		
		//Data 이름 단위
		virtual char* GetName();
		virtual bool SetName(char*);
		virtual bool AppendName(char*);

		//Data 단위
		virtual bool Parsing();		//필수 : Data!=null(Read할때 반드시 필요)
		virtual bool SetData(char *);
		virtual char* GetData();
		virtual bool AppendData(char*);

		//제어로직 리모컨
		virtual void Ctrl_Box(Logic_Ctrl* Ctrl);
	private:
		nDynamic::DynamicStr Data_Name;	//파일의 이름
		nDynamic::DynamicStr Data;		//데이터 값
		Logic_Ctrl* Ctrl;	//인터페이스 + DI 결합 로직 구현
	};
}

using namespace Model_Interface;

namespace Json_Struct {
	class Json_Logic_Ctrl : public Logic_Ctrl {
		//혹여 확장이 된다면
	};

	class Data_Json : public CRUD_Struct{
	public:
		Data_Json() {
			
		}
		virtual ~Data_Json() {}

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