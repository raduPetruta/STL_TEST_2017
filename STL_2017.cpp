#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <limits.h>
using namespace std;

ifstream fi("input.txt");
ofstream fo("output.txt");

struct cuvinte {
    string textCuv;
    int pozX; int pozY;
    int width; int height;
};
struct templates {
    int id;
    string cuvSus; float pondereSus = 0;
    string cuvDr; float pondereDr = 0;
    string cuvJos; float pondereJos = 0;
    string cuvSt; float pondereSt = 0;
};

int n; int t; int idOfWordToSearch;
int pozX = 0, pozY = 0;

vector<cuvinte>CUVINTE;
vector<cuvinte>goodWordsX, goodWordsY;    //devide the words in two vectors that keep the words that are on the same x axis and y axis as the toSearch word
vector<templates>TEMPLATES;
vector<templates>goodTEMPLATES;
cuvinte cuvUp, cuvDown, cuvLeft, cuvRight;
string toSearch;

void getInput() {

    fi >> n;
    //get all the words
    while (n--) {
        cuvinte temp;
        fi >> temp.textCuv >> temp.pozX >> temp.pozY >> temp.width >> temp.height;
        CUVINTE.push_back(temp);
    }
    fi >> t;
    //get all the templates
    while (t--) {
        templates temp;
        fi >> temp.id;
        //if a word in a template is "-" it doesn't have a cost
        fi >> temp.cuvSus;
        if (temp.cuvSus != "-")
            fi >> temp.pondereSus;

        fi >> temp.cuvDr;
        if (temp.cuvDr != "-")
            fi >> temp.pondereDr;

        fi >> temp.cuvJos;
        if (temp.cuvJos != "-")
            fi >> temp.pondereJos;

        fi >> temp.cuvSt;
        if (temp.cuvSt != "-")
            fi >> temp.pondereSt;

        TEMPLATES.push_back(temp);
    }
    fi >> idOfWordToSearch;
    fi.close();
}

void sortTheAxisVectors() {
    //sort the words on both axis according to their axis index
    sort(goodWordsX.begin(), goodWordsX.end(), [](cuvinte A, cuvinte B) {
        return A.pozY < B.pozY;
    });
    sort(goodWordsY.begin(), goodWordsY.end(), [](cuvinte A, cuvinte B) {
        return A.pozX < B.pozX;
    });
}

void findNeighbourWords() {
    //find the positions of the given word in every axis vector to find its neighbours
    for (int i = 0; i < goodWordsX.size(); i++) {
        if (goodWordsX[i].textCuv == toSearch) {
            pozX = i; break;
        }
    }
    for (int i = 0; i < goodWordsY.size(); i++) {
        if (goodWordsY[i].textCuv == toSearch) {
            pozY = i; break;
        }
    }

    //find neighbour words on Y axis
    if (pozX == 0 && goodWordsX.size() >= 1) {
        cuvUp = goodWordsX[pozX + 1];
        cuvDown = goodWordsX[0];
    }
    else if (pozX == goodWordsX.size() - 1 && goodWordsX.size() >= 1) {
        cuvUp = goodWordsX[pozX];
        cuvDown = goodWordsX[pozX - 1];
    }
    else if (pozX - 1 >= 0 && pozX + 1 < goodWordsX.size()) {
        cuvUp = goodWordsX[pozX + 1];
        cuvDown = goodWordsX[pozX - 1];
    }
    else if (goodWordsX.size() == 1) {
        cuvUp = goodWordsX[0];
        cuvDown = cuvUp;
    }

    //find neighbour words on X Axis
    if (pozY == 0 && goodWordsY.size() >= 1) {
        cuvLeft = goodWordsY[0];
        cuvRight = goodWordsY[pozY + 1];
    }
    else if (pozY == goodWordsY.size() - 1 && goodWordsY.size() >= 1) {
        cuvLeft = goodWordsY[pozY - 1];
        cuvRight = goodWordsY[pozY];
    }
    else if (pozY - 1 >= 0 && pozY - 1 < goodWordsY.size()) {
        cuvLeft = goodWordsY[pozY - 1];
        cuvRight = goodWordsY[pozY + 1];
    }
    else if (goodWordsY.size() == 1) {
        cuvLeft = goodWordsY[0];
        cuvRight = cuvLeft;
    }
}
 
void Solve() {

    vector<cuvinte>::iterator itToSearch;
    itToSearch = CUVINTE.begin() + idOfWordToSearch;   //get the toSearch word
    toSearch = itToSearch->textCuv;

    //we eliminate the words that don't have x and y coordinates the same as the word that we re searching
    //and the words that overlap
    for (int i = 0; i < CUVINTE.size(); i++) {   
        if (i != itToSearch - CUVINTE.begin()) {
            //check for height boundaries
            if (CUVINTE[i].pozX == itToSearch->pozX) {
                if (CUVINTE[i].pozY > itToSearch->pozY) {
                    if (CUVINTE[i].pozY - CUVINTE[i].height >= itToSearch->pozY) {
                        goodWordsX.push_back(CUVINTE[i]);
                        continue;
                    }
                }
                else if (CUVINTE[i].pozY < itToSearch->pozY) {
                    if (itToSearch->pozY - itToSearch->height >= CUVINTE[i].pozY) {
                        goodWordsX.push_back(CUVINTE[i]);
                        continue;
                    }
                }
            }
            //check for width boundaries
            if (CUVINTE[i].pozY == itToSearch->pozY) {
                if (CUVINTE[i].pozX < itToSearch->pozX) {
                    if (CUVINTE[i].pozX + CUVINTE[i].width < itToSearch->pozX) {
                        goodWordsY.push_back(CUVINTE[i]);
                        continue;
                    }
                }
                else if(CUVINTE[i].pozX > itToSearch->pozX) {
                    if (itToSearch->pozX + itToSearch->width <= CUVINTE[i].pozX) {
                        goodWordsY.push_back(CUVINTE[i]);
                        continue;
                    }
                }
            }
        }
        //add the toSearch word to both vectors so we can get the neighbours
        else {
            goodWordsX.push_back(CUVINTE[i]); goodWordsY.push_back(CUVINTE[i]);
        }
    }
    CUVINTE.clear();
    sortTheAxisVectors();

    //for every case of vectors dimensions we find for the given word its neighbours
    //for neighbour words on Y Axis and X Axis
    findNeighbourWords();

    //we need to search for every given template wheter one of its words matches the neighbours of the "toSearch" word and if it matches we need to see if the postion in the template is equal to the position int OXY plane
    //iterator for the vector of templates
    vector<templates>::iterator it;
    float pondereMaxGlobal = INT_MIN;
    int idMax = 0;
    
    for (it = TEMPLATES.begin(); it != TEMPLATES.end(); it++) {
        
        float sum = 0.00;
        //check for position match
        if (it->cuvSt == cuvLeft.textCuv) 
            sum += it->pondereSt;
        if (it->cuvDr == cuvRight.textCuv)
            sum += it->pondereDr;
        if (it->cuvJos == cuvDown.textCuv)
            sum += it->pondereJos;
        if (it->cuvSus == cuvUp.textCuv)
            sum += it->pondereSus;

        //find the maximum cost of a template and save it and its id
        if (sum > pondereMaxGlobal) {
            pondereMaxGlobal = sum;
            idMax = it->id;
        }
        //cout << sum << " ";
    }
    //cout << pondereMaxGlobal;
    fo << idMax << endl;
    fo.close();
}

int main() {
    getInput();
    Solve();
    return 0;
}