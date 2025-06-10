#include "Lib.h"

namespace Model_Interface {
	//�⺻ ����� Default CRUD�̸� 
	//�ܼ��� File�̶�°Ϳ� ������ ���� �ڷᱸ���� ��������
	//�⺻����̱� ������ ���� Ȯ���ڴ� �����Ӱ� ������� ������
	bool CRUD_Struct::Create(const char* Path) {
		//1. ������ �� ������ �������(������ο����ϴ°���)
		//2. ������ �ҷ��� Data�� ���� �����ؾ���
		if (Ctrl->Excep_Path(Path) == Ctrl->IsDirectory) {
			if (Ctrl->Excep_Data(&Data) == Ctrl->YesData) {
				//��μ� ������ ����
				//Data�� ������� �����ؾ���
				size_t len = std::strlen(Path);
				nDynamic::DynamicStr fullPath(1024);
				fullPath.OperStr(Path);
				
				if (Path[len - 1] != '\\') {
					fullPath.AddWord("\\");
				}

				char* Extension = GetExtension();

				fullPath.AddStr(Data_Name.p_d_str);
				fullPath.AddStr(Extension);

				printf("%s\n", fullPath.p_d_str);


				if (Ctrl->Excep_Path(fullPath.p_d_str) != Ctrl->NotFound) {
					//���� ������ �ֽ��ϴ�.
					printf("���� ������ �����մϴ�.\n");
					return Ctrl->Fail;
				}

				HANDLE hFile = CreateFileA(
					fullPath.p_d_str,			//���ϰ��
					GENERIC_WRITE,				//�������
					0,							//���� ����
					NULL,						//�⺻ ���� �Ӽ�
					CREATE_ALWAYS,				//�׻� �� ���� ����
					FILE_ATTRIBUTE_NORMAL,		//�Ϲ� ���� �Ӽ�
					NULL						//���ø� ����
				);

				if (hFile == INVALID_HANDLE_VALUE) {
					printf("���ϻ�������\n");
					return Ctrl->Fail;
				}

				DWORD bytesWritten;
				


				return Ctrl->Success;
			}
			printf("�����ϰ��� �ϴ� �����Ͱ� �����ϴ�.\n");
			return Ctrl->Fail;
		}
		else if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			//�����ΰ��
			printf("������ ��� �ƴ°ɱ�?\n");
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
	//Data�� �̸�����
	char* CRUD_Struct::GetName() {
		if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NoData) {
			printf("�̸��� �����ϴ�.\n");
			return Data_Name.p_d_str;
		}
		else if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NullData) {
			printf("���ڿ��� Null���Դϴ�.\n");
			return nullptr;
		}
		return Data_Name.p_d_str;
	}

	bool CRUD_Struct::SetName(char* new_Name) {
		if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NullData) {
			printf("���ڿ��� Null���Դϴ�.\n");
			return 0;
		}

		if (new_Name == nullptr) {
			printf("�Ű������� �߸��� ���Դϴ�.\n");
			return 0;
		}

		Data_Name.DeleteStr();

		Data_Name.OperStr(new_Name);

		return 1;
	}

	bool CRUD_Struct::AppendName(char* new_Name) {
		if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NullData) {
			printf("���ڿ��� Null���Դϴ�.\n");
			return 0;
		}
		
		if (new_Name == nullptr) {
			printf("�Ű������� Null ���Դϴ�.\n");
			return 0;
		}
		Data_Name.UpdateStr(new_Name);
		return 1;
	}

	//File�� �ƴ� �ڷᱸ�� Data�� ������ ���� �޼ҵ��
	bool CRUD_Struct::Parsing() {

		return 0;
	}

	bool CRUD_Struct::SetData(char* new_Data) {
		//�����
		if (Ctrl->Excep_Data(&Data) == Ctrl->NullData) {
			printf("���ڿ��� Null���Դϴ�.\n");
			return 0;
		}
		
		if (new_Data == nullptr) {
			printf("�Ű������� Null ���Դϴ�.\n");
			return 0;
		}



		//Set�̶����� �� �����
		Data.DeleteStr();

		Data.OperStr(new_Data);

		return 1;
	}

	bool CRUD_Struct::AppendData(char* new_Data) {
		//�̾��
		if (Ctrl->Excep_Data(&Data) == Ctrl->NullData) {
			printf("���ڿ��� Null���Դϴ�.\n");
			return 0;
		}

		if (new_Data == nullptr) {
			printf("�Ű������� Null ���Դϴ�.\n");
			return 0;
		}
		
		Data.UpdateStr(new_Data);
		return 1;
	}

	char* CRUD_Struct::GetData() {
		if (Ctrl->Excep_Data(&Data) == Ctrl->NoData) {
			printf("�̸��� �����ϴ�.\n");
			return Data_Name.p_d_str;
		}
		else if (Ctrl->Excep_Data(&Data) == Ctrl->NullData) {
			printf("���ڿ��� Null���Դϴ�.\n");
			return nullptr;
		}

		return Data.p_d_str;
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
		//path�� "C:\Users\NSH\Documents" �̷������� ������ �������ϱ� R"(C:\Users\NSH\Documents)" �̷��� �ϵ���

		DWORD attrib = GetFileAttributesA(path);

		if (attrib == INVALID_FILE_ATTRIBUTES) {
			//��γ� ������ �������� ����
			return PathStatus::NotFound;
		}
		else {
			//��ΰ� �����ϱ� ��(�������� �������� ��)
			if (attrib & FILE_ATTRIBUTE_DIRECTORY)
				//������
				return PathStatus::IsDirectory;
			else
				//������
				return PathStatus::IsFile;
		}
	}

	Logic_Ctrl::DataStatus Logic_Ctrl::Excep_Data(nDynamic::DynamicStr* Data) {
		//�����Ͱ� �ִ��� �Ǵ�
		if (Data->p_d_str[0] == '\0') {
			return DataStatus::NoData;
		}
		else if (Data->p_d_str == nullptr) {
			return DataStatus::NullData;
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