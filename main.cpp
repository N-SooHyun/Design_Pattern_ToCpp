#include <iostream>

#include "xml_parser.h"
#include "Dom_Tree.h"


int main() {
	//객체지향에 대한 깊은 이해를 위해서
	//계층구조에 대한 특화 프로그램인 XML,DOM
	//디자인 패턴에 대해서 간략하게 배워보는 시간 가져보기	

	
	Xml_Parser_Main();

	//User_Interface::UI_Main();

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