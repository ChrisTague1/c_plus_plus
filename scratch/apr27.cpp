#include <iostream>
#include <string>
#include <vector>

struct MyData {
    std::string name;
    int age;

    MyData(const std::string& name, int age) : name(name), age(age) {
        std::cout << "MyData constructed: " << name << ", " << age << std::endl;
    }

    MyData(const MyData& other) : name(other.name), age(other.age) {
        std::cout << "MyData copied: " << name << ", " << age << std::endl;
    }

    MyData(MyData&& other): name(std::move(other.name)), age(other.age) {
        // comment this constructor out and you'll see that copy gets called when you do std::move
        // since it doesn't know what to do with an rvalue
        std::cout << "MyData moved: " << name << ", " << age << std::endl;
    }
};

class MyDataHolder {
    std::vector<MyData> data_points;

   public:
    void add_data(MyData data) {
        std::cout << "Going to push back..." << std::endl;
        data_points.push_back(std::move(data));
    }
};

int main() {
    MyDataHolder data_holder;

    // this section will construct, copy into the function, then move
    MyData data = {"chris", 24};
    data_holder.add_data(data);

    // this will optimize to construct directly in the function, then move
    // data_holder.add_data({"chris", 24});
}