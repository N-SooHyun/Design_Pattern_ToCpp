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

	class Coffee_Menu_Tree {
	public:
		Coffee_Menu_Tree() :coffee_name(new nDynamic::DynamicStr(1024)), price(0) {}
		~Coffee_Menu_Tree() { 
			delete coffee_name;
		}

		void test();

	private:
		nDynamic::DynamicStr* coffee_name;
		int price;
		struct Coffee_Opt opt;
	};
}

#endif