void XmlObjParserTree(nFile::ReadFile* xml_file) {
	//계층형 구조로써 동작한는 프로그램
	char c; 
	int data_cnt = 0;
	int obj_cnt = 0;
	int data_size = 1024;				//데이터문자 개수
	int tag_size = 1024;				//태그문자 개수
	int attr_size = 1024;				//속성문자 개수
	DynamicStr data(data_size);			//데이터
	DynamicStr tag_name(tag_size);		//태그이름
	DynamicStr attr_name(attr_size);	//속성명
	DynamicStr attr_data(attr_size);	//속성데이터
	
	
	XmlObj* Root = nullptr;
	XmlObj* tag = nullptr;

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

			if (Root == nullptr) Root = new XmlObj();		//루트 없을때는 루트에 만들기
			else tag = new XmlObj();

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
				
				/*tag = new XmlObj(tag_name.p_d_str);
				if (tr_tag_root == false) {
					tr_tag_root = true;
					tag_root = tag; 
				}
				printf("%d. tag : %s\n", ++obj_cnt,tag->obj_name->p_d_str);*/
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
			//printf("%d. data : %s\n",++data_cnt, data.p_d_str);
			data.EmptyStr(data_size);
		}

	} while (i<xml_file->current_size_str);

	
}

