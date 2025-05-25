#include "Lib.h"

namespace Model_Interface {
	//�⺻ ����� Default CRUD�̸� 
	//�ܼ��� File�̶�°Ϳ� ������ ���� �ڷᱸ���� ��������
	bool CRUD_Struct::Create(const char* Path) {
		//1. ������ �� ������ �������(������ο����ϴ°���)
		//2. ������ �ҷ��� Data�� ���� �����ؾ���
		if (Ctrl->Excep_Path(Path) == Ctrl->IsDirectory) {
			if (Ctrl->Excep_Data(&Data) == Ctrl->YesData) {
				//��μ� ������ ����
			}
			printf("�����ϰ��� �ϴ� �����Ͱ� �����ϴ�.\n");
			return Ctrl->Fail;
		}
		else{
			printf("�����ϰ��� �ϴ� ������ ��θ� �ùٸ��� �ۼ����ּ���\n");
			return Ctrl->Fail;
		}
	}

	bool CRUD_Struct::Read(const char* Path) {
		//�д°��� �� �����̾����
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {

		}
		else
			return Ctrl->Fail;
	}

	bool CRUD_Struct::Update(const char* Path) {
		//�������ִ°��� �� �����̾����(�����)
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			//
		}
		else
			return Ctrl->Fail;

	}

	bool CRUD_Struct::Delete(const char* Path) {
		//�������ִ°��� �� �����̾����
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			if (DeleteFileA(Path)) {
				printf("���� ���� ����\n");
			}
			else {
				printf("���� ���� ���� : %d\n", GetLastError());
			}
		}
		else {
			printf("�����ϰ��� �ϴ� ������ ��θ� �ùٸ��� �ۼ����ּ���\n");
			return Ctrl->Fail;
		}
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

	void CRUD_Struct::Ctrl_Box(Logic_Ctrl* Ctrl) {
		this->Ctrl = Ctrl;
	}

//---------------------------------------------Logic Ctrl----------------------------------------------------

	//�����͸� ó�����ִ� �ణ�� �������� ����
	Logic_Ctrl::PathStatus Logic_Ctrl::Excep_Path(const char* path) {
		//path �ۼ� ����
		//�����  "..\\Access_File_Security_Git\\Xml\\test.txt"
		//������ "C:\\Users\\NSH\\Documents\\Visual_studio_pro\\Design_Pattern_ToCpp"
		//path�� "C:\Users\NSH\Documents" �̷������� ������ �������ϱ� "R(C:\Users\NSH\Documents)" �̷��� �ϵ���

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

	Logic_Ctrl::DataStatus Logic_Ctrl::Excep_Data(nDynamic::DynamicStr* Data) {
		//�����Ͱ� �ִ��� �Ǵ�
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