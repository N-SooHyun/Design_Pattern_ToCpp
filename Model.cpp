#include "Lib.h"

namespace Model_Interface {
	//�⺻ ����� Default CRUD�̸� 
	//�ܼ��� File�̶�°Ϳ� ������ ���� �ڷᱸ���� ��������
	bool CRUD_Struct::Create(const char* Path) {
		//1. ������ �� ������ �������(������ο����ϴ°���)
		//2. ������ �ҷ��� Data�� ���� �����ؾ���
		if (Ctrl->Excep_Path(Path) == Ctrl->IsDirectory) {
			

		}
		else
			return Ctrl->Fail;
	}

	bool CRUD_Struct::Read(const char* Path) {
		//�д°��� �� �����̾����
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {

		}
		else
			return Ctrl->Fail;
	}

	bool CRUD_Struct::Update(const char* Path) {
		//�������ִ°��� �� �����̾����
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {

		}
		else
			return Ctrl->Fail;

	}

	bool CRUD_Struct::Delete(const char* Path) {
		//�������ִ°��� �� �����̾����
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			
		}
		else
			return Ctrl->Fail;
	}

//---------------------------------------------Data Logic----------------------------------------------------

	//File�� �ƴ� �ڷᱸ�� Data�� ������ ���� �޼ҵ��
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

//---------------------------------------------Logic Ctrl----------------------------------------------------
	char* Path_Parsing(const char* path) {
		char word = NULL;
		nDynamic::DynamicStr new_path(1024);
		int i = 0;

		do {
			if (word == '\\') {

			}
			word = path[i];
			i++;
		} while (1);

	}

	//�����͸� ó�����ִ� �ణ�� �������� ����
	bool Logic_Ctrl::Excep_Path(const char* path) {
		//path �ۼ� ����
		//�����  "..\\Access_File_Security_Git\\Xml\\test.txt"
		//������ "C:\\Users\\NSH\\Documents\\Visual_studio_pro\\Design_Pattern_ToCpp"
		//path�� "C:\Users\NSH\Documents �̷������� "\"�� 1���� �־ ���� �� �ֵ��� �Ľ��ϵ���

		
		

		DWORD attrib = GetFileAttributesA(path);

		if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
			//���丮 �����(����)
			return PathStatus::IsDirectory;
		}
		else if (attrib == INVALID_FILE_ATTRIBUTES) {
			//��γ� ������ �������� ����

			return PathStatus::NotFound;
		}
		else {
			//Ư�� ���ϴ����
			return PathStatus::IsFile;
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