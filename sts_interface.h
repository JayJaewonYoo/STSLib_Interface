#ifndef STS_INTERFACE_
#define STS_INTERFACE_
/* 
 * Class used for STS interfacing between legal state set and transition function.
*/

#include <fstream>
#include <sstream>
#include <cstring>
#include <string.h>
#include <iostream>
#include <istream>
#include <bdd.h>
#include "sts.h"
#include "event.h"
#include "bdd_synthesis.h"

// Structure declarations:
struct legalStateNode {
	std::string component;
	unsigned long long stateNumber;
};
/*struct legalNode {
	std::string identifier_value;
	std::string identifier_name;
	legalNode *zero;
	legalNode *one;
};

struct identifierNode {
	std::string identifier;
	identifierNode *next;
};

struct prevNode {
	legalNode *access;
	prevNode *prev;
};
struct stateNode {
	unsigned int *array;
	stateNode *next;
};*/

class sts_interface {
	public:
		// legalNode* legalStateTree(void) {return root;};
		sts_interface(sts inputSTS, bdd_synthesis* inputBDDsynthesis) {sts_interface_stateSetCompute(inputSTS, inputBDDsynthesis);}
		//void sts_interface_stateSetCompute(sts agv, bdd reachable);
		void beginUI(void);
		void emptyScreen(void);
		
		~sts_interface() {}
	private:
		// Function declarations:
		void sts_interface_stateSetCompute(sts inputSTS, bdd_synthesis* inputBDDsynthesis);
		void produceLegalBehavior(std::string name, std::string title);
		void getEvents(sts agv);
		void getStates(sts agv);
		std::vector< std::vector<legalStateNode*> > calculateValidStateSets(map<string, event> events);
		void printValidStateSets(void);
		void showBDD(std::string title, bdd inputBDD);
		inline bool checkFileExists(const std::string& file);
		void printBDD_dotty(std::string title, bdd inputBDD);
		void deltaFunc(std::string select);
		void gammaFunc(std::string select);
		void setState(bool initial);
		unsigned int findOccurrenceN(const string& source, size_t position, const string& target, size_t n);
		void undoDelta(std::string select);
		void undoGamma(std::string select);
		void outputControlFunctions(void);
		map<std::string, event> validEventsDelta(bdd targetBDD, std::string type);
		void replaceInString(std::string& source, std::string search, std::string replace);

		// Variables:
		bdd currBDD;
		bdd originalBDD;
		bdd currState;
		std::map<std::string, event> allEvents;
		std::map<std::string, event> controllableEvents;
		std::map<std::string, event> uncontrollableEvents;
		std::vector<std::string> components;
		std::vector<unsigned int> componentSizes; // store number of BDD nodes for each component, ie: componentSizes[i] == 2 means that components[i] will have 2 nodes, components[i]_0 and components[i]_1
		std::vector < std::vector<legalStateNode*> > validStateSets;
		std::map<size_t, bdd> prevDeltaBDD;
		std::map<size_t, bdd> prevDeltaState;
		std::map<size_t, bdd> prevGammaBDD;
		std::map<size_t, bdd> prevGammaState;
		std::vector<std::string> deltaBDD_events;
		std::vector<std::string> deltaState_events;
		std::vector<std::string> gammaBDD_events;
		std::vector<std::string> gammaState_events;
		sts agv;
		bdd_synthesis* syn;
		bool stateSelected;



		/*legalNode* newLegalNode(std::string value, std::string name);
		prevNode* addPrevNode(prevNode *list, legalNode *currNode);
		identifierNode* updateIdentifiers(std::string value);
		stateNode* addStateNode(stateNode *list, unsigned int *input);
		unsigned int searchIdentifiers(std::string value);

		// Values used in sts_interface_compute:
		unsigned int depth;
		unsigned int maxDepth;
		legalNode *root;
		legalNode *mainNode;
		prevNode *prevList;
		identifierNode *storedIdentifiers;*/
};

#endif /* STS_INTERFACE_ */
