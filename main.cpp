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
	T** obj_arr;		//�����͹迭�� ������ �ҵ�
};

int main() {
	//��ü���⿡ ���� ���� ���ظ� ���ؼ�
	//���������� ���� Ưȭ ���α׷��� XML,DOM
	//������ ���Ͽ� ���ؼ� �����ϰ� ������� �ð� ��������

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
*/