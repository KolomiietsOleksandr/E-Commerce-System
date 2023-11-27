#include "Product.h"
#include <iostream>

Product::Product(const std::string& productType, const std::string& name, double price, int quantity)
        : productType(productType), name(name), price(price), quantityInStock(quantity) {}

Product::Product(const Product& other)
        : productType(other.productType), name(other.name), price(other.price), quantityInStock(other.quantityInStock) {}

Product::Product(Product&& other) noexcept
        : name(std::move(other.name)), price(other.price), quantityInStock(other.quantityInStock) {
    other.name = "";
    other.price = 0;
    other.quantityInStock = 0;
}

std::string Product::getName() const {
    return name;
}

double Product::getPrice() const {
    return price;
}

std::string Product::getType() const {
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
