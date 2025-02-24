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
�̸� ��ü�������� �Ľ��Ѵٰ� �����ϸ�
class parent{};
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{};
3���� Ŭ������ ������ �����
class parent{ son s, daughter d; }
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{public: string hair, gender; daughter(){gender="female", hair="black";}};
parent.s.gender == male �̷��������� ���� �� �� �ֵ��� �ڷᱸ���� ����� �Ľ��ϴ� ���α׷��� ���� �� �ִ�
*/

namespace file {
	class read_file {
	public:
		FILE* pFile = NULL;
		char* xml_content;
		int capacity;
		int current_size;

		read_file(char* path) {
			//fopen_s(&pFile, path, "r");
			fopen_s(&pFile, path, "r");
			capacity = 1024;
			xml_content = new char[capacity];
			current_size = 0;
			copy_file();
		}
		~read_file() {
			delete xml_content;
			fclose(pFile);
		}
		void copy_file() {
			char c;
			char* temp;
			for (current_size = 0; ; current_size++) {
				c = fgetc(pFile);
				if (c == EOF) break;
				else if (current_size == capacity - 1) {
					capacity *= 2;
					temp = xml_content;
					xml_content = new char[capacity];
					for (int i = 0; i < current_size; i++) {
						xml_content[i] = temp[i];
					}
					delete temp;
				}
				xml_content[current_size] = c;
			}
			xml_content[current_size] = '\0';
			temp = new char[current_size+1];
			for (int i = 0; i<current_size; i++) {
				temp[i] = xml_content[i];
			}
			temp[current_size] = '\0';
			capacity = current_size;
			delete xml_content;
			xml_content = temp;
			fclose(pFile);
		}
	};
}

namespace xml_parser {
	void open_xml_file(file::read_file* file) {
		printf("%s\n", file->xml_content);
		printf("%d\n", file->current_size);
	}

	void xml_parser_main() {
		char path[] = "directory\\test.xml";
		file::read_file test(path);
		printf("%s\n", test.xml_content);
		//open_xml_file(&test);
	}
}






