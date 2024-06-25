#include "Action.h"
#include "Exception.h"

Action::Action() {

}

Action::~Action() {
	if (this->memberNode != NULL) {
		delete this->memberNode;
		this->memberNode = NULL;
	}
}

/********************** CompleteAction **************************/

CompleteAction::CompleteAction(const Node& node) {
	try {
		char data = node.getData();
		this->memberNode = new DataNode(node, data);
	}
	catch (InvalidRequest e) {
		this->memberNode = new Node(node);
	}
}

CompleteAction::~CompleteAction() {

}

// Helper function for CompleteAction::act()
// It recursively ands the given argument Nodes (namely not only themselves but also their children)
// It returns the result as a totally new tree (reference to a new Node object)
Node* act_helper(Node* cnode1, Node* cnode2) {

	if (cnode1->getId() == cnode2->getId()) {
		Node* output = *cnode1 & *cnode2;
		for (int i = 0; i < cnode1->getChildren().size(); i++) {
			for (int j = 0; j < cnode2->getChildren().size(); j++)
				if (cnode1->getChildren()[i]->getId() == cnode2->getChildren()[j]->getId()) {
					Node* child = act_helper(cnode1->getChildren()[i], cnode2->getChildren()[j]);
					for (int k=0; k < output->getChildren().size(); k++)
						if (output->getChildren()[k]->getId() == child->getId()) {
							delete output->getChildren()[k];
							output->getChildren()[k] = child;
							break;
						}
					break;
				}
		}
		return output;
	}

}

Node* CompleteAction::act(const Node* node) const {

	Node* otherNode = NULL;
	try {
		char data = node->getData();
		otherNode = new DataNode(*node, data);
	}
	catch (InvalidRequest e) {
		otherNode = new Node(*node);
	}

	Node* result = act_helper(memberNode, otherNode);

	delete otherNode;
	return result;

}

/************************* CutAction ***************************/

CutAction::CutAction(char character) {
	this->memberNode = NULL;
	this->memberChar = character;
}

CutAction::~CutAction() {

}

// Helper function for CutAction::act()
// It counts the "memberChar" inside the children and grandchildren of the given Node
// Returns the total count
int CutAction::count2Gen(Node* node) const {
	int total = 0;
	vector<Node*>& children = node->getChildren();
	for (int i = 0; i < children.size(); i++) {
		try {
			char data = children[i]->getData();
			if (data == memberChar)
				total++;
		}
		catch (InvalidRequest e) {}
		vector<Node*>& grandchildren = children[i]->getChildren();
		for (int j = 0; j < grandchildren.size(); j++) {
			try {
				char data = grandchildren[j]->getData();
				if (data == memberChar)
					total++;
			}
			catch (InvalidRequest e) {}
		}
	}
	return total;
}

// Helper function for CutAction::act()
// It returns true if the given node has at least 1 grandchild, false otherwise
bool doesHaveGrandChild(Node* node) {

	vector<Node*>& children = node->getChildren();
	for (int i = 0; i < children.size(); i++)
		if (children[i]->getChildren().size() > 0)
			return true;
	return false;
}

// Helper function for CutAction::act()
// It completely deletes the children of the given node
void deleteChildren(Node* node) {
	for (int j = 0; j < node->getChildren().size(); j++) {
		delete node->getChildren()[j];
		node->getChildren()[j] = NULL;
	}
	node->getChildren().clear();
}

Node* CutAction::act(const Node* node) const {

	Node* cnode = new Node(*node);
	for (int i = 0; i < cnode->getChildren().size(); i++) {
		Node* child = cnode->getChildren()[i];
		if (doesHaveGrandChild(child)) {
			Node* newChild = act(child);
			delete child;
			cnode->getChildren()[i] = newChild;
		}
		else {
			if (count2Gen(child) >= 2) {
				deleteChildren(child);
				Node* newChild = new DataNode(*child, memberChar);
				delete child;
				cnode->getChildren()[i] = newChild;
			}
		}
	}

	if (count2Gen(cnode) >= 2) {
		deleteChildren(cnode);
		Node* newNode = new DataNode(*cnode, memberChar);
		delete cnode;
		cnode = newNode;
	}

	return cnode;
}

/************************* CompositeAction ***************************/

CompositeAction::CompositeAction() {
	this->memberNode = NULL;
}

CompositeAction::~CompositeAction() {
	this->actions.clear();
}

CompositeAction* CompositeAction::addAction(const Action* action) {
	this->actions.push_back(action);
	return this;
}

Node* CompositeAction::act(const Node* node) const {

	Node* argument = new Node(*node);
	for (int i = 0; i < this->actions.size(); i++) {
		Node* result = this->actions[i]->act(argument);
		delete argument;
		argument = result;
	}
	return argument;

}