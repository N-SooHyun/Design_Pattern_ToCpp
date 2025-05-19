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
		hot = 1,
		iced,
		lukewarm
	};
	enum shot_syrup {
		one = 1,	
		two,		
		three
	};

	typedef struct Coffee_Opt{
		enum Temperature tmper;	//1:hot, 2:iced, 3:lukewarm
		enum shot_syrup shot;
		enum shot_syrup syrup;
	}Tree_Opt;

	class Coffee_Menu_Node {
	public:
		Coffee_Menu_Node() :coffee_name(new nDynamic::DynamicStr(1024)), price(0) {}
		~Coffee_Menu_Node() {
			delete coffee_name;
		}

	private:
		nDynamic::DynamicStr* coffee_name;
		int price;
		struct Coffee_Opt opt;
		friend class Coffee_Tree;
	};

	class Coffee_Tree {
	public:
		Coffee_Tree() : Root_Node(nullptr), Current_Node(nullptr), Left_Node(nullptr), Right_Node(nullptr){}
		~Coffee_Tree() {
			//소멸 반드시 해줘야 하긴해
		}

	private:
		Coffee_Menu_Node* Root_Node;
		Coffee_Menu_Node* Current_Node;
		Coffee_Menu_Node* Left_Node;
		Coffee_Menu_Node* Right_Node;
	};
}

#endif