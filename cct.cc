#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <sys/time.h>

using namespace std;
using namespace std::chrono;

const unsigned long N = 999983L;
static int vs[N];
static long counters[10];

long ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec % 86400) * 1000 + (tv.tv_usec/1000);
}


void do_s(int x) 
{
    switch (x) 
    {
    case 0: counters[0]++; break;
    case 3: counters[3]++; break;
    case 1: counters[1]++; break;
    case 9: counters[9]++; break;
    case 4: counters[4]++; break;
    case 5: counters[5]++; break;
    case 6: counters[6]++; break;
    case 2: counters[2]++; break;
    case 7: counters[7]++; break;
    case 8: counters[8]++; break;
    }
}

void s_lup(int n) 
{
    for (int i = 0; i < n; i++) 
    {
        do_s(vs[i % N]);
    }
}

class B 
{
public:
    virtual void inc() = 0;
};

static B* bs[N];

class D : public B 
{
     // int id = 0;

public:
    // D(int x) : id(x){}
    // virtual void inc() { counters[id]++; }
    virtual void inc() { counters[0]++; }
	
};

class E : public B 
{
     // int id = 0;

public:
    // E(int x) : id(x){}
    // virtual void inc() { counters[id]--; }
    virtual void inc() { counters[1]++; }
	
};



void do_c(B* b) 
{
    b->inc();
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

    for (auto i = 0; i < N; i++) 
    {
        auto rnd = rand() % 10;
        vs[i] = rnd;
        // bs[i] = (i%2) ? (B*)new D(rnd) : (B*)new E(rnd);
        bs[i] = (i%2) ? (B*)new D() : (B*)new E();
		
    }

   for (int i=0; i<10; i++) counters[i] = 0;
    
    auto start_s = ms();
    s_lup(n);
    auto end_s = ms();

    cout << "s-lup counts: " << endl;
    for (const auto &count : counters)
        cout << count << endl;

    for (int i=0; i<10; i++) counters[i] = 0;

    auto start_c = ms();
    c_lup(n);
    auto end_c = ms();

    cout << "c-lup counts: " << endl;
    for (const auto& count : counters)
        cout << count << endl;

    auto s_time = (end_s - start_s);
    auto c_time = (end_c - start_c);
    auto diff = (c_time - s_time);
    auto s_ns = (s_time * 1000000.0) / n;
    auto c_ns = (c_time * 1000000.0) / n;
    auto diff_ns = (diff * 1000000.0) / n;

    printf("switch-ns: %g, class-ns: %g, diff-ns: %g, factor: %g\n", s_ns, c_ns, diff_ns, c_ns / s_ns);
}