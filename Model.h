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
		~Create_File() {}

		//�ڷᱸ�� Data�� ������� ������ ����
		virtual bool Create(const char* Path) = 0;
	};

	class Read_File {
	public:
		Read_File() {}
		~Read_File() {}

		//�ڷᱸ�� Data�� ������� ������ �о����(�Ľ��� ���Ե�)
		virtual bool Read(const char* Path) = 0;
	};

	class Update_File {
	public:
		Update_File() {}
		~Update_File() {}

		//�ڷᱸ�� Data�� ������� ���������� �������
		virtual bool Update(const char* Path) = 0;
	};

	class Delete_File {
	public:
		Delete_File() {}
		~Delete_File() {}

		//�ڷᱸ���� ������� ������ ������
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

		virtual bool Create(const char* Path);	//�ʼ� : Path, Data
		virtual bool Read(const char* Path);	//�ʼ� : Path, Data
		virtual bool Update(const char* Path);	//�ʼ� : Path, Data
		virtual bool Delete(const char* Path);	//�ʼ� : Path, Data
		virtual bool Parsing();		//�ʼ� : Data!=null(Read�Ҷ� �ݵ�� �ʿ�)


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