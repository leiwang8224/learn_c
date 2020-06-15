#ifndef Practice4_h
#define Practice4_h

// make THE_SIZE in Practice4.c availabel to other .c files
extern int THE_SIZE;

// gets and sets an internal static var in Practice4.c
int get_age();
void set_age(int age);

//updates a static var that's inside update_ratio
double update_ratio(double ratio);
void print_size();

#endif
