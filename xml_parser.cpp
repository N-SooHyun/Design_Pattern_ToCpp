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
		DynamicStr* obj_name;						//������Ʈ�� �̸�(�±׸�)
		//map<string, vector<XmlObj*>> children;		//�ڽ� �±׵�
		XmlObj* children;							//�ڽ� �±׵�
		void* data;									//������

		XmlObj(char* _obj_name){
			int init_size = 1024;
			obj_name = new DynamicStr(init_size);
			for (int i = 0; ; i++) {
				if (_obj_name[i] == '\0') {
					obj_name->AsgOperStr(i, '\0');
					obj_name->FitSizeStr();
					break;
				}
				obj_name->AsgOperStr(i, _obj_name[i]);
			}
		}

		~XmlObj() {
			delete obj_name;
			delete children;
		}

		
	};
	class XmlAttr {
	public:
		DynamicStr* attr_name;
		DynamicStr* attr_data;
	};

	using namespace nDynamic;
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
		int data_size = 1024;				//�����͹��� ����
		int tag_size = 1024;				//�±׹��� ����
		int attr_size = 1024;				//�Ӽ����� ����
		DynamicStr data(data_size);			//������
		DynamicStr tag_name(tag_size);		//�±��̸�
		DynamicStr attr_name(attr_size);	//�Ӽ���
		DynamicStr attr_data(attr_size);	//�Ӽ�������
		
		XmlObj* tag_root;
		XmlObj* tag;

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
					tag = new XmlObj(tag_name.p_d_str);
					if (tr_tag_root == false) {
						tr_tag_root = true;
						tag_root = tag; 
					}
					//printf("tag : %s\n", tag->obj_name->p_d_str);
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
				printf("data : %s\n", data.p_d_str);
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






