#pragma once
#ifndef SOLID
#define SOLID
#include <stdio.h>
#include <Windows.h>

void Coffee_Main();


//UI
//Root Class
namespace UI_Class {
	class Main_UI {
	public:
		Main_UI() : select(0) {}
		~Main_UI() {}
		
		void First_Main();
		virtual void Side_Main_Select_Coffee() = 0;
	protected:
		int select;
	};


	class Side_UI : public Main_UI{
	public:
		Side_UI() {}
		~Side_UI() {}

		void Side_Main_Select_Coffee();
	private:

	};

}

//Control
//Root Class
class Coffee {
	Coffee() {}
	~Coffee() {}


};


#endif