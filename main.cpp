#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Product {
public:
    Product(const string& name, double price, int quantity)
            : name(name), price(price), quantityInStock(quantity) {}

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

    virtual double calculateTotalCost() const {
        return price * quantityInStock;
    }

    virtual void displayDetails() const {
        cout << "Name: " << name
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
    Electronics(const string& name, double price, int quantity,
                const string& brand, const string& model, const string& powerConsumption)
            : Product( name, price, quantity), brand(brand), model(model), powerConsumption(powerConsumption) {}

    void displayDetails() const override {
        Product::displayDetails();
        cout << ", Brand: " << brand << ", Model: " << model << ", Power Consumption: " << powerConsumption;
    }

private:
    string brand;
    string model;
    string powerConsumption;
};

class Books : public Product {
public:
    Books(const string& name, double price, int quantity,
          const string& author, const string& genre, const string& ISBN)
            : Product( name, price, quantity), author(author), genre(genre), ISBN(ISBN) {}

    void displayDetails() const override {
        Product::getName();
        cout << ", Author: " << author << ", Genre: " << genre << ", ISBN: " << ISBN;
    }

private:
    string author;
    string genre;
    string ISBN;
};

class Clothing : public Product {
public:
    Clothing(const string& name, double price, int quantity,
             const string& size, const string& color, const string& material)
            : Product( name, price, quantity), size(size), color(color), material(material) {}

    void displayDetails() const override {
        Product::displayDetails();
        cout << ", Size: " << size << ", Color: " << color << ", Material: " << material;
    }

private:
    string size;
    string color;
    string material;
};

class Order {
public:
    Order(int id, const string& customer) : orderID(id), customer(customer), orderStatus("Pending") {}

    void addProduct(const Product& product) {
        products.push_back(product);
    }

    double calculateTotalCost() const {
        double totalCost = 0.0;
        for (const auto& product : products) {
            totalCost += product.calculateTotalCost();
        }
        return totalCost;
    }

    void changeOrderStatus(const string& status) {
        orderStatus = status;
    }

    void displayDetails() const {
        cout << "Order ID: " << orderID << ", Customer: " << customer << ", Order Status: " << orderStatus
             << ", Total Cost: $" << calculateTotalCost() << endl;

        for (const auto& product : products) {
            product.displayDetails();
            cout << endl;
        }
    }

private:
    int orderID;
    string customer;
    vector<Product> products;
    string orderStatus;
};

class ProductCatalog {
public:
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    void updateProduct(const Product& updatedProduct) {
        for (auto& product : products) {
            if (product.getType() == updatedProduct.getType()) {
                product = updatedProduct;
                break;
            }
        }
    }

    void removeProduct(string productName) {
        products.erase(remove_if(products.begin(), products.end(),
                                 [productName](const Product& p) { return p.getName() == productName; }),
                       products.end());
    }

    void viewProducts() const {
        cout << "----------------------Catalog----------------------" << endl;
        for (const auto& product : products) {
            product.displayDetails();
            cout << endl;
        }
        cout << "---------------------------------------------------" << endl;
    }

private:
    vector<Product> products;
};

class Inventory {
public:
    void manageStock(string productType, int quantity) {
        for (auto& product : products) {
            if (product.getType() == productType) {
                product.setQuantityInStock(product.getQuantityInStock() + quantity);
                break;
            }
        }
    }

    void notifyLowStock() const {
        for (const auto& product : products) {
            cout << product.getName();
            cout << product.getQuantityInStock();
            if (product.getQuantityInStock() < lowStockThreshold) {
                cout << "Low stock for product: " << product.getName() << endl;
            }
        }
    }

    vector<Product> generateRestockList() const {
        vector<Product> restockList;
        for (const auto& product : products) {
            if (product.getQuantityInStock() < lowStockThreshold) {
                restockList.push_back(product);
            }
        }
        return restockList;
    }

private:
    vector<Product> products;
    int lowStockThreshold = 10;
};

void readProductConfig(const string& filename, ProductCatalog& catalog) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        istringstream iss(line);
        string productType, productName, brand, model, powerConsumption, author, genre, ISBN, size, color, material;
        double price;
        int quantity;

        getline(iss, productType, ',');
        getline(iss, productName, ',');
        iss >> price;
        iss.ignore();
        iss >> quantity;

        if (productType == "Electronics") {
            getline(iss, brand, ',');
            getline(iss, model, ',');
            getline(iss, powerConsumption);
            Electronics electronic(productName, price, quantity, brand, model, powerConsumption);
            catalog.addProduct(electronic);
        } else if (productType == "Books") {
            getline(iss, author, ',');
            getline(iss, genre, ',');
            getline(iss, ISBN);
            Books book(productName, price, quantity, author, genre, ISBN);
            catalog.addProduct(book);
        } else if (productType == "Clothing") {
            getline(iss, size, ',');
            getline(iss, color, ',');
            getline(iss, material);
            Clothing clothing(productName, price, quantity, size, color, material);
            catalog.addProduct(clothing);
        }
    }
}

int main() {
    ProductCatalog catalog;
    Inventory iventory;
    readProductConfig("product_config.txt", catalog);
    catalog.viewProducts();

    catalog.removeProduct("Laptop");
    catalog.viewProducts();

    iventory.notifyLowStock();

    return 0;
}