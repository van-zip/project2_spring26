#include "types.h"

extern struct NODE *root;
extern struct NODE *cwd;

// make directory
void mkdir(char pathName[]) {
	// TO BE IMPLEMENTED
	//
	// YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW
	if (pathName == NULL || strcmp(pathName, "") == 0 ||
		strcmp(pathName, "/") == 0) {
		printf("MKDIR ERROR: no path provided\n");
		return;
	}

	char baseName[64];
	char dirName[128];
	struct NODE *parent = splitPath(pathName, baseName, dirName);
	if (parent == NULL) {
		return;
	}

	struct NODE *curr = parent->childPtr;
	while (curr != NULL) {
		if (strcmp(curr->name, baseName) == 0 && curr->fileType == 'D') {
			printf("MKDIR ERROR: directory %s already exists\n", pathName);
			return;
		}
		curr = curr->siblingPtr;
	}

	struct NODE *newNode = (struct NODE *)malloc(sizeof(struct NODE));
	strcpy(newNode->name, baseName);
	newNode->fileType = 'D';
	newNode->parentPtr = parent;
	newNode->childPtr = NULL;
	newNode->siblingPtr = NULL;

	if (parent->childPtr == NULL) {
		parent->childPtr = newNode;
	} else {
		struct NODE *temp = parent->childPtr;
		while (temp->siblingPtr != NULL) {
			temp = temp->siblingPtr;
		}
		temp->siblingPtr = newNode;
	}

	printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

// handles tokenizing and absolute/relative pathing options
struct NODE *splitPath(char *pathName, char *baseName, char *dirName) {
	// NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
	// rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
	// SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

	// check for edge case where it's /
	if (strcmp(pathName, "/") == 0) {
		strcpy(dirName, "/");
		strcpy(baseName, "");
		return root;
	}

	char *lastSlash = strrchr(pathName, '/');
	if (lastSlash) {
		int dirLen = lastSlash - pathName;
		if (dirLen == 0) {
			strcpy(dirName, "/");
		} else {
			strncpy(dirName, pathName, dirLen);
			dirName[dirLen] = '\0';
		}
		strcpy(baseName, lastSlash + 1);
	} else {
		strcpy(dirName, "");
		strcpy(baseName, pathName);
	}

	// determine the starting node
	struct NODE *curr;
	if (pathName[0] == '/') {
		curr = root;
	} else {
		curr = cwd;
	}

	char tempDir[128];
	strcpy(tempDir, dirName);
	char *token = strtok(tempDir, "/");

	while (token != NULL) {
		struct NODE *found = curr->childPtr;
		while (found != NULL) {
			if (strcmp(found->name, token) == 0) {
				break;
			}
			found = found->siblingPtr;
		}
		if (found == NULL) {
			printf("ERROR: directory %s does not exist\n", token);
			return NULL;
		}
		curr = found;
		token = strtok(NULL, "/");
	}

	return curr;
}
