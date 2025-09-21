#include <functional>
#include <iostream>
#include <vector>

struct Product {
	int code;
	std::string name;
	int price;

	Product(): code(-1), name(""), price(-1) {};

	Product(int a, std::string b, int c): code(a), name(b), price(c) {};

	bool isEmpty() const {
		return code == -1;
	}

	bool isDeleted() const {
		return code == -2;
	}
};

class HashTable {
	private:
	std::vector<Product> table;
	std::vector<Product> data;
	int capacity;
	int size;

	int hashFunction(int key) const {
		return key % capacity;
	}

	int linearProbing(int key, int attempt) const {
		return (hashFunction(key) + attempt) % capacity;
	}
	void reHash() {
		std::cout << "Rehashing...\n";
		
		std::vector<Product> oldTable = table;
		int oldCapacity = capacity;
		capacity *= 2;
		table.clear();
		table.resize(capacity);
		
		for (int i = 0; i < oldCapacity; ++i) {
			if (!oldTable[i].isEmpty() && !oldTable[i].isDeleted())
			insert(oldTable[i]);
		}
	}

	public:
	HashTable(int a = 10): capacity(a), size(0) {
		table.resize(capacity);
		default_init();
	}

	void default_init() {
		insert(Product(1001, "PIM", 555));
		insert(Product(2002, "PAM", 666));
		insert(Product(3003, "RAM", 777));
		insert(Product(4004, "FFF", 888));
		insert(Product(5005, "ASD", 999));
		insert(Product(6006, "JKL", 1111));
		insert(Product(7007, "RURURU", 2222));
	}

	void insert(const Product &product) {

		if ((double)(size + 1) / capacity > 0.75)
			reHash();

		int key = product.code;
		int attempt = 0;
		int index;

		while (attempt < capacity) {
			index = linearProbing(key, attempt);
			if (table[index].code == key) {
				std::cout << "Product with key " << key << " already exist\n";
				break;
			}
			else if (table[index].isEmpty() || table[index].isDeleted()){
				table[index] = product;
				data.push_back(product);
				++size;
				std::cout << "Product '" << product.name << "' added with index " << index << '\n';
				break;
			}
			++attempt;
		}

	}

	Product *search(int key) {
		int attempt = 0;
		int index;

		while (attempt < capacity && !table[index].isEmpty()) {
			index = linearProbing(key, attempt);

			if (table[index].isEmpty())
				return nullptr;

			if (table[index].code == key && !table[index].isDeleted()) {
				return &table[index];
			}

			++attempt;
		}

		return nullptr;
	}

	void remove(int key) {
		int attempt = 0;
		int index;

		while (attempt < capacity && !table[index].isEmpty()) {
			index = linearProbing(key, attempt);

			if (table[index].isEmpty())
				break;

			if (table[index].code == key && !table[index].isDeleted()) {
				table[index].code = -2;

				for (auto el = data.begin(); el != data.end(); ++el) {
					if (el->code == key) {
						data.erase(el);
						break;
					}
				}

				--size;
				std::cout << "Product with key " << key << " deleted from index " << index << '\n';
				break;
			}
			++attempt;
		}
	}
    void displayTable() const {
        std::cout << "Capacity: " << capacity << ", Size: " << size << "\n";

        for (size_t i = 0; i < capacity; ++i) {
		std::cout << "Index: " << i << " Code: " << table[i].code << " Name: " << table[i].name << " Price: " << table[i].price << "\n"; 
        }
    }

};

int main() {
	HashTable hashTable;

	hashTable.displayTable();
	std::cout << "\nInsert: code 5555 name GGGG price 4444\n";
	hashTable.insert(Product(5555, "GGGG", 4444));
	hashTable.displayTable();
	
	hashTable.insert(Product(5555, "GGGG", 4444));
	
	hashTable.displayTable();

	return 0;


}
