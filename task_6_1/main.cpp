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
		size = 0;
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
		insert(Product(100112, "PIM", 555));
		insert(Product(200232, "PAM", 666));
		insert(Product(300312, "RAM", 777));
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

		while (attempt < capacity) {
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

		while (attempt < capacity) {
			index = linearProbing(key, attempt);

			if (table[index].isEmpty() || table[index].isDeleted()) {
				std::cout << "not found\n";
				break;
			}

			if (table[index].code == key && !table[index].isDeleted()) {
				table[index].code = -2;

				for (auto el = data.begin(); el != data.end(); ++el) {
					if (el->code == key) {
						data.erase(el);
						break;
					}
				}

				--size;
				std::cout << "product with key: " << key << " deleted from index: " << index << '\n';
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
	hashTable.default_init();
	
	while (true) {
		int action;
		std::cout << "1 - insert\n2 - search\n3 - remove\n4 - display\n5 - exit\n\nAction: ";
		std::cin >> action;
		switch (action) {
			case 1: {
				int code, price;
				std::string name;
				std::cout << "code: ";
				std::cin >> code;
				std::cout << "name: ";
				std::cin >> name;
				std::cout << "price: ";
				std::cin >> price;
				std::cout << "insert: " << code << ' ' << name << ' ' << price << '\n';
				hashTable.insert(Product(code, name, price));
				break;
			}

			case 2: {
				int key;
				std::cout << "code: ";
				std::cin >> key;
				Product *found = hashTable.search(key);
				if (found == nullptr)
					std::cout << "not found\n";
				else
					std::cout << "elements name: " << found->name << " elements price: " << found->price << '\n';
				delete found;
				break;
			}

			case 3: {
				int key;
				std::cout << "code: ";
				std::cin >> key;
				hashTable.remove(key);
				break;
			}

			case 4: {
				hashTable.displayTable();
				break;
			}

			case 5: {
				return 0;
			}
		}
	}

}
