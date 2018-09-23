#include "quinn_p1.h"
#include "Filereader.h"
#define DEBUG true

using namespace std;

vector<string> parser(string str){
	vector<string> toReturn = vector<string>();
	string s = "";
	for(unsigned int i=0; i<str.size(); i++){
		char c = str.at(i);
		if(c=='\t'){
			if(s.size()>0){
				toReturn.push_back(s);
				s="";
			}
		} else {
			s.append(1, c);
		}
	}
	if(s.size()>0)
		toReturn.push_back(s);
	return toReturn;
}

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
	
	unsigned long max = stoul(argv[3], nullptr, 10);
	
	Turing turing = Turing(argv[2], max);
	
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
			if(!turing.addState(parser(str)))
				if(DEBUG)
					cout << "ERROR! State not added!" << endl;
		} else if(str.find("transition", 0)==0){
			if(DEBUG)
				cout << "\tTransition: Adding... " << endl;
			if(!turing.addTrans(parser(str)))
				if(DEBUG)
					cout << "ERROR! Transition not added!" << endl;
		}
		if(DEBUG)
			cout << "\t\tAdded!\t" << str << endl;
		reader.next();
	}
	
	turing.begin();
	
	return 0;
}

Turing::Turing(string str, unsigned long max){
	this->states=vector<State>();
	this->cur=-1;
	this->start=-1;
	this->str=str;
	this->index=0;
	this->counter=0;
	this->max=max;
}

//Entry into program
bool Turing::begin(){
	this->cur=this->start;
	cout << this->cur;
	if(!this->go('R')){
		cout << " quit" << endl;
		return false;
	}
	return true;
}

bool Turing::go(char x){
	if(this->counter==0)
		cout << this->cur;
	else
		cout << "->" << this->cur;
	if(this->counter++>this->max)
		return false;
	State * s = getState(this->cur);
	if(s->isAccept()){
		cout << " accept" << endl;
		return true;
	} else if(s->isReject()){
		cout << " reject" << endl;
		return true;
	}
	char c = this->str.at(this->index);
	if(DEBUG){
		cout << "\t(" << this->cur << "," << c << "),(" << this->index+1 << "/" << this->str.size()  << ")" << endl;
	}
	if(s->hasTrans(c)){
		Transition * t = s->getTrans(c);
		if(DEBUG){
			cout << "\t:";
			t->print();
		}
		this->cur=t->getR();
		this->str.replace(this->index, 1, 1, t->getB());
		if(t->getX()=='L'){
			if(this->index==0)
				return false;
			else
				this->index--;
		} else if(t->getX()=='R'){
			if(((unsigned int) this->index)==this->str.size()-1)
				return false;
			else
				this->index++;
		} else
			return false;
		return this->go(t->getX());
	} else if(DEBUG){
		if(x=='L'){
			if(this->index==0){
				s->print();
				return false;
			} else
				this->index--;
		} else if(x=='R'){
			if(((unsigned int) this->index)==this->str.size()-1){
				s->print();
				return false;
			} else
				this->index++;
		} else
			return false;
		return this->go(x);
		
	}
	return false;
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

bool Turing::addState(vector<string> str){
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
		return this->getState(t.getQ())->addTrans(t);
	}
	this->addState(State(t.getQ()));
	return this->addTrans(t);
}

bool Turing::addTrans(vector<string> str){
	Transition t = Transition(str);
	return this->addTrans(t);
}



State::State(vector<string> str){
	string s = "state";
	if(DEBUG)
		cout << "(str.size()==" << str.size() << "),[num=";
	this->num = stoi(str[1], nullptr, 10);
	if(DEBUG)
		cout << this->num << "]";
	
	this->start=this->accept=this->reject=false;
	for(unsigned int i=2; i<str.size(); i++){
		if(DEBUG)
			cout << ",(str[" << i << "]==" << str[i] << ")";
		s = "start";
		if(str[i].compare(s)==0){
			this->start=true;
			if(DEBUG)
				cout << ",[" << s << "]";
		}
		s = "accept";
		if(str[i].compare(s)==0){
			this->accept=true;
			if(DEBUG)
				cout << ",[" << s << "]";
		}
		s = "reject";
		if(str[i].compare(s)==0){
			this->reject=true;
			if(DEBUG)
				cout << ",[" << s << "]";
		}
	}
	if(DEBUG)
		cout << endl;
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

bool State::addTrans(vector<string> str){
	Transition t = Transition(str);
	return this->addTrans(t);
}

void State::print()const{
	cout << "[" << this->num;
	if(this->start)
		cout << ",START";
	if(this->accept)
		cout << ",ACCEPT";
	if(this->reject)
		cout << ",REJECT";
	cout << "{" << endl;
	for(unsigned int i=0; i<this->trans.size(); i++)
		trans[i].print();
	cout << "\t}]" << endl;
}



Transition::Transition(vector<string> str){
	string t = "transition\t";
	if(DEBUG)
		cout << "[q=";
	this->q = stoi(str[1], nullptr, 10);
	if(DEBUG)
		cout << this->q << "],[a=";
	this->a = str[2].at(0);
	if(this->a=='_')
		this->a=' ';
	if(DEBUG)
		cout << this->a << "],[r=";
	this->r= stoi(str[3], nullptr, 10);
	if(DEBUG)
		cout << this->r << "],[b=";
	this->b = str[4].at(0);
	if(this->b=='_')
		this->b=' ';
	if(DEBUG)
		cout << this->b << "],[x=";
	this->x = str[5].at(0);
	if(DEBUG)
		cout << this->x << "]" << endl;
}

bool Transition::equals(Transition o) const {
	return o.getQ()==this->getQ() && o.getA()==this->getA() && o.getR()==this->getR() && o.getB()==this->getB() && o.getX()==this->getX();
}

void Transition::print() const{
	cout << "[" << this->getQ() << "," << this->getA() << "," << this->getR() << "," << this->getB() << "," << this->getX() << "]" << endl;
}

