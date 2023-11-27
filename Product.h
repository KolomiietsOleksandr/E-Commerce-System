// Product.h

#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
public:
    Product(const std::string& productType, const std::string& name, double price, int quantity);
    Product(const Product& other);
    Product(Product&& other) noexcept;

    std::string getName() const;
    double getPrice() const;
    std::string getType() const;
    int getQuantityInStock() const;
    void setQuantityInStock(int quantity);
    void displayMain() const;
    virtual void displayDetails() const;

protected:
    std::string productType;
    std::string name;
    double price;
    int quantityInStock;
};

#endif // PRODUCT_H
