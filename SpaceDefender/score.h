#ifndef SCORE_H
#define SCORE_H
#include <vector>

// Tracks score changes and stores previous scores for basic persistence.
class Score
{
private:
    unsigned int points = 0;                 // current score value
    std::vector<unsigned int> scoresVector;  // stored past scores (for highscore)

public:
    Score();                                 // initializes score system

    void incrementPoints(unsigned int newPoints = 1); //add to score
    void decrementPoints(unsigned int newPoints = 1); //subtract from score
    void resetPoints();                                 //clear current score
    unsigned int getPoints();                           //return current score

    void saveScore();                                   //push score into vector
    std::vector<unsigned int> loadScore();              //return saved scores
    void cleanScore();                                  //clear score history
    unsigned int getHighscore();                        //compute max saved score
};

#endif // SCORE_H
