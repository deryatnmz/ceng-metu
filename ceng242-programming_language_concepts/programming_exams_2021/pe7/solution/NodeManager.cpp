
#include "NodeManager.h"

NodeManager::NodeManager() {

}

NodeManager::~NodeManager() {

	for (int i = 0; i < trees.size(); i++) {
		delete trees[i];
		trees[i] = NULL;
	}
	trees.clear();
}

// Helper function for setDataToNode()
// Returns the rank of the tree (whose root node is given) among the given container of trees
// If it can not find the given node id as the inital node of some tree, then it returns -1
int findRank(vector<Node*> treeSet, int id) {
	for (int i = 0; i < treeSet.size(); i++)
		if (treeSet[i]->getId() == id)
			return i;
	return -1;
}

// Helper function for setDataToNode() and getNode()
// Initially, it assumes that the node whose id is given definitely exists at somewhere in the given treeSet and
// It tries to return the reference pointer for "parent of that node" and "the node itself"
// Note that if that node is the root of some tree, then its parent will be returned as NULL
// It returns a vector of size 2 which includes the <parent-child> pointers
// If the node whose id is given does not exist, then returns <NULL, NULL>
vector<Node*> finder(vector<Node*>& treeSet, int id) {
	vector<Node*> pair;	// holds <parent-child> where child is the node whose id is given
						// namely, this will be a vector of size 2

	for (int i = 0; i < treeSet.size(); i++) {
		if (treeSet[i]->getId() == id) {
			pair.push_back(NULL);
			pair.push_back(treeSet[i]);
			return pair;
		}

		pair = finder(treeSet[i]->getChildren(), id);
		if (pair[1] != NULL) {		// if the child is found
			if (pair[0] == NULL)	// if the parent is not in the deeper nodes
				pair[0] = treeSet[i];	// then current node (treeSet[i]) is the parent
			return pair;
		}
		else
			pair.clear();
	}

	pair.push_back(NULL);
	pair.push_back(NULL);
	return pair;
}

void NodeManager::addRelation(int parent, int child) {

	Node* parentNode = NULL, * childNode = NULL;

	vector<Node*> found = finder(this->trees, parent);
	if (found[1] == NULL) {
		parentNode = new Node(parent);
		this->trees.push_back(parentNode);
	}
	else
		parentNode = found[1];
	found.clear();

	found = finder(this->trees, child);
	if (found[1] == NULL)
		childNode = new Node(child);
	else {
		childNode = found[1];
		int rank = findRank(this->trees, child);
		this->trees.erase(this->trees.begin() + rank);
	}
	found.clear();

	*parentNode += *childNode;
}

void NodeManager::setDataToNode(int id, char data) {
	DataNode* dataNode = new DataNode(id, data);
	Node* node = NULL;

	vector<Node*> found = finder(this->trees, id);
	if (found[1] != NULL) {
		node = found[1];
		if (found[0] == NULL) {
			int rank = findRank(this->trees, id);
			this->trees[rank] = dataNode;
		}
		else {
			int rank = findRank(found[0]->getChildren(), id);
			found[0]->getChildren()[rank] = dataNode;
		}
	}

	if (node == NULL)
		this->trees.push_back(dataNode);	// may get returned exception
	else {
		for (int i = 0; i < node->getChildren().size(); i++)
			*dataNode += *node->getChildren()[i];
		node->getChildren().clear();
	}

	delete node;
}

const Node& NodeManager::getNode(int id) {
	vector<Node*> found = finder(this->trees, id);
	return *found[1];
}