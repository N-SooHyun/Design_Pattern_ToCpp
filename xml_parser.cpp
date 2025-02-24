#include <stdio.h>
#include <string.h>

//XML은 <>형태로 싸여진 계층형 데이터 구조 문서이다.
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

이런 형태로써 구성이 되어있는것이 xml파일의 구조이며
이러한 xml 파일을 객체지향을 통해서 각각의 태그마다 객체로써 동작하도록 진행하는 파싱 프로그램을 만들어보면서
xml에 대한 이해와 더불어서 객체지향에 대한 이해 그리고 계층형 자료구조에 대한 이해를 한번에 잡아보는 시간을 가져보자
*/

//1. xml은 상속관계가 아닌 포함관계
//2. <parent> <child> <child> ... </> 태그내 여러 태그는 허용과 더불어 같은 이름태그도 허용
//3. <parent> data1, data2 ... </> 태그내 여러 데이터는 불가능
//4. <parent> data1 <> </> 태그내 태그와 data는 같이 못있음 오직 태그 혹은 데이터 1개만 있어야함
//5. data의 형태는 문자, 숫자 타입 구분없이 문자열로 저장된다.

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
이를 객체지향으로 파싱한다고 가정하면
class parent{};
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{};
3개의 클래스가 별도로 생기며
class parent{ son s, daughter d; }
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{public: string hair, gender; daughter(){gender="female", hair="black";}};
parent.s.gender == male 이런느낌으로 접근 할 수 있도록 자료구조를 만들어 파싱하는 프로그램을 만들 수 있다
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






