#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

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

		DynamicStr() {}

		DynamicStr(int size) : capacity_str(size), current_size_str(0){
			p_d_str = new char[capacity_str];
		}
		~DynamicStr() {
			printf("Str �Ҹ���\n");
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
		DynamicArr() :currentPos(0), capacity(0), obj_arr(nullptr) {};
		~DynamicArr() {
			delete[] obj_arr;
		};
		DynamicArr(const DynamicArr& other) {
			printf("DynamicArr ���������\n");
		}

		void Init_Attr_Arr() {
			if (obj_arr != nullptr) {
				delete[] obj_arr;
			}
			capacity = 4;				//�ʱ� �Ӽ����� 4����?
			obj_arr = new T[capacity];
			currentPos = 0;
		}
	

	public:
		int currentPos;
		int capacity;
		T* obj_arr;
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
		AttrObj() {};
		~AttrObj() {
			printf("�Ҹ��� AttrObj : %p\n", this);
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
		XmlObj() {};
		~XmlObj() {
			printf("�Ҹ��� XmlObj : %p\n", this);
			delete TagName;
			delete AttrArr;
		};
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


	public:
		DynamicStr* TagName;		//��ü�� �ڽ��� �̸�
		DynamicArr<AttrObj>* AttrArr;		//������ �Ӽ�'��'
	};



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
		delete testName;

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
		

		
		
		

		//printf("%s\n", testTag.AttrArr->obj_arr[1].Getname());
	}
}



/*		25.03.17 �Ҹ��� �̽������� �Ӽ��迭 ������ ������ �ϱ� �ߴµ� ��ư �װ� �̾ �ϸ� �ɵ� 
namespace nDynamic {

	class DynamicStr {		//���ڿ��� �������� �־��ִ� Ŭ���� ���Կ����ڵ� ����
	public:
		char* p_d_str;
		int current_size_str;
		int capacity_str;

		DynamicStr() {}

		DynamicStr(int size) : capacity_str(size), current_size_str(0){
			p_d_str = new char[capacity_str];
		}
		~DynamicStr() {
			printf("Str �Ҹ���\n");
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

	template <typename T>
	class DynamicArr {
	public:
		DynamicArr() :currentPos(0), capacity(0), obj_arr(nullptr) {};
		~DynamicArr() {
			delete[] obj_arr;
		};
		DynamicArr(const DynamicArr& other) {
			printf("DynamicArr ���������\n");
		}

		void Init_Attr_Arr() {
			if (obj_arr != nullptr) {
				delete[] obj_arr;
			}
			capacity = 4;				//�ʱ� �Ӽ����� 4����?
			obj_arr = new T[capacity];
			currentPos = 0;
		}

		//�迭ũ�� ����
		void Add_Attr_Capacity() {
			int old_capacity = capacity;
			capacity = static_cast<int>(capacity*1.5);
			T* new_attr_arr = new T[capacity];

			for (int i = 0; i < old_capacity; i++) {
				new_attr_arr[i] = obj_arr[i];
			}
			delete[] obj_arr;
			obj_arr = new_attr_arr;
		}

		//�迭�� ����
		void operator<<(T& add_arr_node) {
			if (currentPos >= capacity) {
				Add_Attr_Capacity();
			}
			obj_arr[currentPos++] = add_arr_node;
		}

		//��� �Ӽ� �迭�� �ִ� �� ���
		void All_Attr_Print() {
			for (int i = 0; i < currentPos; i++) {
				printf("%s \n", obj_arr[i].GetName());
			}
		}

		T* Pocusing_Get_Attr(int pos) {
			if (pos >= currentPos) {
				printf("�迭 ũ�⿡ �´� ��ġ�� ���� �Է����ּ���(���� �迭ũ�� : %d\n", currentPos);
				return NULL;
			}
			obj_arr+pos;
		}

	public:
		int currentPos;
		int capacity;
		T* obj_arr;
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
		AttrObj() {};
		~AttrObj() {
			printf("�Ҹ��� AttrObj : %p\n", this);
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
		XmlObj() {};
		~XmlObj() {
			printf("�Ҹ��� XmlObj : %p\n", this);
			delete TagName;
			delete AttrArr;
		};
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
		//�Ӽ� �ʱ�ȭ
		void Init_Attr() {
			if (AttrArr == nullptr) {
				AttrArr = new DynamicArr<AttrObj>();
				AttrArr->Init_Attr_Arr();
			}
		}
		//�Ӽ��� �� �߰�
		void Add_Attr(AttrObj* attr_node) {
			Init_Attr();
			*AttrArr << *attr_node;
		}

		void operator<<(AttrObj& add_attr) {
			Init_Attr();
			*AttrArr << add_attr;
		}

		AttrObj* operator[](int pos) const {
			return AttrArr->Pocusing_Get_Attr(pos);
		}

	public:
		DynamicStr* TagName;		//��ü�� �ڽ��� �̸�
		DynamicArr<AttrObj>* AttrArr;		//������ �Ӽ�'��'
	};



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
		delete testName;

		printf("�±� �̸� : %s\n", testTag.GetName());

		printf("\n �Ӽ� �߰� 2��\n");

		DynamicStr* AttrName1 = new DynamicStr(11);
		DynamicStr* AttrName2 = new DynamicStr(11);

		for (int i = 0; i < 10; i++) {
			AttrName1->AsgOperStr(i, 'B');
			AttrName2->AsgOperStr(i, 'C');
		}
		AttrName1->FitSizeStr();
		AttrName2->FitSizeStr();

		AttrObj* Attr1 = new AttrObj();
		AttrObj* Attr2 = new AttrObj();

		Attr1->SetName(AttrName1);
		Attr2->SetName(AttrName2);

		testTag << *Attr1;
		testTag << *Attr2;


		testTag.AttrArr->All_Attr_Print();

		testTag[0];


		//printf("%s\n", testTag.AttrArr->obj_arr[1].Getname());
	}
}
*/




