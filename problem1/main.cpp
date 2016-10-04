#include <assert.h>
#include <iostream>
#include <limits>

int main()
{
	std::cout << std::endl 
		<< "Project Euler - Problem 1" << std::endl
		<< "-------------------------" << std::endl;

	const static int limit = 1000;
	const static int d1 = 3;
	const static int d2 = 5;

	assert(d1 < d2 && d2 <= limit); //precondition

	int r = 0;

	for(int i = d1; i != limit; ++i) {
		if(i % d1 == 0 || i % d2 == 0) {
			assert(std::numeric_limits<int>::max() - i >= r); //check for overflow
			r += i;
		}
	}

	std::cout << "Answer: " << r << std::endl << std::endl;

	return 0;
}
