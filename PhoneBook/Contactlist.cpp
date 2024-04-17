#include <iostream>
#include <fstream>
#include <string>
#include <limits>

class Phonebook {
    struct Node {
        std::string name;
        std::string phone_no;
        Node* left;
        Node* right;

        Node(const std::string& name, const std::string& phone_no)
            : name(name), phone_no(phone_no), left(NULL), right(NULL) {}
    };

    Node* head;

public:
    Phonebook() : head(NULL) {
        loadFromFile();
    }

    ~Phonebook() {
        clear();
    }

    void insert(const std::string& name, const std::string& phone_no) {
        Node** current = &head;
        Node* prev = NULL;

        while (*current) {
            prev = *current;
            if ((*current)->name == name) {
                std::cout << "Name is already present!" << std::endl;
                return;
            } else if ((*current)->name > name) {
                current = &(*current)->left;
            } else {
                current = &(*current)->right;
            }
        }

        *current = new Node(name, phone_no);
        saveToFile();
    }

    void remove(const std::string& name) {
        if (head == NULL) {
            std::cout << "Phonebook is empty!" << std::endl;
            return;
        }

        bool found = false;
        Node* parent = NULL;
        Node* current = head;

        while (current != NULL) {
            if (current->name == name) {
                found = true;
                break;
            } else if (current->name > name) {
                parent = current;
                current = current->left;
            } else {
                parent = current;
                current = current->right;
            }
        }

        if (!found) {
            std::cout << "Contact not found!" << std::endl;
            return;
        }

        if (current->left == NULL && current->right == NULL) {
            // Case 1: Node has no children
            if (parent == NULL) {
                // Current node is the root
                delete current;
                head = NULL;
            } else if (parent->left == current) {
                parent->left = NULL;
                delete current;
            } else {
                parent->right = NULL;
                delete current;
            }
        } else if (current->left != NULL && current->right != NULL) {
            // Case 3: Node has two children
            Node* successor = current->right;
            Node* successorParent = current;

            while (successor->left != NULL) {
                successorParent = successor;
                successor = successor->left;
            }

            current->name = successor->name;
            current->phone_no = successor->phone_no;

            if (successorParent == current) {
                // Right child of current has no left child
                successorParent->right = successor->right;
            } else {
                successorParent->left = successor->right;
            }

            delete successor;
        } else {
            // Case 2: Node has one child
            Node* child = (current->left != NULL) ? current->left : current->right;

            if (parent == NULL) {
                // Current node is the root
                head = child;
                delete current;
            } else if (parent->left == current) {
                parent->left = child;
                delete current;
            } else {
                parent->right = child;
                delete current;
            }
        }

        saveToFile();
        std::cout << "Contact deleted successfully!" << std::endl;
    }

    void search(const std::string& query) {
        searchByName(head, query);
        searchByPhone(head, query);
    }

    void showAscending() {
        showAscending(head);
    }

    void showDescending() {
        showDescending(head);
    }

    void modify(const std::string& name, const std::string& newPhone) {
        Node* contact = findContact(head, name);
        if (contact) {
            contact->phone_no = newPhone;
            saveToFile();
            std::cout << "Contact modified successfully!" << std::endl;
        } else {
            std::cout << "Contact not found!" << std::endl;
        }
    }

    void clear() {
        clearTree(head);
        head = NULL;
    }

private:
    Node* createNode(const std::string& name, const std::string& phone_no) {
        return new Node(name, phone_no);
    }

    Node* findContact(Node* root, const std::string& name) {
        if (root == NULL || root->name == name) {
            return root;
        }

        if (root->name > name) {
            return findContact(root->left, name);
        } else {
            return findContact(root->right, name);
        }
    }

    void searchByName(Node* root, const std::string& name) {
        if (root) {
            searchByName(root->left, name);
            if (root->name.find(name) != std::string::npos) {
                std::cout << "Name: " << root->name << ", Phone No.: " << root->phone_no << std::endl;
            }
            searchByName(root->right, name);
        }
    }

    void searchByPhone(Node* root, const std::string& phone) {
        if (root) {
            searchByPhone(root->left, phone);
            if (root->phone_no.find(phone) != std::string::npos) {
                std::cout << "Name: " << root->name << ", Phone No.: " << root->phone_no << std::endl;
            }
            searchByPhone(root->right, phone);
        }
    }

    void showAscending(Node* root) {
        if (root) {
            showAscending(root->left);
            std::cout << "Name: " << root->name << ", Phone No.: " << root->phone_no << std::endl;
            showAscending(root->right);
        }
    }

    void showDescending(Node* root) {
        if (root) {
            showDescending(root->right);
            std::cout << "Name: " << root->name << ", Phone No.: " << root->phone_no << std::endl;
            showDescending(root->left);
        }
    }

    void clearTree(Node* root) {
        if (root) {
            clearTree(root->left);
            clearTree(root->right);
            delete root;
        }
    }

    void loadFromFile() {
        std::ifstream file("phonebook.txt");
        if (file) {
            clear();
            std::string name, phone_no;
            while (std::getline(file, name) && std::getline(file, phone_no)) {
                insert(name, phone_no);
            }
            file.close();
            std::cout << "Phonebook data loaded successfully!" << std::endl;
        } else {
            std::cout << "No existing phonebook data found." << std::endl;
        }
    }

    void saveTreeToFile(std::ofstream& file, Node* root) {
        if (root) {
            saveTreeToFile(file, root->left);
            file << root->name << std::endl;
            file << root->phone_no << std::endl;
            saveTreeToFile(file, root->right);
        }
    }

    void saveToFile() {
        std::ofstream file("phonebook.txt", std::ios::trunc);
        if (file) {
            saveTreeToFile(file, head);
            file.close();
            std::cout << "Phonebook data saved successfully!" << std::endl;
        } else {
            std::cout << "Error: Failed to open the file." << std::endl;
        }
    }
};

int main() {
    Phonebook phonebook;

    while (true) {
        std::cout << "\n----- MENU -----\n";
        std::cout << "1. Add Contact\n";
        std::cout << "2. Delete Contact\n";
        std::cout << "3. Search Contact\n";
        std::cout << "4. Show in Ascending Order\n";
        std::cout << "5. Show in Descending Order\n";
        std::cout << "6. Modify Contact\n";
        std::cout << "7. Exit Program\n";
        int choice;
        std::cout << "Enter Choice: ";
        std::cin >> choice;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::cout << "Enter Name: ";
                std::string name;
                std::getline(std::cin, name);

                std::cout << "Enter Phone No.: ";
                std::string phone;
                std::getline(std::cin, phone);

                phonebook.insert(name, phone);
                break;
            }
            case 2: {
                std::cout << "Enter Exact Name to delete: ";
                std::string name;
                std::getline(std::cin, name);

                phonebook.remove(name);
                break;
            }
            case 3: {
                std::cout << "Enter Name or Phone No. to search: ";
                std::string query;
                std::getline(std::cin, query);

                phonebook.search(query);
                break;
            }
            case 4:
                std::cout << "\nPrinting in Ascending Order:\n";
                phonebook.showAscending();
                break;
            case 5:
                std::cout << "\nPrinting in Descending Order:\n";
                phonebook.showDescending();
                break;
            case 6: {
                std::cout << "Enter Exact Name to modify: ";
                std::string name;
                std::getline(std::cin, name);

                std::cout << "Enter New Phone No.: ";
                std::string newPhone;
                std::getline(std::cin, newPhone);

                phonebook.modify(name, newPhone);
                break;
            }
            case 7:
                return 0;
            default:
                std::cout << "Enter the correct number!" << std::endl;
                break;
        }
    }

    return 0;
}

