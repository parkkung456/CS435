#include <stdio.h>
#include <setjmp.h>

jmp_buf jump_buffer;
static int c;
void function1() {
	printf("Function 1\n");
	c += 1; //addon one count (count = number of round)
	if (c == 1) //assign buffer = 1 if it's the first round
		longjmp(jump_buffer, 1);
	else //else, assign buffer = 5
		longjmp(jump_buffer, 5);
}

void function2() {
	c += 1;
	printf("Function 2\n");
	longjmp(jump_buffer, 2);
}

void function3() {
	c += 1;
	printf("Function 3\n");
	longjmp(jump_buffer, 3);
}

void function4() {
	c += 1;
	printf("Function 4\n");
	longjmp(jump_buffer, 4);
}

void function5() {
	c += 1;
	printf("Function 5\n");
	if (c == 6) //assign buffer = 6 if it's the sixth round
		longjmp(jump_buffer, 6);
	else
		longjmp(jump_buffer, 5);
}

void function6() {
	c += 1;
	printf("Function 6\n");
	longjmp(jump_buffer, 7);
}

int function7() {
	c += 1;
	printf("Function 7\n");
	longjmp(jump_buffer, 8);
	return c; //return to main()
}

void function8() {
	c += 1;
	printf("Function 8\n");
	longjmp(jump_buffer, 8);
}

int main() {
	int jump_result;
	printf("main\n");
	if ((jump_result = setjmp(jump_buffer)) == 0) {
		function1();
	} 
	else if (jump_result == 1) {
		function2();
	} 
	else if (jump_result == 2) {
        	function3();
   	} 
	else if (jump_result == 3) {
        	function4();
    	} 
	else if (jump_result == 4) {
	        function1();
    	} 
	else if (jump_result == 5) {
        	function5();
    	} 
	else if (jump_result == 6) {
        	function6();
   	 } 
	else if (jump_result == 7) {
        	function7();
   	} 
	else if (c == 8) {
		printf("main\n");
        	function8();
    	} 
	printf("end\n");
	return 0;
}

