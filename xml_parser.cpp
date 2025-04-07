#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <Windows.h>

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
		bool just_shallow_copy;

		DynamicStr(int size) : capacity_str(size), current_size_str(0), just_shallow_copy(false){
			p_d_str = new char[capacity_str];
		}
		~DynamicStr() {
			printf("Str 소멸자\n");
			if (just_shallow_copy) {
				just_shallow_copy = false;
				return;
			}
			delete[] p_d_str;
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

	//템플릿 동적 배열
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
			printf("DynamicArr 복사생성자\n");
		}
		
		//동적 배열 초기화
		void InitDyArr() {
			if (obj_arr == nullptr) {
				capacity = 4;
				obj_arr = new T*[capacity];
			}
		}

		//배열 크기 증가(예외 검사용)
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
		
		//속성 삽입
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
		T** obj_arr;		//포인터 배열을 활용
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
			printf("AttrObj 복사생성자\n");
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
		DynamicStr* AttrName;		//속성인 자신의 이름
		DynamicStr* AttrData;		//속성의 데이터
	};
	
	class XmlObj {
	public:
		XmlObj() : TagName(nullptr) {};
		~XmlObj() {
			printf("소멸자 XmlObj : %p\n", this);
			delete TagName;
			//속성들 소멸자
			//delete &AttrArr;		이거 없어도 애체오 AttrArr이 알아서 소멸함
		} 
		XmlObj(const XmlObj& other) {
			printf("XmlObj 복사생성자\n");
		}

		//태그 이름 설정하기
		void SetName(DynamicStr* name) {
			TagName = DynamicStr::SetStr(name);
		}
		//태그이름 가져오기
		char* GetName() {
			return TagName->p_d_str;
		}

		//태그 이름 설정하기
		void SetData(DynamicStr* data) {
			TagData = DynamicStr::SetStr(data);
		}
		//태그이름 가져오기
		char* GetData() {
			return TagData->p_d_str;
		}

		//속성 배열 생성 및 초기화
		void InitAttrArr() {
			AttrArr.InitDyArr();
		}

		//태그 배열 생성 및 초기화
		void InitTagArr() {
			XmlObjArr.InitDyArr();
		}

		//사용자 코드
		//속성배열 삽입
		void AddAttrObj(DynamicStr* AttrName) {
			if (AttrArr.obj_arr == nullptr) InitAttrArr();
			AttrArr.InsertDyArr(AttrName);
		}

		//태그배열 삽입
		void AddXmlObj(XmlObj *XmlNode) {
			if (XmlObjArr.obj_arr == nullptr) InitTagArr();
			XmlObjArr.InsertDyArr<XmlObj>(XmlNode);
			XmlNode->Parent = this;
		}
		
	public:
		XmlObj* Parent;				//부모 객체
		DynamicStr* TagData;		//객체인 나의 데이터(Tag당 1개)
		DynamicStr* TagName;		//객체인 자신의 이름
		DynamicArr<AttrObj> AttrArr;		//나만의 속성'들'
		DynamicArr<XmlObj> XmlObjArr;		//하위 객체
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

		printf("태그 이름 : %s\n", testTag.GetName());

		printf("\n 속성 추가 2개\n");

		//속성이름 2개 생성
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

		else if (c == '<') {		//Tag모드
			xml_current_pos++;

			xml_end_ck = false;
			for (int j = 0; ; j++, xml_current_pos++) {
				c = xml_str[xml_current_pos];
				if (c == '>') break;
				else if (c == '/') {
					xml_end_ck = true;
				}
				else if (c == ' ') {
					//속성 넣어주면 됨
				}
			}
			//반복문 빠져나오면 태그 생성해줌
		}
		else if (c != '>') {		//무조건 데이터라는 얘기임
			
		}



	}
	

}



