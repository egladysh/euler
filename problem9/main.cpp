#include <assert.h>
#include <iostream>

int main()
{
	std::cout << std::endl 
		<< "Project Euler - Problem 9" << std::endl
		<< "-------------------------" << std::endl;

	const static int SUM = 1000;

	/*
	 * a + b + c = SUM
	 * c = SUM - a - b
	 *
	 * a^2 + b^2  = (SUM - a - b)^2
	 * ... obvious transformations
	 * 2*SUM*a - SUM^2 = 2b*(a-SUM)
	 * b = SUM*(2a - SUM)/2(a-SUM)  and must be an integer and a < SUM so a < SUM/2 for b to be positive
	 *
	 * we just find the first 'a' that gives a whole 'b'... O(N) complexity
	 */


	assert(SUM!=0);

	int a = 1;
	int b = 0;
	for(; a < SUM/2; ++a) {
		if((SUM*(2*a - SUM)) % (2*(a-SUM)) == 0) {  //must be a whole int
			b = SUM*(2*a-SUM)/(2*(a - SUM));
			break;
		}
	}

	if(b) {
		int c = SUM - a - b;
		assert( a*a + b*b == c*c );
		std::cout << "a=" << a << " b=" << b << " c=" << c << std::endl;
		std::cout << "Answer: " << a*b*c << std::endl << std::endl;
	}
	else {
		std::cout << "Answer: not found" << std::endl << std::endl;
	}

	return 0;
}
