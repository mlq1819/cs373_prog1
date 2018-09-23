#include "Turing.h"
#include "Filereader.h"
#define DEBUG true

using namespace std;

int main(int argc, char *argv[]){
	if(argc!=4){ //if bad command
		if(argc<4)
			cout << "ERROR: Not enough arguments" << endl;
		else
			cout << "ERROR: Too many arguments" << endl;
		return 1;
	}
	ifstream file;
	file.open(argv[1]);
	if(!file.is_open()){
		cout << "ERROR: Bad file" << endl;
		return 1;
	}
	FileReader reader(&file);
	if(!reader.readFile()){
		cout << "ERROR: Bad File Reader" << endl;
		return 1;
	}
	
	Turing turing = Turing();
	
	if(DEBUG){
		reader.start();
		cout << "Printing Text:" << endl;
		while(!reader.atEnd()){
			cout << reader.current() << endl;
			reader.next();
		}
		cout << "Printed Text!\n" << endl;
	}
	
	reader.start();
	while(!reader.atEnd()){
		string str = reader.current();
		if(DEBUG)
			cout << "Start\t" << str << endl;
		if(str.find("state", 0)==0){
			if(DEBUG)
				cout << "\tState: Adding... ";
			turing.addState(str);
		} else if(str.find("transition", 0)==0){
			if(DEBUG)
				cout << "\tTransition: Adding... " << endl;
			turing.addTrans(str);
		}
		if(DEBUG)
			cout << "\t\tAdded!" << str << endl;
		reader.next();
	}
	
	return 0;
}

Turing::Turing(){
	this->states=vector<State>();
	this->cur=-1;
	this->start=-1;
}

bool Turing::addState(State s){
	if(this->hasState(s.getNum())){
		State s0 = *(this->getState(s.getNum()));
		return s0.equals(s);
	}
	if(s.isStart()){
		if(this->hasStart())
			return false;
		this->start=s.getNum();
		this->cur=this->start;
	}
	this->states.push_back(s);
	return true;
}

bool Turing::addState(string str){
	State s = State(str);
	return this->addState(s);
}

bool Turing::hasState(int num) const{
	for(unsigned int i=0; i<this->states.size(); i++){
		if(this->states[i].getNum()==num)
			return true;
	}
	return false;
}

State * Turing::getState(int num){
	for(unsigned int i=0; i<this->states.size(); i++){
		if(this->states[i].getNum()==num)
			return &(this->states[i]);
	}
	return NULL;
}

bool Turing::hasTrans(int num, char a) const{
	for(unsigned int i=0; i<this->states.size(); i++){
		if(this->states[i].getNum()==num)
			return this->states[i].hasTrans(a);
	}
	return false;
}

Transition * Turing::getTrans(int num, char a){
	for(unsigned int i=0; i<this->states.size(); i++){
		if(this->states[i].getNum()==num)
			return this->states[i].getTrans(a);
	}
	return NULL;
}

bool Turing::addTrans(Transition t){
	if(this->hasTrans(t.getQ(), t.getA())){
		Transition t0 = *(this->getTrans(t.getQ(), t.getA()));
		return t0.equals(t);
	}
	if(this->hasState(t.getQ())){
		this->getState(t.getQ())->addTrans(t);
		return true;
	}
	this->addState(State(t.getQ()));
	return true;
}

bool Turing::addTrans(string str){
	Transition t = Transition(str);
	return this->addTrans(t);
}



State::State(string str){
	size_t index=0;
	string s = "state\t";
	index+=s.size();
	if(DEBUG)
		cout << "[num=";
	this->num = stoi(str.substr(index, -1), &index, 10);
	index++;
	s = "start";
	if(DEBUG)
		cout << this->num << "],[" << s << "=";
	if(str.substr(index, s.size()).compare(s)==0)
		this->start=true;
	s = "accept";
	if(DEBUG)
		cout << this->start << "],[" << s << "=";
	else if(str.substr(index, s.size()).compare(s)==0)
		this->accept=true;
	s = "reject";
	if(DEBUG)
		cout << this->accept << "],[" << s << "=";
	else if(str.substr(index, s.size()).compare(s)==0)
		this->reject=true;
	if(DEBUG)
		cout << this->reject << "]" << endl;
}

State::State(int num, bool start, bool accept, bool reject){
	this->num=num;
	this->start=start;
	if((!accept ||  !reject) || !(accept && reject)){
		this->accept = accept;
		this->reject = reject;
	}
}

State::State(int num){
	this->num=num;
	this->start=this->accept=this->reject=false;
}

bool State::equals(State o) const {
	return o.getNum()==this->getNum() && o.isStart()==this->isStart() && o.isAccept()==this->isAccept() && o.isReject()==this->isReject();
}

bool State::hasTrans(char a) const {
	for(unsigned int i=0; i<this->trans.size(); i++){
		if(this->trans[i].getA()==a)
			return true;
	}
	return false;
}

Transition * State::getTrans(char a){
	for(unsigned int i=0; i<this->trans.size(); i++){
		if(this->trans[i].getA()==a)
			return &(this->trans[i]);
	}
	return NULL;
}

bool State::addTrans(Transition t){
	if(this->hasTrans(t.getA())){
		Transition t0 = *(this->getTrans(t.getA()));
		return t0.equals(t);
	}
	this->trans.push_back(t);
	return true;
}

bool State::addTrans(string str){
	Transition t = Transition(str);
	return this->addTrans(t);
}



Transition::Transition(string str){
	size_t index=0;
	string t = "transition\t";
	index+=t.size();
	if(DEBUG)
		cout << "[q=";
	this->q = stoi(str.substr(index, -1), &index, 10);
	index++;
	if(DEBUG)
		cout << this->q << "],[a=";
	this->a = str.substr(index, -1).at(index);
	index++;
	if(this->a=='_')
		this->a=' ';
	if(DEBUG)
		cout << this->a << "],[r=";
	this->r= stoi(str.substr(index, -1), &index, 10);
	index++;
	if(DEBUG)
		cout << this->r << "],[b=";
	this->b = str.substr(index, -1).at(index);
	index++;
	if(this->b=='_')
		this->b=' ';
	if(DEBUG)
		cout << this->b << "],[x=";
	this->x = str.substr(index, -1).at(index);
	index++;
	if(DEBUG)
		cout << this->x << "]" << endl;
}

bool Transition::equals(Transition o) const {
	return o.getQ()==this->getQ() && o.getA()==this->getA() && o.getR()==this->getR() && o.getB()==this->getB() && o.getX()==this->getX();
}
