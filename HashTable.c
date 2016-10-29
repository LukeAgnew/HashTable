#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK_SIZE 100
#define TABLE_SIZE 65536

struct node {
	char * data;
	struct node * next;
};

struct list {
	struct node * head;
	struct node * tail;
};

struct stack {
	char elements[STACK_SIZE];
	int top;
};

struct hashtable {
	int size;
	struct list ** table;
};

struct hashtable * hashtable_new(int size);
void hashtable_free(struct hashtable * this);
void insert(struct hashtable * this, char * item);
void removeElement(struct hashtable * this, char * item);
int lookup(struct hashtable * this, char * item);
void print(struct hashtable * this);
unsigned hash_string(char * str);
void push(char element);
char pop(void);
int isEmpty(void);

struct stack operators;

struct hashtable * hashtable_new(int size)
{
	struct hashtable *ht;
	int i;
	
	ht = malloc(sizeof(struct hashtable));

	ht -> size = size;
	ht -> table = malloc(sizeof(struct list *) * (ht -> size));

	for (i = 0; i < (ht -> size); i++)
	{
		struct list * list;
		list = malloc(sizeof(struct list));
		list -> head = NULL;
		ht -> table[i] = list;
	}

	return ht;
}

void hashtable_free(struct hashtable * this)
{
	int i;
	struct list * bucket;
	struct node * p, * q;
	
	for (i = 0; i < this -> size; i++)
	{
		bucket = this -> table[i];

		p = bucket -> head; 

		while (p != NULL) 
		{
			q = p;
			p = p -> next;

			free(q);				
		}
		
		bucket -> head = NULL;
		bucket -> tail = NULL;
		free(bucket);
	}
	
	free(this -> table);
	free(this);
}

void insert(struct hashtable * this, char * item)
{
	unsigned hash = hash_string(item);
	
	struct list * bucket;
	
	bucket = this -> table[hash];

	struct node * p;
	p = malloc(sizeof(struct node));

	p -> data = item;
	p -> next = NULL;

	if (bucket -> head == NULL)
	{
		bucket -> tail = p;
		bucket -> head = p;
	}
	else
	{
		bucket -> tail -> next = p;
		bucket -> tail = p;
	}
}

void removeElement(struct hashtable * this, char * item)
{
	unsigned hash = hash_string(item);
	
	struct list * bucket;
	struct node * p, * q;

	bucket = this -> table[hash];

	p = bucket -> head;

	while (p != NULL)
	{
		if (strcmp(item, p -> data) == 0)
		{
			q = p -> next;
			free(p);

			p = q;
			bucket -> head = p;
		}
		else
		{
			p = p -> next;
		}
	}
}

int lookup(struct hashtable * this, char * item)
{
	unsigned hash = hash_string(item);

	struct list * bucket;
	struct node * n;

	bucket = this -> table[hash];

	for (n = bucket -> head; n != NULL; n = n -> next)
	{
		if (strcmp(n -> data, item) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void print(struct hashtable * this)
{
	struct list * bucket;
	struct node * n;

	int i;
	
	for (i = 0; i < (this -> size); i++)
	{
		bucket = this -> table[i];

		if (bucket -> head != NULL)
		{
			for (n = bucket -> head; n != NULL; n = n -> next)
			{
				char * myChar = n -> data;
			
				printf("%d: %s\n", i, myChar);
			}
		}
	}
}

unsigned hash_string(char * str)
{
	unsigned hash = 0;
	for (int i = 0; str[i] != '\0' && hash < TABLE_SIZE; i++)
	{
		hash = hash * 37 + str[i];
	}

	
	hash = hash % TABLE_SIZE;
	
	return hash;
}

void push(char element)
{
  	if (operators.top == (STACK_SIZE - 1))
  	{
	        printf("Stack is Full \n");
  		return;
  	}
  	else
  	{
		operators.top = operators.top + 1;
		operators.elements[operators.top] = element;
  	}
	return;
}

char pop()
{
	char element;

	element = operators.elements[operators.top];
	operators.top = operators.top - 1;

	return(element);
}

int isEmpty()
{
	if (operators.top == -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char *argv[])
{	
	int count;
	
	operators.top = -1;

	if (argc > 1)
	{
		struct hashtable *htable = hashtable_new(TABLE_SIZE);	
	
		for (count = 0; count < argc; count++)
		{
			char * item = argv[count];

			if (*item == '+' || *item == '-')
			{
				push(*item);		
			}
			else if (*item == '=')
			{
				print(htable);
			}
			else
			{
				char operator = pop();

				if (operator == '+')
				{
					insert(htable, item);
				}
				else
				{
					removeElement(htable, item);
				}
			}		
		}
		hashtable_free(htable);	
	}
}

