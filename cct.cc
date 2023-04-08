#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

long ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec % 86400) * 1000 + (tv.tv_usec/1000);
}

static int vs[1000];

static int zero  = 0;
static int one = 0;

void do_s(int x) {
  switch(x) {
    case 0: zero++; break;
    case 1: one++; break;
  }
}


void s_lup(int n) {
  for (int i=0; i<n; i++) {
    do_s(vs[i%1000]);
  }
}

class B {
  public:
    static int zero;
    static int one;  
    virtual void inc() = 0;
};

int B::zero = 0;
static B* bs[1000];

class D : public B {
  public:
    virtual void inc() {zero++;}
};

void do_c(B* b) {
 b->inc(); 
}

void c_lup(int n) {
  for (int i=0; i<n; i++) {
    do_c(bs[i%1000]);
  }
}

int main(int ac, char** av)
{
  long n = atol(av[1]);

  for (int i=0; i<1000; i++) {
    vs[i] = i%2;
    bs[i] = new D();
  }

	long start = ms();
  s_lup(n);
  long end_s = ms();
  c_lup(n);
  long end_c = ms();
  int s_time = (end_s - start);
  int c_time = (end_c - end_s);
  int diff = (c_time - s_time);
  double s_ns = (s_time * 1000000.0) / n;
  double c_ns = (c_time * 1000000.0) / n;
  double diff_ns = (diff * 1000000.0) / n;
  printf("switch: %d, class: %d, diff: %d;\n",s_time, c_time, diff);
  printf("switch-ns: %g, class-ns: %g, diff-ns: %g\n", s_ns, c_ns, diff_ns);
}

