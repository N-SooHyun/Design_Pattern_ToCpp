#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <Windows.h>
#include "FileBuf.h"
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
		
		//속성이름으로 생성이랑 삽입 동시에
		void InsertDyArr(DynamicStr *NodeName) {
			AddDyArr();
			T* new_attr = new T;
			new_attr->SetName(NodeName);
			obj_arr[current_pos++] = new_attr;
		}

		//속성만 가지고 삽입
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



void XmlDataStruct(char* xml_str);

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
			AttrName = name;
		} 

		char* GetName() {
			return AttrName->p_d_str;
		}

		void SetData(DynamicStr* data) {
			AttrData = data;
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
		XmlObj() : TagName(nullptr), Parent(nullptr), TagData(nullptr) {};
		~XmlObj() {
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

		//태그 데이터 설정하기
		void SetData(DynamicStr* data) {
			TagData = DynamicStr::SetStr(data);
		}
		//태그데이터 가져오기
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

		void AddAttrObj(AttrObj* Attr) {
			if (AttrArr.obj_arr == nullptr) InitAttrArr();
			AttrArr.InsertDyArr<AttrObj>(Attr);
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

	void XmlParseMain() {
		using namespace nDynamic;

		char path[] = "..\\Access_File_Security_Git\\test.xml";
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
using namespace nXml_Parser;
XmlObj* Parser_Test(char* xml_str) {
	int xml_str_max = 1024;
	int xml_current_pos = 0;
	char c;
	bool xml_end_ck = false;
	bool attr_data_end_ck = false;
	DynamicStr* AttrName = nullptr;
	DynamicStr* TagName = nullptr;
	DynamicStr* AttrData = nullptr;
	DynamicStr* Data = nullptr;
	XmlObj* RootXml = nullptr;
	XmlObj* XmlPtr = nullptr;
	XmlObj* ParentXmlPtr = nullptr;
	AttrObj* AttrPtr = nullptr;
	

	for (;; xml_current_pos++) {
		c = xml_str[xml_current_pos];

		if (c == '\0') break;

		if (c == '\n' || c == ' ' || c == '\t') continue;

		else if (c == '<') {		//Tag모드
			xml_current_pos++;
			xml_end_ck = false;

			c = xml_str[xml_current_pos];

			if (c == '!') {	//주석 처리
				xml_end_ck = true;
			}
			else if (c == '/') {
				xml_end_ck = true;
				if (XmlPtr->Parent != nullptr) {
					XmlPtr = XmlPtr->Parent;
				}
				//최상위 루트임
			}
			else if (c == '?') {	//버전
				xml_end_ck = true;
			}
			else {
				TagName = new DynamicStr(xml_str_max);
				if (XmlPtr != nullptr) {
					//자신이 '누군가'의 자식 태그임
					ParentXmlPtr = XmlPtr;
					XmlPtr = new XmlObj();
					ParentXmlPtr->AddXmlObj(XmlPtr);
				}
				else
					XmlPtr = new XmlObj();
			}
			for (int j = 0; ; j++, xml_current_pos++) {
				c = xml_str[xml_current_pos];
				if (c == '>') break;
				else if (c == ' ' && !xml_end_ck) {
					//속성 모드
					while (1) {
						xml_current_pos++;
						AttrName = new DynamicStr(xml_str_max);
						AttrData = new DynamicStr(xml_str_max);
						for (int k = 0; ; k++, xml_current_pos++) {
							c = xml_str[xml_current_pos];
							if (c == '=') break;
							AttrName->AsgOperStr(k, c);
						}
						bool start_attr_ck = false;
						xml_current_pos++;
						for (int k = 0; ; k++, xml_current_pos++) {
							c = xml_str[xml_current_pos];
							if (c == '\"') {	//속성 데이터 종료
								if (start_attr_ck) {
									xml_current_pos++;
									break;
								}
								else {
									start_attr_ck = true;
									k--;
									continue;
								}
							}
							AttrData->AsgOperStr(k, c);
						}
						AttrName->FitSizeStr();
						AttrData->FitSizeStr();
						AttrPtr = new AttrObj();
						AttrPtr->SetName(AttrName);
						AttrPtr->SetData(AttrData);
						XmlPtr->AddAttrObj(AttrPtr);					

						c = xml_str[xml_current_pos];
						if (c == ' ') continue;
						else if (c == '>') {
							xml_current_pos--;
							xml_end_ck = true;
							break;
						}
					}
				}
				if (xml_end_ck) continue;
				TagName->AsgOperStr(j, c);
			}
			//반복문 빠져나오면 태그 생성해줌
			if (TagName != nullptr) {
				TagName->FitSizeStr();
				XmlPtr->SetName(TagName);
				TagName = nullptr;
			}
		}
		else if (c != '>') {		//무조건 데이터라는 얘기임
			xml_current_pos--;
			Data = new DynamicStr(xml_str_max);
			for (int i = 0; ; i++) {
				c = xml_str[++xml_current_pos];

				if (c == '<') {
					xml_current_pos--;
					break;
				}
				Data->AsgOperStr(i, c);
			}
			Data->FitSizeStr();
			XmlPtr->SetData(Data);
		}
	}

	for (;;) {
		if (XmlPtr->Parent == nullptr) {
			RootXml = XmlPtr;
			break;
		}
	}

	return RootXml;

	//delete RootXml;
}

class UI_Xml{
public:
	UI_Xml(XmlObj* Node) {
		CurrentNode = Node;
		child_obj_list_num = 0;
		child_attr_list_num = 0;
		print_use = true;
	}
	UI_Xml() :CurrentNode(nullptr) {}
	~UI_Xml() {
	}

	//현재 태그의 이름 가져오기
	void GetCurrentNodeName() {
		if(print_use)
			printf("Node Name : <%s>\n", CurrentNode->GetName());
	}

	//상위 태그의 이름을 가져오기
	void GetCurrentNodeParentName() {
		if (print_use) {
			if (CurrentNode->Parent == nullptr) {
				printf("%s가 최상위 Node입니다.\n", CurrentNode->GetName());
				return;
			}
			printf("ParentNode : <%s>\n", CurrentNode->Parent->GetName());
		}
	}

	//현재 태그의 데이터 있으면 가져오기
	void GetCurrentNodeData() {
		if (print_use) {
			if (CurrentNode->TagData != nullptr)
				printf("Data : %s\n", CurrentNode->GetData());
			else
				printf("No Data\n");
		}
	}

	//현재 태그의 하위 속성 이름 데이터 리스트 출력
	void GetCurrentNodeChildAttrList() {
		child_attr_list_num = 0;
		for (int i = 1; i <= CurrentNode->AttrArr.current_pos; i++, this->child_attr_list_num++) {
			if (print_use) {
				printf("%d. AttrName : %s\AttrData : %s\n", i, CurrentNode->AttrArr.obj_arr[i-1]->GetName(), CurrentNode->AttrArr.obj_arr[i - 1]->GetData());
			}
		}
	}

	//현재 태그의 하위 객체 리스트 출력
	bool GetCurrentNodeChildObjList() {
		child_obj_list_num = 0;
		for (int i = 1; i<=CurrentNode->XmlObjArr.current_pos; i++, this->child_obj_list_num++) {
			if (print_use) {
				printf("%d. %s\n", i,CurrentNode->XmlObjArr.obj_arr[i-1]->GetName());
			}
		}
		if (child_obj_list_num == 0) {
			printf("하위 객체가 없습니다.\n");
			return false;
		}
		return true;
	}

	//부모 노드로 돌아가기
	void SetCurrentNodeParent() {
		if (CurrentNode->Parent == nullptr) {
			printf("현재 노드가 최상위 노드입니다.\n");
			return;
		}
		CurrentNode = CurrentNode->Parent;
		printf("Node 변경 완료!\n");
	}

	//선택한 객체로 Node 변경하기
	void SetCurrentNodeSelectObj(int pos) {
		if (child_obj_list_num == 0) {
			print_use = false;
			this->GetCurrentNodeChildObjList();
			print_use = true;
		}
		if (pos > child_obj_list_num || pos < 1) {
			printf("범위가 잘못 되었습니다.\n");
			return;
		}
		CurrentNode = CurrentNode->XmlObjArr.obj_arr[pos - 1];
		printf("Node 변경 완료!\n");
	}

private:
	XmlObj* CurrentNode;
	int child_obj_list_num;
	int child_attr_list_num;
	bool print_use;
};

void XmlDataStruct(char* xml_str) {
	XmlObj* Root = Parser_Test(xml_str);

	UI_Xml UI(Root);

	int cmd = -1;

	while (1) {
		system("cls");  // 화면을 지우기 전에 유용한 정보는 보여주기

		// 현재 노드 정보 표시
		UI.GetCurrentNodeParentName();
		UI.GetCurrentNodeName();
		UI.GetCurrentNodeData();
		printf("================================\n");

		// 메뉴 출력
		printf("1. 객체 선택하기\n");
		printf("2. 객체 조회하기\n");
		printf("3. 현재 노드 속성 조회\n");
		printf("4. 현재 노드 데이터 조회\n");
		printf("5. 부모 노드로 돌아가기\n");
		printf("6. 종료\n");
		printf("================================\n");

		// 사용자에게 입력을 받음
		printf("원하는 작업을 선택하세요 (1-5): ");
		scanf_s("%d", &cmd);

		// 선택에 따른 처리
		switch (cmd) {
		case 1:
			printf("\n[객체 선택]\n");
			if (UI.GetCurrentNodeChildObjList()){  // 객체 목록 표시
				printf("선택할 객체 번호를 입력하세요: ");
				scanf_s("%d", &cmd);
				UI.SetCurrentNodeSelectObj(cmd);  // 선택한 객체 처리
			}
			break;
		case 2:
			printf("\n[객체 조회]\n");
			UI.GetCurrentNodeChildObjList();  // 객체 목록 표시
			break;
		case 3:
			printf("\n[현재 노드 속성 조회]\n");
			UI.GetCurrentNodeChildAttrList();  // 노드 속성 조회
			break;
		case 4:
			printf("\n[현재 노드 데이터 조회]\n");
			UI.GetCurrentNodeData();  // 노드 데이터 조회
			break;
		case 5:
			printf("\n[부모 노드로 돌아가기]\n");
			UI.SetCurrentNodeParent();
			break;
		case 6:
			printf("\n프로그램을 종료합니다.\n");
			return;  // 종료
		default:
			printf("\n잘못된 입력입니다. 다시 시도하세요.\n");
			break;
		}

		// 작업 완료 후 다시 메뉴로 돌아가게끔
		printf("\n계속하려면 Enter 키를 누르세요...\n");
		getchar();  // 이전 scanf_s에서 남은 개행 문자를 처리
		getchar();  // 실제로 Enter를 기다림
	}

	delete Root;
}

nFile::ReadFile* File_Struct() {
	int cmd;
	File_Handle::File_Main* Dir = new File_Handle::File_Main("..\\Access_File_Security_Git\\Xml", ".xml");
	while (1) {
		printf("<파일 목록>\n");
		printf("================================\n");
		Dir->Files_Print();
		printf("================================\n");
		printf("파싱하고자 하는 파일을 선택하여 주세요\n");
		scanf_s("%d", &cmd);
		Dir->Select_File(cmd);

		printf("선택한 파일경로 : %s\n", Dir->file_buffer->pXml_Content);
		break;
		/*if(Dir->file_buffer != nullptr){
			nFile::ReadFile* xml_file = new nFile::ReadFile(Dir->file_path);
			return xml_file;
		}*/
	}
	return Dir->file_buffer;
}


void Xml_Parser_Main() {
	char path[] = "..\\Access_File_Security_Git\\Xml\\RollMount_EtherCAT.xml";
	nFile::ReadFile test(path);

	//XmlDataStruct(test.pXml_Content);
	printf("Xml파일을 파싱하여 1개의 자료형 데이터로 만들어주는 프로그램입니다.\n");
	
	nFile::ReadFile *file = File_Struct();

	XmlDataStruct(file->pXml_Content);
	
}