#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include "headers.h"

using namespace std;

class Architecture {
private:
	
	vector<vector<int> > memory;
	vector<int> dests;
	vector<bool> destsSet;
	int maxLength;
	int width;
	int timeStep;
	
public:
	int MUL_TIME = 2;
	int DIV_TIME = 3;
	int ADD_TIME = 1;
	int SUB_TIME = 1;
	int SHIFT_TIME = 1;
	
	Architecture() {
		init(256, 3);
	}
	
	// width = number of commands (probably 256)
	// maxLength = longest number of clk cycles a command can take
	void init(int width, int maxLength) {
		this->maxLength = maxLength;
		this->width = width;
		timeStep = 0;
		memory = vector<vector<int> >(maxLength + 1);
		for (int i = 0; i < maxLength + 1; i++) {
			memory[i] = vector<int>(width, 0);
		}
		dests = vector<int>(width, 0);
		destsSet = vector<bool>(width, false);
		
		memory[0][0] = 6;
		memory[0][1] = 1;
		
		// setup constant memory
		for (int i = 0; i < memory.size(); i++) {
			for (int j = 192; j <= 255; j++) {
				memory[i][j] = j - 192;
			}
		}
	}
	
	void runCommand(int source, int dest) {
		dests[dest] = memory[timeStep % memory.size()][source];
		destsSet[dest] = true;
	}
	
	void executeTimeStep() {
		// debug (100-102)
		for (int i = 100; i <= 102; i++) {
			if (destsSet[i]) {
				cout << timeStep << ": " << i << " => " << dests[i] << endl;
				destsSet[i] = false;
			}
		}
		
		// register file (0-63)
		// transfers new value or holds old value
		for (int i = 0; i < 64; i++) {
			if (destsSet[i]) {
				destsSet[i] = false;
				memory[(timeStep + 1) % memory.size()][i] = dests[i];
			} else {
				memory[(timeStep + 1) % memory.size()][i] = memory[timeStep % memory.size()][i];
			}
		}
		
		// multiplier (64-65)
		if (destsSet[64] || destsSet[65]) {
			destsSet[64] = false;
			destsSet[65] = false;
			memory[(timeStep + MUL_TIME) % memory.size()][64] = dests[64] * dests[65]; 
		}
		
		// divider (66-67)
		if (destsSet[66] || destsSet[67]) {
			destsSet[66] = false;
			destsSet[67] = false;
			if (dests[67] != 0) {
				memory[(timeStep + DIV_TIME) % memory.size()][66] = dests[66] / dests[67]; 
			} else {
				cout << "ERROR: Dividing by 0 at timestep " << timeStep << endl;
			}
		}
		
		// adder (68-69)
		if (destsSet[68] || destsSet[69]) {
			destsSet[68] = false;
			destsSet[69] = false;
			memory[(timeStep + ADD_TIME) % memory.size()][68] = dests[68] + dests[69]; 
		}
		
		// subtractor (70-71)
		if (destsSet[70] || destsSet[71]) {
			destsSet[70] = false;
			destsSet[71] = false;
			memory[(timeStep + SUB_TIME) % memory.size()][70] = dests[70] - dests[71]; 
		}
		
		// left shifter (72-73)
		if (destsSet[72] || destsSet[73]) {
			destsSet[72] = false;
			destsSet[73] = false;
			memory[(timeStep + SHIFT_TIME) % memory.size()][72] = dests[72] << dests[73]; 
		}
		
		// right shifter (74-75)
		if (destsSet[74] || destsSet[75]) {
			destsSet[74] = false;
			destsSet[75] = false;
			memory[(timeStep + SHIFT_TIME) % memory.size()][74] = dests[74] >> dests[75]; 
		}
		
		// delete old values
		for (int i = 0; i < 128; i++) {
			memory[(timeStep) % memory.size()][i] = 0;
		}
		
		timeStep++;
	}

};

#endif