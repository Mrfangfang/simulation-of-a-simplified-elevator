﻿// strengthen.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"pch.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include "Windows.h"
#include <conio.h>
using namespace std;

class T {
public:
	int size;   // 当前容量
	int index[50];    // 乘客下标
	int now_f;     // 当前楼层
	bool next;    //     1 up   0 down
public:
	void UP();    // 上电梯
	void DOWN();  // 下电梯
	void change();
    T() {
		size = 0;
		index[50] = 0;

	}
};

class F {
public:
	int size;
	int index[50];
};    // 层楼

class Per {
public:
	int times;   // 次数
	int next_f;       //   目标楼层
	Per() {
		times = 0;
		next_f = 1;
	}  // 构造函数
	void F_Leave(int N, int n);   // 离开楼层
	void T_Leave(int n);     //  离开电梯
	//void flash(int n);
};
T t;
F f[11];
Per per[100];
int M, m, L, mark = 0;
//void F_Leave(int N, int n);   // 离开楼层
//void T_Leave(int n);     //  离开电梯
void flash(int n);    //  第n层的人按电梯
void show();    //  界面

int main() {
	int p = 1;
	cout << "请输入总人数:";
	cin >> M;
	f[1].size = M;
	for (int i = 0; i < M; i++) {
		f[1].index[i] = i;                 //（M人）第一层乘客下标初始化
	}
	cout << "请输入电梯最大承载量:";
	cin >> m;
	cout << "请输入乘坐次数:";
	cin >> L;
	flash(1);    // 所有人在一楼按电梯
	system("cls");
	show();
	cout << endl;
	Sleep(1000);
	while ((mark <= M)||(t.size)) {
		t.next = 1;   // 上楼     电梯一开始在1楼所以一定是在上楼
		for (int i = p; i <= 9; i++, p = i) {   //电梯通过for循环逐级上升
			t.now_f = i;
			t.DOWN();
			t.UP();
			t.change();
			system("cls");
			show();
			cout << endl;
			Sleep(1000);
            /*下面代码的作用
当前电梯如果没有人则对上面的楼层进行人员检测
如果所有楼层都无人则q会增加到11，t.next=0
电梯里有人则不会通过if语句
楼上只有有人就不会通过第二个if语句
*/
			int q = 0;
			if (t.size == 0) {
				for (q = i + 1; q <= 10; q++) {
					if (f[q].size != 0) {
						break;
					}
				}
				if (q == 11) {
					p = i;
					t.next = 0;   // 下楼
					break;
				}
			}
		}
		t.next = 0;  // 下楼
		for (int i = p; i >= 2; i--, p = i) {
			t.now_f = i;
			t.DOWN();
			t.UP();
			t.change();
			system("cls");
			show();
			cout << endl;
			Sleep(1000);
			/*下面代码的作用
当前电梯如果没有人则对上面的楼层进行人员检测
如果所有楼层都无人则q会减少到0，t.next=1
电梯里有人则不会通过if语句
楼上只有有人就不会通过第二个if语句
*/
			int q = 0;
			if (t.size == 0) {
				for (q = i - 1; q >= 1; q--) {
					if (f[q].size != 0) {
						break;
					}
				}
				if (q == 0) {
					t.next = 1;  // 上楼
					p = i;
					break;
				}
			}
		}
	}
	system("pause");
	return 0;
}

void show() {    //  界面
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取控制台句柄    
	for (int i = 10; i >= 1; i--) {
		if (t.now_f == i) {   // 电梯在当前层
			//SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED); // 设置为红色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);// 设置为绿色
			cout << i << "\t____\t:";
			if (f[i].size == 0) {
				cout << "\n";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				continue;
			}
			cout << "共有 " << f[i].size << " 人:";
			for (int j = 0; j < f[i].size; j++) {
				int k = f[i].index[j];
				if ((per[k].times == L) && (i == 1)) {
					SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED); // 设置为红色
					cout << f[i].index[j] << " (" << per[k].next_f << ")   ";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else {
					cout << f[i].index[j] << " (" << per[k].next_f << ")   ";
				}
			}
			cout << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		}
		else {
			//SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);// 设置为黄色
			cout << i << "\t____\t:";
			if (f[i].size == 0) {
				cout << "\n";
				continue;
			}
			cout << "共有 " << f[i].size << " 人:";
			for (int j = 0; j < f[i].size; j++) {
				cout << f[i].index[j] << " (" << per[f[i].index[j]].next_f << ")   ";
			}
			cout << endl;
		}
	}
	cout << "\n\n________________\n_____电梯_______\t:电梯上人数：" << t.size << endl;
	cout << "\n\t";
	for (int i = 0; i < t.size; i++) {
		cout << t.index[i] << " (" << per[t.index[i]].next_f << ")   ";;
	}
	cout << endl;
}

void T::change() {   //  随机按电梯
	
	if (t.now_f == 1) {
		flash(2);
	}
	else {
		if (t.now_f == 10) {
			flash(9);
		}
		else {
			if (t.next) {   // up
				flash(t.now_f - 1);
			}
			else {    //  down
				flash(t.now_f + 1);
			}
		}
	}
}
/*flash(n)
1、判断这一层是否有人
2、这一层的人是否乘梯次数已满
3、未满则设置新的目的地
*/
void flash(int n) {    //  第n层的人按电梯
	if (f[n].size == 0) {
		return;
	}
	for (int i = 0; i < f[n].size; i++) {
		int j = f[n].index[i];
		if ((per[j].times >= (L+1)) && (per[j].next_f == n)) {  //调整L
			mark++;							//mark作为判断外部的while循环条件
			per[j].next_f = 1;
			continue;
		}
		if (per[j].next_f == n) {  //如果目标楼层是当前楼层就就随机生成目标楼层数(已经完成前一次请求)
			int k;
			do {
				k = rand() % 10 + 1;
			} while (k == n);
			per[j].next_f = k;              //设置目标楼层             
			per[j].times++;
		}
	}
}

void Per::F_Leave(int N, int n) {   // 离开楼层
	int i, j;
	for (i = 0; i < f[N].size; i++) {
		if (n == f[N].index[i]) {
			break;
		}
	}
	for (j = i; j < f[N].size; j++) {
		f[N].index[j] = f[N].index[j + 1];
	}
	f[N].size--;
}

void Per::T_Leave(int n) {     //  离开电梯
	int i, j;
	for (i = 0; i < t.size; i++) {
		if (n == t.index[i]) {
			break;
		}
	}
	for (j = i; j < t.size; j++) {
		t.index[j] = t.index[j + 1];
	}
	t.size--;
}
void T::DOWN() {    // 下电梯
	if (t.size == 0) {
		return;
	}
	int i = 0;
	while (i < t.size) {
		int j = t.index[i];
		if (per[j].next_f == t.now_f) {
			per[j].T_Leave(j);
			f[t.now_f].index[f[t.now_f].size] = j;//index里面的标号比size小一
			f[t.now_f].size++;
		}
		else {
			i++;
		}
	}
}
void T::UP() {
	if (f[t.now_f].size == 0) {
		return;
	}
	int i = 0;
	while (i < f[t.now_f].size) {
		int j = f[t.now_f].index[i];  //这样j就等于人的编号
		if (t.next && (per[j].next_f - t.now_f > 0) && (t.size < m)) {
			per[j].times++;
			per[j].F_Leave(t.now_f, j);
			t.index[t.size] = j;
			t.size++;
		}
		else {
			if ((!t.next) && (per[j].next_f - t.now_f < 0) && (t.size < m)) {
				per[j].times++;
				per[j].F_Leave(t.now_f, j);
				t.index[t.size] = j;
				t.size++;
			}
			else {
				i++;
			}
		}
	}
	

}


//void change();   //  随机按电梯
//void UP();    // 上电梯
//void DOWN();    // 下电梯
