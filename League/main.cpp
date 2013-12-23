#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "team.h"
#include "league.h"
#include "game.h"
using namespace std;

//Team members : Vidran Abdovich - 312064829, Ofir Aghai -
//----------------------------------------------//
//The methods 'sort' - sorts lexicoraphically	//
//a vector of strings using quick sort algorithm//
//and then prints the vector as an output.		//
//----------------------------------------------//
void sort(vector<string> v){
	int sz = v.size();
	for (int i = 0; i < sz; i++){
		for (int j = i+1; j < sz; j++){
			if (v[i].compare(v[j]) > 0){ 
				string tmp;
				tmp = v[i];
				v[i] = v[j];
				v[j] = tmp;
			}
		}
	}
	for (int i = 0; i < sz; i++){
		cout<<i+1<<". "<<v[i]<<endl;
	}
}
//-----------------------------------------------//
// This method converts a string containing a	 //
// a month name in its shorter version, into an  //
// integer type .e.g OCT -> 10					 //
//-----------------------------------------------//
int monthToInt(string month)
{
	std::transform(month.begin(), month.end(), month.begin(), ::tolower);

	if (month.compare("jan")==0) {return 1;}
	else if (month.compare("feb")==0) {return 2;}
	else if (month.compare("mar")==0) {return 3;}
	else if (month.compare("apr")==0) {return 4;}
	else if (month.compare("may")==0) {return 5;}
	else if (month.compare("june")==0) {return 6;}
	else if (month.compare("july")==0) {return 7;}
	else if (month.compare("aug")==0) {return 8;}
	else if (month.compare("sep")==0) {return 9;}
	else if (month.compare("oct")==0) {return 10;}
	else if (month.compare("nov")==0) {return 11;}
	else if (month.compare("dec")==0) {return 12;}
	else return -1;
}

//----------------------------------------------//
//This methods 'splitStr' recives a string type	//
//and returns a vector of strings where each	//
//index stores a word from the sentence that is //
//splitted.										//
//----------------------------------------------//
vector<string> splitStr(string str){
	string buf;				
	stringstream ss(str);	
	vector<string> tokens;	// Create vector to hold our words

	while (ss >> buf){
		if (!buf.empty())	
		tokens.push_back(buf);
	}
	return tokens;
}

//-----------------------------------------------//
//The methods 'sort' - sorts lexicoraphically	//
//a vector of strings using quick sort algorithm//
//and then prints the vector as an output to	//
// file.										//
//----------------------------------------------	//
void sortToFile(vector<string> v,string outputFileName){
	ofstream fileWriter;
	fileWriter.open(outputFileName, ios_base::app);
	int sz = v.size();
	for (int i = 0; i < sz; i++){
		for (int j = i+1; j < sz; j++){
			if (v[i].compare(v[j]) > 0){ 
				string tmp;
				tmp = v[i];
				v[i] = v[j];
				v[j] = tmp;
			}
		}
	}
	fileWriter<<"Teams"<<endl<<"-------"<<endl;
	for (int i = 0; i < sz; i++){
		fileWriter<<i+1<<". "<<v[i]<<endl;
	}
	fileWriter<<endl;
}
//------------------------------------------------//
// before sending to splitStr()					  //
// from "aab. tr, ew, .sdff" => "aab tr ew sdff"  //
// for ex. delimetersRemover(str, ",.()\0", ' ' );//			
//will delete "."   ","   "("   ")"				  //
//we need to write '\0' at the end				  //
//------------------------------------------------//
string delimetersRemover(string str, string delimitersVector, char to ){		
	//for (int i=0; i<delimitersVector.size(); i++){
	int i=0;
	while (i < delimitersVector.size()){
		//change from "*" => " "
		size_t found1 = str.find_first_of(delimitersVector[i]);
		while (found1!= string::npos){
			str[found1]=to;
			found1=str.find_first_of(delimitersVector[i], found1+1);
		}
		i++;
	}
	return str;
}
//------------------------------------------------//
//This method updates the games database. how?	  //
//1. it save every line from Games database into a//
//vector of strings. Then updates the desired line//
//2. Overites the games database file			  //
//------------------------------------------------//
void updateGamesDatabase(int roundNumber , string day, string month, string year,string database)
{
	ofstream fileWriter;
	ifstream fileReader;
	if (database.empty())
	{
		fileReader.open("games.db");
	}
	else
	{
		
		// create file in path
		string tmp=".\\"+database+"\\games.db";
		fileReader.open(tmp);
	}
	string tmp;
	string updatedLine;
	vector<string> lines;
	//Save every line from games database to a vector
	while(getline(fileReader, tmp))
	{
		lines.push_back(tmp);
	}
	fileReader.close();
	if (database.empty())
	{
	fileWriter.open("games.db",ios::out);
	}
	else
	{
		// Make dir
		string s = "mkdir .\\"+database;
		system(s.c_str());
		// create file in path
		string tmp=".\\"+database+"\\games.db";
		fileWriter.open(tmp,ios::out);
	}
	//make the correction
	for(int i=0; i<lines.size() ; i++)
	{
		updatedLine="";
		vector<string> tempVector = splitStr(lines.at(i));
		if (tempVector[0].compare("game")==0 && tempVector[1].compare(to_string(roundNumber))==0)
		{
			tempVector[2]=month;
			tempVector[3]=day;
			tempVector[4]=year;
			updatedLine=tempVector[0]+" "+tempVector[1]+" "+tempVector[2]+" "+tempVector[3]+" "+tempVector[4];
			lines[i]=updatedLine;
		}
	}

	//overwrite the games database file
	for(int i=0; i < lines.size(); i++)
	{
		fileWriter<<lines[i]<<endl;
	}
}


//------------------------------------------------ //
//Thie method corrects a wrong date in a league	  //
//game.											  //
//------------------------------------------------ //
void correctGameDate(string str, vector<team>* teams,string database){
	//Remove all kind of special signs from the date.
	str= delimetersRemover(str, ",.\0", ' ' );
	vector<string> tokenizedString = splitStr(str);
	int gameNum=stoi(tokenizedString[2]);
	int day = stoi(tokenizedString[4]);
	int month = monthToInt(tokenizedString[3]);
	int year = stoi(tokenizedString[5]);
	Date newDate(day,month,year);

	//iterate league teams and update the date of every game that matches
	//the given game number.
	for (int i=0; i< teams->size() ; i++)
	{
		for(int j=0 ; j<teams->at(i).getGames()->size(); j++)
		{
			if(teams->at(i).getGames()->at(j)->getRoundNum()==gameNum)
			{
				teams->at(i).getGames()->at(j)->setDate(newDate);
			}
		}

	}

	//now update the date in games.db file
	updateGamesDatabase(gameNum , tokenizedString[4],tokenizedString[3],tokenizedString[5],database);
}

//------------------------------------------------//
//Thie method returns the database name from the  //
//command line arguments if is set				  //
//------------------------------------------------//
string analyzeDbFromArgv(int argc, char** argv)
{
	vector<string> argvVector;
	//push every argument into a vector of strings
	for (int i=0; i<argc; i++){
	string argvString(argv[i]);
	argvVector.push_back(argvString);
	}

	//db analyze
	for (int i=0; i<argvVector.size(); i++)
	{

		if (argvVector[i]=="-db")
			return argvVector[i+1];
	}
return "database";

}

//------------------------------------------------//
//Thie method returns the output file name from   //
//the command line arguments if is set			  //
//------------------------------------------------//
string analyzeInputFromArgv(int argc, char** argv)
{
	vector<string> argvVector;
	//push every argument into a vector of strings
	for (int i=0; i<argc; i++){
	string argvString(argv[i]);
	argvVector.push_back(argvString);
	}

	//input file analyze
	for (int i=0; i<argvVector.size(); i++)
	{

		if (argvVector[i]=="-i")
			return argvVector[i+1];
	}
return "@";

}

//------------------------------------------------//
//Thie method returns the input file name from    //
//the command line arguments if is set			  //
//------------------------------------------------//
string analyzeOutputFromArgv(int argc, char** argv)
{
	vector<string> argvVector;
	//push every argument into a vector of strings
	for (int i=0; i<argc; i++){
	string argvString(argv[i]);
	argvVector.push_back(argvString);
	}

	//input file analyze
	for (int i=0; i<argvVector.size(); i++)
	{

		if (argvVector[i]=="-o")
			return argvVector[i+1];
	}
return "@";

}


//-----------------------------------------------//
//This methods 'check_input' recieces users input//
//and directs it to the correct case in the	     //
//Switch/Case menu.								 //
//---------------------------------------------- //
int check_input(string str)
{
	//lower every char in str
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	// check empty string
	if (str == "" || str == "\n"|| str=="show" || str=="game" || str=="date" || str=="team") 
		return -1;

	vector<string> tokenized = splitStr(str);

	//In terminal - 'show teams'
	if (tokenized[0]=="show" && tokenized[1]=="teams") {
		return 1;
	}
	// In terminal - 'show league'
	else if (tokenized[0]=="show" && tokenized[1]=="league") {
		return 2;
	}
	// In terminal - 'show teamA - teamB'
        else if (tokenized[0]=="show" && tokenized[1]!="teams" && tokenized[1]!="league") {
                bool ifHaveMinus= false;
                for each (string s in tokenized) {
            if (s== "-"){
                                ifHaveMinus = true;
                                break;
                        }
        }
                if (ifHaveMinus == true)
                        return 3;
                else return -1;
        }
	//In terminal - 'help'
	else if (tokenized[0]=="help") {
		return 5;
	}
	//Admin cases are equal to 10+
	//In terminal - 'register teams'
	else if (tokenized[0]=="register" && tokenized[1]=="teams") {
		return 10;
	}

	//In terminal - for ex. 'game 1, oct. 27, 2013'
	else if (tokenized[0]== "game" && tokenized.size()>=5){		//size of words
		return 11;
	}
	//In terminal - 
	else if (tokenized[0]== "correction" && tokenized[1]== "game"){
		return 12;
	}

	//In terminal - 
	else if (tokenized[0]== "correction" && tokenized[1].compare("game")!=0){
		return 13;
	}
	// //In terminal - 'exit'
	else if(tokenized[0]=="exit"){
		return 777;
	}

	// return -1 if command is not vaild
	return -1;
}

void addTeam(string database,ifstream* fileReader) {
	//Adds a team. Two things happen next.
	//(-) The team name is appended to the file teams.db.
	//(-) The team is appended to the league.db file and all its league
	//	   parameters are reset to zero.(Points, number of games ..)
	cout<<"Enter team name:"<<endl;
	ofstream outputToTeamsDb;
	//ifstream fileReader;
	string tmp;
	if(database.empty())
	{
	outputToTeamsDb.open("teams.db", ios_base::app);
	}
	else
	{
		// Make dir
		string s = "mkdir .\\"+database;
		system(s.c_str());
		// create file in path
		string tmp=".\\"+database+"\\teams.db";
		outputToTeamsDb.open(tmp, ios_base::app);
	}
	//get a team name from input file if exist. otherwise read inpu from keyboard
	
	if (fileReader->good())
	{
		do{
			getline(*fileReader,tmp);
		}while(tmp.compare("show")==0 || tmp.compare("Show")==0 || 
				tmp.compare("date")==0 || tmp.compare("Date")==0 ||
				tmp.compare("game")==0 || tmp.compare("Game")==0 ||
				tmp.compare("teams")==0 || tmp.compare("Teams")==0);
	}
	else
	{
		do{
			getline(cin,tmp);
		}while(tmp.compare("show")==0 || tmp.compare("Show")==0 || 
				tmp.compare("date")==0 || tmp.compare("Date")==0 ||
				tmp.compare("game")==0 || tmp.compare("Game")==0 ||
				tmp.compare("teams")==0 || tmp.compare("Teams")==0);
	}
	//write the team name into teams database file
	while(tmp.compare(";")!=0){
		outputToTeamsDb<<tmp<<endl;
		if (fileReader->good())
		{
			do{getline(*fileReader,tmp);
				}while(tmp.compare("show")==0 || tmp.compare("Show")==0 || 
				tmp.compare("date")==0 || tmp.compare("Date")==0 ||
				tmp.compare("game")==0 || tmp.compare("Game")==0 ||
				tmp.compare("teams")==0 || tmp.compare("Teams")==0);
		}
		else
		{
			do{getline(cin,tmp);
				}while(tmp.compare("show")==0 || tmp.compare("Show")==0 || 
				tmp.compare("date")==0 || tmp.compare("Date")==0 ||
				tmp.compare("game")==0 || tmp.compare("Game")==0 ||
				tmp.compare("teams")==0 || tmp.compare("Teams")==0);
		}
	}	

	outputToTeamsDb.close();
	
}
//-----------------------------------------------//
//A method that writes a string to games.db file //
//-----------------------------------------------//
void writeToGamesDB(string str,string database)
{
	ofstream of;
	if(database.empty())
	{
	of.open("games.db", ios_base::app);
	}
	else
	{
		
		// create file in path
		string tmp=".\\"+database+"\\games.db";
		of.open(tmp, ios_base::app);
	}
	of<<str<<endl;
	of.close();
}

vector<team> readTeamsFile(string database){
	vector<team> teams;
	ifstream fileReader;
	string tmp;
	team tmpTeam;
	if(database.empty())
	{
	fileReader.open("teams.db");
	}
	else
	{
		
		string tmp=".\\"+database+"\\teams.db";
		fileReader.open(tmp);
	}
	while(getline(fileReader,tmp)) {
			tmpTeam=team(tmp);
		if ( tmpTeam.getName()!="\n" && tmpTeam.getName() !="" && tmpTeam.getName() !=" ")
			teams.push_back(tmpTeam);
	}
	fileReader.close();

	return teams;
}


//-----------------------------------------------//
//This method reads to teams.db file and outputs //
//to the screen a list of all registered games	 //
//-----------------------------------------------//

void showTeams(string outputFileName,string database){
	string teamName;
	ifstream fileReader;
	if(database.empty())
	{
	fileReader.open("teams.db");
	}
	else
	{
		// create file in path
		string tmp=".\\"+database+"\\teams.db";
		fileReader.open(tmp);
	}
	vector<string> tmp;
	while(getline(fileReader,teamName)) {
		tmp.push_back(teamName);
	}
	cout<<"Teams"<<endl<<"-------"<<endl;
	if (outputFileName.compare("@")==0)
		sort(tmp);
	else{
		sort(tmp);
		sortToFile(tmp, outputFileName);
	}
}

void showGame_Between_Two_Teams(string str, league* league, string outputFileName){
        str = delimetersRemover(str, (".,()\0"), ' ' );                        //change from "." and "," to=> ' ' (space)
        vector<string> lineVector = splitStr(str);                                //make vector from string

        // remove from vector teamA name and "-"
        lineVector.erase( lineVector.begin() );
        //save Game temp details
        //game newGame = game();
        int index=-1;
        string teamHome_name="";
        string teamAway_name="";

        //----(
        // Find and create teamA name
        for each (string s in lineVector) {
                if (s== "-"){
                        break;
                }else{
                        index++;
                }
        }
        if (index>-1){        //check for valid input
                //vector <string> teamA;
                                
                for (int i=0; i<=index; i++){
                        if (teamHome_name.size()==0)
                                teamHome_name =lineVector[i];                                        
                        else teamHome_name +=" "+lineVector[i];
                }
                delimetersRemover(teamHome_name, "-\0", ' ');
                //newGame.setHomeGroup(teamA_name);

                //check teamHome_name AAAAAAAAAAAAAAAAA
                bool b= false;
                for each (team t in *league->getTeams() ){
                        if (teamHome_name.compare(t.getName() )==0){
                                b=true;
                                break;
                        }
                }
                if (b==false){
                        cout<<"error: "<<teamHome_name+ " is not vaild (change in the code for dont push this game to vector at the end)"<<endl;
                        //return game();
                }
        }
        //----)

        //----( Way-2
        // Find and create teamB name
        //index+=2;        
        
        // remove from vector teamA name and "-"
        lineVector.erase( lineVector.begin(),lineVector.begin()+index+2 );

        //romeve all "-" from the string and create new string from that and split that to vector with splitStr()
        string s="";
        for (index=0; index< lineVector.size(); index++){
                lineVector.at(index) = delimetersRemover(lineVector.at(index), ("-\0"), ' ' );                        //change from "." and "," to=> ' ' (space)        
                if (s.size()==0)
                                s =lineVector.at(index);                                        
                        else s +=" "+lineVector.at(index);
        }
        lineVector = splitStr(s);

        //identify the nameB
        for (index=0; index<lineVector.size(); index++) {
                if ( atoi(lineVector[index].c_str()) ==0 ) {
                        if (teamAway_name.size()==0)
                                teamAway_name =lineVector[index];                                        
                        else teamAway_name +=" "+lineVector[index];
                }
                else{
                        break;
                }
        }
        //newGame.setAwayGroup(teamB_name);

        //check groupB-name
        bool b= false;
        for each (team t in *league->getTeams()){
                if (teamAway_name.compare(t.getName() )==0){
                        b=true;
                        break;
                }
        }
        if (b==false){
                cout<<"error: "<<teamAway_name+ " does not vaild (change in the code for dont push this game to vector at the end)"<<endl;
                //return game();
        }
        //----)

        // check if 2 groups names are the same
        if (teamHome_name.compare( teamAway_name )==0) {
                cout<<"error: "<<teamAway_name+ " does not vaild (change in the code for dont push this game to vector at the end)"<<endl;
                //return game();
        }

        vector<Date> dateSeen;
        bool seen = false;
        //check for games between those fucking teams:
        for each (team t in *league->getTeams()){
                for each(game* g in *t.getGames()){
                        if (g->getHomeGroup().compare( teamHome_name )==0 && g->getAwayGroup().compare( teamAway_name )==0){
                                // save all date in vector - for dont print out the game twice
                                for each (Date d in dateSeen){
                                        if (g->getDate().operator==(d))
                                                seen = true;
                                }
                                if (seen == false){
                                        cout << g->getHomeGroup()<<" - "<<g->getAwayGroup()<<"  "<<g->getHomeFinalScore()<<":"<<g->getAwayFinalScore()<<" "<<g->getHomeMidScore()<<":"<<g->getAwayMidScore()<<endl;

                                        dateSeen.push_back( g->getDate() );

                                        //write to output file
                                        if (outputFileName.compare("@")!=0 ) {
                                                if (dateSeen.size()==1){
                                                        //first time output - so print also the user command
                                                        ofstream fileWriter;
                                                        fileWriter.open(outputFileName, ios_base::app);
                                                        fileWriter <<str<<endl;
                                                        fileWriter << g->getHomeGroup()<<" - "<<g->getAwayGroup()<<"  "<<g->getHomeFinalScore()<<":"<<g->getAwayFinalScore()<<" "<<g->getHomeMidScore()<<":"<<g->getAwayMidScore()<<endl<<endl;
                                                }
                                                else{
                                                        ofstream fileWriter;
                                                        fileWriter.open(outputFileName, ios_base::app);
                                                        fileWriter<< g->getHomeGroup()<<" - "<<g->getAwayGroup()<<"  "<<g->getHomeFinalScore()<<":"<<g->getAwayFinalScore()<<" "<<g->getHomeMidScore()<<":"<<g->getAwayMidScore()<<endl<<endl;
                                                }
                                        }


                                }
                        }
                }
        }
}

void help(){
	ifstream fileReader;
	string tmp;
	fileReader.open("help.txt");
	while(getline(fileReader,tmp)) {
		cout<<tmp<<endl;
	}
	fileReader.close();
}
//-----------------------------------------------//
// create from line "Game x, mon. xx, xxxx" game //
//object										 //
//-----------------------------------------------//
game saveGameDetailsTemp(vector<string> lineVector){
	Date roundDate = Date();
	
	// RoundNumber
	int round = stoi( lineVector[1] ); //convert from string to int

	// Month
	int month = monthToInt( lineVector[2] );					//change from month(string) to (int)
	if (month>=1 && month<=12)
		roundDate.month = month;
	
	// Day
	int day = stoi( lineVector[3] );
	if (day>=1 && day<=31)
		roundDate.day = day;

	// Year
	int year = stoi( lineVector[4] );
	if (year>0)
		roundDate.year = year;

	//save Game temp details
	game tempDetails = game(round, roundDate);
	//..
	return tempDetails;
}

game functionToCreateNewGameObject(string str , game& gameTempDetails, bool writeToFile, vector<team> teams, league* leaguePtr,string database){
	str = delimetersRemover(str, (".,()\0"), ' ' );			//change from "." and "," to=> ' ' (space)
	vector<string> lineVector = splitStr(str);				//make vector from string

	//save Game temp details
	game newGame = game();
	int index=-1;

	//----(
	// Find and create teamA name
	for each (string s in lineVector) {
		if (s== "-"){
			break;
		}else{
			index++;
		}
	}
	if (index>-1){	//check for valid input
		//vector <string> teamA;
		string teamA_name;
		teamA_name.clear();
		for (int i=0; i<=index; i++){
			if (teamA_name.size()==0)
				teamA_name =lineVector[i];					
			else teamA_name +=" "+lineVector[i];
		}
		delimetersRemover(teamA_name, "-\0", ' ');
		newGame.setHomeGroup(teamA_name);

		//check groupA-name AAAAAAAAAAAAAAAAA
		bool b= false;
		for each (team t in teams){
			if (newGame.getHomeGroup().compare(t.getName() )==0){
				b=true;
				break;
			}
		}
		if (b==false){
			cout<<"error: "<<newGame.getHomeGroup()+ " is not vaild (change in the code for dont push this game to vector at the end)"<<endl;
			return game();
		}
	}
	//----)

	//----( Way-2
	// Find and create teamB name
	string teamB_name;
	teamB_name.clear();
	//index+=2;	
	
	// remove from vector teamA name and "-"
	lineVector.erase( lineVector.begin(),lineVector.begin()+index+2 );

	//romeve all "-" from the string and create new string from that and split that to vector with splitStr()
	string s="";
	for (index=0; index< lineVector.size(); index++){
		lineVector.at(index) = delimetersRemover(lineVector.at(index), ("-\0"), ' ' );			//change from "." and "," to=> ' ' (space)	
		if (s.size()==0)
				s =lineVector.at(index);					
			else s +=" "+lineVector.at(index);
	}
	lineVector = splitStr(s);

	//identify the nameB
	for (index=0; index<lineVector.size(); index++) {
		if ( atoi(lineVector[index].c_str()) ==0 ) {
			if (teamB_name.size()==0)
				teamB_name =lineVector[index];					
			else teamB_name +=" "+lineVector[index];
		}
		else{
			break;
		}
	}
	newGame.setAwayGroup(teamB_name);

	//check groupB-name
	bool b= false;
	for each (team t in teams){
		if (newGame.getAwayGroup().compare(t.getName() )==0){
			b=true;
			break;
		}
	}
	if (b==false){
		cout<<"error: "<<newGame.getAwayGroup()+ " does not vaild (change in the code for dont push this game to vector at the end)"<<endl;
		return game();
	}
	//----)

	// check if 2 groups names are the same
	if (newGame.getHomeGroup().compare( newGame.getAwayGroup() )==0) {
		cout<<"error: "<<newGame.getAwayGroup()+ " does not vaild (change in the code for dont push this game to vector at the end)"<<endl;
		return game();
	}

	//----(
	// check teams scores
	if (b==true){
	newGame.setHomeFinalScore( atoi(lineVector[index].c_str()) );
		
	index++;
	newGame.setAwayFinalScore( atoi(lineVector[index].c_str()) );
		
	index++;
	newGame.setHomeMidScore( atoi(lineVector[index].c_str()) );
		
	index++;
	newGame.setAwayMidScore( atoi(lineVector[index].c_str()) );
	//----)

	// Get Round & Date from temp_game_object (that save the date and round)
	newGame.setRoundNum( gameTempDetails.getRoundNum() );
	newGame.setDate( gameTempDetails.getDate() );
	
	if (writeToFile==true)
		writeToGamesDB(str,database);
	
	return newGame;

	}
return game();
}

//----------------------------------------------//
//This methods 'readGameAtRound' recieves 'line'//
//input and make all things to add this game to //
//system. 										//
//'typeFile' - to identify witch of function we //
//need to execute on this line.					//
// typeInput=1  for input from keyboard			//
// typeInput=2  for read from db				//
//----------------------------------------------//
vector<game> readGameAtRound(string line, int typeInput, bool writeToFile, int* lastRound, vector<team>* teamsPtr, league* leaguePtr,string database,ifstream* fileReader) {	
	vector<game> v;
	// type 1 from menu
	if (typeInput==1){
		line = delimetersRemover(line, (",.\0"), ' ' );			//change from "." and "," to=> ' ' (space)
		vector<string> lineVector = splitStr(line);		//make vector from string
		game gameTempDetails = saveGameDetailsTemp( lineVector );		//create gameTemp object with only this Details: game (round, day, month, year)		
		
		// run on the teams vector in league object and retrieve the K round date
		//compare round k date with k+1 round date
		//if (k date)>=(k+1 date) skip to next iteration in this loop.
		//else -> write this game to the data base
		int kRound=0;
		bool breakLoop=false;
		vector<team>* teams = leaguePtr->getTeams();
		//scan all teams vector in league object
		for (int i=0; i<teams->size(); i++)
		{
				//scan all games in a specifig team.
				for (int j=0; j<teams->at(i).getGames()->size() ; j++)
				{
					//check if 
					if (teams->at(i).getGames()->at(j)->getRoundNum() + 1 == gameTempDetails.getRoundNum()  && breakLoop==false)
					{
						if (teams->at(i).getGames()->at(j)->getDate() >= gameTempDetails.getDate())
						{
							breakLoop=true;
							cout<< "Error: This round date is not after the previous round date."<<endl;
							break;
						}
						
					}
					else if (teams->at(i).getGames()->at(j)->getRoundNum() == gameTempDetails.getRoundNum() && breakLoop==false)
					{
						if ((teams->at(i).getGames()->at(j)->getDate() > gameTempDetails.getDate()))
						{
							breakLoop=true;
							cout<< "Error: This round date is not after the previous round date."<<endl;
							break;
						}
					}
				if (breakLoop==true)
					break;
				
				}
			}
			/*else
			{
				breakLoop=
			}*/
		//}

		
		//if (&gameTempDetails != NULL  && breakLoop==false){
		if (&gameTempDetails != NULL && breakLoop==false){
			//read all games
			
			if (gameTempDetails.getRoundNum()<= *lastRound+1 ){
				writeToGamesDB(line,database);
				if (gameTempDetails.getRoundNum() == *lastRound+1)
					*lastRound+=1;
				// Add game lines here until ";"
				string str;
				while(str!= ";"){
					str.clear();
					if (fileReader->good())
					{
						getline(*fileReader,str);
					}
					else
					{
						getline(cin,str);
					}
					
					if (str!= ";"){
						game newGame = functionToCreateNewGameObject(str, gameTempDetails, writeToFile, *teamsPtr, leaguePtr,database);
						// IF team names are valid, push the game to league ->each team -> game vector;
						if (newGame.getHomeGroup().compare("")!=0 && newGame.getAwayGroup().compare("")!=0){
							for(int i=0; i<leaguePtr->getTeams()->size(); i++)
							{
								if (leaguePtr->getTeams()->at(i).getName().compare(newGame.getHomeGroup())==0  
									|| leaguePtr->getTeams()->at(i).getName().compare(newGame.getAwayGroup())==0)
								{
									leaguePtr->getTeams()->at(i).getGames()->push_back(new game(newGame));
									// if this is the home team
									if(leaguePtr->getTeams()->at(i).getName().compare(newGame.getHomeGroup())==0)
									{
										//add points to "points for" and points to "points against"
										leaguePtr->getTeams()->at(i).setPointsFor(leaguePtr->getTeams()->at(i).getPointsFor() + newGame.getHomeFinalScore());
										leaguePtr->getTeams()->at(i).setPointsAgainst(leaguePtr->getTeams()->at(i).getPointsAgainst() + newGame.getAwayFinalScore());
										//if home team is the winner then add 2 points to league points.
										if(newGame.getHomeFinalScore() > newGame.getAwayFinalScore())
											leaguePtr->getTeams()->at(i).setLeaguePoints(leaguePtr->getTeams()->at(i).getLeaguePoints() + 2);
									}

									//if this is the away team
									if(leaguePtr->getTeams()->at(i).getName().compare(newGame.getAwayGroup())==0)
									{
										//add points to "points for" and points to "points against"
										leaguePtr->getTeams()->at(i).setPointsFor(leaguePtr->getTeams()->at(i).getPointsFor() + newGame.getAwayFinalScore());
										leaguePtr->getTeams()->at(i).setPointsAgainst(leaguePtr->getTeams()->at(i).getPointsAgainst() + newGame.getHomeFinalScore());
										//if away team is the winner then add 2 points to league points.
									if(newGame.getHomeFinalScore() < newGame.getAwayFinalScore())
										leaguePtr->getTeams()->at(i).setLeaguePoints(leaguePtr->getTeams()->at(i).getLeaguePoints() + 2);
									}
								}
							}
						}
					}
				}
			}else{
				cout<<"error: you try to insert a game round muche higher. \nlast round was "<<*lastRound<<" (change in the code to return null)"<<endl;
			}
		}
	}else if(typeInput==2){ // type 2 from file "games.db"
		ifstream fileReader;
		string tmp;
		game gameTempDetails;
		fileReader.clear();
	if(database.empty())
	{
		fileReader.open("games.db");
	}
	else
	{
		
		// create file in path
		string tmp=".\\"+database+"\\games.db";
		fileReader.open(tmp);
	}
		while (fileReader.good() ){		
			while(getline(fileReader,tmp) /*  && splitStr(tmp).at(0)!= ";"  */) {			//delete all spaces in tmp
				vector<string> newVector= splitStr(tmp);
				if (newVector.empty() || newVector.at(0)== ";")
					break;

				// if (fileReader!=NULL && tmp.find("session",0) == std::string::npos){
					//read game round & date
					if (tmp.find("game",0) != std::string::npos ){
						tmp = delimetersRemover(tmp, (",.\0"), ' ' );			//change from "." and "," to=> ' ' (space)
						//writeToGamesDB(tmp);
						vector<string> lineVector = splitStr(tmp);		//make vector from string
						gameTempDetails = saveGameDetailsTemp( lineVector );		//create gameTemp object with only this Details: game (round, day, month, year)		
						if ( *lastRound < gameTempDetails.getRoundNum())
							*lastRound = gameTempDetails.getRoundNum();

					}
					//read specific game
					else if(&gameTempDetails != NULL){
						//read all games
						//cout<<"create gameTemp\n";
			
						// Add game lines here until ";"
						game newGame= functionToCreateNewGameObject(tmp, gameTempDetails, writeToFile, *teamsPtr, leaguePtr,database);
						//check if (game == NULL) because one of the teams does not apear in 'teams vector'
						v.push_back( newGame );
					}
				//}
			}
		}
		fileReader.close();
	}
	return v;
}

void user_menu(league* league, const int session, const vector<game>* games, int* lastRound, int argc, char** argv,vector<team>* teams, vector<game>* allGames)
{
	string str;
	int caseNum;
	string database =	analyzeDbFromArgv(argc,argv);
	string outputFileName =	analyzeOutputFromArgv(argc,argv);
	string inputFileName =	analyzeInputFromArgv(argc,argv);
	ifstream fileReader;
	bool wasInputFile = false;
	
	// open input.txt
	fileReader.open(inputFileName);
	if (fileReader.fail())
	{
		cout<<"Failed opening input file : "<<inputFileName<<endl;
	}

	do {
	//if input file is passed as an argument thn initialize input stream
	//and read & xecute lines from it. otherwise, get input from keyboard.
	if (inputFileName.compare("@")!=0 && fileReader.good())
	{
		wasInputFile = true;
		getline(fileReader,str);
	}	
	else
	{
		if (wasInputFile==true) {
			fileReader.close();
					//4924/4974
			break;
		}
		  //closing stream to input file because it reached EOF
		cout<<"type command or type 'help' for list of valid commands."<<endl;
		getline(cin,str);
	}
		str = delimetersRemover(str, ",.()\0", ' ' );	//delete "."   ","   "("   ")"
		caseNum = check_input(str);
		if (caseNum != -1) { // vaild command

			switch(caseNum) {
			case 1: 	//SHOW TEAMS
				showTeams(outputFileName,database);
				break;

			case 2: 
				league->createLeagueTable(outputFileName);
				break;

			case 3: 
				showGame_Between_Two_Teams(str, league, outputFileName);
				break;

			case 5:		// help command - shows content of help file
				help();
				break;

			case 10:
				if (session == 1 && games->size()==0){
				addTeam(database,&fileReader);
				*teams = readTeamsFile(database);
				vector<game> allGames= readGameAtRound("dont need to send here string because send 2 as parameter",2, false, lastRound, teams, NULL,database,&fileReader);	//check the team.name from teamsVector source that created.
				league->setTeams(teams); //construct a league with teams objects. teams dont have games yet.
				league->init(&allGames);		//? add to every team in the league it's games from vector games?
				}
				else
					cout<<"Error : Teams list is sealed."<<endl;
				break;

			case 11:	//read game
				readGameAtRound(str,1, true, lastRound, league->getTeams(), league,database,&fileReader);	//true- write to file
				break;

			case 12:	//game correction
				correctGameDate(str, league->getTeams(),database);	
				break;

			case 13:	//match score correction
				readGameAtRound(str,1, true, lastRound, league->getTeams(), league,database,&fileReader);
				break;
			}


		}else
			cout<<"Invalid action"<<endl;
	}while (caseNum != 777);
}




int incrementSession()
{
	int session=-1;
	ifstream fileReader;
	ofstream fileWriter;
	string tmp;
	fileReader.open("session.db");
	if (fileReader.fail())
	{
		fileWriter.open("session.db");
		fileWriter<<1;
		session=1;
		fileWriter.close();
	}
	else
	{
		getline(fileReader, tmp);
		if (tmp.empty())
		{
			session=0;
			session++;
		}
		else
		{
			session = stoi(tmp) + 1;
		}
		fileWriter.open("session.db");
		fileWriter<<session;
		fileWriter.close();
	}
	return session;
}

int main(int argc , char* argv[]) {

	ifstream fileReader;
	int session = incrementSession();
	cout<<"\t\t\t-Welcome to league tool -"<<endl;
	string database=analyzeDbFromArgv(argc,argv);
	int lastRound= 0;
	vector<team> teams = readTeamsFile(database);
	vector<game> allGames= readGameAtRound("dont need to send here string because send 2 as parameter",2, false, &lastRound, &teams, NULL,database,&fileReader);	//check the team.name from teamsVector source that created.
	
	league league(&teams); //construct a league with teams objects. teams dont have games yet.
	league.init(&allGames);		//? add to every team in the league it's games from vector games?
	user_menu(&league, session, &allGames,&lastRound,argc,argv,&teams,&allGames);

	system("pause");
	return 0;

}