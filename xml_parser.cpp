#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <Windows.h>
#include "FileBuf.h"
//XML�� <>���·� �ο��� ������ ������ ���� �����̴�.
/*
<root>
	<child>
		<subchild>
		</subchild>
	</child>
</root>
<parent>
	<son>
		<gender>male</gender>
		<hair>black</hair>
	</son>
	<daughter>
		<gender>female</gender>
		<hair>black</hair>
	</daughter>
</parent>

�̷� ���·ν� ������ �Ǿ��ִ°��� xml������ �����̸�
�̷��� xml ������ ��ü������ ���ؼ� ������ �±׸��� ��ü�ν� �����ϵ��� �����ϴ� �Ľ� ���α׷��� �����鼭
xml�� ���� ���ؿ� ���Ҿ ��ü���⿡ ���� ���� �׸��� ������ �ڷᱸ���� ���� ���ظ� �ѹ��� ��ƺ��� �ð��� ��������
*/

//1. xml�� ��Ӱ��谡 �ƴ� ���԰���
//2. <parent> <child> <child> ... </> �±׳� ���� �±״� ���� ���Ҿ� ���� �̸��±׵� ���
//3. <parent> data1, data2 ... </> �±׳� ���� �����ʹ� �Ұ���
//4. <parent> data1 <> </> �±׳� �±׿� data�� ���� ������ ���� �±� Ȥ�� ������ 1���� �־����
//5. data�� ���´� ����, ���� Ÿ�� ���о��� ���ڿ��� ����ȴ�.

/*
<parent>
	<son>
		<gender>male</gender>
		<hair>black</hair>
	</son>
	<daughter>
		<gender>female</gender>
		<hair>black</hair>
	</daughter>
</parent>
�̸� ��ü�������� �Ľ��Ѵٰ� �����ϸ� 2������ �����µ� Tree, SAX
Tree��
class parent{};
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{};
3���� Ŭ������ ������ �����
class parent{ son s, daughter d; }
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{public: string hair, gender; daughter(){gender="female", hair="black";}};
parent.s.gender == male �̷��������� ����� �Ǹ�
SAX�� �ʿ��� �����͸� �̾� ���� �������� �����Ͽ�
*/
namespace nDynamic {

	class DynamicStr {		//���ڿ��� �������� �־��ִ� Ŭ���� ���Կ����ڵ� ����
	public:
		char* p_d_str;
		int current_size_str;
		int capacity_str;
		bool just_shallow_copy;

		DynamicStr(int size) : capacity_str(size), current_size_str(0), just_shallow_copy(false){
			p_d_str = new char[capacity_str];
		}
		~DynamicStr() {
			if (just_shallow_copy) {
				just_shallow_copy = false;
				return;
			}
			delete[] p_d_str;
		}

		void AsgOperStr(int pos, char c) {			//���� ���Կ���
			if (pos-1 >= capacity_str) {
				SizeUpStr();
			}
			p_d_str[pos] = c;
			p_d_str[pos + 1] = '\0';
			current_size_str++;
		}		
		void SizeUpStr() {							//���� ���� �����Ҷ� �������� �÷���
			int old_capacity = capacity_str;
			capacity_str *= 2;
			char* temp = new char[capacity_str];
			for (int i = 0; i < old_capacity; i++) {
				temp[i] = p_d_str[i];
			}
			delete p_d_str;

			p_d_str = temp;
		}

		void EmptyStr(int init_capacity_size) {		//���� ���ڿ� ��������
			delete p_d_str;
			p_d_str = new char[init_capacity_size];
			current_size_str = 0;
			capacity_str = init_capacity_size;
		}

		void FitSizeStr() {							//���ʿ��� ���� ���� ������
			char* temp = new char[current_size_str+1];
			for (int i = 0; i < current_size_str; i++) {
				temp[i] = p_d_str[i];
			}
			temp[current_size_str] = '\0';
			delete p_d_str;
			p_d_str = temp;
		}

		static DynamicStr* SetStr(DynamicStr* name) {
			int pos = 0;
			char* _name = name->p_d_str;
			DynamicStr* ptrName = new DynamicStr(1024);
			for (; (_name[pos] != '\0'); pos++)
				ptrName->AsgOperStr(pos, _name[pos]);
			ptrName->FitSizeStr();

			return ptrName;
		}
	};

	//���ø� ���� �迭
	template <typename T>
	class DynamicArr {
	public:
		DynamicArr() :current_pos(0), capacity(0), obj_arr(nullptr) {};
		~DynamicArr() {

			for (int i = 0; i < current_pos; i++) {
				delete obj_arr[i];
			}
			delete[] obj_arr;
		};

		DynamicArr(const DynamicArr& other) {
			printf("DynamicArr ���������\n");
		}
		
		//���� �迭 �ʱ�ȭ
		void InitDyArr() {
			if (obj_arr == nullptr) {
				capacity = 4;
				obj_arr = new T*[capacity];
			}
		}

		//�迭 ũ�� ����(���� �˻��)
		void AddDyArr() {
			if (current_pos >= capacity) {
				int old_capacity = capacity;
				capacity = (capacity * 2);
				T** new_obj_arr = new T * [capacity];

				for (int i = 0; i < old_capacity; i++) {
					new_obj_arr[i] = obj_arr[i];
				}
				delete[] obj_arr;
				obj_arr = new_obj_arr;
			}
			return;
		}
		
		//�Ӽ��̸����� �����̶� ���� ���ÿ�
		void InsertDyArr(DynamicStr *NodeName) {
			AddDyArr();
			T* new_attr = new T;
			new_attr->SetName(NodeName);
			obj_arr[current_pos++] = new_attr;
		}

		//�Ӽ��� ������ ����
		template<typename para>
		void InsertDyArr(para *NodeName) {
			AddDyArr();
			obj_arr[current_pos++] = NodeName;
		}


	public:
		int current_pos;
		int capacity;
		T** obj_arr;		//������ �迭�� Ȱ��
	};
}



void XmlDataStruct(char* xml_str);

namespace nXml_Parser {
	using namespace std;
	using namespace nDynamic;

	class AttrObj {
	public:
		AttrObj() : AttrName(nullptr), AttrData(nullptr){};
		~AttrObj() {
			delete AttrName;
			delete AttrData;
		}

		AttrObj(const AttrObj& other) {
			printf("AttrObj ���������\n");
		}

		void SetName(DynamicStr* name) {
			AttrName = name;
		} 

		char* GetName() {
			return AttrName->p_d_str;
		}

		void SetData(DynamicStr* data) {
			AttrData = data;
		}

		char* GetData() {
			return AttrData->p_d_str;
		}

	private:
		DynamicStr* AttrName;		//�Ӽ��� �ڽ��� �̸�
		DynamicStr* AttrData;		//�Ӽ��� ������
	};
	
	class XmlObj {
	public:
		XmlObj() : TagName(nullptr), Parent(nullptr), TagData(nullptr) {};
		~XmlObj() {
			delete TagName;
			//�Ӽ��� �Ҹ���
			//delete &AttrArr;		�̰� ��� ��ü�� AttrArr�� �˾Ƽ� �Ҹ���
		} 
		XmlObj(const XmlObj& other) {
			printf("XmlObj ���������\n");
		}

		//�±� �̸� �����ϱ�
		void SetName(DynamicStr* name) {
			TagName = DynamicStr::SetStr(name);
		}
		//�±��̸� ��������
		char* GetName() {
			return TagName->p_d_str;
		}

		//�±� ������ �����ϱ�
		void SetData(DynamicStr* data) {
			TagData = DynamicStr::SetStr(data);
		}
		//�±׵����� ��������
		char* GetData() {
			return TagData->p_d_str;
		}

		//�Ӽ� �迭 ���� �� �ʱ�ȭ
		void InitAttrArr() {
			AttrArr.InitDyArr();
		}

		//�±� �迭 ���� �� �ʱ�ȭ
		void InitTagArr() {
			XmlObjArr.InitDyArr();
		}

		//����� �ڵ�
		//�Ӽ��迭 ����
		void AddAttrObj(DynamicStr* AttrName) {
			if (AttrArr.obj_arr == nullptr) InitAttrArr();
			AttrArr.InsertDyArr(AttrName);
		}

		void AddAttrObj(AttrObj* Attr) {
			if (AttrArr.obj_arr == nullptr) InitAttrArr();
			AttrArr.InsertDyArr<AttrObj>(Attr);
		}

		//�±׹迭 ����
		void AddXmlObj(XmlObj *XmlNode) {
			if (XmlObjArr.obj_arr == nullptr) InitTagArr();
			XmlObjArr.InsertDyArr<XmlObj>(XmlNode);
			XmlNode->Parent = this;
		}
		
	public:
		XmlObj* Parent;				//�θ� ��ü
		DynamicStr* TagData;		//��ü�� ���� ������(Tag�� 1��)
		DynamicStr* TagName;		//��ü�� �ڽ��� �̸�
		DynamicArr<AttrObj> AttrArr;		//������ �Ӽ�'��'
		DynamicArr<XmlObj> XmlObjArr;		//���� ��ü
	};

	void XmlParseMain() {
		using namespace nDynamic;

		char path[] = "..\\Access_File_Security_Git\\test.xml";
		nFile::ReadFile test(path);
		printf("%s\n", test.pXml_Content);

		DynamicStr *testName = new DynamicStr(1024);
		for (int i = 0; i < 10; i++) {
			testName->AsgOperStr(i, 'A');
		}
		testName->FitSizeStr();
		
		XmlObj testTag;
		testTag.SetName(testName);
		//delete testName;

		printf("�±� �̸� : %s\n", testTag.GetName());

		printf("\n �Ӽ� �߰� 2��\n");

		//�Ӽ��̸� 2�� ����
		DynamicStr* AttrName1 = new DynamicStr(11);
		DynamicStr* AttrName2 = new DynamicStr(11);

		for (int i = 0; i < 10; i++) {
			AttrName1->AsgOperStr(i, 'B');
			AttrName2->AsgOperStr(i, 'C');
		}
		AttrName1->FitSizeStr();
		AttrName2->FitSizeStr();

		testTag.AddAttrObj(AttrName1);
		testTag.AddAttrObj(AttrName2);
		testTag.AddAttrObj(AttrName2);
		testTag.AddAttrObj(AttrName1);


		DynamicStr* TagChild = new DynamicStr(1024);

		for (int i = 0; i < 10; i++) {
			TagChild->AsgOperStr(i, 'T');
		}
		TagChild->FitSizeStr();

		XmlObj *ChildTag = new XmlObj();
		ChildTag->SetName(TagChild);

		testTag.AddXmlObj(ChildTag);


		

		//printf("%s\n", testTag.AttrArr->obj_arr[1].Getname());
	}
}


using namespace nDynamic;
using namespace nXml_Parser;
XmlObj* Parser_Test(char* xml_str) {
	int xml_str_max = 1024;
	int xml_current_pos = 0;
	char c;
	bool xml_end_ck = false;
	bool attr_data_end_ck = false;
	DynamicStr* AttrName = nullptr;
	DynamicStr* TagName = nullptr;
	DynamicStr* AttrData = nullptr;
	DynamicStr* Data = nullptr;
	XmlObj* RootXml = nullptr;
	XmlObj* XmlPtr = nullptr;
	XmlObj* ParentXmlPtr = nullptr;
	AttrObj* AttrPtr = nullptr;
	

	for (;; xml_current_pos++) {
		c = xml_str[xml_current_pos];

		if (c == '\0') break;

		if (c == '\n' || c == ' ' || c == '\t') continue;

		else if (c == '<') {		//Tag���
			xml_current_pos++;
			xml_end_ck = false;

			c = xml_str[xml_current_pos];

			if (c == '!') {	//�ּ� ó��
				xml_end_ck = true;
			}
			else if (c == '/') {
				xml_end_ck = true;
				if (XmlPtr->Parent != nullptr) {
					XmlPtr = XmlPtr->Parent;
				}
				//�ֻ��� ��Ʈ��
			}
			else if (c == '?') {	//����
				xml_end_ck = true;
			}
			else {
				TagName = new DynamicStr(xml_str_max);
				if (XmlPtr != nullptr) {
					//�ڽ��� '������'�� �ڽ� �±���
					ParentXmlPtr = XmlPtr;
					XmlPtr = new XmlObj();
					ParentXmlPtr->AddXmlObj(XmlPtr);
				}
				else
					XmlPtr = new XmlObj();
			}
			for (int j = 0; ; j++, xml_current_pos++) {
				c = xml_str[xml_current_pos];
				if (c == '>') break;
				else if (c == ' ' && !xml_end_ck) {
					//�Ӽ� ���
					while (1) {
						xml_current_pos++;
						AttrName = new DynamicStr(xml_str_max);
						AttrData = new DynamicStr(xml_str_max);
						for (int k = 0; ; k++, xml_current_pos++) {
							c = xml_str[xml_current_pos];
							if (c == '=') break;
							AttrName->AsgOperStr(k, c);
						}
						bool start_attr_ck = false;
						xml_current_pos++;
						for (int k = 0; ; k++, xml_current_pos++) {
							c = xml_str[xml_current_pos];
							if (c == '\"') {	//�Ӽ� ������ ����
								if (start_attr_ck) {
									xml_current_pos++;
									break;
								}
								else {
									start_attr_ck = true;
									k--;
									continue;
								}
							}
							AttrData->AsgOperStr(k, c);
						}
						AttrName->FitSizeStr();
						AttrData->FitSizeStr();
						AttrPtr = new AttrObj();
						AttrPtr->SetName(AttrName);
						AttrPtr->SetData(AttrData);
						XmlPtr->AddAttrObj(AttrPtr);					

						c = xml_str[xml_current_pos];
						if (c == ' ') continue;
						else if (c == '>') {
							xml_current_pos--;
							xml_end_ck = true;
							break;
						}
					}
				}
				if (xml_end_ck) continue;
				TagName->AsgOperStr(j, c);
			}
			//�ݺ��� ���������� �±� ��������
			if (TagName != nullptr) {
				TagName->FitSizeStr();
				XmlPtr->SetName(TagName);
				TagName = nullptr;
			}
		}
		else if (c != '>') {		//������ �����Ͷ�� �����
			xml_current_pos--;
			Data = new DynamicStr(xml_str_max);
			for (int i = 0; ; i++) {
				c = xml_str[++xml_current_pos];

				if (c == '<') {
					xml_current_pos--;
					break;
				}
				Data->AsgOperStr(i, c);
			}
			Data->FitSizeStr();
			XmlPtr->SetData(Data);
		}
	}

	for (;;) {
		if (XmlPtr->Parent == nullptr) {
			RootXml = XmlPtr;
			break;
		}
	}

	return RootXml;

	//delete RootXml;
}

class UI_Xml{
public:
	UI_Xml(XmlObj* Node) {
		CurrentNode = Node;
		child_obj_list_num = 0;
		child_attr_list_num = 0;
		print_use = true;
	}
	UI_Xml() :CurrentNode(nullptr) {}
	~UI_Xml() {
	}

	//���� �±��� �̸� ��������
	void GetCurrentNodeName() {
		if(print_use)
			printf("Node Name : <%s>\n", CurrentNode->GetName());
	}

	//���� �±��� �̸��� ��������
	void GetCurrentNodeParentName() {
		if (print_use) {
			if (CurrentNode->Parent == nullptr) {
				printf("%s�� �ֻ��� Node�Դϴ�.\n", CurrentNode->GetName());
				return;
			}
			printf("ParentNode : <%s>\n", CurrentNode->Parent->GetName());
		}
	}

	//���� �±��� ������ ������ ��������
	void GetCurrentNodeData() {
		if (print_use) {
			if (CurrentNode->TagData != nullptr)
				printf("Data : %s\n", CurrentNode->GetData());
			else
				printf("No Data\n");
		}
	}

	//���� �±��� ���� �Ӽ� �̸� ������ ����Ʈ ���
	void GetCurrentNodeChildAttrList() {
		child_attr_list_num = 0;
		for (int i = 1; i <= CurrentNode->AttrArr.current_pos; i++, this->child_attr_list_num++) {
			if (print_use) {
				printf("%d. AttrName : %s\AttrData : %s\n", i, CurrentNode->AttrArr.obj_arr[i-1]->GetName(), CurrentNode->AttrArr.obj_arr[i - 1]->GetData());
			}
		}
	}

	//���� �±��� ���� ��ü ����Ʈ ���
	bool GetCurrentNodeChildObjList() {
		child_obj_list_num = 0;
		for (int i = 1; i<=CurrentNode->XmlObjArr.current_pos; i++, this->child_obj_list_num++) {
			if (print_use) {
				printf("%d. %s\n", i,CurrentNode->XmlObjArr.obj_arr[i-1]->GetName());
			}
		}
		if (child_obj_list_num == 0) {
			printf("���� ��ü�� �����ϴ�.\n");
			return false;
		}
		return true;
	}

	//�θ� ���� ���ư���
	void SetCurrentNodeParent() {
		if (CurrentNode->Parent == nullptr) {
			printf("���� ��尡 �ֻ��� ����Դϴ�.\n");
			return;
		}
		CurrentNode = CurrentNode->Parent;
		printf("Node ���� �Ϸ�!\n");
	}

	//������ ��ü�� Node �����ϱ�
	void SetCurrentNodeSelectObj(int pos) {
		if (child_obj_list_num == 0) {
			print_use = false;
			this->GetCurrentNodeChildObjList();
			print_use = true;
		}
		if (pos > child_obj_list_num || pos < 1) {
			printf("������ �߸� �Ǿ����ϴ�.\n");
			return;
		}
		CurrentNode = CurrentNode->XmlObjArr.obj_arr[pos - 1];
		printf("Node ���� �Ϸ�!\n");
	}

private:
	XmlObj* CurrentNode;
	int child_obj_list_num;
	int child_attr_list_num;
	bool print_use;
};

void XmlDataStruct(char* xml_str) {
	XmlObj* Root = Parser_Test(xml_str);

	UI_Xml UI(Root);

	int cmd = -1;

	while (1) {
		system("cls");  // ȭ���� ����� ���� ������ ������ �����ֱ�

		// ���� ��� ���� ǥ��
		UI.GetCurrentNodeParentName();
		UI.GetCurrentNodeName();
		UI.GetCurrentNodeData();
		printf("================================\n");

		// �޴� ���
		printf("1. ��ü �����ϱ�\n");
		printf("2. ��ü ��ȸ�ϱ�\n");
		printf("3. ���� ��� �Ӽ� ��ȸ\n");
		printf("4. ���� ��� ������ ��ȸ\n");
		printf("5. �θ� ���� ���ư���\n");
		printf("6. ����\n");
		printf("================================\n");

		// ����ڿ��� �Է��� ����
		printf("���ϴ� �۾��� �����ϼ��� (1-5): ");
		scanf_s("%d", &cmd);

		// ���ÿ� ���� ó��
		switch (cmd) {
		case 1:
			printf("\n[��ü ����]\n");
			if (UI.GetCurrentNodeChildObjList()){  // ��ü ��� ǥ��
				printf("������ ��ü ��ȣ�� �Է��ϼ���: ");
				scanf_s("%d", &cmd);
				UI.SetCurrentNodeSelectObj(cmd);  // ������ ��ü ó��
			}
			break;
		case 2:
			printf("\n[��ü ��ȸ]\n");
			UI.GetCurrentNodeChildObjList();  // ��ü ��� ǥ��
			break;
		case 3:
			printf("\n[���� ��� �Ӽ� ��ȸ]\n");
			UI.GetCurrentNodeChildAttrList();  // ��� �Ӽ� ��ȸ
			break;
		case 4:
			printf("\n[���� ��� ������ ��ȸ]\n");
			UI.GetCurrentNodeData();  // ��� ������ ��ȸ
			break;
		case 5:
			printf("\n[�θ� ���� ���ư���]\n");
			UI.SetCurrentNodeParent();
			break;
		case 6:
			printf("\n���α׷��� �����մϴ�.\n");
			return;  // ����
		default:
			printf("\n�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
			break;
		}

		// �۾� �Ϸ� �� �ٽ� �޴��� ���ư��Բ�
		printf("\n����Ϸ��� Enter Ű�� ��������...\n");
		getchar();  // ���� scanf_s���� ���� ���� ���ڸ� ó��
		getchar();  // ������ Enter�� ��ٸ�
	}

	delete Root;
}

nFile::ReadFile* File_Struct() {
	int cmd;
	File_Handle::File_Main* Dir = new File_Handle::File_Main("..\\Access_File_Security_Git\\Xml", ".xml");
	while (1) {
		printf("<���� ���>\n");
		printf("================================\n");
		Dir->Files_Print();
		printf("================================\n");
		printf("�Ľ��ϰ��� �ϴ� ������ �����Ͽ� �ּ���\n");
		scanf_s("%d", &cmd);
		Dir->Select_File(cmd);

		printf("������ ���ϰ�� : %s\n", Dir->file_buffer->pXml_Content);
		break;
		/*if(Dir->file_buffer != nullptr){
			nFile::ReadFile* xml_file = new nFile::ReadFile(Dir->file_path);
			return xml_file;
		}*/
	}
	return Dir->file_buffer;
}


void Xml_Parser_Main() {
	char path[] = "..\\Access_File_Security_Git\\Xml\\RollMount_EtherCAT.xml";
	nFile::ReadFile test(path);

	//XmlDataStruct(test.pXml_Content);
	printf("Xml������ �Ľ��Ͽ� 1���� �ڷ��� �����ͷ� ������ִ� ���α׷��Դϴ�.\n");
	
	nFile::ReadFile *file = File_Struct();

	XmlDataStruct(file->pXml_Content);
	
}