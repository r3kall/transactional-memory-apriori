#ifndef LISTITEMSET_H
#define LISTITEMSET_H

#include "Itemset.h"

//ListElement is the element in ListItemset 
class ListElement {
public:

	TM_SAFE
	ListElement(Itemset* itemPtr);	// initialize a list element

	TM_SAFE
	~ListElement();

	TM_SAFE
	inline ListElement* next() { return Next; }

	TM_SAFE
	inline void set_next(ListElement *n) { Next = n; }

	TM_SAFE
	inline Itemset* item() { return Item; }

	inline void set_item(Itemset *it) { Item = it; }
   
private:
	ListElement *Next;	// next element on list, 
											// NULL if this is the last
	Itemset *Item; 	    // pointer to item on the list
};

class ListItemset {
public:

	TM_SAFE
	ListItemset();			// initialize the list

	TM_SAFE
	~ListItemset();			// de-allocate the list

	TM_SAFE
	void append(Itemset& item); 	// Put item at the end of the list

	TM_SAFE
	Itemset* remove(); 	 	// Take item off the front of the list

	ListElement* node(int);
	void sortedInsert(Itemset*);// Put item into list
	ListElement* sortedInsert(Itemset*, ListElement*);
	void clearlist();

	friend ostream& operator << (ostream&, ListItemset&);

	TM_SAFE
	inline ListElement* first() { return First; }

	TM_SAFE
	inline ListElement* last() { return Last; }

	TM_SAFE
	inline int numitems() { return numitem; }

private:
	ListElement* First;  	// Head of the list, NULL if list is empty
	ListElement* Last;		// Last element of list
	int numitem;
};

#endif // LISTITEMSET_H





