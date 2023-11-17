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
    Order(int id, const string& customer, const vector<Product>& productCatalog)
            : orderID(id), customer(customer), orderStatus("Pending"), productCatalog(productCatalog) {}

    void addProduct(const string& productName) {
        auto productIt = find_if(productCatalog.begin(), productCatalog.end(),
                                 [productName](const Product& p) { return p.getName() == productName; });

        if (productIt != productCatalog.end()) {
            productsInCart.push_back(*productIt);
            cout << "Added product to cart: " << productName << endl;
        } else {
            cout << "Product not found in catalog: " << productName << endl;
        }
    }

    double calculateTotalCost() const {
        double totalCost = 0.0;
        for (const auto& product : productsInCart) {
            totalCost += product.getPrice();
        }
        return totalCost;
    }

    void changeOrderStatus(const string& status) {
        orderStatus = status;
    }

    void displayOrder() {
        cout << "\n--------------------------------Order Details-------------------------------" << endl;
        cout << "Order ID: " << orderID << "\nCustomer: " << customer << "\nOrder Status: " << orderStatus
             << "\nTotal Cost: " << calculateTotalCost() << "$" << endl;

        cout << "------------------------------Products in cart------------------------------" << endl;
        for (const auto& product : productsInCart) {
            cout << "Name: " << product.getName() << " Price: " << product.getPrice() << "$" << endl;
        }

        cout << "---------------------------------Confirming---------------------------------" << endl;
        while (true){
            string answer;
            cout  << "Want to comfirm and pay? Write y/n: ";
            getline(cin, answer);

            if (answer == "y" || answer == "Yes" || answer == "yes") {
                changeOrderStatus("Paid");
                cout << "Order confirmed and paid." << endl;
                break;
            }
            if (answer == "n" || answer == "No" || answer == "no"){
                break;
            }

            else {
                cout << "The command does not exist" << endl;
            }
        }
    }

private:
    int orderID;
    string customer;
    vector<Product> productsInCart;
    string orderStatus;
    const vector<Product>& productCatalog;
};

class ProductCatalog {
public:
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    vector<Product>& getProducts() {
        return products;
    }

    void updateProduct(const Product& updatedProduct) {
        for (auto& product : products) {
            if (product.getName() == updatedProduct.getName()) {
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
    Inventory(vector<Product>& products) : products(products) {}

    void manageStock(string productName, int quantity) {
        for (auto& product : products) {
            if (product.getName() == productName) {
                product.setQuantityInStock(product.getQuantityInStock() + quantity);
                cout << "Added " << quantity << " to " << productName << endl;
                break;
            }
        }
    }

    void notifyLowStock() const {
        bool lowStockFound = false;

        for (const auto& product : products) {
            if (product.getQuantityInStock() < lowStockThreshold) {
                cout << "Low stock for product: " << product.getName() << endl;
                lowStockFound = true;
            }
        }

        if (!lowStockFound) {
            cout << "Nothing in low stock." << endl;
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
    vector<Product>& products;
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
    Inventory inventory(catalog.getProducts());
    readProductConfig("product_config.txt", catalog);
    catalog.viewProducts();

    catalog.removeProduct("Laptop");
    catalog.viewProducts();

    inventory.notifyLowStock();
    inventory.generateRestockList();
    inventory.manageStock("SmartTV", 10);
    inventory.notifyLowStock();

    Order order1(1, "John Doe", catalog.getProducts());
    order1.addProduct("TheCatcherintheRye");
    order1.addProduct("SmartTV");

    order1.displayOrder();


    return 0;
}