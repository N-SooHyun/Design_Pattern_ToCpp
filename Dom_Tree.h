#pragma once
#ifndef DOM_TREE
#define DOM_TREE

namespace nDynamic {

	class DynamicStr {		//문자열들 동적으로 넣어주는 클래스 대입연산자도 있음
	public:
		char* p_d_str;
		int current_size_str;
		int capacity_str;
		bool just_shallow_copy;

		DynamicStr(int size) : capacity_str(size), current_size_str(0), just_shallow_copy(false) {
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
			if (pos - 1 >= capacity_str) {
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
			char* temp = new char[current_size_str + 1];
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
				obj_arr = new T * [capacity];
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
		void InsertDyArr(DynamicStr* NodeName) {
			AddDyArr();
			T* new_attr = new T;
			new_attr->SetName(NodeName);
			obj_arr[current_pos++] = new_attr;
		}

		//속성만 가지고 삽입
		template<typename para>
		void InsertDyArr(para* NodeName) {
			AddDyArr();
			obj_arr[current_pos++] = NodeName;
		}


	public:
		int current_pos;
		int capacity;
		T** obj_arr;		//포인터 배열을 활용
	};
}

namespace Dom_Parser {                    
	using namespace std;
	using namespace nDynamic;
	
}



namespace User_Interface {
	void UI_Main();
}

#endif