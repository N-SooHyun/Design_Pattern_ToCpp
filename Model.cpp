#include "Lib.h"

namespace Tree_Struct {
	//Coffee_Menu_Node Å¬·¡½º ÇÔ¼öÁ¤ÀÇ






	//Coffee_Tree Å¬·¡½º ÇÔ¼öÁ¤ÀÇ
	//»ðÀÔ
	void Coffee_Tree::Insert() {

	}

	//»èÁ¦
	void Coffee_Tree::Delete() {

	}

	//°Ë»ö
	void Coffee_Tree::Select() {

	}



	void Coffee_Tree::start() {
		const char* name_1 = "¾Æ¸Þ¸®Ä«³ë";
		const char* name_2 = "Ä«Æä ¶ó¶¼";
		const char* name_3 = "¹Ù´Ò¶ó ¶ó¶¼";
		const char* name_4 = "¿À°î ¶ó¶¼";
		const char* name_5 = "³ìÂ÷ ¶ó¶¼";


		
		while (1) {
			if (Root_Node == nullptr) {
				Root_Node = new Coffee_Menu_Node();
				Root_Node->coffee_name->OperStr(name_1);
			}


		}

	}
}