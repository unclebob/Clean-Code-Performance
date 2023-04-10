#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <sys/time.h>

using namespace std;
using namespace std::chrono;

const unsigned long N = 999983L;

long ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec % 86400) * 1000 + (tv.tv_usec/1000);
}

long counter[16];

void clearCounters() {
	for (int i=0; i<16; i++)
		counter[i] = 0;
}

enum Type {a,b,c,d};

class B {
public:
	virtual void op1() = 0;
	virtual void op2() = 0;
	virtual void op3() = 0;
	virtual void op4() = 0;	
};

static B* bs[N];
static Type ts[N];


class D0 : public B {
	virtual void op1() {counter[0]++;}
	virtual void op2() {counter[4]++;}
	virtual void op3() {counter[8]++;}
	virtual void op4() {counter[12]++;}
};

class D1 : public B {
	virtual void op1() {counter[1]++;}
	virtual void op2() {counter[5]++;}
	virtual void op3() {counter[9]++;}
	virtual void op4() {counter[13]++;}
};

class D2 : public B {
	virtual void op1() {counter[2]++;}
	virtual void op2() {counter[6]++;}
	virtual void op3() {counter[10]++;}
	virtual void op4() {counter[14]++;}
};

class D3 : public B {
	virtual void op1() {counter[3]++;}
	virtual void op2() {counter[7]++;}
	virtual void op3() {counter[11]++;}
	virtual void op4() {counter[15]++;}
};

void inline switchOp1(Type t) {
	switch(t) {
		case a: counter[0]++;break;
		case b: counter[1]++;break;
		case c: counter[2]++;break;
		case d: counter[3]++;break;
	}
}

void inline switchOp2(Type t) {
	switch(t) {
		case a: counter[4]++;break;
		case b: counter[5]++;break;
		case c: counter[6]++;break;
		case d: counter[7]++;break;
	}
}

void inline switchOp3(Type t) {
	switch(t) {
		case a: counter[8]++;break;
		case b: counter[9]++;break;
		case c: counter[10]++;break;
		case d: counter[11]++;break;
	}
}

void inline switchOp4(Type t) {
	switch(t) {
		case a: counter[12]++;break;
		case b: counter[13]++;break;
		case c: counter[14]++;break;
		case d: counter[15]++;break;
	}
}

void do_s(Type t) {
	switchOp1(t);
	switchOp2(t);
	switchOp3(t);
	switchOp4(t);
}

void do_c(B* b) {
	b->op1();
	b->op2();
	b->op3();
	b->op4();
}

void loadBsArray() {
	for (int i=0; i<N; i++) {
		int r = rand() % 4;
		switch(r) {
			case 0: bs[i] = new D0();break;
			case 1: bs[i] = new D1();break;
			case 2: bs[i] = new D2();break;
			case 3: bs[i] = new D3();break;
		}
	}
}

void loadTsArray() {
	for (int i=0; i<N; i++) {
		int r = rand() % 4;
		switch(r) {
			case 0: ts[i] = a;break;
			case 1: ts[i] = b;break;
			case 2: ts[i] = c;break;
			case 3: ts[i] = d;break;
		}
	}
}

void s_lup(int n) 
{
    for (int i = 0; i < n; i++) 
    {
        do_s(ts[i % N]);
    }
}

void c_lup(int n) 
{
    for (int i = 0; i < n; i++) 
    {
        do_c(bs[i % N]);
    }
}

int main(int ac, char** av)
{
	srand(time(nullptr));

    const auto n = atol(av[1]);

	loadBsArray();
	loadTsArray();
   
   	clearCounters();
    
    auto start_s = ms();
    s_lup(n);
    auto end_s = ms();

    cout << "s-lup counts: " << endl;
    for (const auto &count : counter)
        cout << count << endl;

    clearCounters();

    auto start_c = ms();
    c_lup(n);
    auto end_c = ms();

    cout << "c-lup counts: " << endl;
    for (const auto& count : counter)
        cout << count << endl;

    auto s_time = (end_s - start_s);
    auto c_time = (end_c - start_c);
    auto diff = (c_time - s_time);
    auto s_ns = (s_time * 1000000.0) / n;
    auto c_ns = (c_time * 1000000.0) / n;
    auto diff_ns = (diff * 1000000.0) / n;

    printf("switch-ns: %g, class-ns: %g, diff-ns: %g, factor: %g\n", s_ns, c_ns, diff_ns, c_ns / s_ns);
}
