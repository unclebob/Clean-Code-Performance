#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

#define N 999983

using namespace std;

long ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec % 86400) * 1000 + (tv.tv_usec/1000);
}

static int vs[N];

static long zero = 0;
static long one = 0;
static long two = 0;

void do_s(int x) {
  switch(x) {
    case 0: zero++; break;
    case 1: one++; break;
	case 2: two++; break;
  }
}


void s_lup(int n) {
  for (int i=0; i<n; i++) {
    do_s(vs[i%N]);
  }
}

class B {
  public:
    virtual void inc() = 0;
};

static B* bs[N];

class D0 : public B {
  public:
    virtual void inc() {zero++;}
};

class D1 : public B {
  public:
    virtual void inc() {one++;}
};

class D2 : public B {
  public:
    virtual void inc() {two++;}
};

void do_c(B* b) {
 b->inc(); 
}

void c_lup(int n) {
  for (int i=0; i<n; i++) {
    do_c(bs[i%N]);
  }
}

int main(int ac, char** av)
{
  srand(time(NULL));

  long n = atol(av[1]);

  for (int i=0; i<N; i++) {
    vs[i] = i%3;
	switch(i%3) {
		case 0: bs[i] = new D0(); break;
		case 1: bs[i] = new D1(); break;
		case 2: bs[i] = new D2(); break;
	}
  }
  
  zero=one=two=0;
  long start_s = ms();
  s_lup(n);
  long end_s = ms();
  
  cout<< "s-lup counts: " << zero << " " << one << " " << two << endl;
  
  zero=one=two=0;
  long start_c = ms();
  c_lup(n);
  long end_c = ms();
  
  cout<< "c-lup counts: " << zero << " " << one << " " << two << endl;
  
  int s_time = (end_s - start_s);
  int c_time = (end_c - start_c);
  int diff = (c_time - s_time);
  double s_ns = (s_time * 1000000.0) / n;
  double c_ns = (c_time * 1000000.0) / n;
  double diff_ns = (diff * 1000000.0) / n;
  
  printf("switch-ns: %g, class-ns: %g, diff-ns: %g factor: %g\n", s_ns, c_ns, diff_ns, c_ns/s_ns);
}

