#pragma once
#ifndef MODEL
#define MODEL
#include "Lib.h"

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
		Coffee_Menu_Node* Parent_Node;
		Coffee_Menu_Node* Left_Node;
		Coffee_Menu_Node* Right_Node;
	};

	//Node 관리용 클래스
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
}

#endif