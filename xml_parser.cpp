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
이를 객체지향으로 파싱한다고 가정하면 2가지로 나뉘는데 Tree, SAX
Tree는
class parent{};
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{};
3개의 클래스가 별도로 생기며
class parent{ son s, daughter d; }
class son{ public: string hair, gender; son(){gender = "male", hair="black";} };
class daughter{public: string hair, gender; daughter(){gender="female", hair="black";}};
parent.s.gender == male 이런느낌으로 만들면 되며
SAX는 필요한 데이터만 뽑아 쓰는 느낌으로 진행하여
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
		printf("file에 대한 리소스\n");
		printf("받아온 xml의 용량(capacity) : %d\n", xml_file->capacity);
		printf("크기(current_size) : %d\n", xml_file->current_size);
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
		printf("총 만들어져야 하는 객체 개수 : %d\n", obj_cnt);
	}

	void XmlObjParserTree(nFile::ReadFile* xml_file) {
		//계층형 구조로써 동작한는 프로그램
		char c; 
		int data_cnt = 0;
		int data_size = 1024;
		DynamicStr data(data_size);
		for (int i = 0; (i < xml_file->current_size); i++) {
			for (;; i++) {
				if ((c = xml_file->pXml_Content[i]) != ' ') break;
			}
			c = xml_file->pXml_Content[i];
			if (c == '<') {		//태그일때
				for (;; i++) {
					c = xml_file->pXml_Content[i];
					//속성 추가 하는 거 추후 만들 예정
					if (c == '>') {
						//객체 생성
						c = xml_file->pXml_Content[++i];
						break;
					}
				}
			}
			if (c != '\n' && c!=' ' && c != '\0') {	//데이터일때
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
				printf("%d번째 Data : %s\n", data_cnt, data.p_d_char);
				data.EmptyStr(data_size);
			}
		}
		printf("데이터 개수 : %d\n", data_cnt);
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






