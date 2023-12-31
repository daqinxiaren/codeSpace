#ifndef __MYTREE_h
#define __MYTREE_H

typedef struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

TreeNode* createNode(int val);
TreeNode* buildCompleteBinaryTree(int *arr, int start, int len);

void postOrder(TreeNode* root, int arr[], int len);

void inOrder(TreeNode* root);

void preOrder(TreeNode* root);

void reverseArray(int arr[], int size);


#endif


