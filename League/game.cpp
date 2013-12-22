#include "game.h"

using namespace std;

game::game (int roundNum, Date date){
        this->roundNum = roundNum;
        this->date = Date(date.day, date.month, date.year);
}
game::game(const game& g)
{
	date = g.date;
	homeGroup=g.homeGroup;
	awayGroup=g.awayGroup;
	roundNum = g.roundNum;
    homeMidScore=g.homeMidScore;
    homeFinalScore=g.homeFinalScore;
    homeExtensionScore=g.homeExtensionScore;
    awayMidScore=g.awayMidScore;
    awayFinalScore=g.awayFinalScore;
    awayExtensionScore=g.awayExtensionScore;
}
game::game(){
        date = Date(0,0,0);
		homeGroup="";
        awayGroup="";
        roundNum = 0;
        homeMidScore=0;
        homeFinalScore=0;
        homeExtensionScore=0;
        awayMidScore=0;
        awayFinalScore=0;
        awayExtensionScore=0;
}