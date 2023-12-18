#include <iostream>
#include <fstream>
#include <string>

using namespace std;
string userName[] = {"admin", "cashier"};
string password[] = {"1111", "0000"};
string usn, pwd;
string loggedInUser = "";

struct item
{
    string itemId;
    string itemName;
    double price;
    int quantity;
    string expiryDate;
};

struct Order
{
    string itemId;
    string itemName;
    int quantity;
    double totalCost;
};

// login Function
void login()
{
    string usn, pwd;
    cout << "Enter Username: ";
    cin >> usn;
    cout << "Enter Password: ";
    cin >> pwd;

    for (int i = 0; i < 2; i++)
    {
        if (userName[i] == usn && password[i] == pwd)
        {
            loggedInUser = usn;
            return;
        }
    }

    cout << "\nIncorrect username or password. Try again.\n"
         << endl;
    system("pause");
}

// Company Details
void companyDetails()
{
    cout << "Amber Bakery" << endl;
    cout << "Address :49 Featherstone Street, LONDON" << endl;
    cout << "Hours: Open- 9.00am  Close-11.00pm" << endl;
    cout << "Email: amberbakery@gmail.com" << endl;
    cout << "Contact number:  01224 211670 \n\n"
         << endl;
}

// View Items
int viewItems()
{
    int itemCount = 0;
    ifstream inFile("bakeryitems.txt");

    if (!inFile)
    {
        cout << "Error opening file." << endl;
    }

    string line;
    while (getline(inFile, line))
    {
        cout << line << endl;
        itemCount++; // getting item count
    }
    return itemCount;
    inFile.close();
}

// Add Items
item items[100];
void addItems()
{
    int y = viewItems();
    ofstream itemFile("bakeryitems.txt", ios::app); // opening bakery items text file for reading and appending
    int i;
    cout << "\n\n\t\tEnter how many items to add\n"
         << endl;
    cin >> i;
    int g = y + i;

    for (int x = 0; x < i; ++x)
    {
        for (y; y < g; y++)
        {

            cout << "Enter Item ID:  (ex: '008' . Do not use an existing Item ID)" << endl;
            cin >> items[x].itemId;
            itemFile << items[x].itemId << "\t\t";
            cout << "Enter Item Name: (Item name should start with a Capital letter)" << endl;
            cin >> items[x].itemName;
            itemFile << items[x].itemName << "\t\t";
            cout << "Enter Item Price: (ex: 54)" << endl;
            cin >> items[x].price;
            itemFile << items[x].price << "\t\t";
            cout << "Enter Available Quantity: (ex: 20)" << endl;
            cin >> items[x].quantity;
            itemFile << items[x].quantity << "\t\t";
            cout << "Enter Expiry Date: (xx/x/xxxx)" << endl;
            cin >> items[x].expiryDate;
            itemFile << items[x].expiryDate << endl;
            cout << "Successfully added " << x + 1 << " items\n"
                 << endl;
        }
    }
}

// Delete Items
void deleteItems()
{
    viewItems();
    cout << "\n\n";
    string itemIdToDelete;
    cout << "Enter the item ID to delete: ";
    cin >> itemIdToDelete;

    ifstream inFile("bakeryitems.txt");
    ofstream tempFile("temp.txt"); // creating a temp file to hold items

    if (!inFile || !tempFile)
    {
        cout << "Error opening file." << endl;
        return;
    }

    string header;
    getline(inFile, header);    // Read and store the header line
    tempFile << header << endl; // Write back the header line to temp file

    item currentItem;
    while (inFile >> currentItem.itemId >> currentItem.itemName >> currentItem.price >> currentItem.quantity >> currentItem.expiryDate)
    {
        if (currentItem.itemId != itemIdToDelete)
        {
            tempFile << currentItem.itemId << "\t\t" << currentItem.itemName << "\t\t" << currentItem.price << "\t\t"
                     << currentItem.quantity << "\t\t" << currentItem.expiryDate << endl;
        }
        else
        {
            cout << "Item deleted successfully." << endl;
        }
    }

    inFile.close();
    tempFile.close();

    remove("bakeryitems.txt");             // removing existing file
    rename("temp.txt", "bakeryitems.txt"); // renaming temp file as the original file
}

// Add to Cart
void addToCart()
{
    int itemCount = viewItems();
    cout << "\n\n";

    int numItemsToAdd;
    cout << "Enter the number of items to add to cart: ";
    cin >> numItemsToAdd;

    // Store the order details in the "orders.txt" file
    int orderCount = 0;

    ifstream nFile("sales.txt"); // Open the orders file for reading
    string lin;
    while (getline(nFile, lin))
    {
        orderCount++;
    }
    nFile.close();
    ofstream kFile("sales.txt", ios::app);
    kFile << "-----Order ID: " << orderCount << " -----" << endl;
    kFile.close();

    ifstream inFile("bakeryitems.txt");
    if (!inFile)
    {
        cout << "Error opening file." << endl;
        return;
    }

    string header;
    getline(inFile, header); // Read and store the header line

    item items[100]; // Create an array to store items from the file

    for (int i = 0; i < itemCount; ++i)
    {
        inFile >> items[i].itemId >> items[i].itemName >> items[i].price >> items[i].quantity >> items[i].expiryDate;
    }

    inFile.close();

    item cart[100]; // Create an array to store items in the cart
    int cartItemCount = 0;
    double totalCost = 0; // Variable to store the total cost of the items

    for (int i = 0; i < numItemsToAdd; ++i)
    {
        string itemIdToAdd;
        cout << "\nEnter the item ID to add to cart: ";
        cin.ignore();
        getline(cin, itemIdToAdd);

        int quantityToAdd;
        cout << "\nEnter the quantity to add to cart: ";
        cin >> quantityToAdd;

        bool itemFound = false;
        for (int j = 0; j < itemCount; ++j)
        {
            if (items[j].itemId == itemIdToAdd)
            {
                itemFound = true;
                if (quantityToAdd > items[j].quantity)
                {
                    cout << "Quantity not available for item " << items[j].itemName << "." << endl;
                }
                else
                {
                    cout << "\nItem added to cart: " << items[j].itemName << endl;
                    items[j].quantity -= quantityToAdd; // Decrease available quantity
                    cart[cartItemCount] = items[j];     // Add item to cart array
                    cartItemCount++;
                    double itemCost = items[j].price * quantityToAdd;
                    totalCost += itemCost;                              // Update total cost
                    cout << "Bill for this item: " << itemCost << endl; // Display item bill

                    ofstream orderFile("sales.txt", ios::app); // opening sales text file to append
                    if (orderFile)
                    {
                        Order order;
                        order.itemId = items[j].itemId;
                        order.itemName = items[j].itemName;
                        order.quantity = quantityToAdd;
                        order.totalCost = itemCost;

                        orderFile << "Item ID: " << order.itemId << "\tItem Name: " << order.itemName
                                  << "\t\t\tQuantity: " << order.quantity << "\tCost: " << order.totalCost << endl;

                        orderFile.close();
                    }
                    else
                    {
                        cout << "Error opening orders file for writing." << endl;
                    }

                    break;
                }
            }
        }

        if (!itemFound)
        {
            cout << "Item not found." << endl;
        }
    }

    // Rewrite the text file with updated quantities
    ofstream outFile("bakeryitems.txt");
    if (!outFile)
    {
        cout << "Error opening file." << endl;
        return;
    }

    outFile << header << endl; // Write the header line to the output file

    for (int i = 0; i < itemCount - 1; i++)
    {
        outFile << items[i].itemId << "\t\t" << items[i].itemName << "\t\t" << items[i].price << "\t\t" << items[i].quantity << "\t\t" << items[i].expiryDate << endl;
    }

    outFile.close();
    // Display the total bill
    cout << "\nTotal Bill: Rs." << totalCost << endl;
    // ...

    // Add bill amount to order file

    ofstream orderFile("sales.txt", ios::app);
    if (orderFile)
    {
        orderFile << "\n**** Total Amount : Rs." << totalCost << " ****\n\n"
                  << endl;
        orderFile.close();
    }
    else
    {
        cout << "Error opening orders file for writing." << endl;
    }

    // ...
}

// Edit Items
void editItems()
{
    viewItems();
    cout << "\n\n";
    string itemIdToEdit;
    cout << "Enter the item ID to edit: ";
    cin >> itemIdToEdit;

    ifstream inFile("bakeryitems.txt");
    ofstream tempFile("temp.txt"); // Creating a temp file to hold items

    if (!inFile || !tempFile)
    {
        cout << "Error opening file." << endl;
        return;
    }

    string header;
    getline(inFile, header);    // Read and store the header line
    tempFile << header << endl; // Write back the header line to temp file

    item currentItem;
    while (inFile >> currentItem.itemId >> currentItem.itemName >> currentItem.price >> currentItem.quantity >> currentItem.expiryDate)
    {
        if (currentItem.itemId == itemIdToEdit)
        {
            cout << "Enter new Item Name: ";
            cin >> currentItem.itemName;
            cout << "Enter new Item Price: ";
            cin >> currentItem.price;
            cout << "Enter new Available Quantity: ";
            cin >> currentItem.quantity;
            cout << "Enter new Expiry Date: ";
            cin >> currentItem.expiryDate;
        }
        tempFile << currentItem.itemId << "\t\t" << currentItem.itemName << "\t\t" << currentItem.price << "\t\t"
                 << currentItem.quantity << "\t\t" << currentItem.expiryDate << endl;
    }

    inFile.close();
    tempFile.close();

    remove("bakeryitems.txt");             // Removing existing file
    rename("temp.txt", "bakeryitems.txt"); // Renaming temp file as the original file
    cout << "Item edited successfully." << endl;
}

// view sales details
void salesDetails()
{
    ifstream salesFile("sales.txt");
    string line;
    while (getline(salesFile, line))
    {
        cout << line << endl;
    }
    salesFile.close();
}

// search items

void searchItemsByName()
{
    cout << "Enter a keyword to search for bakery items  (case sensitive, First letter is Capital): ";
    string searchStr;
    cin >> searchStr;

    ifstream inFile("bakeryitems.txt");
    if (!inFile)
    {
        cout << "Error opening file." << endl;
        return;
    }

    string header;
    getline(inFile, header); // Read and store the header line

    item currentItem;
    bool found = false;

    int i; // Loop variable of type int

    while (inFile >> currentItem.itemId >> currentItem.itemName >> currentItem.price >> currentItem.quantity >> currentItem.expiryDate)
    {
        bool match = true;

        // Check if the item name starts with the search string (case-sensitive)
        if (currentItem.itemName.length() >= searchStr.length())
        {
            for (i = 0; i < searchStr.length(); i++)
            {
                // Compare characters
                if (currentItem.itemName[i] != searchStr[i])
                {
                    match = false;
                    break;
                }
            }
        }
        else
        {
            match = false;
        }

        if (match)
        {
            cout << currentItem.itemId << "\t\t" << currentItem.itemName << "\t\t" << currentItem.price << "\t\t"
                 << currentItem.quantity << "\t\t" << currentItem.expiryDate << endl; // display the items with item name starting with searched keywords
            found = true;
        }
    }

    inFile.close();

    if (!found)
    {
        cout << "No items found with names starting with '" << searchStr << "'." << endl;
    }
}

// INT MAIN STARTS

int main()
{
    system("color 03");

    while (true)
    {

        int choice;
        cout << "+++++++++++++++++ Amber Bakery Automated Billing System +++++++++++++++++\n\n\n\n"
             << endl;
        cout << "(1) Login as Admin\n(2) Login as Cashier\n(3) View Bakery Details \n(4) Exit\nEnter selection:" << endl;
        cin >> choice;

        switch (choice)
        {

        case 1:

            login();

            if (loggedInUser == userName[0])
            {
                system("cls");
                // Admin menu
                int k = 0;
                while (k == 0)
                {
                    int adminSel;
                    cout << "\n\n\t\tLogged in as Admin\n\n"
                         << endl;
                    cout << "\t\tPress 1 to View Available Items\n\t\tPress 2 to Add Items\n\t\tPress 3 to Delete Items\n\t\tPress 4 to Edit Items\n\t\tPress 5 to Search Items \n\t\tPress 6 to Manage Sales Details" << endl;
                    cout << "\t\tPress 7 to Logout" << endl;
                    cin >> adminSel;

                    switch (adminSel)
                    {
                    case 1:
                        // Calling View Items Function
                        system("cls");
                        viewItems();
                        system("pause");
                        system("cls");
                        break;
                    case 2:
                        // Calling Add items Function
                        system("cls");
                        addItems();
                        system("pause");
                        system("cls");
                        break;

                    case 3:
                        // calling Delete items function
                        system("cls");
                        deleteItems();
                        system("pause");
                        system("cls");
                        break;

                    case 4:
                        // calling edit items function
                        system("cls");
                        editItems();
                        system("pause");
                        system("cls");
                        break;

                    case 5:
                        // calling search items function
                        system("cls");
                        searchItemsByName();
                        system("pause");
                        system("cls");
                        break;
                    case 6:
                        // calling sales details function
                        system("cls");
                        salesDetails();
                        system("pause");
                        system("cls");
                        break;

                    case 7:
                        // LogOut
                        cout << "logging out" << endl;
                        system("cls");
                        k = 1;
                        break;
                    default:
                        cout << "Invalid choice. Please select again." << endl;
                        break;
                    }
                }
            }
            break;

        case 2:
            login();
            if (loggedInUser == userName[1])
            {
                system("cls");
                // Cashier menu
                int l = 0;
                while (l == 0)
                {

                    int cashierSel;
                    cout << "\n\n\t\tLogged in as Cashier\n\n"
                         << endl;
                    cout << "\t\tPress 1 to View Available Items\n\t\tPress 2 to Add items to the Cart\n\t\tPress 3 to View Sales Details\n\t\tPress 4 to Search Items\n\t\tPress 5 to Logout" << endl;
                    cin >> cashierSel;
                    switch (cashierSel)
                    {
                    case 1:
                        // calling view items function
                        system("cls");
                        viewItems();
                        system("pause");
                        system("cls");
                        break;

                    case 2:
                        // calling add to cart function
                        system("cls");
                        addToCart();
                        system("pause");
                        system("cls");
                        break;

                    case 3:
                        // calling sales details function
                        system("cls");

                        salesDetails();
                        system("pause");
                        system("cls");
                        break;

                    case 4:
                        // calling search items function
                        system("cls");
                        searchItemsByName();
                        system("pause");
                        system("cls");
                        break;

                    case 5:
                        // LogOut
                        cout << "logging out" << endl;
                        system("cls");
                        l = 1;
                        break;

                    default:
                        cout << "Invalid choice. Please select again." << endl;
                        break;
                    }
                }
            }
            break;

        case 3:
            system("cls");
            companyDetails();
            system("pause");
            system("cls");
            break;

        case 4:
            cout << "Exiting the program." << endl;
            return 0;

        default:
            cout << "Invalid choice. Please select again." << endl;
            break;
        }
    }

    return 0;
}
