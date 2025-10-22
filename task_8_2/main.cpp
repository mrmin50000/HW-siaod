#include <iostream>
#include <string>
#include <vector>
#include <climits>

struct Matrix {
	int rows;
	int cols;

	Matrix(int r, int c) : rows(r), cols(c) {};
};

std::string greedyMatrixChainMin(std::vector<Matrix> &matrices, std::vector<int> &order) {
	if (matrices.empty())
		return "";
	if (matrices.size() == 1)
		return "A0";

	std::vector<Matrix> temp = matrices;
	std::vector<std::string> expressions(temp.size());
	order.clear();

	for (int i = 0; i < temp.size(); ++i) {
		expressions[i] = "A" + std::to_string(i);
	}

	while (temp.size() > 1) {
		int minCost = INT_MAX;
		int bestIndex = 0;

		for (int i = 0; i < temp.size() - 1; ++i) {
			int cost = temp[i].rows * temp[i].cols * temp[i + 1].cols;
			if (cost < minCost) {
				minCost = cost;
				bestIndex = i;
			}
		}

		Matrix newMatrix(temp[bestIndex].rows, temp[bestIndex + 1].cols);
		std::string newExpr = "(" + expressions[bestIndex] + expressions[bestIndex + 1] + ")";

		temp[bestIndex] = newMatrix;
		expressions[bestIndex] = newExpr;

		temp.erase(temp.begin() + bestIndex + 1);
		expressions.erase(expressions.begin() + bestIndex + 1);

		order.push_back(bestIndex);
	}

	return expressions[0];
}

std::string greedyMatrixChainMax(std::vector<Matrix> &matrices, std::vector<int> &order) {
	if (matrices.empty())
		return "";
	if (matrices.size() == 1)
		return "A0";

	std::vector<Matrix> temp = matrices;
	std::vector<std::string> expressions(temp.size());
	order.clear();

	for (int i = 0; i < temp.size(); ++i) {
		expressions[i] = "A" + std::to_string(i);
	}

	while (temp.size() > 1) {
		int maxCost = -1;
		int bestIndex = 0;

		for (int i = 0; i < temp.size() - 1; ++i) {
			int cost = temp[i].rows * temp[i].cols * temp[i + 1].cols;
			if (cost > maxCost) {
				maxCost = cost;
				bestIndex = i;
			}
		}

		Matrix newMatrix(temp[bestIndex].rows, temp[bestIndex + 1].cols);
		std::string newExpr = "(" + expressions[bestIndex] + expressions[bestIndex + 1] + ")";

		temp[bestIndex] = newMatrix;
		expressions[bestIndex] = newExpr;

		temp.erase(temp.begin() + bestIndex + 1);
		expressions.erase(expressions.begin() + bestIndex + 1);
		order.push_back(bestIndex);
	}

	return expressions[0];
}

int calculateOperations(std::vector<Matrix> &matrices) {
	if (matrices.size() <= 1)
		return 0;

	int total = 0;
	std::vector<Matrix> temp = matrices;

	Matrix res = temp[0];
	for (int i = 1; i < temp.size(); ++i) {
		total += res.rows * res.cols * temp[i].cols;
		res = Matrix(res.rows, temp[i].cols);
	}

	return total;
}

int calculateOrderedOperations(std::vector<Matrix> &matrices, std::vector<int> &order) {
	if (matrices.size() <= 1)
		return 0;

	std::vector<Matrix> temp = matrices;
	int total = 0;

	for (int i : order) {
		if (i < 0 || i >= temp.size() - 1)
			continue;
	
		total += temp[i].rows * temp[i].cols * temp[i + 1].cols;
		temp[i] = Matrix(temp[i].rows, temp[i + 1].cols);
		temp.erase(temp.begin() + i + 1);
	}

	return total;
}

int main() {
	std::vector<Matrix> matrices = {
		Matrix(5, 10),
		Matrix(10, 3),
		Matrix(3, 12),
		Matrix(12, 5),
		Matrix(5, 50),
		Matrix(50, 6)
	};
	
	std::cout << "Matrices: ";
	for (int i = 0; i < matrices.size(); ++i) {
		std::cout << "A" << i << "(" << matrices[i].rows << "x" << matrices[i].cols << ") ";
	}
	std::cout << "\n\n";

	std::vector<int> minOrder, maxOrder;

	std::string minExpr = greedyMatrixChainMin(matrices, minOrder);
	int minOps = calculateOrderedOperations(matrices, minOrder);

	std::cout << "Greedy algorithm (minimum):\nParenthesis placement: " << minExpr << "\nNumber of scalar multiplications: " << minOps << "\n\n";

	std::string maxExpr = greedyMatrixChainMax(matrices, maxOrder);
	int maxOps = calculateOrderedOperations(matrices, maxOrder);

	std::cout << "Greedy algorithm (maximum):\nParenthesis placement: " << maxExpr << "\nNumber of scalar multiplications: " << maxOps << "\n\n";
	
	int baseMul = calculateOperations(matrices);
	std::cout << "Basic multiplication (without optimization): " << baseMul << " operations\n";

	return 0;

}
