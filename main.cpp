#include "headers.h"

int makeConst(int val) {
	if (val > 63 || val < 0) {
		cout << "Error making constant: " << val << endl;
	}
	return 192 + val;
}

int main() {
	
	Architecture arch;
	
	arch.runCommand(makeConst(63), 64);
	arch.runCommand(makeConst(10), 65);
	arch.executeTimeStep();
	arch.executeTimeStep();
	arch.runCommand(64, 100);
	arch.executeTimeStep();
}