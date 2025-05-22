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

	class Data_Login {
	public:
		Data_Login() {}
		virtual ~Data_Login() {}

		//Data�� �ڷᱸ��ȭ �����(Default�� ���ڿ�)
		virtual bool Parsing() = 0;
		virtual bool SetData() = 0;
		virtual bool GetData() = 0;

	};

	//���� ó���� �ణ�� ó�� ������ ���� Ŭ���� ������ �������̽���
	class Logic_Ctrl {
	public:
		Logic_Ctrl() {}
		virtual ~Logic_Ctrl() {}
		virtual bool Excep_Path(const char* path);
		virtual bool Excep_Data(nDynamic::DynamicStr* Data);
	};

	class CRUD_Struct : public Create_File, public Read_File, public Update_File, public Delete_File, public Data_Login {
	public:
		CRUD_Struct() : Data(1024), Ctrl(nullptr) {}
		virtual ~CRUD_Struct() { 
			if(Ctrl != nullptr)
				delete Ctrl;
		}

		//File ����
		virtual bool Create(const char* Path);	//�ʼ� : Path, Data
		virtual bool Read(const char* Path);	//�ʼ� : Path, Data
		virtual bool Update(const char* Path);	//�ʼ� : Path, Data
		virtual bool Delete(const char* Path);	//�ʼ� : Path, Data
		
		//Data ����
		virtual bool Parsing();		//�ʼ� : Data!=null(Read�Ҷ� �ݵ�� �ʿ�)
		virtual bool SetData();
		virtual bool GetData();

		//������� ������
		virtual void Ctrl_Box(Logic_Ctrl* Ctrl);
	private:
		nDynamic::DynamicStr Data;
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
			Ctrl_Box(new Json_Logic_Ctrl);
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