#include <iostream>

#include "xml_parser.h"

class dummy {
public:
	dummy(char* name, char* data) {
		this->name = name;
		this->data = data;
	}
	dummy() {}
	~dummy() {
		
	}

	char* name;
	char* data;
};

template <typename T>
class test {
public:
	test() {
		currentPos = 0;
		capacity = 3;
		obj_arr = new T * [capacity];
		for (int i = 0; i < capacity; i++) {
			obj_arr[i] = nullptr;
		}
	}
	~test() {
		delete[] obj_arr;
	}

	void Add_Arr() {
		int old_capacity = capacity;
		capacity += 3;

		T** new_obj_arr = new T*[capacity];

		for (int i = 0; i < old_capacity; i++) {
			new_obj_arr[i] = obj_arr[i];
		}
		delete[] obj_arr;
		obj_arr = new_obj_arr;
	}

	void ck() {
		if (currentPos >= capacity) {
			Add_Arr();
			return;
		}
		return;
	}

	void Delete_Arr_Data() {
		
	}

	void operator<<(T& insert_data) {
		ck();
		obj_arr[currentPos++] = &insert_data;
	}

	int currentPos;
	int capacity;
	T** obj_arr;		//포인터배열을 만들어야 할듯
};

int main() {
	//객체지향에 대한 깊은 이해를 위해서
	//계층구조에 대한 특화 프로그램인 XML,DOM
	//디자인 패턴에 대해서 간략하게 배워보는 시간 가져보기

	nXml_Parser::XmlParseMain();


	


	return 0;
}






/*
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
			if (c == '<') {		//태그일때
				tr_end_tag = false;
				c = xml_file->pXml_Content[++i];
				for (int j = 0; ; i++, j++) {
					c = xml_file->pXml_Content[i];
					if (c == '>') break;
					if ((c == '/') && (xml_file->pXml_Content[i-1] == '<')) tr_end_tag = true;
					else if (c == ' ') {}//attr_input = true;	//속성입력
					if (tr_end_tag != true) {		//태그 이름 넣어주기
						tag_name.OperStr(j, c);
					}
				}
				if (tr_end_tag != true) {			//태그 종료 아닐때 태그 생성
					if (tr_attr_input == true) {	//속성값 있다면

					}
					tag_name.FitSizeStr();
					tag = new XmlObj(tag_name.p_d_str, tag_name.current_size_str);
					printf("Tag : %s\n", tag_name.p_d_str);
					tag_name.EmptyStr(tag_size);
				}
			}
			//데이터일때
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
*/