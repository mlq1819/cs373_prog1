#ifndef MQ_TURING_H
#define MQ_TURING_H
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
		Transition(std::string str);
		bool equals(Transition o) const;
		int getQ()const{return this->q;};
		char getA()const{return this->a;};
		int getR()const{return this->r;};
		char getB()const{return this->b;};
		char getX()const{return this->x;};
};

class State{
	private:
		bool start;
		bool accept;
		bool reject;
		int num;
		std::vector<Transition> trans;
		friend class Turing;
		Trans * getTrans(char a) const;
	public:
		State(std::string);
		State(int num, bool start, bool accept, bool reject);
		State(int num);
		bool equals(State o) const;
		bool addTrans(Transition t);
		bool addTrans(std::string str);
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
		State * getState(int num) const;
		Trans * getTrans(int num, char a) const;
	public:
		Turing();
		~Turing();
		bool hasStart(){return this->start!=-1;};
		int getCurrent(){return this->current;};
		bool addState(State s);
		bool addState(std::string str);
		bool hasState(int num) const;
		bool hasTrans(int num, char a);
		bool addTrans(Transition t);
		bool addTrans(std::string);
};

#endif