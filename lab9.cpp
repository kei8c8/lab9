#include <iostream>
#include <vector>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

class message{
	protected:
		string mess1; //the message needs to be accessed by derived class
	public:
		message(); //default constructor
		message(string); //constructor that takes in message
		virtual ~message();
		virtual void print();//prints message

};

class morseCodeMessage : public message{ //class derived from message
	Lab9_template.c
	lab9.cpp

	private:
		string mcm; //morse code message
	public:
		morseCodeMessage(); //default constructor
		morseCodeMessage(string); //parametric constructor if given string initially
		void translate(); //translates to morse code
		void print(); //prints the regular message and morse code message
		int MorseCodeToLights(void); //displayes mcm with the lights
};

class messageStack{
	private:
		vector <message*> stack; //defines a vector stack of type message
	public:
		messageStack(message*); //constructor takes one message object
		~messageStack(); //to deallocate memory
		void push(message*); //pushes messages onto stack
		message* pop(void); //returns message popped off stack
		void printStack(void); //prints all messages on stack
};

int main() {
	morseCodeMessage* mess = new morseCodeMessage;
	//calls default constructor & message default constructor

	messageStack stack1(mess); //calls messageStack parametric constructor
	cout << "\nNew Stack:\n";
	stack1.printStack(); //calls printStack method

	return 0;
}

message::message(void){
	cout << "\nPlease enter a message: "; //prompts user for message
	getline(cin, mess1, '\n'); //takes in message
}

message::message(string mess){
	mess1 = mess; //sets given string to string member of class
}

message::~message(void){
	//no memory to be freed
}

void message::print(void){
	cout << mess1 <<endl; //prints message to the string
	return;
}

messageStack::messageStack(message* mess){
	stack.push_back(mess); //pushes first message onto stack
}

messageStack::~messageStack(void){
	//no memory to be freed
}

void morseCodeMessage::translate(void){
	char c;
	//for(auto &c:mess1){ //loops through every character of the string from message
	for(unsigned int i = 0; i < mess1.length(); i++) {
		c = mess1.at(i);
		switch(c){
			case 'A':
			case 'a':
				mcm += ".-";
				break;
			case 'B':
			case 'b':
				mcm += "-...";
				break;
			case 'C':
			case 'c':
				mcm += "-.-.";
				break;
			case 'D':
			case 'd':
				mcm += "-..";
				break;
			case 'e':
			case 'E':
				mcm += ".";
				break;
			case 'F':
			case 'f':
				mcm += "..-.";
				break;
			case 'G':
			case 'g':
				mcm += "--.";
				break;
			case 'H':
			case 'h':
				mcm += "....";
				break;
			case 'I':
			case 'i':
				mcm += "..";
				break;
			case 'J':
			case 'j':
				mcm += ".---";
				break;
			case 'K':
			case 'k':
				mcm += "-.-";
				break;
			case 'L':
			case 'l':
				mcm += ".-..";
				break;
			case 'M':
			case 'm':
				mcm += "--";
				break;
			case 'N':
			case 'n':
				mcm += "-.";
				break;
			case 'O':
			case 'o':
				mcm += "---";
				break;
			case 'P':
			case 'p':
				mcm += ".--.";
				break;
			case 'Q':
			case 'q':
				mcm += "--.-";
				break;
			case 'R':
			case 'r':
				mcm += ".-.";
				break;
			case 'S':
			case 's':
				mcm += "...";
				break;
			case 'T':
			case 't':
				mcm += "-";
				break;
			case 'U':
			case 'u':
				mcm += "..-";
				break;
			case 'V':
			case 'v':
				mcm += "...-";
				break;
			case 'W':
			case 'w':
				mcm += ".--";
				break;
			case 'X':
			case 'x':
				mcm += "-..-";
				break;
			case 'Y':
			case 'y':
				mcm += "-.--";
				break;
			case 'Z':
			case 'z':
				mcm += "--..";
				break;
			case '.':
				mcm += ".-.-.-";
				break;
			default:
				mcm += " ";
				break;
		}
	}

}


int morseCodeMessage::MorseCodeToLights(void){
	int fd;		// for the file descriptor of the special file we need to open.
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR

	fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
	if(fd == -1){
		printf("\n error\n");
		return(-1);  // failed open
	}

	// We need to map Address 0x80840000 (beginning of a memory page)
	// An entire memory page is mapped. We need to point to the beginning of the page.
	BasePtr = (unsigned long*)mmap(NULL,getpagesize(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
	if(BasePtr == MAP_FAILED){
		printf("\n Unable to map memory space \n");
		return(-2);
	}  // failed mmap

	// To access other registers in the page, we need to offset the base pointer to reach the
	// corresponding addresses. Those can be found in the board's manual.
	PBDR = BasePtr + 1;		// Address of port B DR is 0x80840004
	PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014

	*PBDDR |= 0x80;			// configures port B7 as output (Green LED)
	*PBDDR &= 0xFFFFFFFE;	// configures port B0 as input (first push button). You could use: &= ~0x01
//*************************************************
/*
	//red '.':
	*PBDDR |= 0x20;
	*PBDR |= 0x20;
	sleep(1);
	*PBDR &= ~0x20;
	sleep(1);

	//yellow '-':
	*PBDDR |= 0x40; // makes B6 output
	*PBDR |= 0x40; //yellow
	sleep(1);
	*PBDR &= ~0x40;
	sleep(1);

	//all off after letter
    *PBDDR |= 0xE0; //makes B7, B6, & B5 all outputs
    *PBDR &= ~0xE0;
    sleep(2);

	//green end of word:
	*PBDDR |= 0x80;
    *PBDR |= 0x80;
	sleep(2);
    *PBDR &= ~0x80;

*/
//**********************************************************
	char c;
	//all off before
    *PBDDR |= 0xE0; //makes B7, B6, & B5 all outputs
    *PBDR &= ~0xE0;
    sleep(2);
	//loops through every character of the string from message
	for(unsigned int i = 0; i < mess1.length(); i++) {
		int j=0;
		c = mess1.at(i);
		switch(c){
			case 'A':
			case 'a':
				//mcm += ".-";
				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);
				break;
			case 'B':
			case 'b':
				//mcm += "-...";
				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				for(j=0;j<3;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case 'C':
			case 'c':
				//mcm += "-.-.";
				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);

					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case 'D':
			case 'd':
				//mcm += "-..";
				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				for(j=0;j<2;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case 'e':
			case 'E':
				//mcm += ".";
				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);
				break;
			case 'F':
			case 'f':
				//mcm += "..-.";
				for(j=0;j<2;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);
				break;
			case 'G':
			case 'g':
				//mcm += "--.";
				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);
				break;
			case 'H':
			case 'h':
				//mcm += "....";
				for(j=0;j<4;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case 'I':
			case 'i':
				//mcm += "..";
				for(j=0;j<2;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case 'J':
			case 'j':
				//mcm += ".---";
				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				for(j=0;j<3;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}
				break;
			case 'K':
			case 'k':
				//mcm += "-.-";
				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);
				break;
			case 'L':
			case 'l':
				//mcm += ".-..";
				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				for(j=0;j<2;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case 'M':
			case 'm':
				//mcm += "--";
				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}
				break;
			case 'N':
			case 'n':
				//mcm += "-.";
				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);
				break;
			case 'O':
			case 'o':
				//mcm += "---";
				for(j=0;j<3;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}
				break;
			case 'P':
			case 'p':
				//mcm += ".--.";
				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);
				break;
			case 'Q':
			case 'q':
				//mcm += "--.-";
				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);
				break;
			case 'R':
			case 'r':
				//mcm += ".-.";
				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);
				break;
			case 'S':
			case 's':
				//mcm += "...";
				for(j=0;j<3;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case 'T':
			case 't':
				//mcm += "-";
				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);
				break;
			case 'U':
			case 'u':
				//mcm += "..-";
				for(j=0;j<2;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);
				break;
			case 'V':
			case 'v':
				//mcm += "...-";
				for(j=0;j<3;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);
				break;
			case 'W':
			case 'w':
				//mcm += ".--";
				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}
				break;
			case 'X':
			case 'x':
				//mcm += "-..-";
				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				for(j=0;j<2;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}

				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);
				break;
			case 'Y':
			case 'y':
				//mcm += "-.--";
				//yellow '-':
				*PBDDR |= 0x40; // makes B6 output
				*PBDR |= 0x40; //yellow
				sleep(1);
				*PBDR &= ~0x40;
				sleep(1);

				//red '.':
				*PBDDR |= 0x20;
				*PBDR |= 0x20;
				sleep(1);
				*PBDR &= ~0x20;
				sleep(1);

				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}
				break;
			case 'Z':
			case 'z':
				//mcm += "--..";
				for(j=0;j<2;j++){
					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}

				for(j=0;j<2;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);
				}
				break;
			case '.':
				//mcm += ".-.-.-";
				for(j=0;j<3;j++){
					//red '.':
					*PBDDR |= 0x20;
					*PBDR |= 0x20;
					sleep(1);
					*PBDR &= ~0x20;
					sleep(1);

					//yellow '-':
					*PBDDR |= 0x40; // makes B6 output
					*PBDR |= 0x40; //yellow
					sleep(1);
					*PBDR &= ~0x40;
					sleep(1);
				}
				break;
			default:
				//mcm += " ";
				//green end of word:
				*PBDDR |= 0x80;
			    *PBDR |= 0x80;
				sleep(2);
			    *PBDR &= ~0x80;
				break;
		}

		//all off after letter
	    *PBDDR |= 0xE0; //makes B7, B6, & B5 all outputs
	    *PBDR &= ~0xE0;
	    sleep(2);
	}

	close(fd);
	return 0;
}

void morseCodeMessage::print(void){ //have to make redifinition
	cout <<  "Text: " << mess1 << endl; //prints text
	cout << "Morse Code: " << mcm << endl; //prints morse code
	MorseCodeToLights(); //displays lights on board
	return;
}

morseCodeMessage::morseCodeMessage(void){
	translate(); //calls translate when morseCodeMessage is initialized
}

morseCodeMessage::morseCodeMessage(string s) : message(s){  //sends string to message constructor
	translate(); //calls translate when morseCodeMessage is initialized with a given string
}

void messageStack::push(message* mess2){
	stack.push_back(mess2); //appends next message onto stack
	return;
}

message* messageStack::pop(void){
	//defines message to assign the popped message to
	message* mess3 = stack.back(); //assigns the last message to the string
	stack.pop_back(); //deletes the last message from the stack
	return mess3; //returns message
}

void messageStack::printStack(void){
	int len=0;
	len = stack.size(); //finds the size of the vector

	while(len>0){
		stack[len-1]->print(); //prints current element referenced by len
		--len; //decrements len to go to the next element
	}
}