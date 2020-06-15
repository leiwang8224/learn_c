#include "Practice4.h"
#include <stdio.h>

const char *MY_NAME = "Lei";

void scope_demo(int count) {
	printf("count is %d", count);
	if (count > 10) {
		int count = 100; //bad bug
		printf("count in this scope is %d", count);
	}
	printf("count is at exit: %d", count);
	count = 3000;
	printf("count after assign: %d", count);
}

int main(int argc, char *argv[]) {
	// test out THE_AGE accessors
	printf("My name: %s, age: %d", MY_NAME, get_age());
	set_age(100);
	printf("My age is now: %d", get_age());

	//test out THE_SIZE extern
	printf("THE_SIZE is: %d", THE_SIZE);
	print_size();

	THE_SIZE = 9;

	printf("THE SIZE is now: %d", THE_SIZE);
	print_size();

	//test the ratio function static 
	printf("Ratio at first: %f", update_ratio(2.0));
	printf("Ratio again: %f", update_ratio(10.0));
	printf("Ratio once more: %f", update_ratio(300.0));

	// test the scope demo
	int count = 4;
	scope_demo(count);
	scope_demo(count * 20);
	printf("count after calling scope_demo: %d", count);
	return 0;
}
