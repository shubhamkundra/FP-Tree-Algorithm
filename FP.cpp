#include <iostream>
#include <utility>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
//#include <bits/stdc++.h>
#include <queue>
#define pb push_back
#define MAX 10000
using namespace std;

double min_threshold = 0.6;
double min_conf_threshold = 0.9;


class frequent{
public:
    int ftrie[MAX][21];
    int runTill;
    vector<int>fset;
    int fcount;
    int Count[MAX][21];
    frequent(){
        for(int i=0;i<MAX;i++){
            for(int j=0;j<=20;j++){
                ftrie[i][j]=-1;
                Count[i][j]=0;
            }
        }
        fcount=0;
    }
};

frequent* mainNode = new frequent();


queue<frequent*>q;
vector<int>data[MAX];
vector<vector<int> >frequentItemSet;
char tempstore[100];
int line = 0,k = 1;
double meanValues[] = {0, 3.8, 120.9, 69.1, 20.5 ,79.8 , 32.0 ,0.5 ,33.2};
map<set<int>,int> finalmap[12];
vector<pair<set<int>,set<int> > >finalRules[12];
map<int,string>decode;


void insert(int line){
	int x = data[line].size(),node = 0;

	for(int i = 0; i<x;i++){
		int idx = data[line][i];
		if(mainNode->ftrie[node][idx] == -1){
			mainNode->ftrie[node][idx] = k++;

		}
		mainNode->Count[node][idx]++;
		node = mainNode->ftrie[node][idx];

	}

	mainNode->ftrie[node][20] = 1;


}

void dcode(){
    decode[2]="Number of times pregnant < 3.8   ";
    decode[3]="Number of times pregnant > 3.8   ";
    decode[4]="Plasma glucose concentration < 120.9   ";
    decode[5]="Plasma glucose concentration > 120.9   ";
    decode[6]="Diastolic blood pressure < 69.1   ";
    decode[7]="Diastolic blood pressure > 69.1   ";
    decode[8]="Triceps skin fold thickness < 20.5   ";
    decode[9]="Triceps skin fold thickness > 20.5   ";
    decode[10]="2-Hour serum insulin < 79.8   ";
    decode[11]="2-Hour serum insulin > 79.8   ";
    decode[12]="Body mass index < 32.0   ";
    decode[13]="Body mass index > 32.0   ";
    decode[14]="Diabetes pedigree function < 0.5   ";
    decode[15]="Diabetes pedigree function > 0.5   ";
    decode[16]="Age < 33.2   ";
    decode[17]="Age > 33.2   ";
    decode[18]="Class variable = 0   ";
    decode[19]="Class variable = 1   ";
}

void print(int tempTrie[MAX][21],int node,int k){
	if(tempTrie[node][20]==1){
		cout<<"\n"<<"PJaABKA";
		cout<<tempTrie[node][k]<<"\n";
		return;
	}
	for(int i=0;i<21;i++){
		if(tempTrie[node][i]!=-1){
			cout<<i<<" ";
			print(tempTrie,tempTrie[node][i],k);
		}
	}
}

void print(frequent* tempNode){
    for(int i=0;i<=20;i++){
        if(tempNode->ftrie[0][i]!=-1){
            cout<<tempNode->Count[0][i]<<" ";
        }
    }
    cout<<endl;
}

int prune(int val,frequent* tempNode,int node){
    int count=0;
    if(tempNode->ftrie[node][val]!=-1){
        tempNode->ftrie[node][20]=1;
        return tempNode->Count[node][val];
    }
    if(tempNode->ftrie[node][20]==1){
        return 0;
    }
    for(int i=0;i<=20;i++){
        if(tempNode->ftrie[node][i]!=-1){
            int pp = prune(val,tempNode,tempNode->ftrie[node][i]);
            if(pp==0){
                tempNode->ftrie[node][i]=-1;
            }
            count+=pp;
            tempNode->Count[node][i]=count;
        }
    }
    return count;
}


void rules_generator(set<int> FI)
{

    int sc;
    double conf;
    sc=finalmap[FI.size()][FI];
    set<int> temp;
    vector<set<int> > cr[20],fr[20];
    for(set<int>::iterator it=FI.begin();it!=FI.end();it++)
    {
        set<int>golu;
        golu.insert(*it);   
        cr[1].push_back(golu);
    }
    
    int x=0;
    for(int i=0;i<cr[1].size();i++)
    {
        temp=FI;
        for(set<int>::iterator it=cr[1][i].begin();it!=cr[1][i].end();it++)
        if(temp.find(*it)!=temp.end())
            temp.erase(*it);
        conf=(double)sc/(double)finalmap[temp.size()][temp];

        if(conf>=min_conf_threshold)
        {
            fr[1].push_back(cr[1][i]);
            finalRules[FI.size()].push_back(make_pair(temp,cr[1][i]));
        }

    }
    
    for(int i=2;i<FI.size();i++)
    {
        int x=0;
        for(int j=0;j<fr[i-1].size();j++)
        {
            for(int k=0;k<fr[1].size();k++)
            {
                if(fr[i-1][j].find(*fr[1][k].begin())==fr[i-1][j].end())
                {
                    cr[i].push_back(fr[i-1][j]);
                    cr[i][cr[i].size()-1].insert(*fr[1][k].begin());
                    
                }
            }
        }

        for(int j=0;i<cr[i].size();i++)
        {
            temp=FI;
            for(set<int>::iterator it=cr[i][j].begin();it!=cr[i][j].end();it++)
            if(temp.find(*it)!=temp.end())
                temp.erase(*it);
            conf=(double)sc/(double)finalmap[temp.size()][temp];
            if(conf>=min_conf_threshold)
            {
                fr[i].push_back(cr[i][j]);
                finalRules[FI.size()].push_back(make_pair(temp,cr[i][j]));
            }
        }
    }
}




int main(){


	FILE *inputfile = fopen("FPdata.txt","r+");   //trainingdata file


	

    while(fscanf(inputfile, "%[^\n]s",tempstore)!=EOF){  //storing training data in training data vector
        int len = strlen(tempstore),j = 0;
 		
        string temp = "";
        int ct = 1;

 		for(int i = 0; i<len ;i++){
 			if(i == len-1){
 				temp.pb(tempstore[i]);
 				int val = atoi(temp.c_str());

 				if(val){
 					data[line].pb(2*ct+1);
 				}
 				else{
 					data[line].pb((2*ct));
 				}
 				break;
 			}

 			if(tempstore[i]!=',')temp.pb(tempstore[i]);
 			else if(tempstore[i] == ','){
				double t = atof(temp.c_str());
				if(t>meanValues[ct]){
					
 				data[line].pb(2*ct+1);}
 				else
 				data[line].pb((2*ct));
 				ct++;
 				temp = "";
 			}
 		}
 		line++;
       	fgetc(inputfile);

    }

    for(int i = 0; i<line;i++){
    	insert(i);
    }

    int total=0;

    for(int i=19;i>=2;i--){
    	frequent* tempNode = new frequent();
    	for(int k=0;k<MAX;k++){
    		for(int j=0;j<=20;j++){
    			tempNode->ftrie[k][j]=mainNode->ftrie[k][j];
                tempNode->Count[k][j]=mainNode->Count[k][j];
    		}
    	}

    	tempNode->runTill = (i/2-1)*2+1;
    	tempNode->fset.pb(i);


    	tempNode->fcount = prune(i,tempNode,0);


    	if((double)tempNode->fcount/(double)768.0>min_threshold){
            set<int>temp;
                for(int m=0;m<tempNode->fset.size();m++){
                    temp.insert(tempNode->fset[m]);
                }
            finalmap[temp.size()][temp]=tempNode->fcount;
    	   frequentItemSet.pb(tempNode->fset);
    	   q.push(tempNode);
    	}
    	
    }
    int ct = 0;
    while(!q.empty()){
    	frequent* front_Node  = q.front();
    	q.pop();
    	for(int i = front_Node->runTill; i>=2;i--){
    		frequent* tempNode = new frequent();
    		
    		tempNode->fset = front_Node->fset;
    		tempNode->fset.pb(i);
            tempNode->runTill = (i/2-1)*2+1;
            for(int k=0;k<MAX;k++){
                for(int j=0;j<=20;j++){
                    tempNode->ftrie[k][j] = front_Node->ftrie[k][j];
                    tempNode->Count[k][j] = front_Node->Count[k][j];
                }
            }
    		tempNode->fcount = prune(i,tempNode,0);
            double tempv = (double)tempNode->fcount/(double)768.0;
    	    if(tempv>min_threshold){
                set<int>temp;
                for(int m=0;m<tempNode->fset.size();m++){
                    temp.insert(tempNode->fset[m]);
                }
                finalmap[temp.size()][temp]=tempNode->fcount;
    		    frequentItemSet.pb(tempNode->fset);
    		    q.push(tempNode);
    		}
    		
    	}

    }

    cout<<frequentItemSet.size()<<endl;

    vector<set<int> >frequentItemset[12];                   //for rule generation

    for(int i=0;i<frequentItemSet.size();i++){
        set<int> temp;
        for(int j=0;j<frequentItemSet[i].size();j++)
        {
            temp.insert(frequentItemSet[i][j]);
        }
        frequentItemset[temp.size()].push_back(temp);
    }


    //RULE GENERATION

    dcode();
    for(int i=2;i<11;i++)
    {
        for(int j=0;j<frequentItemset[i].size();j++)
        {
            rules_generator(frequentItemset[i][j]);
        }
    }

    cout<<"min support threshhold="<<min_threshold<<"\n\n\n\nmin confidence threshold ="<<min_conf_threshold<<endl<<"\n\n\n\n\n";    
    int cnt=0;
    for(int i=2;i<11;i++)
    {
        int count =0;
        cout<<"Rules for "<<i<<" cardinality frequentItemset\n";
        for(int j=0;j<finalRules[i].size();j++)
        {
            cnt++;
            cout<<++count<<" : { ";
            for(set<int>::iterator it=finalRules[i][j].first.begin();it!=finalRules[i][j].first.end();it++)
             cout<<decode[*it]<<" ,";
            cout<<"} --->  {";
            for(set<int>::iterator it=finalRules[i][j].second.begin();it!=finalRules[i][j].second.end();it++)
             cout<<decode[*it]<<" ,";
            cout<<"}\n";
        }
        cout<<"\n\n\n\n";

        cout<<"Total no of rules generated ="<<cnt<<"\n\n\n";
    }
}