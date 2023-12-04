#include "Product.h"
#include <iostream>

using namespace std;

Product::Product(const string& productType, const string& name, double price, int quantity)
        : productType(move(productType)), name(move(name)), price(move(price)), quantityInStock(move(quantity)) {}

Product::Product(const Product& other)
        : productType(other.productType), name(other.name), price(other.price), quantityInStock(other.quantityInStock) {}

Product::Product(Product&& other) noexcept
        : name(move(other.name)), price(move(other.price)), quantityInStock(move(other.quantityInStock)) {
    other.name = "";
    other.price = 0;
    other.quantityInStock = 0;
}

string Product::getName() const {
    return name;
}

double Product::getPrice() const {
    return price;
}

string Product::getType() const {
    return productType;
}

int Product::getQuantityInStock() const {
    return quantityInStock;
}

void Product::setQuantityInStock(int quantity) {
    quantityInStock = quantity;
}

void Product::displayMain() const {
    std::cout << "Name: " << name << ", Price: $" << price << ", Quantity in Stock: " << quantityInStock << std::endl;
}

void Product::displayDetails() const {
    std::cout << "Name: " << name << ", Price: $" << price << ", Quantity in Stock: " << quantityInStock << std::endl;
}
