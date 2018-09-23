#include "Turing.h"
#include "Filereader.h"

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
	
	reader.start();
	while(!reader.atEnd()){
		string str = reader.current();
		if(str.find("state", 0)==0)
			turing.addState(str);
		else if(str.find("transition", 0)==0)
			turing.addTrans(str);
		cout << str << endl;
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
	this->num = stoi(str.substr(index, -1), &index, 10);
	index++;
	if(str.substr(index, -1).compare("start")==0)
		this->start=true;
	else if(str.substr(index, -1).compare("accept")==0)
		this->accept=true;
	else if(str.substr(index, -1).compare("reject")==0)
		this->reject=true;
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
	this->q = stoi(str.substr(index, -1), &index, 10);
	index++;
	this->a = str.substr(index, -1).at(index);
	index++;
	this->r= stoi(str.substr(index, -1), &index, 10);
	index++;
	this->b = str.substr(index, -1).at(index);
	index++;
	this->x = str.substr(index, -1).at(index);
	index++;
}

bool Transition::equals(Transition o) const {
	return o.getQ()==this->getQ() && o.getA()==this->getA() && o.getR()==this->getR() && o.getB()==this->getB() && o.getX()==this->getX();
}
