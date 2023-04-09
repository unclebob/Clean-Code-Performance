#include <iostream>
#include <cstdlib>
#include <sys/time.h>

using namespace std;

long getCurrentTimeInMilliseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec % 86400) * 1000 + (tv.tv_usec / 1000);
}

const int ARRAY_SIZE = 1000;
int switchValues[ARRAY_SIZE];
long zeroCount = 0;
long oneCount = 0;

void incrementSwitchValue(int value) {
    switch (value) {
        case 0:
            zeroCount++;
            break;
        case 1:
            oneCount++;
            break;
    }
}

void performSwitchIncrements(long numberOfIncrements) {
    for (int i = 0; i < numberOfIncrements; i++) {
        incrementSwitchValue(switchValues[i % ARRAY_SIZE]);
    }
}

class BaseClass {
public:
    virtual void increment() = 0;
};

BaseClass* classValues[ARRAY_SIZE];

class DerivedClassZero : public BaseClass {
public:
    virtual void increment() { zeroCount++; }
};

class DerivedClassOne : public BaseClass {
public:
    virtual void increment() { oneCount++; }
};

void incrementClassValue(BaseClass* value) {
    value->increment();
}

void performClassIncrements(long numberOfIncrements) {
    for (int i = 0; i < numberOfIncrements; i++) {
        incrementClassValue(classValues[i % ARRAY_SIZE]);
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));

    long numberOfIncrements = atol(argv[1]);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        switchValues[i] = rand() % 2;
        classValues[i] = rand() % 2 ? (BaseClass*) new DerivedClassOne() : (BaseClass*) new DerivedClassZero();
    }

    zeroCount = oneCount = 0;
    long startTime = getCurrentTimeInMilliseconds();
    performSwitchIncrements(numberOfIncrements);
    long endTimeSwitch = getCurrentTimeInMilliseconds();
    
    if (oneCount != zeroCount)
        cout << "Switch mismatch: " << zeroCount << " " << oneCount << endl;

    zeroCount = oneCount = 0;
    performClassIncrements(numberOfIncrements);
    
    if (oneCount != zeroCount)
        cout << "Class mismatch: " << zeroCount << " " << oneCount << endl;

    long endTimeClass = getCurrentTimeInMilliseconds();
    
    int switchTime = (endTimeSwitch - startTime);
    int classTime = (endTimeClass - endTimeSwitch);
    
    printf("Switch: %d, Class: %d\n", switchTime, classTime);

    double switchTimePerIncrement = (switchTime * 1000000.0) / numberOfIncrements;
    double classTimePerIncrement = (classTime * 1000000.0) / numberOfIncrements;
    
    printf("Switch time per increment: %g ns, Class time per increment: %g ns\n", switchTimePerIncrement, classTimePerIncrement);
}
