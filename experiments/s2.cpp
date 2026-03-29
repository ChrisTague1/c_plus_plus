#include <iostream>
#include <memory>
#include <vector>

struct Thingy {
    std::string name;
    Thingy(std::string n) : name(std::move(n)) {
        std::cout << "Created " << name << std::endl;
    }
    ~Thingy() {
        std::cout << "Destroying " << name << std::endl;
    }
    void greet() const {
        std::cout << "Hello, I am " << name << std::endl;
    }
};

struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;

    Node(std::string n) : name(std::move(n)) {
        std::cout << "Created node " << name << std::endl;
    }
    ~Node() {
        std::cout << "Destroyed node " << name << std::endl;
    }
};

int main() {
    {
        std::unique_ptr<Thingy> a = std::make_unique<Thingy>("A");
        a->greet();
        std::cout << "a is " << (a ? "valid" : "null") << std::endl;

        auto b = std::move(a);
        std::cout << "a is " << (a ? "valid" : "null") << std::endl;
        b->greet();

        b.reset();

        std::cout << "b is reset" << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        auto deleter = [](Thingy* r) {
            std::cout << "Custom delete logic for " << r->name << std::endl;
            delete r;
        };

        std::unique_ptr<Thingy, decltype(deleter)> p(new Thingy("Fancy deletable"), deleter);

        p->greet();
    }

    std::cout << "\n\n" << std::endl;

    {
        auto a = std::make_shared<Thingy>("Shared");
        std::cout << "refcount: " << a.use_count() << std::endl;

        {
            auto b = a;
            std::cout << "refcount after copy: " << a.use_count() << std::endl;
        }

        std::cout << "refcount after scope drop: " << a.use_count() << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        auto n1 = std::make_shared<Node>("N1");
        auto n2 = std::make_shared<Node>("N2");

        n1->next = n2;
        n2->prev = n1;

        if (auto locked = n2->prev.lock()) {
            std::cout << "N2's previous is " << locked->name << std::endl;
        }

        // weak ptr to n1 doesn't count so this is 1, not 2
        std::cout << "n1 refcount: " << n1.use_count() << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    {
        std::weak_ptr<Thingy> weak;

        {
            auto shared = std::make_shared<Thingy>("Temporary");
            weak = shared;
            std::cout << "expired? " << weak.expired() << std::endl;
        }

        std::cout << "expired? " << weak.expired() << std::endl;
        auto locked = weak.lock();
        std::cout << "lock result: " << (locked ? "valid" : "null") << std::endl;
    }

    return 0;
}