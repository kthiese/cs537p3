// Alec Scheele
// Kathryn Thiese
//
// Header file for graph

// Struct that holds the targets
struct Info {
        int dpsize;
	int cmdsize;
        char* target;
        struct Command* cmds;
        char** dps;
};

// Struct that holds a command
struct Command {
        char* cmd;
        char** args;
};

// Struct for a node in the graph, holding a target and the next node
struct Node {
        struct Info* info;
        struct Node** children;
        struct Node* next;
};

// List of nodes for the graph
struct NodeList {
        int size;
        struct Node *head;
};

struct Node* createNode(struct Info* info);
struct NodeList* createList(int tsize);
void addTarget(struct NodeList* list,struct Info* info);
struct Node* findTarget(struct NodeList* list, char* target);
int containsCycle(struct NodeList* graph);
int isCycle(struct NodeList* graph, struct Node* node, struct NodeList* visited, struct NodeList* rec);
