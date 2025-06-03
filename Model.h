#pragma once
#ifndef MODEL
#define MODEL
#include "Lib.h"

//CRUD�� ���Ҹ� �ϸ� �Ǵ� ����
//������ HDD, SDD���� �о�ͼ� �ڷᱸ���� ���·� �Ľ̸� �ϴ� ����
//�Ľ̵� �����͸� CRUD�� ���¿� �°� API�� ������ ���ִ� ����


namespace Model_Interface {
	//�ݵ�� �ʿ��� �������̽� ����

	//�ڷᱸ���� �Ѱ��ָ� �ش� �ڷᱸ���� �´� ������ �������
	class Create_File {
	public:
		Create_File() {}
		virtual ~Create_File() {}

		//�ڷᱸ�� Data�� ������� ������ ����
		virtual bool Create(const char* Path) = 0;
		virtual bool CreateWithExtension(const char* Path, const char* extension) {
			return 0;
		}
	};

	class Read_File {
	public:
		Read_File() {}
		virtual ~Read_File() {}

		//�ڷᱸ�� Data�� ������� ������ �о����(�Ľ��� ���Ե�)
		virtual bool Read(const char* Path) = 0;
	};

	class Update_File {
	public:
		Update_File() {}
		virtual ~Update_File() {}

		//�ڷᱸ�� Data�� ������� ���������� �������
		virtual bool Update(const char* Path) = 0;
	};

	class Delete_File {
	public:
		Delete_File() {}
		virtual ~Delete_File() {}

		//�ڷᱸ���� ������� ������ ������
		virtual bool Delete(const char* Path) = 0;
	};
//-----------------------------------------------------------------------------
	class Data_Login {
	public:
		Data_Login() {}
		virtual ~Data_Login() {}

		//Data�� �̸�
		virtual char* GetName() = 0;
		virtual bool SetName(char *) = 0;
		virtual bool AppendName(char *) = 0;

		//Data�� �ڷᱸ��ȭ �����(Default�� ���ڿ�)
		virtual bool Parsing() = 0;
		virtual bool SetData(char*) = 0;
		virtual char* GetData() = 0;
		virtual bool AppendData(char*) = 0;
	};
//-----------------------------------------------------------------------------
	//���� ó���� �ణ�� ó�� ������ ���� Ŭ���� ������ �������̽���
	class Logic_Ctrl {
	public:
		Logic_Ctrl(bool is_dynamic) {
			this->is_dynamic = is_dynamic;
		}
		virtual ~Logic_Ctrl() {}

		inline bool get_isdynamic() { return this->is_dynamic; }
		inline void set_isdynamic() {
			//�ʿ������� �𸣰���
		}

		enum PathStatus {
			NotFound = 0,	//INVALID_FILE_ATTRIBUTES
			IsDirectory,	//FILE_ATTRIBUTE_DIRECTORY
			IsFile			//��������
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

		//File ����
		virtual bool Create(const char* Path);	//�ʼ� : Path, Data
		virtual bool Read(const char* Path);	//�ʼ� : Path, Data
		virtual bool Update(const char* Path);	//�ʼ� : Path, Data
		virtual bool Delete(const char* Path);	//�ʼ� : Path, Data
		
		//Data �̸� ����
		virtual char* GetName();
		virtual bool SetName(char*);
		virtual bool AppendName(char*);

		//Data ����
		virtual bool Parsing();		//�ʼ� : Data!=null(Read�Ҷ� �ݵ�� �ʿ�)
		virtual bool SetData(char *);
		virtual char* GetData();
		virtual bool AppendData(char*);

		//������� ������
		virtual void Ctrl_Box(Logic_Ctrl* Ctrl);
	private:
		nDynamic::DynamicStr Data_Name;	//������ �̸�
		nDynamic::DynamicStr Data;		//������ ��
		Logic_Ctrl* Ctrl;	//�������̽� + DI ���� ���� ����
	};
}

using namespace Model_Interface;

namespace Json_Struct {
	class Json_Logic_Ctrl : public Logic_Ctrl {
		//Ȥ�� Ȯ���� �ȴٸ�
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