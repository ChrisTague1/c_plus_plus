#include <iostream>
#include <vector>
#include <type_traits>

using std::cout;
using std::endl;

struct FastTag {};
struct PerciseTag {};

class HardwareSensor {
   public:
    using data_type = int;
    using processing_policy = FastTag;
};

class LabInstrument {
   public:
    using data_type = double;
    using processing_policy = PerciseTag;
};

void calculate(int data, FastTag) {
    cout << "Fast math: " << data * 2 << " (Integer bit-shift speed)" << endl;
}

void calculate(double data, PerciseTag) {
    cout << "Percise math: " << data * 3.14159265 << " (Floating point)" << endl;
}

template<typename T>
void process_device(T device, typename T::data_type value) {
    calculate(value, typename T::processing_policy{});
}

int main() {
    HardwareSensor sonar;
    LabInstrument laser;

    process_device(sonar, 10);
    process_device(laser, 10.5);
    return 0;
}