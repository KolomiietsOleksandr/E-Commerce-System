#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Product {
public:
    Product(const string& productType, const string& name, double price, int quantity)
            : productType(productType), name(name), price(price), quantityInStock(quantity) {}

    string getName() const {
        return name;
    }

    double getPrice() const {
        return price;
    }

    string getType() const {
        return productType;
    }

    int getQuantityInStock() const {
        return quantityInStock;
    }

    void setQuantityInStock(int quantity) {
        quantityInStock = quantity;
    }

    void displayMain() const {
        cout << "Name: " << name << ", Price: $" << price << ", Quantity in Stock: " << quantityInStock << endl;
    }

    virtual void displayDetails() const {
        cout << "Name: " << name << ", Price: $" << price << ", Quantity in Stock: " << quantityInStock << endl;
    }

protected:
    string productType;
    string name;
    double price;
    int quantityInStock;
};

class Electronics : public Product {
public:
    Electronics(const string& productType , const string& name, double price, int quantity,
                const string& brand, const string& model, const string& powerConsumption)
            : Product(productType, name, price, quantity), brand(brand), model(model), powerConsumption(powerConsumption) {}

    void displayDetails() const {
        Product::displayDetails();
        cout << "Brand: " << brand << ", Model: " << model << ", Power Consumption: " << powerConsumption << endl;
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
            : Product(productType, name, price, quantity), author(author), genre(genre), ISBN(ISBN) {}

    void displayDetails() const override {
        Product::displayDetails();
        cout << "Author: " << author << ", Genre: " << genre << ", ISBN: " << ISBN << endl;
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
            : Product(productType, name, price, quantity), size(size), color(color), material(material) {}

    void displayDetails() const override {
        Product::displayDetails();
        cout << "Size: " << size << ", Color: " << color << ", Material: " << material << endl;
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
            : orderID(id), customer(customer), orderStatus("Pending"), productCatalog(&productCatalog) {}

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

    void filterProducts(const string& productType, const string& additionalDescription) const {
        cout << "----------------------Filtered Products----------------------" << endl;
        for (const auto& product : products) {
            if (product->getType() == productType) {
                product->displayMain();
                if (additionalDescription != "") {
                    cout << "Additional Description: " << additionalDescription << endl;
                }
                cout << endl;
            }
        }
        cout << "--------------------------------------------------------------" << endl;
    }

private:
    vector<Product*> products;
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

void readProductConfig(const string& filename, ProductCatalog& catalog) {
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
            cout << "----------------------Product Details----------------------" << endl;

            Clothing* clothing = dynamic_cast<Clothing*>(foundProduct);
            Electronics* electronics = dynamic_cast<Electronics*>(foundProduct);
            Books* books = dynamic_cast<Books*>(foundProduct);

            if (clothing) {
                clothing->displayDetails();
            }
            else if (electronics) {
                electronics->displayDetails();
            }
            else if (books) {
                books->displayDetails();
            }
            else {
                foundProduct->displayDetails();
            }

            cout << "-----------------------------------------------------------" << endl;
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
        string additionalDescription;

        cout << "Enter the product type: ";
        cin >> productType;

        cout << "Enter additional description (press Enter if not applicable): ";
        cin.ignore();
        getline(cin, additionalDescription);

        catalog.filterProducts(productType, additionalDescription);
    }
};

int main() {
    Processes::start();

    return 0;
}