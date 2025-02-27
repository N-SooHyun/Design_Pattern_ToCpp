#include <stdio.h>
#include <string.h>

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
	class DynamicStr {
	public:
		char* p_d_str;
		int current_size_str;
		int capacity_str;

		char* p_d_data;		//�Ӽ��϶��� ������
		int current_size_data;
		int capacity_data;
		DynamicStr(int size) : capacity_str(size), current_size_str(0), p_d_data(nullptr){
			p_d_str = new char[capacity_str];
		}
		~DynamicStr() { 
			if (p_d_str != nullptr)
				delete p_d_str; 
		}

		void OperStr(int pos, char c) {
			if (pos >= capacity_str) {
				SizeUpStr();
			}
			p_d_str[pos] = c;
			current_size_str++;
		}
		void OperData(int pos, char c) {
			if (pos >= capacity_str) {
				SizeUpStr();
			}
			p_d_data[pos] = c;
			current_size_data++;
		}

		
		void SizeUpStr() {
			int old_capacity = capacity_str;
			capacity_str *= 2;
			char* temp = new char[capacity_str];
			for (int i = 0; i < old_capacity; i++) {
				temp[i] = p_d_str[i];
			}
			delete p_d_str;
			p_d_str = temp;
		}

		void EmptyStr(int init_capacity_size) {
			delete p_d_str;
			p_d_str = new char[init_capacity_size];
			current_size_str = 0;
			capacity_str = init_capacity_size;
		}

		void FitSizeStr() {
			char* temp = new char[current_size_str+1];
			for (int i = 0; i < current_size_str; i++) {
				temp[i] = p_d_str[i];
			}
			temp[current_size_str] = '\0';
			delete p_d_str;
			p_d_str = temp;
		}

		void FitSizeData() {
			char* temp = new char[current_size_data + 1];
			for (int i = 0; i < current_size_str; i++) {
				temp[i] = p_d_data[i];
			}
			temp[current_size_str] = '\0';
			delete p_d_data;
			p_d_data = temp;
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
	class XmlObj {
	public:
		char *obj_tag_name;		//���� �迭 �̸�
		//���� �Ӽ� ��� ���ҽ� �ʿ�
		XmlObj* child_obj;		//���� �迭 �ڽ� �±׵�
		int child_obj_size;		//���� �迭 �ڽ� �±׵��� ����
		void* data;				//Ȥ�� �±װ� ���� �����͸� �ִ� ���

		XmlObj(char *_obj_tag_name, int name_size) : obj_tag_name(new char[name_size+1]) {
			for (int i = 0; i < name_size; i++) {
				obj_tag_name[i] = _obj_tag_name[i];		//�̸� ����
			}
			obj_tag_name[name_size] = '\0';
		}
		XmlObj() {}

		
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
		int data_size = 1024;
		int tag_size = 1024;
		int attr_cnt = 1024;				//�Ӽ�����
		DynamicStr data(data_size);			//������
		DynamicStr tag_name(tag_size);		//�±��̸�
		
		XmlObj* tag_root = new XmlObj();
		XmlObj* tag;

		bool tr_end_tag = false;
		bool tr_attr_input = false;

		for (int i = 0; (i < xml_file->current_size_str); i++) {
			c = xml_file->pXml_Content[i];
			for (;; i++) {
				c = xml_file->pXml_Content[i];
				if (c == '\n' || c == ' ') {
					continue;
				}
				else {
					break;
				}
			}
			if (c == '<') {		//�±��϶�
				tr_end_tag = false;
				c = xml_file->pXml_Content[++i];
				for (int j = 0; ; i++, j++) {
					c = xml_file->pXml_Content[i];
					if (c == '>') break;
					if ((c == '/') && (xml_file->pXml_Content[i-1] == '<')) tr_end_tag = true;
					else if (c == ' ') {}//attr_input = true;	//�Ӽ��Է�
					if (tr_end_tag != true) {		//�±� �̸� �־��ֱ�
						tag_name.OperStr(j, c);
					}
				}
				if (tr_end_tag != true) {			//�±� ���� �ƴҶ� �±� ����
					if (tr_attr_input == true) {	//�Ӽ��� �ִٸ�

					}
					tag_name.FitSizeStr();
					tag = new XmlObj(tag_name.p_d_str, tag_name.current_size_str);
					printf("Tag : %s\n", tag_name.p_d_str);
					tag_name.EmptyStr(tag_size);
				}
			}
			//�������϶�
			if (c != '>' && tr_end_tag == false) {
				char ck;
				for (int j = 0; ; i++,j++) {
					c = xml_file->pXml_Content[i];
					data.OperStr(j, c);
					if (((ck = xml_file->pXml_Content[i + 1]) == '<') && ((ck = xml_file->pXml_Content[i + 2]) == '/')) {
						i += 2;
						tr_end_tag = true;
						break;
					}
				}
				data.FitSizeStr();
				//printf("data : %s\n", data.p_d_str);
				data.EmptyStr(data_size);
			}
		}
		printf("������ ���� : %d\n", data_cnt);
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






