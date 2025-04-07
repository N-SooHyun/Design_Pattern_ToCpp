#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <Windows.h>

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
			printf("Str �Ҹ���\n");
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
		
		//�Ӽ� ����
		void InsertDyArr(DynamicStr *NodeName) {
			AddDyArr();
			T* new_attr = new T;
			new_attr->SetName(NodeName);
			obj_arr[current_pos++] = new_attr;
		}

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

namespace nFile {
	class ReadFile {
	public:
		FILE* pFile = NULL;
		char* pXml_Content;
		int capacity;
		int current_size_str;

		ReadFile(char* path) {
			//fopen_s(&pFile, path, "r");
			fopen_s(&pFile, path, "r");
			capacity = 1024;
			pXml_Content = new char[capacity];
			current_size_str = 0;
			CopyFile();
		}
		~ReadFile() {
			delete pXml_Content;
			fclose(pFile);
		}
		void CopyFile() {
			char c;
			char* temp;
			for (current_size_str = 0; ; current_size_str++) {
				c = fgetc(pFile);
				if (c == EOF) break;
				else if (current_size_str == capacity - 1) {
					capacity *= 2;
					temp = pXml_Content;
					pXml_Content = new char[capacity];
					for (int i = 0; i < current_size_str; i++) {
						pXml_Content[i] = temp[i];
					}
					delete temp;
				}
				pXml_Content[current_size_str] = c;
			}
			pXml_Content[current_size_str] = '\0';
			temp = new char[current_size_str+1];
			for (int i = 0; i<current_size_str; i++) {
				temp[i] = pXml_Content[i];
			}
			temp[current_size_str] = '\0';
			capacity = current_size_str;
			delete pXml_Content;
			pXml_Content = temp;
			fclose(pFile);
		}
	};
}

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
			AttrName = DynamicStr::SetStr(name);
		} 

		char* GetName() {
			return AttrName->p_d_str;
		}

		void SetData(DynamicStr* data) {
			AttrData = DynamicStr::SetStr(data);
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
		XmlObj() : TagName(nullptr) {};
		~XmlObj() {
			printf("�Ҹ��� XmlObj : %p\n", this);
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

		//�±� �̸� �����ϱ�
		void SetData(DynamicStr* data) {
			TagData = DynamicStr::SetStr(data);
		}
		//�±��̸� ��������
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

	void XmlDataStruct_UI_UX() {
		printf("<Xml Data Struct>");
		char c;
		using namespace nDynamic;
		
	}

	void XmlParseMain() {
		using namespace nDynamic;

		char path[] = "directory\\test.xml";
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
void Parser_Test(char* xml_str) {
	int xml_str_max = 100;
	int xml_current_pos = 0; 
	char c;
	bool xml_end_ck = false;

	DynamicStr* AttrName;
	

	for (;; xml_current_pos++) {
		c = xml_str[xml_current_pos];

		if (c == '\n' || c == ' ') continue;

		else if (c == '<') {		//Tag���
			xml_current_pos++;

			xml_end_ck = false;
			for (int j = 0; ; j++, xml_current_pos++) {
				c = xml_str[xml_current_pos];
				if (c == '>') break;
				else if (c == '/') {
					xml_end_ck = true;
				}
				else if (c == ' ') {
					//�Ӽ� �־��ָ� ��
				}
			}
			//�ݺ��� ���������� �±� ��������
		}
		else if (c != '>') {		//������ �����Ͷ�� �����
			
		}



	}
	

}



