#pragma once
#ifndef MODEL
#define MODEL
#include "Lib.h"

//CRUD의 역할만 하면 되는 영역
//데이터 HDD, SDD에서 읽어와서 자료구조의 형태로 파싱만 하는 역할
//파싱된 데이터를 CRUD의 형태에 맞게 API를 제공만 해주는 역할

//coffee menu 데이터 구조 Trie구조
namespace Trie_Struct {
	class Coffee_Menu_Trie {
	public:
		Coffee_Menu_Trie() {}
		~Coffee_Menu_Trie() {}
	};
}

//coffee menu 데이터 구조 tree구조
namespace Tree_Struct {
	enum Temperature {
		no_temp = 0,
		hot,
		iced,
		lukewarm
	};
	enum shot_syrup {
		no_shot_syrup = 0,
		one,	
		two,		
		three
	};

	typedef struct Coffee_Opt{
		enum Temperature tmper;	//1:hot, 2:iced, 3:lukewarm
		enum shot_syrup shot;
		enum shot_syrup syrup;
	}Tree_Opt;

	//Node용 클래스
	class Coffee_Menu_Node {
	public:
		Coffee_Menu_Node() :Left_Node(nullptr), Right_Node(nullptr), Parent_Node(nullptr),
							coffee_name(new nDynamic::DynamicStr(1024)), 
							price(0),
							opt{no_temp, no_shot_syrup, no_shot_syrup}
							{}
		~Coffee_Menu_Node() {
			delete coffee_name;
		}

	private:
		nDynamic::DynamicStr* coffee_name;
		int price;
		struct Coffee_Opt opt;
		friend class Coffee_Tree;
		friend class Coffee_Cart;
		Coffee_Menu_Node* Parent_Node;
		Coffee_Menu_Node* Left_Node;
		Coffee_Menu_Node* Right_Node;
	};

	//Node 관리용 클래스(커피메뉴 관리용 클래스)
	class Coffee_Tree {
	public:
		Coffee_Tree() : Root_Node(nullptr), Current_Node(nullptr){}
		~Coffee_Tree() {
			//소멸 반드시 해줘야 하긴해
		}

		//개발자용 메소드들
		//삽입
		void Insert();
		//삭제
		void Delete();
		//검색
		void Select();


		//API 사용자용 메소드들
		void start();

	private:
		Coffee_Menu_Node* Root_Node;
		Coffee_Menu_Node* Current_Node;
	};

	//Node 주문시 장바구니 관리 클래스(선형구조)
	class Coffee_Cart {
	public:

	private:

	};
}



namespace Json_Struct {
	
}

namespace Xml_Struct {

}

namespace Ini_Struct {

}

#endif