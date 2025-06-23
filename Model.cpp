#include "Lib.h"

namespace Model_Interface {

	

	//�⺻ ����� Default CRUD�̸� 
	//�ܼ��� File�̶�°Ϳ� ������ ���� �ڷᱸ���� ��������
	//�⺻����̱� ������ ���� Ȯ���ڴ� �����Ӱ� ������� ������
	bool CRUD_Struct::Create(const char* Path) {
		//1. ������ �� ������ �������(������ο����ϴ°���)
		//2. ������ �ҷ��� Data�� ���� �����ؾ���
		int FileStatus = Ctrl->Excep_Path(Path);
		if (FileStatus == Ctrl->IsDirectory) {
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

				if (Ctrl->Excep_Data(&Data_Name) == Ctrl->NoData) {
					printf("���� �������� Name�� �������� �ʽ��ϴ�.\n");
					return Ctrl->Fail;
				}

				fullPath.AddStr(Data_Name.p_d_str);
				fullPath.AddStr(Extension);

				printf("%s\n", fullPath.p_d_str);

				if (!Ctrl->WinFileCreate(fullPath.p_d_str, &Data)) {
					//���ϻ��� ����
					return Ctrl->Fail;
				}


				return Ctrl->Success;
			}
			printf("�����ϰ��� �ϴ� �����Ͱ� �����ϴ�.\n");
			return Ctrl->Fail;
		}
		else if (FileStatus == Ctrl->IsFile) {
			//�����ΰ�� ������ �̹� �����Ѵٴ°���
			char s;
			printf("�̹� �ش� ������ �����մϴ�.\n");
			printf("�ش� ������ ��������ұ��?(y,Y or n,N) : ");
			scanf_s("%c", &s);

			switch (s) {
			case 'n': case 'N':
				printf("���� ������ ����մϴ�.\n");
				return Ctrl->Fail;
			case 'y': case 'Y':
				break;
			default:
				printf("�߸� �����ϼ̽��ϴ�.\n");
				printf("���� ������ ����մϴ�.\n");
				return Ctrl->Fail;
			}


			//���� �����
			printf("�����Կ�~~");
			if (!Ctrl->WinFileCreate(Path, &Data)) {
				//���ϻ��� ����
				return Ctrl->Fail;
			}

			

		}
		else if (FileStatus == Ctrl->NewFile) {
			//�����ΰ���ε� ������ �������� �ʴ°��
			printf("������ �������� �ʴ� ����Դϴ�.\n");
			printf("�ش� ��η� ������ �����帱�Կ�\n");
			if (!Ctrl->WinFileCreate(Path, &Data)) {
				//���ϻ��� ����
				return Ctrl->Fail;
			}

			

		}
		else{
			printf("�����ϰ��� �ϴ� ������ ��θ� �ùٸ��� �ۼ����ּ���\n");
			return Ctrl->Fail;
		}
	}

	bool CRUD_Struct::Read(const char* Path) {
		//�д°��� ������ �����̾�� ��
		//1. ���������Ϳ� ������ �����? Ȥ�� Append?
		if (Ctrl->Excep_Path(Path) == Ctrl->IsFile) {
			if (Ctrl->WinFileRead(Path, &Data, &Data_Name) == Ctrl->Fail) {
				printf("�б� ����\n");
				return Ctrl->Fail;
			}



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
			//���� ����϶��� �̷��� ������ ����� �ʹٴ� �ǹ̰� �ɼ��� ����
			size_t len = std::strlen(path);
			nDynamic::DynamicStr fullPath(1024);
			fullPath.OperStr(path);
			len--;
			for (; ;) {
				if (fullPath.p_d_str[len] == '\\') {
					break;
				}
				fullPath.DelWord();
				len--;
			}
			
			attrib = GetFileAttributesA(fullPath.p_d_str);

			if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
				//���� ���� ������
				//���� ���簡 �������� �������� Ȯ���ڸ� ���ؼ� ã�ƾ���
				len = std::strlen(path);
				for (;; len--) {
					if (path[len] == '.') {
						//���� ���簡 ������ 
						return PathStatus::NewFile;
					}
					else if (path[len] == '\\') {
						//���� ���簡 ������
						return PathStatus::NotFound;
					}
				}

			}
			else {
				//���� ���� ��ü�� ���� ������ ��ΰ�
				return PathStatus::NotFound;
			}

			//������ ����϶��� NotFound�ϴ°� ����
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

	Logic_Ctrl::ExcepStatus Logic_Ctrl::WinFileCreate(const char* path, const nDynamic::DynamicStr* Data) {
		HANDLE hFile = CreateFileA(
			path,			//���ϰ��
			GENERIC_WRITE,				//�������
			0,							//���� ����
			NULL,						//�⺻ ���� �Ӽ�
			CREATE_ALWAYS,				//�׻� �����
			FILE_ATTRIBUTE_NORMAL,		//�Ϲ� ���� �Ӽ�
			NULL						//���ø� ����
		);


		if (hFile == INVALID_HANDLE_VALUE) {
			printf("���� ���� ����: ");
			std::cerr << GetLastError() << std::endl;
			return Fail;
		}

		DWORD written;

		if (!WriteFile(hFile, Data->p_d_str, Data->capacity_str, &written, NULL)) {
			DWORD err = GetLastError();
			std::cerr << "���� Write ���� ���� �ڵ� : " << err << std::endl;
			return Fail;
		}

		CloseHandle(hFile);

		return Success;
	}


	Logic_Ctrl::ExcepStatus Logic_Ctrl::WinFileRead(const char* path, const nDynamic::DynamicStr* Data, const nDynamic::DynamicStr* Data_Name) {
		HANDLE hFile = CreateFileA(
			path,			//���ϰ��
			GENERIC_READ,				//�������
			0,							//���� ����
			NULL,						//�⺻ ���� �Ӽ�
			OPEN_EXISTING,				//�б� �������� ����
			FILE_ATTRIBUTE_NORMAL,		//�Ϲ� ���� �Ӽ�
			NULL						//���ø� ����
		);


		if (hFile == INVALID_HANDLE_VALUE) {
			printf("���� �б� ����: ");
			std::cerr << GetLastError() << std::endl;
			return Fail;
		}

		DWORD read;

		if (!ReadFile(hFile, Data->p_d_str, Data->capacity_str, &read, NULL)) {
			DWORD err = GetLastError();
			std::cerr << "���� �����ε� ���� ���� �ڵ� : " << err << std::endl;
			return Fail; 
		}

		Data->p_d_str[read] = '\0';

		CloseHandle(hFile);

		return Success;
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