#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


using namespace std;


class Product {
public:
    Product(string& productType, const string& name, double price, int quantity)
            : productType(productType), name(name), price(price), quantityInStock(quantity) {}


    string getType() const {
        return productType;
    }

    string getName() const {
        return name;
    }

    double getPrice() const {
        return price;
    }

    int getQuantityInStock() const {
        return quantityInStock;
    }

    void setQuantityInStock(int quantity) {
        quantityInStock = quantity;
    }

    // Calculate total cost
    virtual double calculateTotalCost() const {
        return price * quantityInStock;
    }

    virtual void displayDetails() const {
        std::cout << "Product ID: " << productType << ", Name: " << name
                  << ", Price: $" << price << ", Quantity in Stock: " << quantityInStock;
    }

protected:
    string productType;
    string name;
    double price;
    int quantityInStock;
};


class Electronics : public Product {
public:
    Electronics(string& productType, const string& name, double price, int quantity,
                const string& brand, const string& model, const string& powerConsumption)
            : Product(productType, name, price, quantity), brand(brand), model(model), powerConsumption(powerConsumption) {}

    void displayDetails() const override {
        Product::displayDetails();
        std::cout << ", Brand: " << brand << ", Model: " << model << ", Power Consumption: " << powerConsumption;
    }

private:
    string brand;
    string model;
    string powerConsumption;
};


class Books : public Product {
public:
    Books(string& productType, const string& name, double price, int quantity,
          const string& author, const string& genre, const string& ISBN)
            : Product(productType, name, price, quantity), author(author), genre(genre), ISBN(ISBN) {}

    void displayDetails() const override {
        Product::displayDetails();
        std::cout << ", Author: " << author << ", Genre: " << genre << ", ISBN: " << ISBN;
    }

private:
    string author;
    string genre;
    string ISBN;
};


class Clothing : public Product {
public:
    Clothing(string& productType, const string& name, double price, int quantity,
             const string& size, const string& color, const string& material)
            : Product(productType, name, price, quantity), size(size), color(color), material(material) {}

    void displayDetails() const override {
        Product::displayDetails();
        std::cout << ", Size: " << size << ", Color: " << color << ", Material: " << material;
    }

private:
    string size;
    string color;
    string material;
};