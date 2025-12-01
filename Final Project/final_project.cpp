#include <iostream>
#include <vector>
#include <string>

using namespace std;

class PricingStrategy {
public:
    virtual ~PricingStrategy() {}
    virtual double getPrice(double basePrice) = 0;
};

class RegularPricing : public PricingStrategy {
public:
    double getPrice(double basePrice) override {
        return basePrice;
    }
};

class DiscountPricing : public PricingStrategy {
    double percent;
public:
    DiscountPricing(double p) {
        percent = p;
    }
    double getPrice(double basePrice) override {
        return basePrice * (1.0 - percent);
    }
};

class Item {
private:
    string name;
    double basePrice;
    PricingStrategy* strategy;
public:
    Item(string n, double p, PricingStrategy* s) {
        name = n;
        basePrice = p;
        strategy = s;
    }

    string getName() {
        return name;
    }

    double getPrice() {
        if (strategy == nullptr) {
            return basePrice;
        }
        return strategy->getPrice(basePrice);
    }
};

class Department {
private:
    string name;
    vector<Item*> items;
public:
    Department(string n) {
        name = n;
    }

    string getName() {
        return name;
    }

    Item* addItem(string itemName, double price, PricingStrategy* strategy) {
        Item* item = new Item(itemName, price, strategy);
        items.push_back(item);
        return item;
    }

    void showItems() {
        cout << "Items in " << name << " department" << endl;
        for (size_t i = 0; i < items.size(); i++) {
            cout << i << " " << items[i]->getName()
                 << " price " << items[i]->getPrice() << endl;
        }
        cout << endl;
    }

    Item* getItemByIndex(int index) {
        if (index < 0 || index >= (int)items.size()) {
            return nullptr;
        }
        return items[index];
    }
};

class ShoppingCart {
private:
    vector<Item*> items;
public:
    void add(Item* item) {
        if (item != nullptr) {
            items.push_back(item);
        }
    }

    void showCart() {
        cout << "Items in cart" << endl;
        for (size_t i = 0; i < items.size(); i++) {
            cout << items[i]->getName()
                 << " price " << items[i]->getPrice() << endl;
        }
    }

    double getTotal() {
        double total = 0.0;
        for (size_t i = 0; i < items.size(); i++) {
            total += items[i]->getPrice();
        }
        return total;
    }
};

class Customer {
private:
    string name;
    ShoppingCart cart;
public:
    Customer(string n) {
        name = n;
    }

    string getName() {
        return name;
    }

    ShoppingCart& getCart() {
        return cart;
    }

    void addToCart(Item* item) {
        cart.add(item);
    }
};

class DepartmentFactory {
public:
    Department* createDepartment(const string& name) {
        return new Department(name);
    }
};

class Store {
private:
    string name;
    vector<Department*> departments;
    vector<Customer*> customers;

    static Store* instance;

    Store(string n) {
        name = n;
    }
public:
    Store(const Store&) = delete;
    Store& operator=(const Store&) = delete;

    static void init(const string& n) {
        if (instance == nullptr) {
            instance = new Store(n);
        }
    }

    static Store* getInstance() {
        return instance;
    }

    void addDepartment(Department* d) {
        departments.push_back(d);
    }

    void addCustomer(Customer* c) {
        customers.push_back(c);
        cout << c->getName() << " entered store " << name << endl;
    }

    void checkout(Customer* c) {
        cout << "Checkout for " << c->getName() << endl;
        c->getCart().showCart();
        cout << "Total " << c->getCart().getTotal() << endl;
    }

    vector<Department*>& getDepartments() {
        return departments;
    }
};

Store* Store::instance = nullptr;

int main() {
    Store::init("Online Store");
    Store* store = Store::getInstance();

    DepartmentFactory factory;

    Department* books = factory.createDepartment("Books");
    Department* music = factory.createDepartment("Music");

    store->addDepartment(books);
    store->addDepartment(music);

    RegularPricing regular;
    DiscountPricing discount(0.10);

    Item* b1 = books->addItem("C plus plus Basics", 40.0, &regular);
    Item* b2 = books->addItem("Data Structures Book", 50.0, &regular);
    Item* m1 = music->addItem("Greatest Hits Album", 30.0, &discount);

    books->showItems();
    music->showItems();

    Customer* alice = new Customer("Alice");
    store->addCustomer(alice);

    alice->addToCart(b1);
    alice->addToCart(b2);
    alice->addToCart(m1);

    store->checkout(alice);

    return 0;
}
