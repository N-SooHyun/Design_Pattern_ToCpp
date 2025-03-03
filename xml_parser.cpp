#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

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
	class DynamicStr {		//문자열들 동적으로 넣어주는 클래스 대입연산자도 있음
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

		void AsgOperStr(int pos, char c) {			//문자 대입연산
			if (pos-1 >= capacity_str) {
				SizeUpStr();
			}
			p_d_str[pos] = c;
			p_d_str[pos + 1] = '\0';
			current_size_str++;
		}		
		void SizeUpStr() {							//문자 개수 부족할때 동적으로 늘려줌
			int old_capacity = capacity_str;
			capacity_str *= 2;
			char* temp = new char[capacity_str];
			for (int i = 0; i < old_capacity; i++) {
				temp[i] = p_d_str[i];
			}
			delete p_d_str;
			p_d_str = temp;
		}

		void EmptyStr(int init_capacity_size) {		//동적 문자열 해제해줌
			delete p_d_str;
			p_d_str = new char[init_capacity_size];
			current_size_str = 0;
			capacity_str = init_capacity_size;
		}

		void FitSizeStr() {							//불필요한 동적 영역 없애줌
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
		DynamicStr* obj_name;						//오브젝트의 이름(태그명)
		//map<string, vector<XmlObj*>> children;		//자식 태그들
		XmlObj* children;							//자식 태그들
		void* data;									//데이터

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
		printf("file에 대한 리소스\n");
		printf("받아온 xml의 용량(capacity) : %d\n", xml_file->capacity);
		printf("크기(current_size_str) : %d\n", xml_file->current_size_str);
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
		printf("총 만들어져야 하는 객체 개수 : %d\n", obj_cnt);
	}

	void XmlObjParserTree(nFile::ReadFile* xml_file) {
		//계층형 구조로써 동작한는 프로그램
		char c; 
		int data_cnt = 0;
		int data_size = 1024;				//데이터문자 개수
		int tag_size = 1024;				//태그문자 개수
		int attr_size = 1024;				//속성문자 개수
		DynamicStr data(data_size);			//데이터
		DynamicStr tag_name(tag_size);		//태그이름
		DynamicStr attr_name(attr_size);	//속성명
		DynamicStr attr_data(attr_size);	//속성데이터
		
		XmlObj* tag_root;
		XmlObj* tag;

		bool tr_end_tag = false;		//태그 생성인지 /인지 체크해주는 트리거
		bool tr_attr = false;
		bool tr_tag_root = false;
		bool tr_attrs = false;

		int i = -1;
		do {
			i++;
			c = xml_file->pXml_Content[i];
			
			if (c == '\n' || c == ' ') {	//공백 제거
				continue;
			}
			else if (c == '<') {	//태그 생성(속성도 추가)
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
					else if (c == ' ') {	//속성 생성(속성명, 데이터 1쌍으로 이루어짐 ex:attr="data")
						int k = -1;
						tr_attr = true;
						do {				//속성명
							i++, k++;
							c = xml_file->pXml_Content[i];
							if (c == ' ') {			//다중속성인경우
								tr_attrs = true;
								attr_data.EmptyStr(1024);
								attr_name.EmptyStr(1024);
								continue;
							}
							else if (c == '\"') {	//속성데이터
								int l = -1;
								do {
									i++, l++;
									c = xml_file->pXml_Content[i];
									if (c == '\"') {	//속성데이터 끝임을 암시
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
							else {					//속성명
								attr_name.AsgOperStr(k, c);
							}
						} while (1);
						if (tr_attrs) {//속성이 여러개

						}
						else {//속성이 1개

						}
					}
				} while (1);
				if (tr_end_tag != true) {		//태그가 </> 이게 아닐때만 만들어주기
					tag = new XmlObj(tag_name.p_d_str);
					if (tr_tag_root == false) {
						tr_tag_root = true;
						tag_root = tag; 
					}
					//printf("tag : %s\n", tag->obj_name->p_d_str);
				}
			}
			else if (c != '>' && tr_end_tag == false) {		//데이터일때
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
				//데이터를 넣어주면 된다 태그에
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






