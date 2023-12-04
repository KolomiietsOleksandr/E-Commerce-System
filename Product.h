#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

using namespace std;

class Product {
public:
    Product(const string& productType, const string& name, double price, int quantity);
    Product(const Product& other);
    Product(Product&& other) noexcept;

    string getName() const;
    double getPrice() const;
    string getType() const;
    int getQuantityInStock() const;
    void setQuantityInStock(int quantity);
    void displayMain() const;
    virtual void displayDetails() const;

protected:
    string productType;
    string name;
    double price;
    int quantityInStock;
};

#endif
