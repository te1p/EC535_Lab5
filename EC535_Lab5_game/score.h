#ifndef SCORE_H
#define SCORE_H
#include <vector>

class Score
{
private:
    unsigned int points = 0;
    std::vector<unsigned int> scoresVector;

public:
    Score();
    void incrementPoints(unsigned int newPoints = 1);
    void decrementPoints(unsigned int newPoints = 1);
    void resetPoints();
    unsigned int getPoints();
    void saveScore();
    std::vector<unsigned int> loadScore();
    void cleanScore();
    unsigned int getHighscore();
};

#endif // SCORE_H
