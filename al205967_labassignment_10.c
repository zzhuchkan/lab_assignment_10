#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    struct Trie* next[26];
    int count;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node != NULL) {
        node->count = 0;
        for (int i = 0; i < 26; i++) {
            node->next[i] = NULL;
        }
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL || *word == '\0') {
        return;
    }

    int index = *word - 'a';
    if (pTrie->next[index] == NULL) {
        pTrie->next[index] = createTrie();
    }

    if (*(word + 1) == '\0') {
        pTrie->next[index]->count++;
    } else {
        insert(pTrie->next[index], word + 1);
    }
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int k;
    int len = strlen(word);

    for(k=0;k<len;k++){
        int nextIndex = word[k] - 'a';

        if (pTrie->next[nextIndex] == NULL)
            return 0;

        pTrie = pTrie->next[nextIndex];
    }
    return pTrie->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL) {
        return NULL;
    }

    for (int i = 0; i < 26; i++) {
        if (pTrie->next[i] != NULL) {
            deallocateTrie(pTrie->next[i]);
        }
    }

    free(pTrie);
    return NULL;
}



// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
  FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int wordCount;
    fscanf(file, "%d", &wordCount);

    char **words = (char **)malloc(wordCount * sizeof(char *));
    if (words == NULL) {
        fclose(file);
        perror("Memory allocation failed");
        return -1;
    }

    for (int i = 0; i < wordCount; i++) {
        char buffer[100];
        fscanf(file, "%s", buffer);
        pInWords[i] = strdup(buffer);
    }

    fclose(file);
    return wordCount;
}

int main(void)
{
	char *inWords[256];

	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}

	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}
