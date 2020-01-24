#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct contact {
	char* name;
	char phone[9]; //"421-1354" is 8 chars plus a null terminator
};

void printContact(struct contact);
void passThePointer(struct contact*);

typedef struct contact CONTACT;

int main(int argc, char** argv){
	CONTACT joe;
	joe.name = "Joe";
	strncpy(joe.phone, "555-1212", sizeof(joe.phone));
	printContact(joe);
	//passThePointer(&joe);

	CONTACT boss;

	CONTACT myContacts[10];
	myContacts[0].name = "Liz";
	strncpy(myContacts[0].phone, "518-2649", sizeof(myContacts[0].phone));
	
	printContact(myContacts[0]);
	//passThePointer(&myContacts[0]);

	CONTACT* alice = malloc(sizeof(CONTACT));
	alice->name= "Alice";
	strncpy(alice->phone, "212-2122", sizeof(alice->phone));

	printContact(*alice);
	//passThePointer(alice);
}

void printContact(CONTACT c){
	printf("Name: %s\n", c.name);
	printf("Phone: %s\n", c.phone);
}

void passThePointer(CONTACT* p){
	printf("Name: %s\n", p->name);
	printf("Phone: %s\n", p->phone);
}
