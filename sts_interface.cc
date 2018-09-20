// Created by Jay Jaewon Yoo

#include "sts_interface.h"
#include "bdd_synthesis.h"
#include "automaton.h"
#include "bdd_interface.h"
#include <bdd.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <cctype>

/********************************************************************/
/*// Legal Nodes:
legalNode* sts_interface::newLegalNode(std::string value, std::string name) {
	legalNode *temp = new legalNode();
	temp -> identifier_value = value;
	temp -> identifier_name = name;
	temp -> zero = NULL;
	temp -> one = NULL;
	
	return temp;
}

// Previous Nodes:
prevNode* sts_interface::addPrevNode(prevNode *list, legalNode *currNode) {
	prevNode *temp = new prevNode();
	temp -> prev = list;
	temp -> access = currNode;

	return temp;
}

// Dynamic array of previously visited identifier_values:
identifierNode* sts_interface::updateIdentifiers(std::string value) {
	identifierNode *temp = new identifierNode;
	temp -> identifier = value;
	temp -> next = NULL;
	
	return temp;
}

unsigned int sts_interface::searchIdentifiers(std::string value) {
	identifierNode *temp = new identifierNode();
	int res = 0;
	while(temp != NULL) {
		if(temp -> identifier == value) {
			res = 1;
			break;
		}
		temp = temp -> next;
	}
	
	return res;
}

// Post order depth first traversal to find specific and store it in storage:
void searchNodes(legalNode* node, legalNode* storage, std::string value) {
	unsigned int check = 0;
	if(node == NULL || check) return;

	searchNodes(node -> zero, storage, value);
	searchNodes(node -> one, storage, value);
	if(node	-> identifier_value == value) {
		storage = node;
		check = 1;
	}
}		

// Update list of valid state sets:
stateNode* addStateNode(stateNode *list, unsigned int *input) {	
	stateNode *temp = new stateNode();
	temp -> next = list;
	temp -> array = input;

	return temp;
}
void sts_interface::sts_interface_treeCompute(std::string name) {
	std::ifstream reachableBehavior(name);
	std::ofstream textFile;
	textFile.open("legalBehavior.txt");
	std::string text;

	depth = 0;
	maxDepth = 0;
	root = NULL;
	mainNode = NULL;
	prevList = NULL;
	storedIdentifiers = NULL;

	while(getline(reachableBehavior, text)) {
		if(text.find("-> 0 ") == std::string::npos && text.find("0 [shape=box, label=\"0\", style=filled, shape=box, height=0.3, width=0.3];") == std::string::npos) {
		       textFile << text << "\n";
		       if(text.size() > 1) {
			       std::string endString = text.substr(text.size() - 3, 3);

			       if(root == NULL && endString == "\"];") {
				       root = newLegalNode(text.substr(0, text.find(" ")), text.substr(text.find("\"") + 1, text.find("]") - text.find("\"") - 2));
				       mainNode = root;
				       storedIdentifiers = updateIdentifiers(mainNode -> identifier_value);
			       } else {
				       if(endString == "d];") {
					       std::string identifier = text.substr(text.find("=") + 1, 6);
					       std::string value = text.substr(text.find(">") + 2, text.find("[") - text.find(">") - 3);
					       if(identifier == "dotted") {
						       if(searchIdentifiers(value)) {
							       legalNode *tempNode;
							       searchNodes(root, tempNode, value);
							       mainNode -> zero = tempNode;
						       } else {
							       mainNode -> zero = newLegalNode(value, ""); 
							       storedIdentifiers = updateIdentifiers(mainNode -> zero -> identifier_value);
						       }
					       } else if(identifier == "filled") {
						       if(searchIdentifiers(value)) {
							       legalNode *tempNode;
							       searchNodes(root, tempNode, value);
							       mainNode -> one = tempNode;
						       } else {
							       mainNode -> one = newLegalNode(value, "");
							       storedIdentifiers = updateIdentifiers(mainNode -> one -> identifier_value);
						       }
					       }
				       } else if(endString == "\"];") {
					       std::string currNodeValue = text.substr(0, text.find("[") - 1);


					       if(std::stoi(mainNode -> identifier_value) > std::stoi(currNodeValue)) {
						      prevList = addPrevNode(prevList, mainNode);
						      if(mainNode -> zero -> identifier_value == currNodeValue) {
							      mainNode = mainNode -> zero;
							      depth++;
							      if(depth > maxDepth) maxDepth = depth;
						      } else if(mainNode -> one -> identifier_value == currNodeValue) {
							      mainNode = mainNode -> one;
							      depth++;
							      if(depth > maxDepth) maxDepth = depth;
						      } else {
							      std::cout << "ERROR, PROCESS ABORTED!" << std::endl;
							      break;
						      }
						      if(mainNode -> identifier_name == "") {
							      mainNode -> identifier_name = text.substr(text.find("\"") + 1, text.find("]") - text.find("\"") - 2);
						      }
					       } else {
						       while(prevList -> prev != NULL) {
							       mainNode = prevList -> access;
							       depth--;

							       // Deleting a start node of prevList:
							       prevNode *temp = prevList;
							       prevList = prevList -> prev;
							       free(temp);
							       
							       if(mainNode -> zero != NULL) {
								       if(mainNode -> zero -> identifier_value == currNodeValue) {
									       prevList = addPrevNode(prevList, mainNode);
									       mainNode = mainNode -> zero;
									       depth++;
									       if(depth > maxDepth) maxDepth = depth;
									       break;
								       }
							       } if(mainNode -> one != NULL) {
								       if(mainNode -> one -> identifier_value == currNodeValue) {
									       prevList = addPrevNode(prevList, mainNode);
									       mainNode = mainNode -> one;
									       depth++;
									       if(depth > maxDepth) maxDepth = depth;
									       break;
								       }
							       }
						       }
						       if(mainNode -> identifier_name == "") {
							       mainNode -> identifier_name = text.substr(text.find("\"") + 1, text.find("]") - text.find("\"") - 2);
						       }
					       }
				       }	       
			       }
		       }
		}
	}
	// depth = 0;
	textFile.close();
}*/
/********************************************************************/
// Create legal state set:
void sts_interface::produceLegalBehavior(std::string name, std::string title) {
	std::ifstream reachableBehavior(name);
	std::ofstream textFile;

	if(title == "legalBehavior_current") {
		textFile.open("legalBehavior_current");
	} else if(title == "legalBehavior_original") {
		textFile.open("legalBehavior_original");
	}
	std::string text;

	while(std::getline(reachableBehavior, text)) {
		if(text.find("-> 0 ") == std::string::npos && text.find("0 [shape=box, label=\"0\", style=filled, shape=box, height=0.3, width=0.3];") == std::string::npos) textFile << text << "\n";
	}
	textFile.close();
}

/********************************************************************/
void sts_interface::getEvents(sts agv) {
	// All Events:
	set<event> eventSet = agv.Sigma();
	ostringstream stream;
	map<string, event> eventList;
	for(set<event>::iterator i = eventSet.begin(); i != eventSet.end(); ++i) {
		event e = *i;
		stream << e;
		eventList.insert(pair <string, event> (stream.str(), e));
		stream.str("");
	}
	// Testing:
	//for(map<string, event>::iterator i = allEvents.begin(); i != allEvents.end(); i++) std::cout << i->first << ":" << i->second << std::endl;

	allEvents = eventList;
	
	// Controllable Events:
	eventSet = agv.Sigma_c();
	eventList.clear();
	for(set<event>::iterator i = eventSet.begin(); i != eventSet.end(); ++i) {
		event e = *i;
		stream << e;
		eventList.insert(pair <string, event> (stream.str(), e));
		stream.str("");
	}

	controllableEvents = eventList;

	// Uncontrollable Events:
	eventSet = agv.Sigma_uc();
	eventList.clear();
	for(set<event>::iterator i = eventSet.begin(); i != eventSet.end(); ++i) {
		event e = *i;
		stream << e;
		eventList.insert(pair <string, event> (stream.str(), e));
		stream.str("");
	}

	uncontrollableEvents = eventList;
}

/********************************************************************/
void sts_interface::getStates(sts agv) {
	std::vector<string> componentsList;
	ostringstream stream;
	map<state, holon> holonMap = agv.H();
	for(map<state, holon>::iterator i = holonMap.begin(); i != holonMap.end(); ++i) {
		state temp = i -> first;
		stream << temp;
		componentsList.push_back(stream.str());
		stream.str("");
	}

	// Testing:
	//for(std::vector<string>::iterator i = components.begin(); i != components.end(); ++i) std::cout << *i << std::endl;
	
	components = componentsList;
}

/********************************************************************/
std::vector< std::vector<legalStateNode*> > sts_interface::calculateValidStateSets(map<string, event> events) {
	unsigned int numComponents = components.size();
	
	std::ifstream stateSetTemp("tempOutput_toBeDeleted");
	std::string raw;
	std::vector< std::vector<legalStateNode*> > validStateSets;
	std::vector<legalStateNode*> tempVector;

	std::getline(stateSetTemp, raw);
	std::string temp;
	std::string currComponent;
	std::string tempNum;

	while(raw.find("<") != temp.npos) {
		temp = raw.substr(1, raw.find(">") - 1);
		tempVector.clear();
		for(int i = 0; i < numComponents; ++i) {
			currComponent = components[i];
			tempNum = "";
			while(temp.find(currComponent) != temp.npos) {
				tempNum = temp.substr(temp.find(":") + 1, temp.find(",") - temp.find(":") - 1) + tempNum;
				if(temp.find(" ") != temp.npos) temp.erase(0, temp.find(" ") + 1);
				else break;
			}
			legalStateNode* tempNode = new legalStateNode();
			tempNode -> component = currComponent;
			unsigned long long binaryTempNum = 0;
			for(int j = tempNum.length() - 1; j >= 0; --j) if(tempNum.at(j) == '1') binaryTempNum += pow(2, tempNum.length() - j - 1);
			
			tempNode -> stateNumber = binaryTempNum;
			tempVector.push_back(tempNode);
		}
		validStateSets.push_back(tempVector);
		raw.erase(0, raw.find(">") + 1);
	}
	stateSetTemp.close();

	std::remove("tempOutput_toBeDeleted");

	return validStateSets;
	
	// Testing output:
	/*for(int i = 0; i < validStateSets.size(); ++i) {
		cout << "Solution " << i + 1 << endl;
		for(int j = 0; j < validStateSets[0].size(); ++j) {
			cout << validStateSets[i][j] -> component << " at state " << validStateSets[i][j] -> stateNumber << endl;
		}
		cout << endl;
	}*/
}

/********************************************************************/
void sts_interface::sts_interface_stateSetCompute(sts inputSTS, bdd_synthesis* inputBDDsynthesis) {

	// Determine OS:
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// is Windows
	isLinux = false;
	#else
	// is Linux
	isLinux = true;
	#endif

	stateSelected = false;
	agv = inputSTS;
	syn = inputBDDsynthesis;
	bdd c = syn -> Pcon();
	originalBDD = syn -> R(c);
	FILE *ofile = fopen("tempOutput_toBeDeleted", "w");
	if(ofile == NULL) {
		std::cout << "ERROR WITH COMPUTING STATE SET" << std::endl;
		return;
	}
	bdd_fprintset(ofile, originalBDD);
	fclose(ofile);

	std::ifstream inputFile("tempOutput_toBeDeleted");
	std::string tempText1;
	std::string tempText2;
	while(std::getline(inputFile, tempText1)) {
		tempText2 = tempText1.substr(1, tempText1.find(">") - 1);
	}	
	
	getEvents(agv);	
	getStates(agv);


	for(int i = 0; i < components.size(); i++) {
		unsigned int maxNum = 1;
		while(1) {
			if(tempText2.find(components[i]) == std::string::npos) {
				break;
			} else {
				int currNum = std::stoi(tempText2.substr(tempText2.find(components[i]) + components[i].size() + 1, tempText2.find(":", tempText2.find(components[i])) -(tempText2.find(components[i]) + components[i].size() + 1)).c_str()) + 1;
				if(currNum > maxNum) maxNum = currNum;
				tempText2.erase(tempText2.find(components[i]), tempText2.find(":", tempText2.find(components[i])));
			}
		}
		componentSizes.push_back(maxNum);
	}

	validStateSets = calculateValidStateSets(allEvents);
	currBDD = syn -> R(c);
	
	std::remove("tempOutput_toBeDeleted");
	
	return;
}
/********************************************************************/
void sts_interface::printValidStateSets() {
	for(int i = 0; i < validStateSets.size(); ++i) {
		std::cout << "State " << i + 1 << std::endl;
		for(int j = 0; j < validStateSets[0].size(); ++j) {
			std::cout << validStateSets[i][j] -> component << " at State " << validStateSets[i][j] -> stateNumber << std::endl;
		}
		std::cout << std::endl;
	}
}
/********************************************************************/
void sts_interface::emptyScreen() {
	for(int i = 0; i < 100; i++) cout << endl;
}
/********************************************************************/
void sts_interface::showBDD(std::string title, bdd inputBDD) {
	printBDD_dotty(title, inputBDD);
	if(isLinux) {
		system(("dotty " + title).c_str());
	} else {
		system(("dotty " + title).c_str());
	}
}
/********************************************************************/
void sts_interface::printBDD_dotty(std::string title, bdd inputBDD) {
	char* newTitle = new char[title.length() + 1];
	strcpy(newTitle, title.c_str());
	bdd_fnprintdot(newTitle, inputBDD);
	delete [] newTitle;
}
/********************************************************************/
inline bool sts_interface::checkFileExists(const std::string& file) {
	struct stat buffer;
	return (stat (file.c_str(), &buffer) == 0);
}
/********************************************************************/
void sts_interface::deltaFunc(std::string select) {
	event inputEvent;
	bdd newBDD;
	bool cancelled = false;
	std::string selectedEvent = "";
	std::cout << "Select event to apply to DES. Type \"--cancel\" to cancel at anytime. Type \"--help\" to repeat these instructions." << std::endl;	


	map<std::string, event> validEvents;
	if(select == "DES") {
		validEvents = validEventsDelta(currBDD, "BDD");
	} else if(select == "state") {
		if(stateSelected) {
			validEvents = validEventsDelta(currState, "state");
		} else {
			std::cout << "\nState not selected. Returning to main menu. Select a state using the --set-state function.";
			cancelled = true;
		}
	}

	if(!cancelled) {	
		std::cout << "\nAvailable options:" << std::endl;
		for(map<string, event>::iterator i = validEvents.begin(); i != validEvents.end(); i++) std::cout << i -> first << std::endl;
		std::cout << std::endl;
	}

	while(1 && !cancelled) {
		std::getline(std::cin, selectedEvent);
		if(selectedEvent == "--cancel") {
			cancelled = true;
			std::cout << "\n--delta function cancelled. Returning to main menu." << std::endl;
			break;
		}
		else if(selectedEvent == "--help") {
			std::cout << "Select event to apply to DES. Type \"--cancel\" to cancel at anytime. Type \"--help\" to repeat these instructions. \n\nAvailable options:" << std::endl;		
			for(map<string, event>::iterator i = validEvents.begin(); i != validEvents.end(); i++) std::cout << i -> first << std::endl;
			std::cout << std::endl;
		} else if(validEvents.find(selectedEvent) == validEvents.end()) {
			std::cout << "\nEvent not found. Try again:\n" << std::endl;
		} else if(validEvents.find(selectedEvent) != validEvents.end()) {
			inputEvent = validEvents.find(selectedEvent) -> second;
			break;
		} else {	
			std::cout << "\nInvalid Input. Enter \"--help\" for options. Try again:\n" << std::endl;
		}	
	}
	if(!cancelled) {
		std::cout << "Selected Event is: " + selectedEvent + ".\n" << std::endl;
		std::cout << "Apply delta..." << std::endl;
		bool valid = true;
	
		if(select == "DES") {	
			newBDD = syn -> Delta(currBDD, inputEvent);
			printBDD_dotty("tempOutput_toBeDeleted", newBDD);
			int count = 0;
			std::string text;
			std::ifstream validCheck("tempOutput_toBeDeleted");
			while(std::getline(validCheck, text)) count++;
			if(count <= 4) {
				valid = false;
				std::cout << "Illegal event for current DES. Returning to main menu." << std::endl;
			}
			std::remove("tempOutput_toBeDeleted");
			if(valid) {
				prevDeltaBDD.insert(pair <size_t, bdd> (prevDeltaBDD.size() + 1, currBDD));
				deltaBDD_events.push_back(selectedEvent);
				currBDD = newBDD;

				std::cout << "Complete. Delta has been applied to current BDD. Returning to main menu." << std::endl;
			}
		} else if(select == "state") {
			newBDD = syn -> Delta(currState, inputEvent);
			printBDD_dotty("tempOutput_toBeDeleted", newBDD);
			int count = 0;
			std::string text;
			std::ifstream validCheck("tempOutput_toBeDeleted");
			while(std::getline(validCheck, text)) count++;
			if(count <= 4) {
				valid = false;
				std::cout << "Illegal event for current state. Returning to main menu." << std::endl;
			}
			std::remove("tempOutput_toBeDeleted");
			if(valid) {
				prevDeltaState.insert(pair <size_t, bdd> (prevDeltaState.size() + 1, currState));
				deltaState_events.push_back(selectedEvent);
				currState = newBDD;

				std::cout << "Complete. Delta has been applied to current state. Returning to main menu." << std::endl;
			}
		}

	}
}
/********************************************************************/
void sts_interface::gammaFunc(std::string select) {
	event inputEvent;
	bdd newBDD;
	bool cancelled = false;
	std::string selectedEvent = "";

	if(select == "state" && !stateSelected) {
		cancelled = true;
		std::cout << "\nState not selected. Returning to main menu. Select a state using the --set-state function." << std::endl;
	} else {
		std::cout << "Select event to apply to DES. Type \"--cancel\" to cancel at anytime. Type \"--help\" to repeat these instructions. \n\nAvailable options:" << std::endl;		
		for(map<string, event>::iterator i = allEvents.begin(); i != allEvents.end(); i++) std::cout << i -> first << std::endl;
		std::cout << std::endl;
	}

	while(1 && !cancelled) {
		std::getline(std::cin, selectedEvent);
		if(selectedEvent == "--cancel") {
			cancelled = true;
			std::cout << "\n--gamma function cancelled. Returning to main menu." << std::endl;
			break;
		}
		else if(selectedEvent == "--help") {
			std::cout << "Select event to apply to DES. Type \"--cancel\" to cancel at anytime. Type \"--help\" to repeat these instructions. \n\nAvailable options:" << std::endl;		
			for(map<string, event>::iterator i = allEvents.begin(); i != allEvents.end(); i++) std::cout << i -> first << std::endl;
			std::cout << std::endl;
		} else if(allEvents.find(selectedEvent) == allEvents.end()) {
			std::cout << "\nEvent not found. Try again:\n" << std::endl;
		} else if(allEvents.find(selectedEvent) != allEvents.end()) {
			inputEvent = allEvents.find(selectedEvent) -> second;
			break;
		} else {	
			std::cout << "\nInvalid Input. Enter \"--help\" for options. Try again:\n" << std::endl;
		}	
	}
	if(!cancelled) {
		std::cout << "Selected Event is: " + selectedEvent + ".\n" << std::endl;
		std::cout << "Apply gamma..." << std::endl;
		bool valid = true;
	
		if(select == "DES") {	
			newBDD = syn -> Gamma(currBDD, inputEvent);
			printBDD_dotty("tempOutput_toBeDeleted", newBDD);
			int count = 0;
			std::string text;
			std::ifstream validCheck("tempOutput_toBeDeleted");
			while(std::getline(validCheck, text)) count++;
			if(count <= 4) {
				valid = false;
				std::cout << "Illegal event for current DES. Returning to main menu." << std::endl;
			}
			std::remove("tempOutput_toBeDeleted");
			if(valid) {
				prevGammaBDD.insert(pair <size_t, bdd> (prevGammaBDD.size() + 1, currBDD));
				gammaBDD_events.push_back(selectedEvent);
				currBDD = newBDD;

				std::cout << "Complete. Gamma has been applied to current BDD. Returning to main menu." << std::endl;
			}
		} else if(select == "state") {
			newBDD = syn -> Gamma(currState, inputEvent);
			printBDD_dotty("tempOutput_toBeDeleted", newBDD);
			int count = 0;
			std::string text;
			std::ifstream validCheck("tempOutput_toBeDeleted");
			while(std::getline(validCheck, text)) count++;
			if(count <= 4) {
				valid = false;
				std::cout << "Illegal event for current state. Returning to main menu." << std::endl;
			}
			std::remove("tempOutput_toBeDeleted");
			if(valid) {
				prevGammaState.insert(pair <size_t, bdd> (prevGammaState.size() + 1, currState));
				gammaState_events.push_back(selectedEvent);
				currState = newBDD;

				std::cout << "Complete. Gamma has been applied to current state. Returning to main menu." << std::endl;
			}
		}
	}
}
/********************************************************************/
unsigned int sts_interface::findOccurrenceN(const string& source, size_t position, const string& target, size_t n) {
	unsigned int foundIndex = source.find(target, position);
	if(0 == n || std::string::npos == foundIndex) {
		return foundIndex;
	}
	return findOccurrenceN(source, foundIndex + 1, target, n - 1);
}
/********************************************************************/
void sts_interface::setState(bool initial) {
	std::string selectedState = "";
	bool cancelled = false;
	std::vector<legalStateNode*> currStateSet;
	std::string path = "";

	if(!initial) {
		std::cout << "Select state to set. To see available states, return to the main menu and select \"--print-states-all-original\". Type \"--cancel\" to cancel at anytime.  Type \"--help\" to repeat these instructions. \n\nAvailable options:" << std::endl;
		for(int i = 0; i < validStateSets.size(); ++i) std::cout << i + 1 << std::endl;
		std::cout << std::endl;

		while(1 && !cancelled) {
			std::getline(std::cin, selectedState);
			if(selectedState == "--cancel") {
				cancelled = true;
				std::cout << "\n--set-state function cancelled. Returning to main menu." << std::endl;
				break;
			}
			else if(selectedState == "--help") {
				std::cout << "Select state to set. To see available states, return to the main menu and select \"--print-states-all-original\". Type \"--cancel\" to cancel at anytime.  Type \"--help\" to repeat these instructions. \n\nAvailable options:" << std::endl;		
				for(int i = 0; i < validStateSets.size(); ++i) std::cout << i + 1 << std::endl;
				std::cout << std::endl;
			} else {
				if(selectedState == "0") {
					std::cout << "\nState not found. Try again:\n" << std::endl;
				} else {
					std::string::const_iterator iterator = selectedState.begin();
					while(iterator != selectedState.end() && std::isdigit(*iterator)) ++iterator;
					if(!selectedState.empty() && iterator == selectedState.end()) {
						if(std::stoi(selectedState) > validStateSets.size()) {
							std::cout << "\nState not found. Try again:\n" << std::endl;
						} else {
							currStateSet = validStateSets[std::stoi(selectedState) - 1];
							break;
						}
					} else {
						std::cout << "\nState not found. Try again:\n" << std::endl;
					}
				}
			}
		}
	} else {
		// Old:
		//currStateSet = validStateSets[0];
		
		// New:
		subST tempSTo = agv.STo();
		map<state, holon> holonMapTemp = agv.H();
		state currState;
		
		/*
		for(map<state, holon>::iterator i = holonMapTemp.begin(); i != holonMapTemp.end(); ++i) {
			currState = i -> first;
			bdd_interface bdd_interface_object(agv);
			bdd thetaRes = bdd_interface_object.Theta(tempSTo, currState);
			std::cout << currState << ": ";
			bdd_printset(thetaRes);
			std::cout << std::endl;
		}*/
		

		// Reference:
		/*
		for(int i = 0; i < validStateSets.size(); ++i) {
			std::cout << "State " << i + 1 << std::endl;
			for(int j = 0; j < validStateSets[0].size(); ++j) {
				std::cout << validStateSets[i][j] -> component << " at State " << validStateSets[i][j] -> stateNumber << std::endl;
			}
			std::cout << std::endl;
		}
		*/

		bdd_interface bdd_interface_object(agv);
		unsigned long long found = 0;

		for(int i = 0; i < validStateSets.size(); ++i) {
			int j =0;
			for(map<state, holon>::iterator it = holonMapTemp.begin(); it != holonMapTemp.end(); ++it) {
				currState = it -> first;
				bdd thetaRes = bdd_interface_object.Theta(tempSTo, currState);

				FILE *ofile = fopen("tempOutput_toBeDeleted", "w");
				bdd_fprintset(ofile, thetaRes);
				fclose(ofile);

				std::string text;
				std::ifstream inputFile("tempOutput_toBeDeleted");
				unsigned long long value = 0;
				while(std::getline(inputFile, text)) {
					int count = 0;
					while(text.find(":") != std::string::npos) {
						std::string tempText = text.substr(text.find(":") + 1, 1);
						replaceInString(text, text.substr(text.find(":"), 2), "");
						value += std::stoi(tempText) * pow(2, count);
						count++;
					}
				}
				std::remove("tempOutput_toBeDeleted");

				if(validStateSets[i][j] -> stateNumber == value) {
					found++;
					//break;
				}
				j++;
			}
			if(found == validStateSets[0].size()) {
				currStateSet = validStateSets[i];
				break;
			}
		}
	}
	if(!cancelled) {
		for(int i = 0; i < validStateSets[0].size(); ++i) {
			unsigned long long currNumber = currStateSet[i] -> stateNumber;
			std::string tempString;
			if(currNumber != 0) {
				while(currNumber != 0) {
					tempString = (currNumber % 2 == 0 ? "0":"1") + tempString;
					currNumber /= 2;
				}
			} else {
				tempString = "0";
			}
			int zeroes_to_add = componentSizes[i] - tempString.size();
			for(int j = 0; j < zeroes_to_add; j++) {
				tempString = "0" + tempString;
			}
			path = tempString + path;
			// Testing:
			//std::cout << currStateSet[i] -> component << " at State " << currStateSet[i] -> stateNumber << std::endl;
		}

		std::reverse(path.begin(), path.end());

		FILE* pfile;
		pfile = fopen("tempOutput_toBeDeleted", "w");
		std::string title = "tempOutput_toBeDeleted";
		char* newTitle = new char[title.length() + 1];
		strcpy(newTitle, title.c_str());
		bdd_fnsave(newTitle, originalBDD);
		fclose(pfile);
		std::vector<std::string> bdd_lines;
		std::vector<std::string> bdd_lines_backup;
		std::string text;
		std::ifstream inputFile("tempOutput_toBeDeleted");
		while(std::getline(inputFile, text)) bdd_lines.push_back(text);
		bdd_lines.erase(bdd_lines.begin());
		bdd_lines.erase(bdd_lines.begin());
		bdd_lines_backup = bdd_lines;
		std::reverse(bdd_lines.begin(), bdd_lines.end());

		unsigned long long size = bdd_lines.size();
		unsigned long long j = 0;
		std::string tempString2;
		bool first = true;
		std::string currNumber;
		for(unsigned long long i = 0; i < size; i++) {
			tempString2 = bdd_lines.at(i);
			if(first) {
				if(path.at(j) == '0') {		
					currNumber = tempString2.substr(findOccurrenceN(tempString2, 0, " ", 1) + 1, findOccurrenceN(tempString2, 0, " ", 2) - findOccurrenceN(tempString2, 0, " ", 1) - 1);
					tempString2.replace(findOccurrenceN(tempString2, 0, " ", 2) + 1, findOccurrenceN(tempString2, 0, " ", 3) - findOccurrenceN(tempString2, 0, " ", 2) - 1, "0");
				} else if(path.at(j) == '1') {
					currNumber = tempString2.substr(findOccurrenceN(tempString2, 0, " ", 2) + 1, findOccurrenceN(tempString2, 0, " ", 3) - findOccurrenceN(tempString2, 0, " ", 2) - 1);
					tempString2.replace(findOccurrenceN(tempString2, 0, " ", 1) + 1, findOccurrenceN(tempString2, 0, " ", 2) - findOccurrenceN(tempString2, 0, " ", 1) - 1, "0");
				}
				first = false;
				j++;
			} else {
				if(tempString2.substr(0, tempString2.find(" ")) == currNumber) {
					if(path.at(j) == '0') {
						currNumber = tempString2.substr(findOccurrenceN(tempString2, 0, " ", 1) + 1, findOccurrenceN(tempString2, 0, " ", 2) - findOccurrenceN(tempString2, 0, " ", 1) - 1);
						tempString2.replace(findOccurrenceN(tempString2, 0, " ", 2) + 1, findOccurrenceN(tempString2, 0, " ", 3) - findOccurrenceN(tempString2, 0, " ", 2) - 1, "0");
					} else if(path.at(j) == '1') {
						currNumber = tempString2.substr(findOccurrenceN(tempString2, 0, " ", 2) + 1, findOccurrenceN(tempString2, 0, " ", 3) - findOccurrenceN(tempString2, 0, " ", 2) - 1);
						tempString2.replace(findOccurrenceN(tempString2, 0, " ", 1) + 1, findOccurrenceN(tempString2, 0, " ", 2) - findOccurrenceN(tempString2, 0, " ", 1) - 1, "0");
					}
					j++;
				}
			}
			bdd_lines.at(i) = tempString2;
		}
		std::reverse(bdd_lines.begin(), bdd_lines.end());

		for(unsigned long long i = 0; i < bdd_lines.size(); i++) {
			if(bdd_lines.at(i) != bdd_lines_backup.at(i)) {
				if(isLinux) {
					system(("sed -i \'s/" + bdd_lines_backup.at(i) + "/" + bdd_lines.at(i) + "/g\' tempOutput_toBeDeleted").c_str());
				} else {
					system(("sed -i \'s/" + bdd_lines_backup.at(i) + "/" + bdd_lines.at(i) + "/g\' tempOutput_toBeDeleted").c_str());
				}
			}
		}
		
		bdd_fnload(newTitle, currState);

		std::remove("tempOutput_toBeDeleted");
		stateSelected = true;

		if(!initial) {
			std::cout << "Complete. State " << std::stoi(selectedState) << " has been set as current state. Returning to main menu." << std::endl;
		} else {
			std::cout << "\nComplete. Returning to main menu." << std::endl;
		}
	}
}
/********************************************************************/
void sts_interface::undoDelta(std::string select) {
	if(select == "BDD") {
		if(prevDeltaBDD.size() == 0) {
			std::cout << "No previous deltas found. Returning to main menu." << std::endl;
		} else {
			std::map<size_t, bdd>::const_iterator i = prevDeltaBDD.find(prevDeltaBDD.size());	
			currBDD = i -> second;
			prevDeltaBDD.erase(i);
			deltaBDD_events.pop_back();
			std::cout << "Undo complete." << std::endl;
		}
	} else if(select == "state") {
		if(prevDeltaState.size() == 0) {
			std::cout << "No previous deltas found. Returning to main menu." << std::endl;
		} else {
			std::map<size_t, bdd>::const_iterator i = prevDeltaState.find(prevDeltaState.size());	
			currState = i -> second;
			prevDeltaState.erase(i);
			deltaState_events.pop_back();
			std::cout << "Undo complete." << std::endl;
		}
	}
}
/********************************************************************/
void sts_interface::undoGamma(std::string select) {
	if(select == "BDD") {
		if(prevGammaBDD.size() == 0) {
			std::cout << "No previous gammas found. Returning to main menu." << std::endl;
		} else {
			std::map<size_t, bdd>::const_iterator i = prevGammaBDD.find(prevGammaBDD.size());	
			currBDD = i -> second;
			prevGammaBDD.erase(i);
			gammaBDD_events.pop_back();
			std::cout << "Undo complete." << std::endl;
		}
	} else if(select == "state") {
		if(prevGammaState.size() == 0) {
			std::cout << "No previous deltas found. Returning to main menu." << std::endl;
		} else {
			std::map<size_t, bdd>::const_iterator i = prevGammaState.find(prevGammaState.size());	
			currState = i -> second;
			prevGammaState.erase(i);
			gammaState_events.pop_back();
			std::cout << "Undo complete." << std::endl;
		}
	}
}
/********************************************************************/
void sts_interface::outputControlFunctions() {
	for(set<event>::const_iterator et = syn -> Sigma_c().begin(); et != syn -> Sigma_c().end(); et++) {
		std::string fn(et -> label());
		fn += ".elig";
		char* f = new char[fn.size() + 1];
		strcpy(f, fn.c_str());
		bdd_fnprintdot(f, syn -> Elig(*et));
	}
}
/********************************************************************/
map<std::string, event> sts_interface::validEventsDelta(bdd targetBDD, std::string type) {
	bool valid;
	map<std::string, event> res = allEvents;
	for(map<std::string, event>::iterator it = allEvents.begin(); it != allEvents.end(); it++) {
		bdd newBDD = syn -> Delta(targetBDD, it -> second);
		printBDD_dotty("tempOutput_toBeDeleted", newBDD);
		int count = 0;
		std::string text;
		std::ifstream validCheck("tempOutput_toBeDeleted");
		while(std::getline(validCheck, text)) count ++;

		if(count <= 4) {
			res.erase(it -> first);
			std::remove("tempOutput_toBeDeleted");
		} else {
			std::remove("tempOutput_toBeDeleted");
	
			for(set<event>::const_iterator et = syn -> Sigma_c().begin(); et != syn -> Sigma_c().end(); et++) {// auto& elem : Sigma-c
				std::string fn(et -> label());
				if(fn == it -> first) {
					valid = false; // Valid if for at least one of the lines in sourceText, all the elements of one of the parts of controlText appear.
					
					// New:
					/*bdd tempBDD = bdd_and(targetBDD, syn -> Elig(*et));

					string name1(et->label()); // Remove later.
					name1 += ".elig"; // Remove later.
					char* name2 = new char[name1.size() + 1]; // Remove later.
					strcpy(name2, name1.c_str()); // Remove later.
					bdd_fnprintdot(name2, tempBDD); // Remove later. 
					
					if(tempBDD == targetBDD) valid = true;*/

					// Old: 
					// Check if bddprintset of control function appears in targetBDDprintset
					FILE *ofile = fopen("tempControlFunction_toBeDeleted", "w");
					bdd_fprintset(ofile, syn -> Elig(*et));
					fclose(ofile);
					FILE *ofile2 = fopen("tempSourceBDD_toBeDeleted", "w");
					bdd_fprintset(ofile2, targetBDD);
					fclose(ofile2);

					std::ifstream controlInput;
					controlInput.open("tempControlFunction_toBeDeleted");
					std::string text;
					std::ofstream ofileControl("tempControlFunction_toBeDeleted_updated");
					std::getline(controlInput, text);
					replaceInString(text, ">", "\n");
					replaceInString(text, "<", "");
					ofileControl << text << std::endl;
					ofileControl.close();
					std::remove("tempControlFunction_toBeDeleted");
					
					std::ifstream sourceInput;
					sourceInput.open("tempSourceBDD_toBeDeleted");
					std::ofstream ofileSource("tempSourceBDD_toBeDeleted_updated");
					std::getline(sourceInput, text);
					replaceInString(text, ">", "\n");
					replaceInString(text, "<", "");
					ofileSource << text << std::endl;
					ofileSource.close();
					std::remove("tempSourceBDD_toBeDeleted");

					std::string sourceText;
					std::string controlText;
					std::ifstream ifileSource;
					std::ifstream ifileControl;
					ifileSource.open("tempSourceBDD_toBeDeleted_updated");
					ifileControl.open("tempControlFunction_toBeDeleted_updated");
					while(std::getline(ifileSource, sourceText)) {
						while(std::getline(ifileControl, controlText)) {
							int numComponents = std::count(controlText.begin(), controlText.end(), ',') + 1;
							for(int i = 0; i < numComponents; i++) {
								std::string currCheck;
								if(i == numComponents - 1) {
									currCheck = controlText.substr(0, controlText.size());
									if(sourceText.find(currCheck) == std::string::npos) {
										break;
									} else {
										valid = true;
									}
								} else {
									currCheck = controlText.substr(0, controlText.find(","));
									if(sourceText.find(currCheck) == std::string::npos) {
										// Not found.
										break;
									} else {
										replaceInString(controlText, currCheck + ", ", "");
									}
								}
							}
						}
						break;
					}
					std::remove("tempSourceBDD_toBeDeleted_updated");
					std::remove("tempControlFunction_toBeDeleted_updated");
					
					if(!valid) {
						res.erase(it -> first);
					} else {
						if(type == "state") {
							// Check if state is in list of valid state sets here. 
							valid = false;
							
							FILE *ofile = fopen("tempOutput_toBeDeleted", "w");
							bdd_fprintset(ofile, newBDD);
							fclose(ofile);

							std::string stateText;
							std::ifstream inputFile("tempOutput_toBeDeleted");

							std::getline(inputFile, stateText);
							std::remove("tempOutput_toBeDeleted");
							unsigned long long count;
							unsigned long long tempValue;

							for(int i = 0; i < validStateSets.size(); ++i) {
								std::string tempStateText = stateText;
								for(int j = 0; j < validStateSets[0].size(); ++j) {
									count = 0;
									tempValue = 0;
									while(tempStateText.find(validStateSets[i][j] -> component) != std::string::npos) {
										std::string currentSubstring = tempStateText.substr(tempStateText.find(validStateSets[i][j] -> component), tempStateText.find(validStateSets[i][j] -> component) + validStateSets[i][j] -> component.length() + 1);
										std::string currentSubstring2 = currentSubstring;
										while((currentSubstring.substr(currentSubstring.length() - 1, 1) != "0") && (currentSubstring.substr(currentSubstring.length() - 1, 1) != "1")) {
											currentSubstring = currentSubstring.substr(0, currentSubstring.length() - 1);
										}

										if(currentSubstring.substr(currentSubstring.length() - 1, 1) == "1") tempValue += pow(2, count);

										replaceInString(tempStateText, currentSubstring2, "");
										count++;
									}
									// Testing:
									//cout << "Event " << fn << ", Solution " << i + 1 << ", Component " << validStateSets[i][j] -> component << ": " << tempValue << " compared to " << validStateSets[i][j] -> stateNumber << endl;// Remove later.

									if(tempValue != validStateSets[i][j] -> stateNumber) break;

									if(j == validStateSets[0].size() - 1) valid = true;
								}
								if(valid) break;
							}
							if(!valid) res.erase(it -> first);
						}
					}

				}
			}
		}
	}

	return res;
}
/********************************************************************/
void sts_interface::replaceInString(std::string& source, std::string search, std::string replace) {
	size_t pos = source.find(search);
	while(pos != std::string::npos) {
		source.replace(pos, search.size(), replace);
		pos = source.find(search, pos + search.size());
	}
}
/********************************************************************/
void sts_interface::beginUI() {
	emptyScreen();
	if(isLinux) {
		system("printf \'\e[8;53;108t\'"); // 53 tall, 108 wide
	} else {
		system("mode con: cols=160 lines=53");
	}
	cout << "Welcome to STS Library STS Interface Created by Jay Yoo.\nEnter \"--help\" for options" << endl;
	std::string userInput = "";

	while(1) {
		std::getline(std::cin, userInput);
		if(userInput == "--help") {
			std::cout << "\n\n" << "\e[1mHelp Options\e[0m" << 
				  ":\n____________________________________________________________________________________________________________\n" << std::endl;
			std::cout << "--help                                    Presents options." << std::endl;
			std::cout << "--exit                                    Exits interface." << std::endl;
			std::cout << "--show-bdd-current                        Shows BDD of current DES." << std::endl;
			std::cout << "--show-bdd-current-keep                   Produces dotty file and shows BDD of current DES." << std::endl;
			std::cout << "--show-bdd-current-simplified             Shows simplified BDD of current DES." << std::endl;
			std::cout << "--show-bdd-current-simplified-keep        Produces dotty file and shows simplified BDD of current DES." << std::endl;
			std::cout << "--show-bdd-original                       Shows BDD of original DES." << std::endl;
			std::cout << "--show-bdd-original-keep                  Produces dotty file and shows BDD of original DES." << std::endl;
			std::cout << "--show-bdd-original-simplified            Shows simplified BDD of original DES." << std::endl;
			std::cout << "--show-bdd-original-simplified-keep       Produces dotty file and shows simplified BDD of original DES." << std::endl;
			std::cout << "--show-state                              Shows BDD of current state." << std::endl;
			std::cout << "--show-state-keep                         Produces dotty file and shows BDD of current state." << std::endl;
			std::cout << "--set-state                               Sets state to manipulate." << std::endl;
			std::cout << "--set-state-initial                       Automatically chooses and sets an initial state to manipulate." << std::endl;
			std::cout << "--print-state                             Prints current state." << std::endl;
			std::cout << "--print-state-all                         Prints all states." << std::endl;
			std::cout << "--print-state-all-formatted               Prints all states in a formatted form." << std::endl;
			std::cout << "--print-events-all                        Prints all available events." << std::endl;
			std::cout << "--print-events-controllable               Prints all controllable events." << std::endl;
			std::cout << "--print-events-uncontrollable             Prints all uncontrollable events." << std::endl;
			std::cout << "--print-events-available-bdd              Prints events available to be applied to current BDD in delta." << std::endl;
			std::cout << "--print-events-available-state            Prints events available to be applied to current state in delta." << std::endl;
			std::cout << "--print-events-tracked-delta-bdd          Prints events applied to BDD through delta in order." << std::endl;
			std::cout << "--print-events-tracked-delta-state        Prints events applied to state through delta in order." << std::endl;
			std::cout << "--print-holons                            Prints all states." << std::endl;
			std::cout << "--produce-bdd-current                     Produces dotty file of BDD of current DES." << std::endl;
			std::cout << "--produce-bdd-current-simplified          Produces dotty file of simplified BDD of current DES." << std::endl;
			std::cout << "--produce-bdd-original                    Produces dotty file of BDD of original BDD." << std::endl;
			std::cout << "--produce-bdd-original-simplified         Produces dotty file of simplified BDD of original BDD." << std::endl;
			std::cout << "--produce-control-functions               Produces dotty files of all control functions." << std::endl;
			std::cout << "--clear-terminal                          Clears terminal." << std::endl;
			std::cout << "--reset-bdd                               Reverts current BDD to original BDD." << std::endl;
			std::cout << "--reset-ui                                Resets user interface." << std::endl;
			std::cout << "--delta-bdd                               Applies delta function on current DES." << std::endl;
			std::cout << "--delta-state                             Applies delta function on current state." << std::endl;
			std::cout << "--gamma-bdd                               Applies delta function on current DES." << std::endl;
			std::cout << "--gamma-state                             Applies delta function on current state." << std::endl;
			std::cout << "--undo-delta-bdd                          Reverts most recent delta applied to current BDD." << std::endl;
			std::cout << "--undo-delta-bdd-multiple                 Reverts multiple recent delta applied to current BDD." << std::endl;
			std::cout << "--undo-delta-state                        Reverts most recent delta applied to current state." << std::endl;
			std::cout << "--undo-delta-state-multiple               Reverts multiple recent delta applied to current state." << std::endl;
			std::cout << "--undo-gamma-bdd                          Reverts most recent delta applied to current BDD." << std::endl;
			std::cout << "--undo-gamma-bdd-multiple                 Reverts multiple recent delta applied to current BDD." << std::endl;
			std::cout << "--undo-gamma-state                        Reverts most recent delta applied to current state." << std::endl;
			std::cout << "--undo-gamma-state-multiple               Reverts multiple recent delta applied to current state." << std::endl;
			std::cout << "__________________________________________________________________________________________________________" << std::endl;
			std::cout << std::endl;
		} else if(userInput == "--exit") {
			emptyScreen();
			if(checkFileExists("tempCurrBDD_toBeDeleted")) std::remove("tempCurrBDD_toBeDeleted");
			break;
		} else if(userInput == "--show-bdd-current") {
			showBDD("tempCurrBDD_toBeDeleted", currBDD);
			std::remove("tempCurrBDD_toBeDeleted");
		} else if(userInput == "--show-bdd-current-keep") {
			showBDD("currentBDD", currBDD);
		} else if(userInput == "--show-bdd-current-simplified") {
			printBDD_dotty("tempCurrBDD_toBeDeleted", currBDD);
			produceLegalBehavior("tempCurrBDD_toBeDeleted", "legalBehavior_current");
			if(isLinux) {
				system("dotty legalBehavior_current");
			} else {

			}
			std::remove("tempCurrBDD_toBeDeleted");
			std::remove("legalBehavior_current");
		} else if(userInput == "--show-bdd-current-simplified-keep") {
			printBDD_dotty("tempCurrBDD_toBeDeleted", currBDD);
			produceLegalBehavior("tempCurrBDD_toBeDeleted", "legalBehavior_current");
			if(isLinux) {
				system("dotty legalBehavior_current");
			} else {
				system("dotty legalBehavior_current");
			}
			std::remove("tempCurrBDD_toBeDeleted");
		} else if(userInput == "--show-bdd-original") {
			showBDD("tempCurrBDD_toBeDeleted", originalBDD);
			std::remove("tempCurrBDD_toBeDeleted");
		} else if(userInput == "--show-bdd-original-keep") {
			showBDD("originalBDD", originalBDD);
		} else if(userInput == "--show-bdd-original-simplified") {
			printBDD_dotty("tempCurrBDD_toBeDeleted", originalBDD);
			produceLegalBehavior("tempCurrBDD_toBeDeleted", "legalBehavior_original");
			if(isLinux) {
				system("dotty legalBehavior_original");
			} else {
				system("dotty legalBehavior_original");
			}
			std::remove("tempCurrBDD_toBeDeleted");
			std::remove("legalBehavior_original");
		} else if(userInput == "--show-bdd-original-simplified-keep") {
			printBDD_dotty("tempCurrBDD_toBeDeleted", originalBDD);
			produceLegalBehavior("tempCurrBDD_toBeDeleted", "legalBehavior_original");
			if(isLinux) {
				system("dotty legalBehavior_original");
			} else { 
				system("dotty legalBehavior_original");
			}
			std::remove("tempCurrBDD_toBeDeleted");
		} else if(userInput == "--show-state") {
			if(stateSelected) {
				showBDD("tempOutput_toBeDeleted", currState);
				std::remove("tempOutput_toBeDeleted");
			} else {
				std::cout << "\nState not selected. Returning to main menu. Select a state using the --set-state function." << std::endl;
			}
		} else if(userInput == "--show-state-keep") {
			if(stateSelected) {
				showBDD("state", currState);
			} else {
				std::cout << "\nState not selected. Select a state using the --set-state function." << std::endl;
			}
		} else if(userInput == "--set-state") {
			setState(false);
		} else if(userInput == "--set-state-initial") {
			setState(true);
		} else if(userInput == "--print-state") {
			if(stateSelected) {
				std::cout << std::endl;
				fdd_printset(currState);
				std::cout << std::endl;
			} else {
				std::cout << "State not selected. Returning to main menu. Select a state using the --set-state function." << std::endl;
			}
		} else if(userInput == "--print-state-all") {
			std::cout << std::endl;
			fdd_printset(originalBDD);
			std::cout << std::endl;
		} else if(userInput == "--print-state-all-formatted") {
			printValidStateSets();
		} else if(userInput == "--print-events-all") {
			for(map<string, event>::iterator i = allEvents.begin(); i != allEvents.end(); i++) std::cout << i -> first << std::endl;
		} else if(userInput == "--print-events-controllable") {
			for(map<string, event>::iterator i = controllableEvents.begin(); i != controllableEvents.end(); i++) std::cout << i -> first << std::endl;
		} else if(userInput == "--print-events-uncontrollable") {
			for(map<string, event>::iterator i = uncontrollableEvents.begin(); i != uncontrollableEvents.end(); i++) std::cout << i -> first << std::endl;
		} else if(userInput == "--print-events-available-bdd") {
			map<std::string, event> validEvents = validEventsDelta(currBDD, "BDD");
			for(map<string, event>::iterator i = validEvents.begin(); i != validEvents.end(); i++) {
				if(i != validEvents.begin()) {
					std::cout << ", ";
				}
				std::cout << i -> first;
			}
			std::cout << std::endl;
		} else if(userInput == "--print-events-available-state") {
			if(stateSelected) {
				map<std::string, event> validEvents = validEventsDelta(currState, "state");
				for(map<string, event>::iterator i = validEvents.begin(); i != validEvents.end(); i++) {
					if(i != validEvents.begin()) {
						std::cout << ", ";
					}
					std::cout << i -> first;
				}
				std::cout << std::endl;
			} else std::cout << "\nState not selected. Returning to main menu. Select a state using the --set-state function." << std::endl;
		} else if(userInput == "--print-events-tracked-delta-bdd") {
			if(deltaBDD_events.size() != 0) {
				std::cout << "Events applied: ";
				bool first = true;
				for(int i = 0; i < deltaBDD_events.size(); i++) { 
					if(first) {
						first = false;
					} else {
						std::cout << ">";
					}
					std::cout << deltaBDD_events.at(i);
				}
				std::cout << std::endl;
			} else {
				std::cout << "Delta has not been applied to DES yet. Returning to main menu. Apply delta using --delta-bdd function." << std::endl;
			}
		} else if(userInput == "--print-events-tracked-delta-state") {
			if(deltaState_events.size() != 0) {
				std::cout << "Events applied: ";
				bool first = true;
				for(int i = 0; i < deltaState_events.size(); i++) { 
					if(first) {
						first = false;
					} else {
						std::cout << ">";
					}
					std::cout << deltaState_events.at(i);
				}
				std::cout << std::endl;
			} else {
				std::cout << "Delta has not been applied to state yet. Returning to main menu. Apply delta using --delta-bdd function." << std::endl;
			}
		} else if(userInput == "--print-holons") {
			for(std::vector<string>::iterator i = components.begin(); i != components.end(); ++i) std::cout << *i << std::endl;
		} else if(userInput == "--produce-bdd-current") {
			printBDD_dotty("currentBDD", currBDD);
			std::cout << "BDD Produced." << std::endl;
		} else if(userInput == "--produce-bdd-current-simplified") {
			printBDD_dotty("tempCurrBDD_toBeDeleted", currBDD);
			produceLegalBehavior("tempCurrBDD_toBeDeleted", "legalBehavior_current");
			std::remove("tempCurrBDD_toBeDeleted");
			std::cout << "BDD Produced." << std::endl;
		} else if(userInput == "--produce-bdd-original") {
			printBDD_dotty("originalBDD", originalBDD);
			std::cout << "BDD Produced." << std::endl;
		} else if(userInput == "--produce-bdd-original-simplified") {
			printBDD_dotty("tempCurrBDD_toBeDeleted", originalBDD);
			produceLegalBehavior("tempCurrBDD_toBeDeleted", "legalBehavior_original");
			std::remove("tempCurrBDD_toBeDeleted");
			std::cout << "BDD Produced." << std::endl;
		} else if(userInput == "--produce-control-functions") {
			outputControlFunctions();
			std::cout << "Control Functions Produced." << std::endl;
		} else if(userInput == "--clear-terminal") {
			emptyScreen();
		} else if(userInput == "--reset-bdd") {
			currBDD = originalBDD;
			std::cout << "BDD Reset." << std::endl;
		} else if(userInput == "--reset-ui") {
			emptyScreen();
			std::cout << "Welcome to STS Library STS Interface Created by Jay Yoo.\nEnter \"--help\" for options" << std::endl;
		} else if(userInput == "--delta-bdd") {
			deltaFunc("DES");
		} else if(userInput == "--delta-state") {
			deltaFunc("state");
		} else if(userInput == "--undo-delta-bdd") {
			undoDelta("BDD");
			std::cout << "Returning to main menu." << std::endl;
		} else if(userInput == "--undo-delta-bdd-multiple") {
			std::string numUndo;
			unsigned int n;
			std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
			while(1) {
				std::getline(std::cin, numUndo);
				if(numUndo == "--cancel") {
					std::cout << "\n--undo-delta-bdd-multiple function cancelled. Returning to main menu." << std::endl;
					break;
				} else if(numUndo == "--help") {
					std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
				} else {
					std::string::const_iterator i = numUndo.begin();
					while(i != numUndo.end() && std::isdigit(*i)) ++i;
					if(!numUndo.empty() && (i == numUndo.end())) {
						n = std::stoi(numUndo);
						if(n > prevDeltaBDD.size()) {
							std::cout << "\nSelected number greater than number of deltas applied. Number of deltas applied is: " << prevDeltaBDD.size() << ". Try again:\n" << std::endl;
						} else {
							for(int i = 0; i < n; i++) {
								undoDelta("BDD");
							}
							std::cout << "\nReturning to main menu." << std::endl;
							break;
						}
					} else {
						std::cout << "Not a numerical input. Try again:" << std::endl;
					}
				}
			}
		} else if(userInput == "--undo-delta-state") {
			undoDelta("state");
			std::cout << "Returning to main menu." << std::endl;
		} else if(userInput == "--undo-delta-state-multiple") {
			std::string numUndo;
			unsigned int n;
			std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
			while(1) {
				std::getline(std::cin, numUndo);
				if(numUndo == "--cancel") {
					std::cout << "\n--undo-delta-state-multiple function cancelled. Returning to main menu." << std::endl;
					break;
				} else if(numUndo == "--help") {
					std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
				} else {
					std::string::const_iterator i = numUndo.begin();
					while(i != numUndo.end() && std::isdigit(*i)) ++i;
					if(!numUndo.empty() && (i == numUndo.end())) {
						n = std::stoi(numUndo);
						if(n > prevDeltaState.size()) {
							std::cout << "\nSelected number greater than number of deltas applied. Number of deltas applied is: " << prevDeltaBDD.size() << ". Try again:\n" << std::endl;
						} else {
							for(int i = 0; i < n; i++) {
								undoDelta("state");
							}
							std::cout << "\nReturning to main menu." << std::endl;
							break;
						}
					} else {
						std::cout << "Not a numerical input. Try again:" << std::endl;
					}
				}
			}
		}
		else if(userInput == "--gamma-bdd") {
			gammaFunc("DES");
		} else if(userInput == "--gamma-state") {
			gammaFunc("state");
		} else if(userInput == "--undo-gamma-bdd") {
			undoGamma("BDD");
			std::cout << "Returning to main menu." << std::endl;
		} else if(userInput == "--undo-gamma-bdd-multiple") {
			std::string numUndo;
			unsigned int n;
			std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
			while(1) {
				std::getline(std::cin, numUndo);
				if(numUndo == "--cancel") {
					std::cout << "\n--undo-gamma-bdd-multiple function cancelled. Returning to main menu." << std::endl;
					break;
				} else if(numUndo == "--help") {
					std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
				} else {
					std::string::const_iterator i = numUndo.begin();
					while(i != numUndo.end() && std::isdigit(*i)) ++i;
					if(!numUndo.empty() && (i == numUndo.end())) {
						n = std::stoi(numUndo);
						if(n > prevGammaBDD.size()) {
							std::cout << "\nSelected number greater than number of gammas applied. Number of gammas applied is: " << prevGammaBDD.size() << ". Try again:\n" << std::endl;
						} else {
							for(int i = 0; i < n; i++) {
								undoGamma("BDD");
							}
							std::cout << "\nReturning to main menu." << std::endl;
							break;
						}
					} else {
						std::cout << "Not a numerical input. Try again:" << std::endl;
					}
				}
			}
		} else if(userInput == "--undo-gamma-state") {
			undoGamma("state");
			std::cout << "Returning to main menu." << std::endl;
		} else if(userInput == "--undo-gamma-state-multiple") {
			std::string numUndo;
			unsigned int n;
			std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
			while(1) {
				std::getline(std::cin, numUndo);
				if(numUndo == "--cancel") {
					std::cout << "\n--undo-gamma-state-multiple function cancelled. Returning to main menu." << std::endl;
					break;
				} else if(numUndo == "--help") {
					std::cout << "\nType number of times to undo. Type \"--cancel\" to cancel. Type \"--help\" to see these instrutions again." << std::endl;
				} else {
					std::string::const_iterator i = numUndo.begin();
					while(i != numUndo.end() && std::isdigit(*i)) ++i;
					if(!numUndo.empty() && (i == numUndo.end())) {
						n = std::stoi(numUndo);
						if(n > prevGammaState.size()) {
							std::cout << "\nSelected number greater than number of gammas applied. Number of gammas applied is: " << prevGammaBDD.size() << ". Try again:\n" << std::endl;
						} else {
							for(int i = 0; i < n; i++) {
								undoGamma("state");
							}
							std::cout << "\nReturning to main menu." << std::endl;
							break;
						}
					} else {
						std::cout << "Not a numerical input. Try again:" << std::endl;
					}
				}
			}
		}
		else std::cout << "\nInvalid Input. Enter \"--help\" for options. Try again:" << std::endl;
		std::cout << std::endl;
	}
}
/********************************************************************/
