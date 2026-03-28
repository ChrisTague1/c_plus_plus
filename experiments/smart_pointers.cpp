#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct Resource {
    string name;
    Resource(string n) : name(move(n)) { cout << "  [created " << name << "]\n"; }
    ~Resource() { cout << "  [destroyed " << name << "]\n"; }
    void greet() const { cout << "  Hello from " << name << "\n"; }
};

// --- unique_ptr: sole ownership, zero overhead over raw pointer ---

void unique_ptr_basics() {
    cout << "=== unique_ptr basics ===\n";

    // prefer make_unique (exception-safe, single allocation)
    auto a = make_unique<Resource>("A");
    a->greet();

    // transfer ownership via move
    auto b = move(a);
    cout << "  a is " << (a ? "valid" : "null") << "\n";
    b->greet();

    // reset releases early
    b.reset();
    cout << "  b reset\n";
} // nothing to destroy — already reset

// --- unique_ptr with arrays ---

void unique_ptr_array() {
    cout << "\n=== unique_ptr array ===\n";

    auto arr = make_unique<int[]>(5);
    for (int i = 0; i < 5; i++) arr[i] = i * i;
    for (int i = 0; i < 5; i++) cout << "  arr[" << i << "] = " << arr[i] << "\n";
}

// --- unique_ptr with custom deleter ---

void unique_ptr_custom_deleter() {
    cout << "\n=== unique_ptr custom deleter ===\n";

    auto deleter = [](Resource* r) {
        cout << "  [custom deleter for " << r->name << "]\n";
        delete r;
    };

    unique_ptr<Resource, decltype(deleter)> p(new Resource("Custom"), deleter);
    p->greet();
}

// --- shared_ptr: reference-counted shared ownership ---

void shared_ptr_basics() {
    cout << "\n=== shared_ptr basics ===\n";

    auto a = make_shared<Resource>("Shared");
    cout << "  refcount: " << a.use_count() << "\n";

    {
        auto b = a; // copy increments refcount
        cout << "  refcount after copy: " << a.use_count() << "\n";
        b->greet();
    } // b goes out of scope, refcount decrements

    cout << "  refcount after scope: " << a.use_count() << "\n";
} // last owner destroyed here

// --- shared_ptr in containers ---

void shared_ptr_container() {
    cout << "\n=== shared_ptr in containers ===\n";

    vector<shared_ptr<Resource>> vec;
    auto r = make_shared<Resource>("Contained");

    vec.push_back(r);
    vec.push_back(r);
    cout << "  refcount: " << r.use_count() << "\n"; // 3 (r + 2 in vec)

    vec.clear();
    cout << "  refcount after clear: " << r.use_count() << "\n"; // 1
}

// --- weak_ptr: non-owning observer, breaks cycles ---

struct Node {
    string name;
    shared_ptr<Node> next;
    weak_ptr<Node> prev; // weak to avoid circular reference

    Node(string n) : name(move(n)) { cout << "  [created node " << name << "]\n"; }
    ~Node() { cout << "  [destroyed node " << name << "]\n"; }
};

void weak_ptr_basics() {
    cout << "\n=== weak_ptr (break cycles) ===\n";

    auto n1 = make_shared<Node>("N1");
    auto n2 = make_shared<Node>("N2");

    n1->next = n2;
    n2->prev = n1; // weak — won't prevent n1 from being destroyed

    // lock() promotes weak_ptr to shared_ptr (may return null)
    if (auto locked = n2->prev.lock()) {
        cout << "  N2's prev is " << locked->name << "\n";
    }

    cout << "  n1 refcount: " << n1.use_count() << "\n"; // 1 (weak doesn't count)
}

void weak_ptr_expiry() {
    cout << "\n=== weak_ptr expiry ===\n";

    weak_ptr<Resource> weak;

    {
        auto shared = make_shared<Resource>("Temporary");
        weak = shared;
        cout << "  expired? " << weak.expired() << "\n"; // 0
    }

    cout << "  expired? " << weak.expired() << "\n"; // 1
    auto locked = weak.lock();
    cout << "  lock result: " << (locked ? "valid" : "null") << "\n";
}

// --- passing smart pointers to functions ---

void take_ownership(unique_ptr<Resource> r) {
    cout << "  took ownership of " << r->name << "\n";
} // destroyed here

void observe(const Resource& r) {
    r.greet();
}

void share(shared_ptr<Resource> r) {
    cout << "  sharing " << r->name << ", refcount: " << r.use_count() << "\n";
}

void passing_conventions() {
    cout << "\n=== passing conventions ===\n";

    // pass unique_ptr by value to transfer ownership
    auto u = make_unique<Resource>("Owned");
    take_ownership(move(u));

    // pass by const ref when function just uses the object
    auto s = make_shared<Resource>("Observed");
    observe(*s);

    // pass shared_ptr by value only when function needs to share ownership
    share(s);
    cout << "  refcount back in caller: " << s.use_count() << "\n";
}

int main() {
    unique_ptr_basics();
    unique_ptr_array();
    unique_ptr_custom_deleter();
    shared_ptr_basics();
    shared_ptr_container();
    weak_ptr_basics();
    weak_ptr_expiry();
    passing_conventions();
}
