#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tgmath.h>
//#define DEBUG_ON
#define MAX(x,y) (( (x) > (y) ) ? (x) : (y) )
unsigned long removedvaluecount=0;
unsigned long nodecount=0;
unsigned long finalcount=0;


struct node
{
	unsigned long key;
	struct node* left;
	struct node* right;
	unsigned long value;
};

struct node* rightSubTree;
struct node* leftSubTree;

struct node* newNode(unsigned long key, unsigned long value)
{
	struct node* node = malloc(sizeof(struct node));
	node->key=key;
	node->left=NULL;
	node->right=NULL;
	node->value=value;
	return(node);
}

void printinorder(struct node* node)
{
	if(node == NULL)
	{
		return;
	}	
	printinorder(node->left);
	printf("%lu\t",node->key);
	printinorder(node->right);
}

void printpreorder(struct node* node)
{
	if(node == NULL)
	{
		return;
	}	
	printf("%lu\t",node->key);
	printpreorder(node->left);
	printpreorder(node->right);
}

	
static unsigned long lookup(struct node* node,unsigned long target)
{
	if(node==NULL)
	{
		return(0);
	}
	else
	{
		if(target == node->key)
		{
			return(node->value);
		}
		else
		{
			if(target < node->key)
			{
				return(lookup(node->left,target));
			}
			else
			{
				return(lookup(node->right,target));
			}
		}
	}
}

unsigned long lookupaftersplay(struct node* node, unsigned long target)
{
	if(node != NULL && node->key == target)
	{
		return(node->value);
	}
	else
	{
		return(0);
	}	
}


struct node* insert(struct node* node, unsigned long key, unsigned long value)
{
	if(node==NULL)
	{
		return(newNode(key,value));
	}	
	else
	{
		if(key == node->key)
		{
			node->value = value;
		}
		else
		{
			if(key < node->key)
			{
				node->left =insert(node->left,key,value);
			}
			else
			{
				node->right=insert(node->right,key,value);
			}
		}
		return(node);
	}
}


struct node* delete(struct node* node, struct node* pnode, unsigned long target)
{
	struct node* rchild;
	struct node* rchildparent;
	if(node==NULL)
	{
		return(pnode);
	}
	else
	{
		if(target == node->key)
		{
			if(node->left == NULL && node->right == NULL) //leaf node
			{
				if(pnode == NULL) //special case deleting the root node
				{
					free(node);
					return(NULL);
				}
				if(pnode->left == node)
				{
					pnode->left = NULL;
				}
				else
				{
					pnode->right = NULL;
				}
				free(node);
				return(pnode);
			}
			if(node->left ==NULL ) //one child
			{
				if(pnode == NULL) //deleting root having no left child
				{
					struct node* temp = node;
					node = node->right;
					free(temp);
					return(node);
				}
				if(pnode->left == node)
				{
					pnode->left = node->right;
				}
				else
				{
					pnode->right = node->right;
				}	
				free(node);
				return(pnode);
			}
			if(node->right ==NULL ) //one child
			{
				if(pnode == NULL) //deleting root having no right child
				{
					struct node* temp = node;
					node = node->left;
					free(temp);
					return(node);
				}
				if(pnode->left == node)
				{
					pnode->left = node->left;
				}
				else
				{
					pnode->right = node->left;
				}	
				free(node);
				return(pnode);
			}
		
			//two children case
			rchild = node->right;
			rchildparent=node;
			while(rchild->left != NULL)
			{
				rchildparent=rchild;
				rchild = rchild->left;
			}
			node->key=rchild->key;
			node->value=rchild->value;
			if(rchildparent == node)
			{
				//rchildparent->right=rchild->right;
				node->right=rchild->right;
			}
			else
			{
				//rchildparent->left=NULL;
				rchildparent->left=rchild->right;
			}
			free(rchild);
			if(pnode ==NULL) //root node
			{
				return(node);
			}			
			return(pnode);
		}
		else
		{
			if(target < node->key)
			{
				//return(delete(node->left,node,target));
				delete(node->left,node,target);
				return(node);
			}
			else
			{
				//return(delete(node->right,node,target));
				delete(node->right,node,target);
				return(node);
			}
		}
		
	}
	
}

void splay(struct node* headnode,unsigned long key)
{
	struct node* currentroot;
	struct node* rightSubTreeMin=rightSubTree;
	struct node* leftSubTreeMax=leftSubTree;
	currentroot=headnode->left;
	while(1)
	{
		if(key == currentroot->key)
		{
			headnode->left = currentroot;
			leftSubTreeMax->right = currentroot->left;
			rightSubTreeMin->left = currentroot->right;
			currentroot->left = leftSubTree->right;
			currentroot->right= rightSubTree->left;
			break;
		}
		if(key < currentroot->key)
		{
			struct node* b = currentroot->left;
			if(b != NULL)
			{
				if(key == b->key) //zig case
				{
					rightSubTreeMin->left = currentroot;
					rightSubTreeMin = currentroot;
					currentroot->left=NULL;
					currentroot = b;
				}
				else
				{
					if(key < b->key) //zig-zig case
					{
						if(b->left != NULL)
						{
							currentroot->left = b->right;
							b->right = currentroot;
							rightSubTreeMin->left = b;
							rightSubTreeMin = b;
							currentroot = b->left;
							b->left=NULL;
						}
						else //just do a zig
						{
							rightSubTreeMin->left = currentroot;
							rightSubTreeMin = currentroot;
							currentroot->left=NULL;
							currentroot = b;
						}
					}
					else //zig-zag case 
					{
						if(b->right != NULL)
						{
							rightSubTreeMin->left = currentroot;
							rightSubTreeMin = currentroot;
							currentroot->left=NULL;
							leftSubTreeMax->right = b;
							leftSubTreeMax = b;
							currentroot=b->right;
							b->right=NULL;
						}
						else //just do a zig
						{
							rightSubTreeMin->left = currentroot;
							rightSubTreeMin = currentroot;
							currentroot->left=NULL;
							currentroot = b;
						}

					}
				}
			}
			else
			{
				headnode->left = currentroot;
				leftSubTreeMax->right = currentroot->left;
				rightSubTreeMin->left = currentroot->right;
				currentroot->left = leftSubTree->right;
				currentroot->right= rightSubTree->left;
				break;
			}
		}
		else
		{
			struct node* b = currentroot->right;
			if(b != NULL)
			{
				if(key == b->key) //zag case
				{
					leftSubTreeMax->right = currentroot;
					leftSubTreeMax = currentroot;
					currentroot->right=NULL;
					currentroot = b;
				}
				else
				{
					if(key > b->key) //zag-zag case
					{
						if(b->right != NULL)
						{
							currentroot->right = b->left;
							b->left = currentroot;
							leftSubTreeMax->right = b;
							leftSubTreeMax = b;
							currentroot = b->right;
							b->right=NULL;
						}
						else //just do a zag
						{
							leftSubTreeMax->right = currentroot;
							leftSubTreeMax = currentroot;
							currentroot->right=NULL;
							currentroot = b;
						}
					}
					else //zag-zig case 
					{
						if(b->left != NULL)
						{
							leftSubTreeMax->right = currentroot;
							leftSubTreeMax = currentroot;
							currentroot->right=NULL;
							rightSubTreeMin->left = b;
							rightSubTreeMin = b;
							currentroot=b->left;
							b->left=NULL;
						}
						else //just do a zag
						{
							leftSubTreeMax->right = currentroot;
							leftSubTreeMax = currentroot;
							currentroot->right=NULL;
							currentroot = b;
						}
					}
				}
			}
			else
			{
				headnode->left = currentroot;
				leftSubTreeMax->right = currentroot->left;
				rightSubTreeMin->left = currentroot->right;
				currentroot->left = leftSubTree->right;
				currentroot->right= rightSubTree->left;
				break;
			}
		}
	}
}

void removevalue(struct node* node, struct node* pnode, unsigned long target)
{
	if(node == NULL)
	{
		return;
	}
	if(node->value == target)
	{
		delete(node,pnode,node->key);
		removedvaluecount++;	
	}	
	removevalue(node->left,node,target);
	removevalue(node->right,node,target);
}

void size(struct node* node)
{
	if(node == NULL)
	{
		return;
	}
	if(node->value > 0)
	{
		nodecount++;	
	}	
	size(node->left);
	size(node->right);
}

static unsigned long findmax(struct node* node)
{
	while(node->right != NULL)
	{
		node = node->right;
	}
	return node->value;

}

static unsigned long findmin(struct node* node)
{
	while(node->left != NULL)
	{
		node = node->left;
	}
	return node->value;

}

int findheight(struct node* node)
{
	if(node == NULL)
	{
		return 0;
	}
	return(1+ MAX(findheight(node->left) , findheight(node->right) ));
}
int main()
{
	FILE *fp;
	fp = fopen("input.txt","r");
	struct node* root = newNode(9223372036854775807,9223372036854775807);
	rightSubTree = newNode(9223372036854775807,9223372036854775807);
	leftSubTree  = newNode(-9223372036854775807,-9223372036854775807);
	if(fp !=NULL)
	{
		char line[64];
		char *tline;
		unsigned long lkey;
		while( fgets(line,sizeof(line),fp) !=NULL)
		{
			tline=strtok(line," ");
			if(strstr(tline,"Insert") !=NULL)
			{
				lkey=atol(strtok(NULL, " "));
				insert(root,lkey,atol(strtok(NULL, " ")));
				splay(root,lkey);
			}
			else if(strstr(tline,"Find") !=NULL)
			{
				lkey=atol(strtok(NULL, " "));
				splay(root,lkey);
				lookupaftersplay(root->left,lkey);
			}
			else if(strstr(tline,"Remove") !=NULL)
			{
				lkey=atol(strtok(NULL, " "));
				splay(root,lkey);
				if(root->left->key == lkey)
				{
					delete(root,NULL,lkey);
				}
			}
			else if(strstr(tline,"Min") !=NULL)
			{
				unsigned long t = findmin(root);
				finalcount = finalcount + t ;
				#ifdef DEBUG_ON
					printf("min : %lu\n",t);
				#endif
			}
			else if(strstr(tline,"Max") !=NULL)
			{
				if(root->left != NULL)
				{
					unsigned long t1 = findmax(root->left);
					finalcount = finalcount + t1;
					#ifdef DEBUG_ON
						printf("max : %lu\n",t1);
					#endif
				}
			}
			else if(strstr(tline,"RemValue") !=NULL)
			{
				removedvaluecount=0;
				removevalue(root,NULL,atol(strtok(NULL, " ")));
				finalcount = finalcount + removedvaluecount; 
				#ifdef DEBUG_ON
					printf("removed count: %lu\n",removedvaluecount);
				#endif
			}
			else if(strstr(tline,"Size") !=NULL)
			{
				nodecount=0;
				size(root->left);
				finalcount = finalcount + nodecount; 
				#ifdef DEBUG_ON
					printf("node count: %lu\n",nodecount);
				#endif
			}
			else if(strstr(tline,"IsEmpty") !=NULL)
			{
				if(root->left == NULL)
				{
					printf("empty\n");
				}
				else
				{
					printf("not empty\n");
				}
			}
			
		}
		fclose(fp);
	}
	else
	{
		perror("input.txt");
	}
	printf("%lu\n",finalcount);
}
