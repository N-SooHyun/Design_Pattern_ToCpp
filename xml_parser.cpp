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

		DynamicStr(int size) : capacity_str(size), current_size_str(0){
			p_d_str = new char[capacity_str];
		}
		~DynamicStr() { 
			if (p_d_str != nullptr)
				delete p_d_str; 
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
	class XmlObj {
	public:
		XmlObj(){
			obj_current_size = 0;
			child_current_size = 0;
			obj_attr_current_size = 0;
			obj_data_current_size = 0;

			obj_capacity = 100;
			child_capacity = 10;
			obj_attr_capacity = 10;
			obj_data_capacity = 100;

			obj_name = new DynamicStr(obj_capacity);
			child_obj = new XmlObj[child_capacity];
			attr_obj = new XmlAttr[obj_attr_capacity];
			obj_data = new DynamicStr(obj_data_capacity);
		}


		~XmlObj() {
			delete child_obj;
			delete attr_obj;
			delete obj_data;
			delete obj_name;
		}

		//���� ���� ������
		XmlObj(const XmlObj& other) {
			child_obj = new XmlObj(*other.child_obj);
		}

		//���� ���� ���� ������
		XmlObj& operator=(const XmlObj& other) {
			if (this != &other) {	//�ڱ��ڽ�üũ
				delete child_obj;
				child_obj = new XmlObj(*other.child_obj);
			}
			return *this;
		}

		void Insert_Child() {
			child_current_size++;
			if (child_current_size > child_capacity) {	//�����迭 ũ�� �÷������
				child_capacity *= 2;
				DeepCopy(1);
			}
		}

		void DeepCopy(int type) {
			switch (type) {
				case 1://�ڽĹ迭 �ø���
					XmlObj* new_child_obj = new XmlObj[child_capacity];
					for (int i = 0; i < child_current_size-1; i++) {
						new_child_obj[i] = child_obj[i];
					}
					break;
				case 2:
					break;
				default:
					break;
			}
		}

		//�����迭 ���ʿ��Ѻκ� ���� �����ִ¾�

		//�����迭 �������ִ� ��

		//�����迭 �������� ���� �������ִ¾�
		

	private:
		DynamicStr* obj_name;			//��ü �̸�
		int obj_capacity;				//��ü�̸��� �뷮
		int obj_current_size;			//���� ũ��

		XmlObj* child_obj;				//�����迭 �ڽ� ��ü
		int child_capacity;			//�ڽİ�ü�� �뷮
		int child_current_size;		//�ڽİ�ü�� ���� ũ��
		
		XmlAttr* attr_obj;			//�����迭 �Ӽ� ��ü
		int obj_attr_capacity;			//�Ӽ���ü�� �뷮
		int obj_attr_current_size;		//�Ӽ���ü�� ���� ũ��

		DynamicStr* obj_data;			//������
		int obj_data_capacity;			//������ �뷮
		int obj_data_current_size;		//������ ����ũ�� 
	};



	class XmlAttr {
	public:

	private:
		char* attr_name;			//�Ӽ� �̸�
		int attr_capacity;			//�Ӽ� �뷮
		int attr_current_size;		//�Ӽ� ���� ũ��
		
		char* attr_data;					//�Ӽ� ������
		int attr_data_capacity;			//������ �뷮
		int attr_data_current_size;		//������ ���� ũ��

	}; 

	class XmlParse {
	private:
		char current_word;
		int init_size;
		DynamicStr* tag_name = nullptr;

		XmlObj* Root_Obj = nullptr;
		XmlObj* Parent_Obj = nullptr;		//��ġ �������ִ� ��ü����
		XmlObj* Child_Obj = nullptr;		//��ġ �������ִ� ��ü����
		XmlObj* tag_obj = nullptr;
		XmlAttr* attr_obj = nullptr;

		//���¿� ���� �κ��� üũ���ִ� bool ������
		bool tr_end_tag;		//�±װ� ����Ǿ����� Ȯ���ϴ� ����

		int current_word_pos;
	public:
		XmlParse() : Root_Obj(nullptr), tag_obj(nullptr), attr_obj(nullptr), Parent_Obj(nullptr), Child_Obj(nullptr){
			current_word_pos = -1;
			init_size = 1024;
			tag_name = new DynamicStr(init_size);
		}

		void Obj_Process(int i, char c, nFile::ReadFile* xml_file) {		//��ü ó���� ���� �κ�
			//c == '<'
			if (Root_Obj == nullptr) {
				Root_Obj = new XmlObj();
				Parent_Obj = Root_Obj;
			}
			else {
				tag_obj = new XmlObj();
				Child_Obj = tag_obj;
				//�θ𿡰� �������� �� �ְ���?
			}

			tr_end_tag = false;
			int j = -1;
			do {
				i++, j++;
				c = xml_file->pXml_Content[i];

				if (c == '>') break;
				else if ((c == '/') && (xml_file->pXml_Content[i - 1] == '<')) tr_end_tag = true;
				else if (tr_end_tag != true) tag_name->AsgOperStr(j, c);
				else if (c == ' ') {	//�Ӽ� ����
					Attr_Process(i, c, xml_file);
				}
			} while (1);
			//��ü �������ֱ�
			
		}

		void Attr_Process(int i, char c, nFile::ReadFile* xml_file) {		//�Ӽ� ó���� ���� �κ�
			//c == ' '
			int j = -1;


		}

		void Data_Process() {		//������ ó���� ���� �κ�

		}

		void XmlObjParserNewVersion(nFile::ReadFile* xml_file) {
			//������ ������ XML �Ľ� ����	
			//3�ܰ�� �������� Obj, Attr, Data
			int& i = current_word_pos;
			char& c = current_word;
			do {
				i++;
				c = xml_file->pXml_Content[i];
				if (c == '\n' || c == ' ') continue;		//���� ����

				else if (c == '<') {		//�±� ����
					Obj_Process(i, c, xml_file);
				}


			} while (i < xml_file->current_size_str);

		}
	};

	
	void XmlFileResCk(nFile::ReadFile* xml_file) {
		printf("file�� ���� ���ҽ�\n");
		printf("�޾ƿ� xml�� �뷮(capacity) : %d\n", xml_file->capacity);
		printf("ũ��(current_size_str) : %d\n", xml_file->current_size_str);
	}

	void XmlWordCkToObj(nFile::ReadFile* xml_file) {
		char c;
		int obj_cnt = 0;
		for (int i = 0; i < xml_file->current_size_str; i++) {
			c = xml_file->pXml_Content[i];
			if (c == '<' && (c= xml_file->pXml_Content[++i]) == '/') {
				obj_cnt++;
			}
		}
		printf("�� ��������� �ϴ� ��ü ���� : %d\n", obj_cnt);
	}

	void XmlObjParserTree(nFile::ReadFile* xml_file) {
		//������ �����ν� �����Ѵ� ���α׷�
		char c; 
		int data_cnt = 0;
		int obj_cnt = 0;
		int data_size = 1024;				//�����͹��� ����
		int tag_size = 1024;				//�±׹��� ����
		int attr_size = 1024;				//�Ӽ����� ����
		DynamicStr data(data_size);			//������
		DynamicStr tag_name(tag_size);		//�±��̸�
		DynamicStr attr_name(attr_size);	//�Ӽ���
		DynamicStr attr_data(attr_size);	//�Ӽ�������
		
		XmlObj* Root = nullptr;
		XmlObj* tag = nullptr;

		bool tr_end_tag = false;		//�±� �������� /���� üũ���ִ� Ʈ����
		bool tr_attr = false;
		bool tr_tag_root = false;
		bool tr_attrs = false;

		int i = -1;
		do {
			i++;
			c = xml_file->pXml_Content[i];
			
			if (c == '\n' || c == ' ') {	//���� ����
				continue;
			}
			else if (c == '<') {	//�±� ����(�Ӽ��� �߰�)

				if (Root == nullptr) Root = new XmlObj();		//��Ʈ �������� ��Ʈ�� �����
				else tag = new XmlObj();

				tr_end_tag = false;
				int j = -1;
				do {
					i++, j++;
					c = xml_file->pXml_Content[i];

					if (c == '>') break;
					else if ((c == '/') && (xml_file->pXml_Content[i - 1] == '<')) tr_end_tag = true;
					else if (tr_end_tag != true) {
						tag_name.AsgOperStr(j, c);
					}
					else if (c == ' ') {	//�Ӽ� ����(�Ӽ���, ������ 1������ �̷���� ex:attr="data")
						int k = -1;
						tr_attr = true;
						do {				//�Ӽ���
							i++, k++;
							c = xml_file->pXml_Content[i];
							if (c == ' ') {			//���߼Ӽ��ΰ��
								tr_attrs = true;
								attr_data.EmptyStr(1024);
								attr_name.EmptyStr(1024);
								continue;
							}
							else if (c == '\"') {	//�Ӽ�������
								int l = -1;
								do {
									i++, l++;
									c = xml_file->pXml_Content[i];
									if (c == '\"') {	//�Ӽ������� ������ �Ͻ�
										break;
									}
									else
										attr_data.AsgOperStr(l, c);
								} while (1);
							}
							else if (c == '>') {
								i--;
								break;
							}
							else {					//�Ӽ���
								attr_name.AsgOperStr(k, c);
							}
						} while (1);
						if (tr_attrs) {//�Ӽ��� ������

						}
						else {//�Ӽ��� 1��

						}
					}
				} while (1);
				if (tr_end_tag != true) {		//�±װ� </> �̰� �ƴҶ��� ������ֱ�
					
					/*tag = new XmlObj(tag_name.p_d_str);
					if (tr_tag_root == false) {
						tr_tag_root = true;
						tag_root = tag; 
					}
					printf("%d. tag : %s\n", ++obj_cnt,tag->obj_name->p_d_str);*/
				}
			}
			else if (c != '>' && tr_end_tag == false) {		//�������϶�
				char ck;
				int j = 0;
				do {
					data.AsgOperStr(j, c);
					i++, j++;
					c = xml_file->pXml_Content[i];
					if (c == '<') {
						c = xml_file->pXml_Content[--i];
						break;
					}
				} while (1);
				//�����͸� �־��ָ� �ȴ� �±׿�
				//tag->data = data.p_d_str;
				//printf("%d. data : %s\n",++data_cnt, data.p_d_str);
				data.EmptyStr(data_size);
			}

		} while (i<xml_file->current_size_str);

		
	}

	void XmlParseMain() {
		using namespace nDynamic;

		char path[] = "directory\\test.xml";
		nFile::ReadFile test(path);
		printf("%s\n", test.pXml_Content);
		XmlFileResCk(&test);
		XmlWordCkToObj(&test);
		XmlObjParserTree(&test);

	}
}






