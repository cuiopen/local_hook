
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

int myputs(const char* x){
	printf("myputs:%s\n",x);
	return 0;
}

int mynewputs(const char* x){
	printf("mynewputs:%s\n",x);
	return 0;
}

int myprintf(const char *format, ...){

	va_list args;
	va_start (args, format);
	int result = (int)write(1,"myprintf:", 9);
	result += vprintf (format, args);
	va_end (args);
	return result;
}


#include "local_hook.h"
LOCAL_HOOK(puts, myputs)

#define WRITE_TEST_OUTPUT(x) "\n[Testcheck]You should watch:\n"x"\n"
#define WRITE_TEST(x) write(1,WRITE_TEST_OUTPUT(x), sizeof(WRITE_TEST_OUTPUT(x)))

static void test_func1(){
	typeof(&puts) y = &puts;

	(*y)("1234567890");
	puts("0987654321");
	WRITE_TEST("myputs:1234567890\nmyputs:0987654321\n");
}

static void test_func2(){
	LOCAL_HOOK_START(puts, &mynewputs);
	typeof(&puts) z = &puts;

	(*z)("1234567890");
	puts("0987654321");
	WRITE_TEST("mynewputs:1234567890\nmynewputs:0987654321\n");
}

LOCAL_HOOK_INIT(printf)
static void test_func3(){
	LOCAL_HOOK_START(printf, &myprintf);
	typeof(&puts) z = &puts;

	(*z)("1234567890");
	puts("0987654321");
	printf("this is a pure printf:but gcc will implement it as puts\n");
	printf("%s%d\n","this is a pure printf",1);
	WRITE_TEST("myprintf:mynewputs:1234567890\nmyprintf:mynewputs:0987654321\nmyprintf:mynewputs:this is a pure printf:but gcc will implement it as puts\nmyprintf:this is a pure printf1\n");
}
extern int main1();
int main(){

	test_func1();
	test_func2();
	test_func3();
    #ifdef LOCALITY_TEST
    main1();
    #endif // LOCALITY_TEST
   return 0;
}

