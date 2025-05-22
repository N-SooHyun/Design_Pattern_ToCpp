#pragma once
#include <stdio.h>
#include <Windows.h>
#include <stdint.h>
#include "xml_parser.h"
#include "SOLID.h"
#include <iostream>
#include "FileBuf.h"
#include <string>
#include <string.h>
#include <map>
#include <vector>



namespace nDynamic {
	//���� ���ڿ�
	class DynamicStr {		//���ڿ��� �������� �־��ִ� Ŭ���� ���Կ����ڵ� ����
	public:
		char* p_d_str;
		int current_size_str;
		int capacity_str;
		bool just_shallow_copy;

		DynamicStr(int size) : capacity_str(size), current_size_str(0), just_shallow_copy(false) {
			p_d_str = new char[capacity_str];
		}
		~DynamicStr() {
			if (just_shallow_copy) {
				just_shallow_copy = false;
				return;
			}
			delete[] p_d_str;
		}

		//���ڿ� ���� ���ִ� �޼ҵ�(�����ȣ�� API)
		void OperStr(const char* c) {
			int i = 0;
			for (; c[i] != '\0'; i++) {
				if (i >= capacity_str) {
					SizeUpStr();
				}
				p_d_str[i] = c[i];
			}
			p_d_str[i] = c[i];
			current_size_str = i;
			capacity_str = i;
			this->FitSizeStr();
		}

		void AsgOperStr(int pos, char c) {			//���� ���Կ���
			if (pos - 1 >= capacity_str) {
				SizeUpStr();
			}
			p_d_str[pos] = c;
			p_d_str[pos + 1] = '\0';
			current_size_str++;
		}
		void SizeUpStr() {							//���� ���� �����Ҷ� �������� �÷���
			int old_capacity = capacity_str;
			capacity_str *= 2;
			char* temp = new char[capacity_str];
			for (int i = 0; i < old_capacity; i++) {
				temp[i] = p_d_str[i];
			}
			delete p_d_str;

			p_d_str = temp;
		}

		void EmptyStr(int init_capacity_size) {		//���� ���ڿ� ��������
			delete p_d_str;
			p_d_str = new char[init_capacity_size];
			current_size_str = 0;
			capacity_str = init_capacity_size;
		}

		void FitSizeStr() {							//���ʿ��� ���� ���� ������
			char* temp = new char[current_size_str + 1];
			for (int i = 0; i < current_size_str; i++) {
				temp[i] = p_d_str[i];
			}
			temp[current_size_str] = '\0';
			delete p_d_str;
			p_d_str = temp;
		}

		static DynamicStr* SetStr(DynamicStr* name) {
			int pos = 0;
			char* _name = name->p_d_str;
			DynamicStr* ptrName = new DynamicStr(1024);
			for (; (_name[pos] != '\0'); pos++)
				ptrName->AsgOperStr(pos, _name[pos]);
			ptrName->FitSizeStr();

			return ptrName;
		}
	};

	//���ø� ��ü�� ���� �迭
	template <typename T>
	class DynamicArr {
	public:
		DynamicArr() :current_pos(0), capacity(0), obj_arr(nullptr) {};
		~DynamicArr() {

			for (int i = 0; i < current_pos; i++) {
				delete obj_arr[i];
			}
			delete[] obj_arr;
		};

		DynamicArr(const DynamicArr& other) {
			printf("DynamicArr ���������\n");
		}

		//���� �迭 �ʱ�ȭ
		void InitDyArr() {
			if (obj_arr == nullptr) {
				capacity = 4;
				obj_arr = new T * [capacity];
			}
		}

		//�迭 ũ�� ����(���� �˻��)
		void AddDyArr() {
			if (current_pos >= capacity) {
				int old_capacity = capacity;
				capacity = (capacity * 2);
				T** new_obj_arr = new T * [capacity];

				for (int i = 0; i < old_capacity; i++) {
					new_obj_arr[i] = obj_arr[i];
				}
				delete[] obj_arr;
				obj_arr = new_obj_arr;
			}
			return;
		}

		//�Ӽ��̸����� �����̶� ���� ���ÿ�
		void InsertDyArr(DynamicStr* NodeName) {
			AddDyArr();
			T* new_attr = new T;
			new_attr->SetName(NodeName);
			obj_arr[current_pos++] = new_attr;
		}

		//�Ӽ��� ������ ����
		template<typename para>
		void InsertDyArr(para* NodeName) {
			AddDyArr();
			obj_arr[current_pos++] = NodeName;
		}


	public:
		int current_pos;
		int capacity;
		T** obj_arr;		//������ �迭�� Ȱ��
	};
}

#include "UI_View.h"
#include "Ctrl.h"
#include "Model.h"