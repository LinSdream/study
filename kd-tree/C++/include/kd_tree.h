#include<vector>
#include<list>
#include<algorithm>
#include<iostream>
#include<math.h>
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

		struct RetrospectiveNode {
			KdTreeNode* node;
			float distance;
		};
		
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
			if (left.empty() && right.empty()) {
				// 已经是叶节点了，不需要记录切割线，可以用来辅助判断是否为叶节点
				root->split = -1;
				return;
			}
			if (!left.empty()) {
				root->left = new KdTreeNode();
				buildTree(root,root->left, (split + 1) % 2, left);
			}
			if (!right.empty()) {
				root->right = new KdTreeNode();
				buildTree(root,root->right, (split + 1) % 2, right);
			}
		}

		void internalInOrderTraversal(KdTreeNode* root, CoordinateArray* arr)
		{
			if (root != NULL) {
				internalInOrderTraversal(root->left, arr);
				arr->push_back(root->value);
				internalInOrderTraversal(root->right, arr);
			}
		}

		void internalPreOrderTraversal(KdTreeNode* root, CoordinateArray* arr)
		{
			if (root != NULL) {
				arr->push_back(root->value);
				internalPreOrderTraversal(root->left, arr);
				internalPreOrderTraversal(root->right, arr);
			}
		}

		void internalPostOrderTraversal(KdTreeNode* root, CoordinateArray* arr)
		{
			if (root != NULL) {
				internalPostOrderTraversal(root->left, arr);
				internalPostOrderTraversal(root->right, arr);
				arr->push_back(root->value);
			}
		}

		/// <summary>
		/// 两点求距
		/// </summary>
		/// <returns></returns>
		float distance(const Coordinate& cur, const Coordinate& target)
		{
			// 两点间距离
			return sqrtf(powf((cur[0] - target[0]),2) + powf((cur[1] - target[1]), 2));
		}

		bool compare(const Coordinate& target, KdTreeNode* compareNode, vector< RetrospectiveNode>* retrospectiveList,int totalLength) {
			if (retrospectiveList->size() < totalLength) {
				RetrospectiveNode node;
				node.distance = distance(target, compareNode->value);
				node.node = compareNode;
				retrospectiveList->push_back(node);
				sort(retrospectiveList->begin(), retrospectiveList->end(), [](RetrospectiveNode a, RetrospectiveNode b) {
					return a.distance < b.distance;
					});
				return true;
			}

			float distance = this->distance(target, compareNode->value);
			if ((*retrospectiveList)[retrospectiveList->size() - 1].distance < distance) return false;

			RetrospectiveNode node;
			node.distance = distance;
			node.node = compareNode;
			(*retrospectiveList)[retrospectiveList->size() - 1] = node;
			sort(retrospectiveList->begin(), retrospectiveList->end(), [](RetrospectiveNode a, RetrospectiveNode b) {
				return a.distance < b.distance;
				});
			return true;
		}

		bool existElement(list<KdTreeNode*>* arr, KdTreeNode* element) {
			list<KdTreeNode*>::iterator itr = std::find(arr->begin(), arr->end(), element);
			if (itr != arr->end()) return true;
			return false;
		}

		void retrospective(const Coordinate& target, KdTreeNode* cur,list<KdTreeNode*>* callOnList,
			vector< RetrospectiveNode>* retrospectiveList,int totalLength) 
		{
			if (cur == NULL) return;
			list<KdTreeNode*>::iterator itr = std::find(callOnList->begin(), callOnList->end(), cur);
			if (existElement(callOnList,cur)) {
				retrospective(target, cur->parent, callOnList, retrospectiveList, totalLength);
				return;
			}
			callOnList->push_back(cur);
			compare(target, cur, retrospectiveList, totalLength);
			//叶节点，回溯
			if (cur->split == -1) {
				retrospective(target, cur->parent, callOnList, retrospectiveList, totalLength);
				return;
			}
			//非叶节点，计算与切线的距离
			float distance = 0;
			if (cur->split == 0) {
				Coordinate coordinate(2);
				coordinate[0] = cur->value[0];
				coordinate[1] = 0;
				distance = this->distance(target, coordinate);
			}
			else {
				Coordinate coordinate(2);
				coordinate[0] = 0;
				coordinate[1] = cur->value[1];
				distance = this->distance(target, coordinate);
			}

			// 如果距离大于切线，继续回溯
			if (retrospectiveList->size() == totalLength && distance > (*retrospectiveList)[retrospectiveList->size() - 1].distance) {
				retrospective(target, cur->parent, callOnList, retrospectiveList, totalLength);
				return;
			}
			// 去另一个树,
			//左子树
			if (cur->right!= NULL && !existElement(callOnList,cur->right)) {
				KdTreeNode* node = findLeafNode(target, cur->right);
				retrospective(target, node, callOnList, retrospectiveList, totalLength);
			}
			else if(cur->left!= NULL && !existElement(callOnList,cur->left)) {
				KdTreeNode* node = findLeafNode(target, cur->left);
				retrospective(target, node, callOnList, retrospectiveList, totalLength);
			}
			else {
				//如果只有一个子节点并且是已经访问过的，就继续往上找
				retrospective(target, cur->parent, callOnList, retrospectiveList, totalLength);

			}
		}

		KdTreeNode* findLeafNode(const Coordinate& target,  KdTreeNode* node) {
			if (node->split == -1) {
				return node;
			}
			if (target[node->split] < node->value[node->split]) {
				return findLeafNode(target, node->left);
			}
			return findLeafNode(target, node->right);
		}

		void deleteTree(KdTreeNode* root) {
			if (root == NULL) return;
			deleteTree(root->left);
			deleteTree(root->right);
			delete root;
		}
	public :

		KdTree(const CoordinateArray& arr) {
			_size = arr.size();
			this->_tree = new KdTreeNode();
			buildTree(NULL,this->_tree, 0, arr);
		};

		~KdTree() {
			deleteTree(_tree);
		}

		CoordinateArray find(const Coordinate& target, int count) {
			// find leaf node
			auto leaf = findLeafNode(target, _tree);
			list<KdTreeNode*> callOnList;
			vector< RetrospectiveNode> retrospectiveList;

			//callOnList.push_back(leaf);
			//RetrospectiveNode node;
			//
			//node.distance = distance(target, leaf->value);
			//node.node = leaf;
			//retrospectiveList.push_back(node);
			//
			retrospective(target, leaf, &callOnList, &retrospectiveList, count);
			
			CoordinateArray arr(count);
			for (int i = 0; i < retrospectiveList.size(); i++) {
				arr[i] = retrospectiveList[i].node->value;
			}
			return arr;
		}
		
		CoordinateArray inOrderTraversal() {
			CoordinateArray result;
			internalInOrderTraversal(_tree, &result);
			return result;
		}

		CoordinateArray preOrderTraversal() {
			CoordinateArray result;
			internalPreOrderTraversal(_tree, &result);
			return result;
		}

		CoordinateArray postOrderTraversal() {
			CoordinateArray result;
			internalPostOrderTraversal(_tree, &result);
			return result;
		}

		friend ostream& operator << (ostream& out, const KdTree& tree) {
			
		}
	};
}