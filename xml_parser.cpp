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
		int current_size;

	public:
		char* p_d_char;
		int capacity;
		DynamicStr(int size) : capacity(size), current_size(0){
			p_d_char = new char[capacity];
		}
		~DynamicStr() { delete p_d_char; }
		
		void SizeUpStr() {
			int old_capacity = capacity;
			capacity *= 2;
			char* temp = new char[capacity];
			for (int i = 0; i < old_capacity; i++) {
				temp[i] = p_d_char[i];
			}
			delete p_d_char;
			p_d_char = temp;
		}

		void EmptyStr(int init_capacity_size) {
			delete p_d_char;
			p_d_char = new char[init_capacity_size];
		}
	};
}

namespace nFile {
	class ReadFile {
	public:
		FILE* pFile = NULL;
		char* pXml_Content;
		int capacity;
		int current_size;

		ReadFile(char* path) {
			//fopen_s(&pFile, path, "r");
			fopen_s(&pFile, path, "r");
			capacity = 1024;
			pXml_Content = new char[capacity];
			current_size = 0;
			CopyFile();
		}
		~ReadFile() {
			delete pXml_Content;
			fclose(pFile);
		}
		void CopyFile() {
			char c;
			char* temp;
			for (current_size = 0; ; current_size++) {
				c = fgetc(pFile);
				if (c == EOF) break;
				else if (current_size == capacity - 1) {
					capacity *= 2;
					temp = pXml_Content;
					pXml_Content = new char[capacity];
					for (int i = 0; i < current_size; i++) {
						pXml_Content[i] = temp[i];
					}
					delete temp;
				}
				pXml_Content[current_size] = c;
			}
			pXml_Content[current_size] = '\0';
			temp = new char[current_size+1];
			for (int i = 0; i<current_size; i++) {
				temp[i] = pXml_Content[i];
			}
			temp[current_size] = '\0';
			capacity = current_size;
			delete pXml_Content;
			pXml_Content = temp;
			fclose(pFile);
		}
	};
}

namespace nXml_Parser {
	using namespace nDynamic;
	void XmlFileResCk(nFile::ReadFile* xml_file) {
		printf("file�� ���� ���ҽ�\n");
		printf("�޾ƿ� xml�� �뷮(capacity) : %d\n", xml_file->capacity);
		printf("ũ��(current_size) : %d\n", xml_file->current_size);
	}

	void XmlWordCkToObj(nFile::ReadFile* xml_file) {
		char c;
		int obj_cnt = 0;
		for (int i = 0; i < xml_file->current_size; i++) {
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
		DynamicStr data(data_size);
		for (int i = 0; (i < xml_file->current_size); i++) {
			for (;; i++) {
				if ((c = xml_file->pXml_Content[i]) != ' ') break;
			}
			c = xml_file->pXml_Content[i];
			if (c == '<') {		//�±��϶�
				for (;; i++) {
					c = xml_file->pXml_Content[i];
					//�Ӽ� �߰� �ϴ� �� ���� ���� ����
					if (c == '>') {
						//��ü ����
						c = xml_file->pXml_Content[++i];
						break;
					}
				}
			}
			if (c != '\n' && c!=' ' && c != '\0') {	//�������϶�
				char ck_word;
				for (int j = 0; ; i++, j++) {
					if (j >= data.capacity - 2)
						data.SizeUpStr();
					c = xml_file->pXml_Content[i];
					data.p_d_char[j] = c;
					if ((ck_word = xml_file->pXml_Content[i + 1]) == '<' && (ck_word = xml_file->pXml_Content[i + 2]) == '/') {
						data.p_d_char[j + 1] = '\0';
						break;
					}
				}
				data_cnt++;
				printf("%d��° Data : %s\n", data_cnt, data.p_d_char);
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






