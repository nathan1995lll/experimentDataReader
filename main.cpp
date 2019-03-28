#include "function.h"
int main(){
	std::vector<std::string> position{ "Time","Roll_rate" };
	process("data.txt", "cleaned.txt", position);
	return 0;
}