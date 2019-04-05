#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

struct Footballer{
    char name[50];
    char teamName[50];
    struct Footballer * nextFootballer;
    struct matchDetails * head;
};

typedef struct Footballer Footballer;

struct matchDetails{
    char awayTeam[50];
    int minuteGoal;
    int matchID;
    struct matchDetails * next;
    struct matchDetails * back;
};

typedef struct matchDetails matchDetails;

struct Footballer * head = NULL;

matchDetails* insertMatchDetails(char awayTeam[], int minuteGoal, int matchID){
    matchDetails *temp = (matchDetails*)malloc(sizeof(struct matchDetails));
    
    strcpy(temp->awayTeam, awayTeam);
    temp->minuteGoal = minuteGoal;
    temp->matchID = matchID;
    temp->back = NULL;
    temp->next = NULL;

    return temp;
}

void insertFootballer(char name[], char teamName[], char awayTeam[], int minuteGoal, int matchID){
    Footballer *temp = (Footballer*)malloc(sizeof(Footballer));
    Footballer *temp2 = head;
    Footballer *hold;

    strcpy(temp->name, name);
    strcpy(temp->teamName, teamName);

    temp->head = insertMatchDetails(awayTeam, minuteGoal, matchID);
    temp->nextFootballer = NULL;

    if (head == NULL){
        head = temp;
        return;
    }

    if (strcmp(temp->name, temp2->name) == -1){
            head = temp;
            head->nextFootballer = temp2;
            return;
    }

    while (temp2->nextFootballer != NULL){
        if (strcmp(temp->name, temp2->nextFootballer->name) == 1)
            temp2 = temp2->nextFootballer;

        else{
            temp->nextFootballer = temp2->nextFootballer;
            temp2->nextFootballer = temp;   
            return;
        }
    }

    temp2 ->nextFootballer = temp;
}

void read(char content[]){
    char name[50], teamName[50], awayTeam[50], mgoal[20], mid[20];
    int minuteGoal, matchID;

    Footballer * temp = head;
    matchDetails * temp2;
    matchDetails * hold;

    sscanf(content, "%[^:,]%*c %[^:,]%*c %[^:,]%*c %[^:,]%*c %[^:,]%*c", name, teamName, awayTeam, mgoal, mid);

    minuteGoal = stoi(mgoal);
    matchID = stoi(mid);

    while(temp != NULL){
        if (strcmp(temp->name, name) == 0){
            if (temp->head == NULL)
                temp->head = insertMatchDetails(awayTeam, minuteGoal, matchID);

            else{
                temp2 = temp->head;

                while(temp2->next != NULL){
                    temp2 = temp2->next;
                }
                
                hold = insertMatchDetails(awayTeam, minuteGoal, matchID);
                hold->back = temp2;
                temp2->next = hold;
            }

            return;
        }
        temp = temp->nextFootballer;
    }

    insertFootballer(name, teamName, awayTeam, minuteGoal, matchID);
}

int maxElement(int array[], int count){
    int max, i;
    max = array[0];

    for (i = 1; i < count; i++){
        if (array[i] > max)
            max = array[i];
    }

    return max;
}

void topGoal(int count, ofstream &outfile){
    int footballerArray[count];
    int i = 0, max;
    Footballer * temp = head;
    matchDetails * temp2;

    while (temp != NULL){

        footballerArray[i] = 0;
        temp2 = temp->head;

        while(temp2 != NULL){
            footballerArray[i]++;
            temp2 = temp2->next;
        }
        
        i++;
        temp = temp->nextFootballer;
    }

    temp = head;
    max = maxElement(footballerArray, count);
    outfile << "2)GOAL SCORER" << endl;

    for (i = 0; i < count; i++){

        if (footballerArray[i] == max)
            outfile << temp->name << endl;

        temp = temp->nextFootballer;
    }

}

void goal(ofstream &outfile){
    int countFirst = 0, countSecond = 0;

    Footballer * temp = head;
    matchDetails * temp2;

    while(temp != NULL){

        temp2 = temp->head;

        while(temp2 != NULL){

            if (temp2->minuteGoal < 46)
                countFirst++;
            else
                countSecond++;

            temp2 = temp2->next;
        }
                
        temp = temp->nextFootballer;
    }

    outfile << "1)THE MOST SCORED HALF" << endl;

    if (countFirst > countSecond)
        outfile << 0 << endl;

    else if(countFirst < countSecond)
        outfile << 1 << endl;
    
    else 
        outfile << -1 << endl;
}

void hatTrick(int count, ofstream &outfile){
    int i, matchCount;
    Footballer * temp = head;
    matchDetails * matchTemp;

    outfile << "3)THE NAMES OF FOOTBALLERS WHO SCORED HAT-TRICK" << endl;

    while (temp != NULL){

        for (i = 0; i <= count; i++){
            matchCount = 0;
            matchTemp = temp->head;

            while (matchTemp != NULL){
                if (matchTemp->matchID == i){
                    matchCount++;

                    if (matchCount == 3)
                        outfile << temp->name << endl;
                }

                matchTemp = matchTemp -> next;
            }
        }
        temp = temp->nextFootballer;
    }

}

void printTeams(int count, ofstream &outfile){
    vector<string> strarray;
    Footballer * temp = head;
    int i = 0;
    
    outfile << "4)LIST OF TEAMS" << endl;

    while (temp != NULL){

        if (std::find(strarray.begin(), strarray.end(), temp->teamName) == strarray.end()){
            strarray.push_back(temp->teamName);
            outfile << temp->teamName << endl;
        }

        temp = temp->nextFootballer;
    }
}

void printFootballers(ofstream &outfile){
    Footballer * temp = head;

    outfile << "5)LIST OF FOOTBALLERS" << endl;

    while (temp != NULL){
        outfile << temp->name << endl;
        temp = temp->nextFootballer;
    }
}

void printMatches(Footballer * ff, ofstream &outfile){
    matchDetails * matchTemp;
    outfile << "Matches of " << ff->name << endl;

    matchTemp = ff->head;
        
    while (matchTemp != NULL){
        outfile << "Footballer Name: " << ff->name  << ",Away Team: " << matchTemp->awayTeam << ",Min of Goal: "
            << matchTemp->minuteGoal << ",Match ID: " << matchTemp->matchID << endl;
        matchTemp = matchTemp->next;
    }

}

void printMatchID(Footballer * ff, ofstream &outfile){
    vector<int> count;
    matchDetails * tempMatch = ff->head;

    while (tempMatch != NULL){

        if (std::find(count.begin(), count.end(), tempMatch->matchID) == count.end()){
            count.push_back(tempMatch->matchID);
            outfile << "Footballer Name: " << ff->name << ",Match ID: " << tempMatch->matchID << endl;           
        }

        tempMatch = tempMatch->next;     
    }
}

void printMatchIDReversed(Footballer * ff, ofstream &outfile){
    vector<int> count;
    matchDetails * tempMatch = ff->head;

    while (tempMatch->next != NULL){
        tempMatch = tempMatch->next;
    }
    
    while (tempMatch != NULL){
        if (std::find(count.begin(), count.end(), tempMatch->matchID) == count.end()){
            count.push_back(tempMatch->matchID);
            outfile << "Footballer Name: " << ff->name << ",Match ID: " << tempMatch->matchID << endl;           
        }

        tempMatch = tempMatch->back;  
    }
}

void readOperations(char content[], int count, ofstream &outfile){
    Footballer * temp = head;
    Footballer * ff1;
    Footballer * ff2;
    char name1[50], name2[50];

    sscanf(content, "%[^:,]%*c %[^:,]%*c", name1, name2);

    while (temp != NULL){
        if (strcmp(temp->name, name1) == 0)
            ff1 = temp;
        
        else if(strcmp(temp->name, name2) == 0)
            ff2 = temp;

        temp = temp->nextFootballer;
    }

    if (count == 0){
        outfile << "6)MATCHES OF GIVEN FOOTBALLER" << endl;
        printMatches(ff1, outfile);
        printMatches(ff2, outfile);
    }

    else if (count == 1){
        outfile << "7)ASCENDING ORDER ACCORDING TO MATCH ID" << endl;
        printMatchID(ff1, outfile);
        printMatchID(ff2, outfile);
    }

    else if (count == 2){
        outfile << "8)DESCENDING ORDER ACCORDING TO MATCH ID" << endl;
        printMatchIDReversed(ff1, outfile);
        printMatchIDReversed(ff2, outfile);
    }
}


int main(int argc, char *argv[]){
    ifstream infile(argv[1]);
    ifstream opfile(argv[2]);
    ofstream outfile(argv[3]);

    char content[500];

    int i, count = 0, matchCount = 0, opCount = 0;

    while (!infile.eof()){
        infile.getline(content, 500);
        read(content);
    }
    
    Footballer * temp = head;
    matchDetails * tempMatch;

    while(temp != NULL){    
        tempMatch = temp->head;

        while (tempMatch != NULL){
            if (tempMatch->matchID > matchCount)
                matchCount = tempMatch->matchID;
            
            tempMatch = tempMatch->next;
        }

        count++;
        temp = temp->nextFootballer;
    }
    
    goal(outfile);
    topGoal(count, outfile);
    hatTrick(matchCount, outfile);
    printTeams(count, outfile);
    printFootballers(outfile);

    while (!opfile.eof()){
        opfile.getline(content, 500);
        readOperations(content, opCount, outfile);
        opCount++;
    }

    opfile.close();
    infile.close();
    outfile.close();
}