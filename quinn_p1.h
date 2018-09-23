#ifndef MQ_TURING_H
#define MQ_QUINN_P1_H
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>

class Transition{
	private:
		int q;
		char a;
		int r;
		char b;
		char x;
		friend class State;
	public:
		Transition(std::vector<std::string> str);
		bool equals(Transition o) const;
		int getQ()const{return this->q;};
		char getA()const{return this->a;};
		int getR()const{return this->r;};
		char getB()const{return this->b;};
		char getX()const{return this->x;};
		void print() const;
};

class State{
	private:
		bool start;
		bool accept;
		bool reject;
		int num;
		std::vector<Transition> trans;
		friend class Turing;
		Transition * getTrans(char a);
	public:
		State(std::vector<std::string>);
		State(int num, bool start, bool accept, bool reject);
		State(int num);
		bool equals(State o) const;
		bool addTrans(Transition t);
		bool addTrans(std::vector<std::string> str);
		bool hasTrans(char a) const;
		int getNum()const{return this->num;};
		bool isStart()const{return this->start;};
		bool isAccept()const{return this->accept;};
		bool isReject()const{return this->reject;};
};

class Turing{
	private:
		std::vector<State> states;
		int cur;
		int start;
		int index;
		unsigned long counter;
		unsigned long max;
		State * getState(int num);
		Transition * getTrans(int num, char a);
		bool go();
	public:
		Turing(std::string str, unsigned long max);
		std::string str;
		bool hasStart(){return this->start!=-1;};
		int getCur(){return this->cur;};
		bool addState(State s);
		bool addState(std::vector<std::string> str);
		bool hasState(int num) const;
		bool hasTrans(int num, char a) const;
		bool addTrans(Transition t);
		bool addTrans(std::vector<std::string> str);
		bool begin();
};

#endif