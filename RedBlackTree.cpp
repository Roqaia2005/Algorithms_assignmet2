#include <iostream>

enum NodeColor { RED, BLACK };

struct TreeNode {
    int value;
    NodeColor nodeColor;
    TreeNode* leftChild;
    TreeNode* rightChild;
    TreeNode* parentNode;

    explicit TreeNode(int data) : value(data), nodeColor(RED), leftChild(nullptr), rightChild(nullptr), parentNode(nullptr) {}
};

class RBTree {
private:
    TreeNode* rootNode;

    // Perform left rotation
    void rotateLeft(TreeNode* current) {
        if (current == nullptr || current->rightChild == nullptr)
            return;

        TreeNode* temp = current->rightChild;
        current->rightChild = temp->leftChild;
        if (temp->leftChild != nullptr)
            temp->leftChild->parentNode = current;
        temp->parentNode = current->parentNode;
        if (current->parentNode == nullptr)
            rootNode = temp;
        else if (current == current->parentNode->leftChild)
            current->parentNode->leftChild = temp;
        else
            current->parentNode->rightChild = temp;
        temp->leftChild = current;
        current->parentNode = temp;
    }

    // Perform right rotation
    void rotateRight(TreeNode* current) {
        if (current == nullptr || current->leftChild == nullptr)
            return;

        TreeNode* temp = current->leftChild;
        current->leftChild = temp->rightChild;
        if (temp->rightChild != nullptr)
            temp->rightChild->parentNode = current;
        temp->parentNode = current->parentNode;
        if (current->parentNode == nullptr)
            rootNode = temp;
        else if (current == current->parentNode->leftChild)
            current->parentNode->leftChild = temp;
        else
            current->parentNode->rightChild = temp;
        temp->rightChild = current;
        current->parentNode = temp;
    }

    // Fix violations after insertion
    void resolveInsert(TreeNode* newNode) {
        while (newNode != rootNode && newNode->parentNode->nodeColor == RED) {
            if (newNode->parentNode == newNode->parentNode->parentNode->leftChild) {
                TreeNode* uncle = newNode->parentNode->parentNode->rightChild;
                if (uncle != nullptr && uncle->nodeColor == RED) {
                    newNode->parentNode->nodeColor = BLACK;
                    uncle->nodeColor = BLACK;
                    newNode->parentNode->parentNode->nodeColor = RED;
                    newNode = newNode->parentNode->parentNode;
                } else {
                    if (newNode == newNode->parentNode->rightChild) {
                        newNode = newNode->parentNode;
                        rotateLeft(newNode);
                    }
                    newNode->parentNode->nodeColor = BLACK;
                    newNode->parentNode->parentNode->nodeColor = RED;
                    rotateRight(newNode->parentNode->parentNode);
                }
            } else {
                TreeNode* uncle = newNode->parentNode->parentNode->leftChild;
                if (uncle != nullptr && uncle->nodeColor == RED) {
                    newNode->parentNode->nodeColor = BLACK;
                    uncle->nodeColor = BLACK;
                    newNode->parentNode->parentNode->nodeColor = RED;
                    newNode = newNode->parentNode->parentNode;
                } else {
                    if (newNode == newNode->parentNode->leftChild) {
                        newNode = newNode->parentNode;
                        rotateRight(newNode);
                    }
                    newNode->parentNode->nodeColor = BLACK;
                    newNode->parentNode->parentNode->nodeColor = RED;
                    rotateLeft(newNode->parentNode->parentNode);
                }
            }
        }
        rootNode->nodeColor = BLACK;
    }

    // Replace one subtree with another
    void replaceSubtree(TreeNode* oldNode, TreeNode* newNode) {
        if (oldNode->parentNode == nullptr)
            rootNode = newNode;
        else if (oldNode == oldNode->parentNode->leftChild)
            oldNode->parentNode->leftChild = newNode;
        else
            oldNode->parentNode->rightChild = newNode;
        if (newNode != nullptr)
            newNode->parentNode = oldNode->parentNode;
    }

    // Delete a specific node
    void removeNode(TreeNode* targetNode) {
        if (targetNode == nullptr)
            return;

        TreeNode* tempNode = targetNode;
        TreeNode* childNode = nullptr;
        NodeColor originalColor = tempNode->nodeColor;

        if (targetNode->leftChild == nullptr) {
            childNode = targetNode->rightChild;
            replaceSubtree(targetNode, targetNode->rightChild);
        } else if (targetNode->rightChild == nullptr) {
            childNode = targetNode->leftChild;
            replaceSubtree(targetNode, targetNode->leftChild);
        } else {
            tempNode = findMinimum(targetNode->rightChild);
            originalColor = tempNode->nodeColor;
            childNode = tempNode->rightChild;

            if (tempNode->parentNode == targetNode) {
                if (childNode != nullptr)
                    childNode->parentNode = tempNode;
            } else {
                replaceSubtree(tempNode, tempNode->rightChild);
                tempNode->rightChild = targetNode->rightChild;
                if (tempNode->rightChild != nullptr)
                    tempNode->rightChild->parentNode = tempNode;
            }
            replaceSubtree(targetNode, tempNode);
            tempNode->leftChild = targetNode->leftChild;
            if (tempNode->leftChild != nullptr)
                tempNode->leftChild->parentNode = tempNode;
            tempNode->nodeColor = targetNode->nodeColor;
        }

        if (originalColor == BLACK && childNode != nullptr)
            resolveDelete(childNode);

        delete targetNode;
    }

    // Fix violations after deletion
    void resolveDelete(TreeNode* node) {
        while (node !=  rootNode && node->nodeColor == BLACK) {
            if (node == node->parentNode->leftChild) {
                TreeNode* sibling = node->parentNode->rightChild;

                // Case 1: Sibling is RED
                if (sibling->nodeColor == RED) {
                    // Change sibling's color to BLACK and parent's color to RED
                    sibling->nodeColor = BLACK;
                    node->parentNode->nodeColor = RED;

                    // Perform a left rotation on parent
                    rotateLeft(node->parentNode);
                    sibling = node->parentNode->rightChild;
                }

                // Case 2: Sibling and its children are BLACK
                if ((sibling->leftChild == nullptr || sibling->leftChild->nodeColor == BLACK) &&
                    (sibling->rightChild == nullptr || sibling->rightChild->nodeColor == BLACK)) {
                    sibling->nodeColor = RED;
                    node = node->parentNode;
                } else {
                    // Case 3: Sibling's right child is BLACK, left child is RED
                    if (sibling->rightChild == nullptr || sibling->rightChild->nodeColor == BLACK) {
                        if (sibling->leftChild != nullptr)
                            sibling->leftChild->nodeColor = BLACK;
                        sibling->nodeColor = RED;
                        rotateRight(sibling);
                        sibling = node->parentNode->rightChild;
                    }

                    // Case 4: Sibling's right child is RED
                    sibling->nodeColor = node->parentNode->nodeColor;
                    node->parentNode->nodeColor = BLACK;
                    if (sibling->rightChild != nullptr)
                        sibling->rightChild->nodeColor = BLACK;

                    rotateLeft(node->parentNode);
                    node =  rootNode;
                }
            } else {
                // Symmetric cases when node is the right child of its parent
                TreeNode* sibling = node->parentNode->leftChild;

                // Case 1: Sibling is RED
                if (sibling->nodeColor == RED) {
                    sibling->nodeColor = BLACK;
                    node->parentNode->nodeColor = RED;
                    rotateRight(node->parentNode);
                    sibling = node->parentNode->leftChild;
                }

                // Case 2: Sibling and its children are BLACK
                if ((sibling->leftChild == nullptr || sibling->leftChild->nodeColor == BLACK) &&
                    (sibling->rightChild == nullptr || sibling->rightChild->nodeColor == BLACK)) {
                    sibling->nodeColor = RED;
                    node = node->parentNode;
                } else {
                    // Case 3: Sibling's left child is BLACK, right child is RED
                    if (sibling->leftChild == nullptr || sibling->leftChild->nodeColor == BLACK) {
                        if (sibling->rightChild != nullptr)
                            sibling->rightChild->nodeColor = BLACK;
                        sibling->nodeColor = RED;
                        rotateLeft(sibling);
                        sibling = node->parentNode->leftChild;
                    }

                    // Case 4: Sibling's left child is RED
                    sibling->nodeColor = node->parentNode->nodeColor;
                    node->parentNode->nodeColor = BLACK;
                    if (sibling->leftChild != nullptr)
                        sibling->leftChild->nodeColor = BLACK;

                    rotateRight(node->parentNode);
                    node =  rootNode;
                }
            }
        }

        // Ensure the root is black
        node->nodeColor = BLACK;
    }

    // Find the smallest node in a subtree
    TreeNode* findMinimum(TreeNode* subtreeRoot) {
        while (subtreeRoot->leftChild != nullptr)
            subtreeRoot = subtreeRoot->leftChild;
        return subtreeRoot;
    }

    // Print the tree structure
    void displayTree(TreeNode* subtreeRoot, int indent) {
        constexpr int SPACING = 5;
        if (subtreeRoot == nullptr)
            return;
        indent += SPACING;
        displayTree(subtreeRoot->rightChild, indent);
        std::cout << std::endl;
        for (int i = SPACING; i < indent; i++)
            std::cout << " ";
        std::cout << subtreeRoot->value << "(" << ((subtreeRoot->nodeColor == RED) ? "RED" : "BLACK") << ")" << std::endl;
        displayTree(subtreeRoot->leftChild, indent);
    }

public:
    RBTree() : rootNode(nullptr) {}

    // Insert a value
    void addValue(int val) {
        TreeNode* newNode = new TreeNode(val);
        TreeNode* tempParent = nullptr;
        TreeNode* tempCurrent = rootNode;

        while (tempCurrent != nullptr) {
            tempParent = tempCurrent;
            if (newNode->value < tempCurrent->value)
                tempCurrent = tempCurrent->leftChild;
            else
                tempCurrent = tempCurrent->rightChild;
        }

        newNode->parentNode = tempParent;
        if (tempParent == nullptr)
            rootNode = newNode;
        else if (newNode->value < tempParent->value)
            tempParent->leftChild = newNode;
        else
            tempParent->rightChild = newNode;

        resolveInsert(newNode);
    }

    // Delete a value
    void deleteValue(int val) {
        TreeNode* target = rootNode;
        while (target != nullptr) {
            if (val < target->value)
                target = target->leftChild;
            else if (val > target->value)
                target = target->rightChild;
            else {
                removeNode(target);
                return;
            }
        }
        std::cout << "Value " << val << " not found in the tree." << std::endl;
    }

    // Print the tree structure
    void showTree() {
        displayTree(rootNode, 0);
    }
};

int main() {
    RBTree tree;

    tree.addValue(5);
    tree.addValue(3);
    tree.addValue(7);
    tree.addValue(2);
    tree.addValue(4);
    tree.addValue(6);
    tree.addValue(20);
    tree.addValue(10);
    tree.addValue(30);



    std::cout << "Tree structure:" << std::endl;
    tree.showTree();

    tree.deleteValue(10);
    tree.deleteValue(3);
    tree.deleteValue(7);
    tree.deleteValue(5);
    tree.deleteValue(2);
    tree.deleteValue(30);
    tree.deleteValue(4);

    std::cout << "\nAfter deleting :"<< std::endl;
    tree.showTree();


    return 0;
}
