#include "Edge.h"

Edge::Edge(int idd, Password passw) {
    id = idd;
	password=passw;
	matchingEdge = NULL;
}

Edge::~Edge() {
}

int Edge::getId() const {
	return id;
}

Edge* Edge::getMatchingEdge() const {
    if(matchingEdge)
        return matchingEdge;
    else 
        return NULL;
}


bool Edge::matchWith(Edge& e) {
    if(e.getMatchingEdge() == NULL && this->getMatchingEdge()==NULL && this->matchWithHelper(e.password)){
        this->matchingEdge=&e;
        e.matchingEdge=this;
        return true;
    }
    else
        return false;

}


void Edge::breakMatch() {
    this->getMatchingEdge()->matchingEdge=NULL;
    this->matchingEdge=NULL;

}

StraightEdge::StraightEdge(int i):Edge(i,SEND_ME_STRAIGHT) {
}

StraightEdge::~StraightEdge() {
}

/* This method clones the current object and returns the pointer to its clone.
   Cloning is actually a deep-copy operation, so you need to construct a new
   StraightEdge object.
   Note that if the current edge is matched with some other edge, then its
   matchingEdge should be cloned too and only the clones should be matched 
   with each other.
*/
Edge* StraightEdge::clone() const {
    if(this->getMatchingEdge()==NULL){
        Edge* edge = new StraightEdge(this->getId());
        return edge;
    }
    else{
        Edge* this_edge = new StraightEdge(this->getId());
        Edge* match_edge = new StraightEdge(this->getMatchingEdge()->getId());
        this_edge->matchWith(*match_edge);
        return this_edge;
    }
}


bool StraightEdge::matchWithHelper(Password p) {
    return (p == SEND_ME_STRAIGHT);
}

InwardsEdge::InwardsEdge(int i):Edge(i,SEND_ME_OUTWARDS) {
}
InwardsEdge::~InwardsEdge() {
}

/* This method clones the current object and returns the pointer to its clone.
   Cloning is actually a deep-copy operation, so you need to construct a new
   InwardsEdge object.
   Note that if the current edge is matched with some other edge, then its
   matchingEdge should be cloned too and only the clones should be matched 
   with each other.
*/
Edge* InwardsEdge::clone() const {
    if(this->getMatchingEdge()==NULL){
        Edge* edge = new InwardsEdge(this->getId());
        return edge;
    }
    else{
        Edge* this_edge = new InwardsEdge(this->getId());
        Edge* match_edge = new OutwardsEdge(this->getMatchingEdge()->getId());
        this_edge->matchWith(*match_edge);
        return this_edge;
    }
}


bool InwardsEdge::matchWithHelper(Password p) {
    return (p==SEND_ME_INWARDS);
}

OutwardsEdge::OutwardsEdge(int i):Edge(i,SEND_ME_INWARDS ) {
}
OutwardsEdge::~OutwardsEdge() {
}

/* This method clones the current object and returns the pointer to its clone.
   Cloning is actually a deep-copy operation, so you need to construct a new
   OutwardsEdge object.
   Note that if the current edge is matched with some other edge, then its
   matchingEdge should be cloned too and only the clones should be matched 
   with each other.
*/
Edge* OutwardsEdge::clone() const {
    if(this->getMatchingEdge()==NULL){
        Edge* edge = new OutwardsEdge(this->getId());
        return edge;
    }
    else{
        Edge* this_edge = new OutwardsEdge(this->getId());
        Edge* match_edge = new InwardsEdge(this->getMatchingEdge()->getId());
        this_edge->matchWith(*match_edge);
        return this_edge;
    }
}

bool OutwardsEdge::matchWithHelper(Password p) {
	return (p == SEND_ME_OUTWARDS);
}






