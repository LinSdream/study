#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;

namespace alg {

	typedef vector<vector<float>> CoordinateArray;
	typedef vector<float> Coordinate;

	struct KdTreeNode
	{
		KdTreeNode* parent;
		KdTreeNode* left;
		KdTreeNode* right;

		Coordinate value;
		short split;
	};

	void select(int split, const CoordinateArray& arr, Coordinate* node, CoordinateArray* left, CoordinateArray* right) {

		if (arr.size() <= 0) return;
		if (arr.size() == 1) {
			*node = arr[0];
			return;
		}

		CoordinateArray temp(arr.size());
		for (int i = 0, length = arr.size(); i < length; i++) {
			temp[i] = arr[i];
		}

		for (int i = 0; i < temp.size()- 1; i++) {
			for (int j = 0; j < temp.size() - 1 - i; j ++ )
			{
				if (temp[j][split] >temp[j + 1][split]) {
					auto exchange = temp[j];
					temp[j] = temp[j + 1];
					temp[j + 1] = exchange;
				}
			}
		}

		int index = (temp.size() - 1) / 2;
		*node = temp[index];
		CoordinateArray l(index);
		for (int i = 0; i < index; i++) {
			l[i] = temp[i];
		}

		int length = temp.size() - 1 - index;
		CoordinateArray r(length);
		for (int i = 0; i < length; i++) {
			r[i] = temp[index + 1 + i];
		}

		*left = l;
		*right = r;
	}

	class KdTree {
	private:
		
		KdTreeNode* _tree;
		int _size;

		void buildTree(KdTreeNode* parent , KdTreeNode *root, int split,const CoordinateArray& arr) {
			root->parent = parent;
			root->split = split;
			CoordinateArray left;
			CoordinateArray right;
			select(split, arr, &root->value, &left, &right);
			if (&root->value == NULL) {
				delete root;
				return;
			}
			if (left.empty() && right.empty()) return;
			if (!left.empty()) {
				root->left = new KdTreeNode();
				buildTree(root,root->left, (split + 1) % 2, left);
			}
			if (!right.empty()) {
				root->right = new KdTreeNode();
				buildTree(root,root->right, (split + 1) % 2, right);
			}
		}

		void internalInorderTraversal(KdTreeNode* root, CoordinateArray* arr)
		{
			if (root != NULL) {
				internalInorderTraversal(root->left, arr);
				arr->push_back(root->value);
				internalInorderTraversal(root->right, arr);
			}
		}

	public :

		KdTree(const CoordinateArray& arr) {
			_size = arr.size();
			this->_tree = new KdTreeNode();
			buildTree(NULL,this->_tree, 0, arr);
		};

		~KdTree() {

		}
		
		CoordinateArray inorderTraversal() {
			CoordinateArray result;
			internalInorderTraversal(_tree, &result);
			return result;
		}

		friend ostream& operator << (ostream& out, const KdTree& tree) {
			
		}
	};
}