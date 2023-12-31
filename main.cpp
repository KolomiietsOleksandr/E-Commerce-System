#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Product.h"

using namespace std;

class Electronics : public Product {
public:
    Electronics(const string& productType , const string& name, double price, int quantity,
                 string& brand, string& model, string& powerConsumption)
            : Product(productType, name, price, quantity), brand(move(brand)), model(move(model)), powerConsumption(move(powerConsumption)) {}

    void displayDetails() const {
        Product::displayDetails();
        cout << "Brand: " << brand << ", Model: " << model << ", Power Consumption: " << powerConsumption << endl;
    }

    string getBrand() const {
        return brand;
    }

    string getModel() const {
        return model;
    }

    string getPower() const {
        return powerConsumption;
    }

private:
    string brand;
    string model;
    string powerConsumption;
};

class Books : public Product {
public:
    Books(const string& productType , const string& name, double price, int quantity,
          const string& author, const string& genre, const string& ISBN)
            : Product(productType, name, price, quantity), author(move(author)), genre(move(genre)), ISBN(move(ISBN)) {}

    void displayDetails() const override {
        Product::displayDetails();
        cout << "Author: " << author << ", Genre: " << genre << ", ISBN: " << ISBN << endl;
    }

    string getAuthor() const {
        return author;
    }

    string getGenre() const {
        return genre;
    }

    string getISBN() const {
        return ISBN;
    }

private:
    string author;
    string genre;
    string ISBN;
};

class Clothing : public Product {
public:
    Clothing(const string& productType, const string& name, double price, int quantity,
             const string& size, const string& color, const string& material)
            : Product(productType, name, price, quantity), size(move(size)), color(move(color)), material(move(material)) {}

    void displayDetails() const override {
        Product::displayDetails();
        cout << "Size: " << size << ", Color: " << color << ", Material: " << material << endl;
    }

    string getSize() const {
        return size;
    }

    string getColor() const {
        return color;
    }

    string getMaterial() const {
        return material;
    }

private:
    string size;
    string color;
    string material;
};

class Order {
public:
    Order() : orderID(0), customer(""), orderStatus("Pending") {}

    Order(int id, const string& customer, const vector<Product*>& productCatalog)
            : orderID(move(id)), customer(move(customer)), orderStatus(move("Pending")), productCatalog(move(&productCatalog)) {}

    void addProduct(const string& productName) {
        Product* foundProduct = nullptr;
        for (auto product : *productCatalog) {
            if (product->getName() == productName) {
                foundProduct = product;
                break;
            }
        }

        if (foundProduct != nullptr) {
            productsInCart.push_back(foundProduct);
            cout << "Added product to cart: " << productName << endl;
        }
        else {
            cout << "Product not found in catalog: " << productName << endl;
        }
    }


    double calculateTotalCost() const {
        double totalCost = 0.0;
        for (const auto& product : productsInCart) {
            totalCost += product->getPrice();
        }
        return totalCost;
    }

    void changeOrderStatus(const string& status) const {
        orderStatus = status;
    }

    void displayOrder() const {
        cout << "--------------------------------Order Details-------------------------------" << endl;
        cout << "Order ID: " << orderID << "\nCustomer: " << customer << "\nOrder Status: " << orderStatus
             << "\nTotal Cost: " << calculateTotalCost() << "$" << endl;

        cout << "------------------------------Products in cart------------------------------" << endl;
        for (const auto& product : productsInCart) {
            cout << "Name: " << product->getName() << " Price: " << product->getPrice() << "$" << endl;
        }

        if (orderStatus == "Pending") {
            cout << "---------------------------------Confirming---------------------------------" << endl;
            while (true) {
                string answer;
                cout << "Want to confirm and pay? Write y/n: ";
                cin.ignore();
                getline(cin, answer);

                if (answer == "y" || answer == "Yes" || answer == "yes") {
                    changeOrderStatus("Paid");
                    cout << "Order confirmed and paid." << endl;
                    break;
                }
                if (answer == "n" || answer == "No" || answer == "no") {
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
    vector<Product*> productsInCart;
    mutable string orderStatus;
    const vector<Product*>* productCatalog;
};

class ProductCatalog {
public:
    void addProduct(Product* product) {
        products.push_back(product);
    }

    vector<Product*>& getProducts() {
        return products;
    }

    void updateProduct(Product* updatedProduct) {
        for (auto& product : products) {
            if (product->getName() == updatedProduct->getName()) {
                product = updatedProduct;
                break;
            }
        }
    }

    void removeProduct(string productName) {
        for (auto it = products.begin(); it != products.end(); ) {
            if ((*it)->getName() == productName) {
                it = products.erase(it);
            }
            else {
                ++it;
            }
        }
    }


    void viewProducts() const {
        cout << "----------------------Catalog----------------------" << endl;
        for (const auto& product : products) {
            product->displayMain();
            cout << endl;
        }
        cout << "---------------------------------------------------" << endl;
    }

    void filterProducts(const string& productType, const string& additionalParameter = "") const {
        cout << "----------------------Filtered Products----------------------" << endl;
        for (const auto& product : products) {
            if (product->getType() == productType) {
                if (additionalParameter == ""){
                    product->displayDetails();
                    cout << endl;
                }
                else{
                    if (checkAdditionalParameter(product, additionalParameter)) {
                        product->displayDetails();
                        cout << endl;
                    }
                }
            }
        }
        cout << "--------------------------------------------------------------" << endl;
    }

private:
    vector<Product*> products;

    bool checkAdditionalParameter(const Product* product, const string& additionalParameter) const {
        if (product->getType() == "Electronics") {
            const Electronics* electronicProduct = dynamic_cast<const Electronics*>(product);
            return (electronicProduct &&
                    (electronicProduct->getBrand() == additionalParameter ||
                     electronicProduct->getModel() == additionalParameter ||
                     electronicProduct->getPower() == additionalParameter));
        } else if (product->getType() == "Books") {
            const Books* bookProduct = dynamic_cast<const Books*>(product);
            return (bookProduct &&
                    (bookProduct->getAuthor() == additionalParameter ||
                     bookProduct->getGenre() == additionalParameter ||
                     bookProduct->getISBN() == additionalParameter));
        } else if (product->getType() == "Clothing") {
            const Clothing* clothingProduct = dynamic_cast<const Clothing*>(product);
            return (clothingProduct &&
                    (clothingProduct->getSize() == additionalParameter ||
                     clothingProduct->getColor() == additionalParameter ||
                     clothingProduct->getMaterial() == additionalParameter));
        }
        return true;
    }
};

class Inventory {
public:
    Inventory(vector<Product*>& products) : products(products) {}

    void manageStock(string productName, int quantity) {
        for (auto& product : products) {
            if (product->getName() == productName) {
                product->setQuantityInStock(product->getQuantityInStock() + quantity);
                cout << "Added " << quantity << " to " << productName << endl;
                break;

            }
        }
    }

    void notifyLowStock() const {
        bool lowStockFound = false;

        for (const auto& product : products) {
            if (product->getQuantityInStock() < lowStockThreshold) {
                cout << "Low stock for product: " << product->getName() << endl;
                lowStockFound = true;
            }
        }

        if (!lowStockFound) {
            cout << "Nothing in low stock." << endl;
        }
    }

    vector<Product*> generateRestockList() const {
        vector<Product*> restockList;
        for (const auto& product : products) {
            if (product->getQuantityInStock() < lowStockThreshold) {
                restockList.push_back(product);
            }
        }
        return restockList;
    }

private:
    vector<Product*>& products;
    int lowStockThreshold = 10;
};

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
            cout << "Enter your choice:" << endl;
            cout << "> ";
            cin >> choice;

            switch (choice) {
                case 1:
                    viewCatalog(catalog);
                    break;
                case 2:
                    viewProductDetails(catalog);
                    break;
                case 3:
                    showFilteredProducts(catalog);
                    break;
                case 4:
                    createOrder(order, catalog.getProducts());
                    break;
                case 5:
                    addProductToOrder(order, catalog.getProducts());
                    break;
                case 6:
                    displayOrder(order);
                    break;
                case 7:
                    removeProductFromCatalog(catalog);
                    break;
                case 8:
                    notifyLowStock(inventory);
                    break;
                case 9:
                    generateRestockList(inventory);
                    break;
                case 10:
                    manageStock(inventory);
                    break;
                case 11:
                    cout << "Exiting the program." << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    };

    static void readProductConfig(const string& filename, ProductCatalog& catalog) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            istringstream iss(line);
            string productType, productName, brand, model, powerConsumption, author, genre, ISBN, size, color, material;
            double price;
            int quantity;

            getline(iss, productType, ',');
            getline(iss, productName, ',');
            iss >> price;
            iss.ignore();
            iss >> quantity;
            iss.ignore();

            if (productType == "Electronics") {
                getline(iss, brand, ',');
                getline(iss, model, ',');
                getline(iss, powerConsumption);
                Electronics* electronic = new Electronics(productType, productName, price, quantity, brand, model, powerConsumption);
                catalog.addProduct(electronic);
            }
            else if (productType == "Books") {
                getline(iss, author, ',');
                getline(iss, genre, ',');
                getline(iss, ISBN);
                Books* book = new Books(productType, productName, price, quantity, author, genre, ISBN);
                catalog.addProduct(book);
            }
            else if (productType == "Clothing") {
                getline(iss, size, ',');
                getline(iss, color, ',');
                getline(iss, material);
                Clothing* clothing = new Clothing(productType, productName, price, quantity, size, color, material);
                catalog.addProduct(clothing);
            }
        }
    }

private:
    static void displayMenu() {
        cout << "\n---------------------- Menu ----------------------" << endl;
        cout << "1. View Catalog" << endl;
        cout << "2. View Product details" << endl;
        cout << "3. Show Filtered Products" << endl;
        cout << "4. Create Order" << endl;
        cout << "5. Add Product to Order" << endl;
        cout << "6. Display Order" << endl;
        cout << "7. Remove Product from Catalog" << endl;
        cout << "8. Notify Low Stock" << endl;
        cout << "9. Generate Restock List" << endl;
        cout << "10. Manage Stock" << endl;
        cout << "11. Exit" << endl;
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

        Product* foundProduct = nullptr;
        for (auto& product : catalog.getProducts()) {
            if (product->getName() == productName) {
                foundProduct = product;
                break;
            }
        }

        if (foundProduct != nullptr) {
            foundProduct->displayDetails();
        }
        else {
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
            product->displayDetails();
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

    static void createOrder(Order& order, const vector<Product*>& productCatalog) {
        string customerName;
        cout << "Enter customer name for the order: ";
        cin.ignore();
        getline(cin, customerName);

        order = Order(1, customerName, productCatalog);
        cout << "Order created for customer: " << customerName << endl;
    }

    static void addProductToOrder(Order& order, const vector<Product*>& productCatalog) {
        string productName;
        cout << "Enter the name of the product to add to the order: ";
        cin.ignore();
        getline(cin, productName);
        order.addProduct(productName);
    }

    static void displayOrder(const Order& order) {
        order.displayOrder();
    }

    static void showFilteredProducts(const ProductCatalog& catalog) {
        string productType;
        string additionalParameter;

        cout << "Enter the product type: ";
        cin >> productType;

        cout << "Enter the additional parameter (or press Enter to skip): ";
        cin.ignore();
        getline(cin, additionalParameter);

        catalog.filterProducts(productType, additionalParameter);
    }
};

class Figure
{
    string name;
public:
    Figure(const string name) : name(name) {}

    Figure(Figure& other) : name(other.name) {}

    Figure(Figure&& other) noexcept: name(other.name) {
        name = "";
    }

    void display_name()
    {
        cout << name << endl;
    }

    virtual void set_radius(const double r){}

    virtual ~Figure()
    {

    }
};

class Circle : public Figure
{
    double radius;
public:
    Circle(const string name, const double radius) : Figure(name), radius(radius) {}

    ~Circle() override {}

    void set_radius(const double r)
    {
        radius = r;
        cout << r << endl;
    }
};



int main() {
    Figure* f = new Circle("f", 4);
    f->set_radius(5);

    Figure* c = new Circle("f", 5);
    Circle* b = dynamic_cast<Circle*>(f);
    b->set_radius(10);

//    Processes::start();

    return 0;
}