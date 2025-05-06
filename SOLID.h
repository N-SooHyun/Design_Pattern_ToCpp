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
		Main_UI(){}
		~Main_UI() {}
		
		void First_Main();
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