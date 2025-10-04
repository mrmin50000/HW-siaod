#include <iostream>

class AVLTree {
	private:

	struct Node {
		double value;
		Node *left;
		Node *right;
		int height;

		Node(double val) : value(val), left(nullptr), right(nullptr), height(1) {}
	};
	
	Node *root;
	int nodeCount;
	double sumNodes;

	int getHeight(Node *node) {
		return node ? node->height : 0;
	}

	int getBalance(Node *node) {
		return node ? getHeight(node->left) - getHeight(node->right) : 0;
	}

	void updateHeight(Node *node) {
		if (node)
			node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
	}

	Node *rotateRight(Node *unbalancedNode) {
		Node *newRoot = unbalancedNode->left;
		Node *orphansNode = newRoot->right;

		newRoot->right = unbalancedNode;
		unbalancedNode->left = orphansNode;
		
		updateHeight(unbalancedNode);
		updateHeight(newRoot);

		return newRoot;
	}

	Node *rotateLeft(Node *unbalancedNode) {
		Node *newRoot = unbalancedNode->right;
		Node *orphansNode = newRoot->left;

		newRoot->left = unbalancedNode;
		unbalancedNode->right = orphansNode;
		
		updateHeight(unbalancedNode);
		updateHeight(newRoot);

		return newRoot;
	}
	
	Node *insertRecursive(Node *node, double value) {
		if (!node) {
			++nodeCount;
			sumNodes += value;
			return new Node(value);
		}

		if (value < node->value) {
			node->left = insertRecursive(node->left, value);
		}
		else if (value > node->value) {
			node->right = insertRecursive(node->right, value);
		}
		else {
			return node;
		}

		updateHeight(node);

		int balance = getBalance(node);

		if (balance > 1 && value < node->left->value) {
			return rotateRight(node);
		}

		if (balance < -1 && value > node->right->value) {
			return rotateLeft(node);
		}

		if (balance > 1 && value > node->left->value) {
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}

		if (balance < -1 && value < node->right->value) {
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}

		return node;

	}

	void inOrderRecursive(Node *node) {
		if (node) {
			inOrderRecursive(node->left);
			std::cout << node->value << ' ';
			inOrderRecursive(node->right);
		}
	}

	double sumLeavesRecursive(Node *node) {
		if (!node)
			return 0.0;
		if (!node->left && !node->right)
			return node->value;
		
		return sumLeavesRecursive(node->left) + sumLeavesRecursive(node->right);
	}

	public:

	AVLTree() : root(nullptr), nodeCount(0), sumNodes(0.0) {}
	
	void insert(double value) {
		root = insertRecursive(root, value);
	}

	void inOrderTraversal() {
		std::cout << "inOrderTraversal: ";
		inOrderRecursive(root);
		std::cout << '\n';
	}

	double sumOfLeaves() {
		return sumLeavesRecursive(root);
	}

	double averageOfNodes() {
        	if (nodeCount == 0) 
			return 0.0;
        	return sumNodes / nodeCount;
	}

    	int getNodeCount() {
        	return nodeCount;
    	}


    	bool isEmpty() {
        	return root == nullptr;
    	}
};

void demonstrateAVLTree() {
    AVLTree tree;
    int choice;
    double value;

    while (choice != 0) {
        std::cout << "1. Insert\n";
        std::cout << "2. In-order traversal\n";
        std::cout << "3. Sum of leaves\n";
        std::cout << "4  Average of all nodes\n";
        std::cout << "5. Number of nodes\n";
        std::cout << "0. exit\n";
        std::cout << "Choose option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Input: ";
                std::cin >> value;
                tree.insert(value);
                std::cout << "Element " << value << " was inserted\n";
                break;

            case 2:
                if (tree.isEmpty()) {
                    std::cout << "Tree is empty\n";
                } else {
                    tree.inOrderTraversal();
                }
                break;

            case 3:
                if (tree.isEmpty()) {
                    std::cout << "Tree is empty\n";
                } else {
                    std::cout << "Sum of leaves: " << tree.sumOfLeaves() << "\n";
                }
                break;

            case 4:
                if (tree.isEmpty()) {
                    std::cout << "Tree is empty\n";
                } else {
                    std::cout << "Average if all nodes: " << tree.averageOfNodes() << "\n";
                }
                break;

            case 5:
                std::cout << "Number of nodes: " << tree.getNodeCount() << "\n";
                break;

            case 0:
                std::cout << "Exit\n";
                break;

            default:
                std::cout << "not found\n";
                break;
        	}
	}
}


int main() {
	demonstrateAVLTree();
	return 0;
}
