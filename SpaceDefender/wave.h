#ifndef WAVE_H
#define WAVE_H

class Wave
{
private:
    unsigned int difficulty;
    unsigned int waveLenght;
    int stageType;


public:
    Wave();
    void increaseDiff(unsigned int change);
    void decreaseDiff(unsigned int change);
    void setStageType(int newStageType);
    void startWave(); //This function should start the wave and start spawning enemies in a random pattern that is affected by difficulty and/or stage type
    void startBossWave(); //Special wave for bosses
};

#endif // WAVE_H
