import collections
import random as r

class Node:
    # DO NOT MODIFY THIS CLASS #
    __slots__ = 'value', 'parent', 'left', 'right', 'height'

    def __init__(self, value, parent=None, left=None, right=None):
        """
        Initialization of a node
        :param value: value stored at the node
        :param parent: the parent node
        :param left: the left child node
        :param right: the right child node
        """
        self.value = value
        self.parent = parent
        self.left = left
        self.right = right
        self.height = 0

    def __eq__(self, other):
        """
        Determine if the two nodes are equal
        :param other: the node being compared to
        :return: true if the nodes are equal, false otherwise
        """
        if type(self) is not type(other):
            return False
        return self.value == other.value and self.height == other.height

    def __str__(self):
        """String representation of a node by its value"""
        return str(self.value)

    def __repr__(self):
        """String representation of a node by its value"""
        return str(self.value)

class AVLTree:

    def __init__(self):
        # DO NOT MODIFY THIS FUNCTION #
        """
        Initializes an empty Binary Search Tree
        """
        self.root = None
        self.size = 0

    def __eq__(self, other):
        # DO NOT MODIFY THIS FUNCTION #
        """
        Describe equality comparison for BSTs ('==')
        :param other: BST being compared to
        :return: True if equal, False if not equal
        """
        if self.size != other.size:
            return False
        if self.root != other.root:
            return False
        if self.root is None or other.root is None:
            return True  # Both must be None

        if self.root.left is not None and other.root.left is not None:
            r1 = self._compare(self.root.left, other.root.left)
        else:
            r1 = (self.root.left == other.root.left)
        if self.root.right is not None and other.root.right is not None:
            r2 = self._compare(self.root.right, other.root.right)
        else:
            r2 = (self.root.right == other.root.right)

        result = r1 and r2
        return result

    def _compare(self, t1, t2):
        # DO NOT MODIFY THIS FUNCTION #
        """
        Recursively compares two trees, used in __eq__.
        :param t1: root node of first tree
        :param t2: root node of second tree
        :return: True if equal, False if nott
        """
        if t1 is None or t2 is None:
            return t1 == t2
        if t1 != t2:
            return False
        result = self._compare(t1.left, t2.left) and self._compare(t1.right, t2.right)
        return result
            
    ### Implement/Modify the functions below ###

    def insert(self, node, value):
        """
        Recursive insertion of value into tree that is called, and rebalanced upon insertion.
        Does not insert if value already exists.
        :param node: node to check
        :param value: value to insert
        :return: Doesn't return
        """
        # empty tree
        if self.root is None:
            self.root = Node(value)
            self.size = 1
            return

        # move left subtree
        elif value < node.value:
            if node.left:  # recursive check left
                self.insert(node.left, value)
            else:
                node.left = Node(value, parent=node)
                self.size += 1

        # move right subtree
        elif value > node.value:
            if node.right:  # recursive check right
                self.insert(node.right, value)
            else:
                node.right = Node(value, parent=node)
                self.size += 1

        # rebalances at each stack call
        self.update_height(node)
        self.rebalance(node)

    def remove(self, node, value):
        """
        Recursive removal of value from tree that is called, and rebalanced on removal.
        Does not remove if value does not exist.
        :param node: node to check
        :param value: value to remove
        :return: Doesn't return
        """
        # empty tree
        if not node:
            return

        # if value is found
        elif node.value == value:
            # if it is a full node, choose minimum
            if node.left and node.right:
                # copy value of largest node on left branch into node
                succ_node = self.max(node.left)
                node.value = succ_node.value

                # remove copied node from tree
                self.remove(node.left, succ_node.value)

            # if value is a leaf or single root
            elif not node.left and not node.right:
                self.size -= 1
                # if has parent
                if node.parent:
                    if node.parent.left == node:
                        node.parent.left = None
                    else:
                        node.parent.right = None

                # if root
                else:
                    self.root = None

            # if it has exactly one child
            else:
                self.size -= 1
                # find which child to promote
                change_child = node.left
                if node.right:
                    change_child = node.right

                # if has parent
                if node.parent:
                    # change node parent child to promoted node
                    if node.parent.left == node:
                        node.parent.left = change_child
                    else:
                        node.parent.right = change_child

                # if root
                else:
                    self.root = change_child

                # change promoted node parent
                change_child.parent = node.parent

        # recursive search to find node
        elif value < node.value:
            self.remove(node.left, value)

        # recursive search to find node
        elif value > node.value:
            self.remove(node.right, value)

        # rebalances at each call stack
        self.update_height(node)
        self.rebalance(node)

    def search(self, node, value):
        """
        Searches through tree for value, and returns the node if found. If node, returns
        potential parent node
        :param value: value of node to search for
        :param node: node to check
        :return: Returns node with value if found, and potential parent if not
        """
        # checks for non empty tree
        if node:
            # if node has matching value
            if node.value == value:
                return node

            # move to left branch if value < node value
            elif value < node.value:
                if node.left:
                    return self.search(node.left, value)
                return node  # returns potential parent if no left branch

            # move to right branch if value > node value
            else:
                if node.right:
                    return self.search(node.right, value)
                return node  # returns potential parent if no right branch


    def height(self, node):
        """
        Returns the height of node
        :param node: node to return height of
        :return: height of node if it exists, or else -1
        """
        return node.height if node else -1

    def min(self, node):
        """
        Finds the minimum value starting from the node at tree or subtree root
        :param node: node to check
        :return: minimum node if subtree/tree is not empty
        """
        # if tree is not empty
        if node:
            # if minimum is found
            if node.left is None:
                return node

            return self.min(node.left)

    def max(self, node):
        """
        Finds the maximum value starting from the node at tree or subtree root
        :param node: node to check
        :return: maximum node if subtree/tree is not empty
        """
        # if tree is not empty
        if node:
            # if maximum is found
            if node.right is None:
                return node

            return self.max(node.right)

    def get_size(self):
        """
        Returns size of tree
        :return: size of tree
        """
        return self.size

    def get_balance(self, node):
        """
        Finds the balance factor of the node in the parameters (left-right)
        :param node: node to find the balance factor of
        :return: left tree height minus right tree height
        """
        left_height = self.height(node.left)
        right_height = self.height(node.right)

        return left_height - right_height

    def left_rotate(self, root):
        """
        Rotates tree left at root
        :param root: node to rotate at
        :return: node new root of tree
        """
        new_root = root.right
        root_adopt = new_root.left

        # change root parent and new root relation
        if root.parent:
            if root.parent.left == root:
                new_root.parent = root.parent
                new_root.parent.left = new_root
            else:
                new_root.parent = root.parent
                new_root.parent.right = new_root
        else:
            new_root.parent = None
            self.root = new_root

        # change new_root and root relation
        root.parent = new_root
        new_root.left = root

        # change root and root adopt
        root.right = root_adopt
        if root_adopt:
            root_adopt.parent = root

        # update height
        self.update_height(root)
        self.update_height(new_root)
        # self.update_height(new_root.parent)

        return new_root

    def right_rotate(self, root):
        """
        Rotates tree right at root
        :param root: node to rotate at
        :return: node new tree root
        """
        new_root = root.left
        root_adopt = new_root.right

        # change root parent and new root relation
        if root.parent:
            if root.parent.left == root:
                new_root.parent = root.parent
                new_root.parent.left = new_root
            else:
                new_root.parent = root.parent
                new_root.parent.right = new_root
        else:
            new_root.parent = None
            self.root = new_root

        # change new_root and root relation
        root.parent = new_root
        new_root.right = root

        # change root and root adopt
        root.left = root_adopt
        if root_adopt:
            root_adopt.parent = root

        # update height
        self.update_height(root)
        self.update_height(new_root)
        # self.update_height(new_root.parent)

        return new_root

    def rebalance(self, node):
        """
        Finds case of imbalance if imbalanced, and return node of new root
        :param node: node to rebalance at
        :return: node of new root
        """
        # balance factor
        balance_factor = self.get_balance(node)

        # right rotate
        if balance_factor == 2:
            # left right case (changes to left left case)
            if self.get_balance(node.left) == -1:
                self.left_rotate(node.left)
            return self.right_rotate(node)  # left left case

        # left rotate
        if balance_factor == -2:
            # right left case (changes to right right case)
            if self.get_balance(node.right) == 1:
                self.right_rotate(node.right)
            return self.left_rotate(node)  # right right case

        # already balanced
        return node

    def update_height(self, node):
        """
        Updates the height of node
        :param node: node to update height of
        :return: Doesn't return
        """
        # if not empty tree
        if node:
            left_height = self.height(node.left)
            right_height = self.height(node.right)

            # updates height
            node.height = max(left_height, right_height) + 1
