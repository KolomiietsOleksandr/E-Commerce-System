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
        std::cout << "Order ID: " << orderID << ", Customer: " << customer << ", Order Status: " << orderStatus
                  << ", Total Cost: $" << calculateTotalCost() << std::endl;

        for (const auto& product : products) {
            product.displayDetails();
            std::cout << std::endl;
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

    void removeProduct(string productType) {
        products.erase(remove_if(products.begin(), products.end(),
                                      [productType](const Product& p) { return p.getType() == productType; }),
                       products.end());
    }

    void viewProducts() const {
        for (const auto& product : products) {
            product.displayDetails();
            std::cout << std::endl;
        }
    }

private:
    vector<Product> products;
};

class Inventory {
public:
    // Manage stock levels (add and subtract quantities)
    void manageStock(string productType, int quantity) {
        for (auto& product : products) {
            if (product.getType() == productType) {
                product.setQuantityInStock(product.getQuantityInStock() + quantity);
                break;
            }
        }
    }

    // Notify when products are low in stock
    void notifyLowStock() const {
        for (const auto& product : products) {
            if (product.getQuantityInStock() < lowStockThreshold) {
                std::cout << "Low stock for product: " << product.getName() << std::endl;
            }
        }
    }

    // Generate a list of products that need restocking
    std::vector<Product> generateRestockList() const {
        std::vector<Product> restockList;
        for (const auto& product : products) {
            if (product.getQuantityInStock() < lowStockThreshold) {
                restockList.push_back(product);
            }
        }
        return restockList;
    }

private:
    std::vector<Product> products;
    int lowStockThreshold = 10;
};


// Function to read product information from a configuration file
void readProductConfig(const std::string& filename, ProductCatalog& catalog) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string productType, productName;
        double price;
        int quantity;

        iss >> productType >> productName >> price >> quantity >> productType;

        if (productType == "Electronics") {
            std::string brand, model, powerConsumption;
            iss >> brand >> model >> powerConsumption;
            Electronics electronic(productType, productName, price, quantity, brand, model, powerConsumption);
            catalog.addProduct(electronic);
        } else if (productType == "Books") {
            std::string author, genre, ISBN;
            iss >> author >> genre >> ISBN;
            Books book(productType, productName, price, quantity, author, genre, ISBN);
            catalog.addProduct(book);
        } else if (productType == "Clothing") {
            std::string size, color, material;
            iss >> size >> color >> material;
            Clothing clothing(productType, productName, price, quantity, size, color, material);
            catalog.addProduct(clothing);
        }
    }
}

// Example usage
int main() {
    // Create an instance of the product catalog
    ProductCatalog catalog;

    // Read product information from a configuration file
    readProductConfig("product_config.txt", catalog);

    // Display products in the catalog
    catalog.viewProducts();

    return 0;
}