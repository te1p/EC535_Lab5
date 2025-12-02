#include "score.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

Score::Score(){Score::points = 0;}

void Score::incrementPoints(unsigned int newPoints)
{
    Score::points = Score::points + newPoints;
    std::cout << "Points: ";
    std::cout << Score::points;
    std::cout << '\n';
}

void Score::decrementPoints(unsigned int newPoints)
{
    Score::points = Score::points - newPoints;
    std::cout << "Points: ";
    std::cout << Score::points;
    std::cout << '\n';
}

void Score::resetPoints()
{
    Score::points = 0;
}

unsigned int Score::getPoints()
{return Score::points;}

//Important for all file operations!!!!
//The functions operate a scores.txt that is inside the build
//Not the file inside the source
void Score::saveScore()
{
    std::fstream f;
    f.open("scores.txt", std::ios::app);
    if (!f)
    {
        std::cout<< "Could not find the file\n";
    }
    else
    {
        f << Score::points;
        f << '\n';
        std::cout << "Score appended successfully\n";
    }

    f.close();
}

std::vector<unsigned int> Score::loadScore()
{
    std::vector<unsigned int> scores;
    std::string line;
    std::fstream f;
    f.open("scores.txt", std::ios::in);
    if (!f)
    {
        std::cout<< "Could not find the file";
    }
    else
    {
        while(f >> line)
        {
            scores.push_back(std::strtoul(line.c_str(), 0L, 10));
        }
        Score::scoresVector = scores;
    }

    f.close();
    return scores;
}

void Score::cleanScore()
{
    std::fstream f;
    f.open("scores.txt", std::ios::out);
    if (!f)
    {
        std::cout<< "Could not find the file - clean error\n";
    }
    else
    {
        f << "";
        std::cout << "Score reset\n";
    }

    f.close();

}

unsigned int Score::getHighscore()
{
    unsigned int highscore = 0;

    if (Score::scoresVector.empty())
    {
        std::cout << "Empty Vector - cannot find Highscore\n";
        return 0;
    }
    highscore = *std::max_element(Score::scoresVector.begin(), Score::scoresVector.end());

    return highscore;
}
