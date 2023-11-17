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
             << ", Price: $" << price << ", Quantity in Stock: " << quantityInStock << endl;
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
            : Product(name, price, quantity), brand(brand), model(model), powerConsumption(powerConsumption) {}

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
        : Product(name, price, quantity), author(author), genre(genre), ISBN(ISBN) {}

void displayDetails() const override {
Product::displayDetails();
cout << ", Author: " << author << ", Genre: " << genre << ", ISBN: " << ISBN << endl;
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
            : Product(name, price, quantity), size(size), color(color), material(material) {}

    void displayDetails() const override {
        Product::displayDetails();
        cout << ", Size: " << size << ", Color: " << color << ", Material: " << material << endl;
    }

private:
    string size;
    string color;
    string material;
};

class Order {
public:
    Order() : orderID(0), customer(""), orderStatus("Pending") {}

    Order(int id, const string& customer, const vector<Product>& productCatalog)
            : orderID(id), customer(customer), orderStatus("Pending"), productCatalog(&productCatalog) {}

    void addProduct(const string& productName) {
        auto productIt = find_if(productCatalog->begin(), productCatalog->end(),
                                 [productName](const Product& p) { return p.getName() == productName; });

        if (productIt != productCatalog -> end()) {
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

    void changeOrderStatus(const string& status) const {
        orderStatus = status;
    }

    void displayOrder() const {
        cout << "\n--------------------------------Order Details-------------------------------" << endl;
        cout << "Order ID: " << orderID << "\nCustomer: " << customer << "\nOrder Status: " << orderStatus
             << "\nTotal Cost: " << calculateTotalCost() << "$" << endl;

        cout << "------------------------------Products in cart------------------------------" << endl;
        for (const auto& product : productsInCart) {
            cout << "Name: " << product.getName() << " Price: " << product.getPrice() << "$" << endl;
        }

        if (orderStatus == "Pending"){
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
    }

private:
    int orderID;
    string customer;
    vector<Product> productsInCart;
    mutable string orderStatus;
    const vector<Product>* productCatalog;
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

class Processes {
public:
    static void start() {
        ProductCatalog catalog;
        Inventory inventory(catalog.getProducts());
        Order order;

        readProductConfig("product_config.txt", catalog);

        displayMenu();

        while (true) {
            int choice = 0;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    viewCatalog(catalog);
                    break;
                case 2:
                    viewProductDetails(catalog);
                    break;
                case 3:
                    createOrder(order, catalog.getProducts());
                    break;
                case 4:
                    addProductToOrder(order, catalog.getProducts());
                    break;
                case 5:
                    displayOrder(order);
                    break;
                case 6:
                    removeProductFromCatalog(catalog);
                    break;
                case 7:
                    notifyLowStock(inventory);
                    break;
                case 8:
                    generateRestockList(inventory);
                    break;
                case 9:
                    manageStock(inventory);
                    break;
                case 10:
                    cout << "Exiting the program." << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

private:
    static void displayMenu() {
        cout << "\n---------------------- Menu ----------------------" << endl;
        cout << "1. View Catalog" << endl;
        cout << "2. View Product details" << endl;
        cout << "3. Create Order" << endl;
        cout << "4. Add Product to Order" << endl;
        cout << "5. Display Order" << endl;
        cout << "6. Remove Product from Catalog" << endl;
        cout << "7. Notify Low Stock" << endl;
        cout << "8. Generate Restock List" << endl;
        cout << "9. Manage Stock" << endl;
        cout << "10. Exit" << endl;
        cout << "---------------------------------------------------" << endl;
    }

    static void viewCatalog(const ProductCatalog& catalog) {
        catalog.viewProducts();
    }

    static void viewProductDetails(ProductCatalog& catalog) {
        string productName;
        cout << "Which product are you interested in?" << endl;
        cin.ignore();
        getline(cin, productName);

        auto productIt = find_if(catalog.getProducts().begin(), catalog.getProducts().end(),
                                 [productName](const Product& p) { return p.getName() == productName; });

        if (productIt != catalog.getProducts().end()) {
            cout << "----------------------Product Details----------------------" << endl;
            productIt->displayDetails();
            cout << "-------------------------------------------------------------" << endl;
        } else {
            cout << "Product not found in catalog: " << productName << endl;
        }
    }

    static void removeProductFromCatalog(ProductCatalog& catalog) {
        string productName;
        cout << "Enter the name of the product to remove: ";
        cin.ignore();
        getline(cin, productName);
        catalog.removeProduct(productName);
        cout << "Product removed from the catalog." << endl;
    }

    static void notifyLowStock(const Inventory& inventory) {
        inventory.notifyLowStock();
    }

    static void generateRestockList(const Inventory& inventory) {
        auto restockList = inventory.generateRestockList();
        cout << "Restock List:" << endl;
        for (const auto& product : restockList) {
            product.displayDetails();
            cout << endl;
        }
    }

    static void manageStock(Inventory& inventory) {
        string productName;
        int quantity;
        cout << "Enter the name of the product to manage stock: ";
        cin.ignore();
        getline(cin, productName);
        cout << "Enter the quantity to add: ";
        cin >> quantity;
        inventory.manageStock(productName, quantity);
    }

    static void createOrder(Order& order, const vector<Product>& productCatalog) {
        string customerName;
        cout << "Enter customer name for the order: ";
        cin.ignore();
        getline(cin, customerName);

        order = Order(1, customerName, productCatalog);
        cout << "Order created for customer: " << customerName << endl;
    }

    static void addProductToOrder(Order& order, const vector<Product>& productCatalog) {
        string productName;
        cout << "Enter the name of the product to add to the order: ";
        cin.ignore();
        getline(cin, productName);
        order.addProduct(productName);
    }

    static void displayOrder(const Order& order) {
        order.displayOrder();
    }
};

int main() {
    Processes::start();

    return 0;
}