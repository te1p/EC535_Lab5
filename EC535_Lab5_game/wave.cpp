#include "wave.h"

Wave::Wave() {}

void Wave::increaseDiff(unsigned int change)
{
    Wave::difficulty = Wave::difficulty + change;
}

void Wave::decreaseDiff(unsigned int change)
{
    Wave::difficulty = Wave::difficulty - change;
}

void Wave::setStageType(int newStageType)
{
    Wave::stageType = newStageType;
}

void Wave::startWave()
{
    //startWave controls how a wave operates
    //a wave is a part of the game where enemy spawn in random patterns
    //each wave is controlled by diffiuclty and stageType
    //difficulty & stageType affect the wave only when it starts
    //waveLength will decide how long the wave will go on for. However, not all waves will need to be timed
    //(I could make seperate function for timed waves)
    //However, the difficulty & stageType could be modified during an ongoing stage to affect the next wave

    //startWave will spawn enemies in random pattern at the top of the screen
    //The type of enemy will be decided semi-randomly -> a group of enemies will be chosen based on the stage type & difficulty
    //There will be random intervals between enemies spawning and the amount ->
    //enemies can spawn in groups and each group will be followed by another after 1 to 20 seconds (not sure about time, testing needed)
    //time intervals and enemy number can be affected by diffiuclty
    //On top of that, difficulty will decide how much groups spawn in the given wave

    //General structure:
    //loop for x amount of times
    //spawn a group of y enemies
    //wait for z seconds
    //end loop
}

void Wave::startBossWave()
{
    //similar idea to the startWave, but it will always spawn a boss enemy that will vary based on stageType
    //Additionally there will be less standard enemies, as the focus of the wave is the boss

    //General structure:
    //spawn a boss
    //loop for x amount of times
    //spawn a group of y enemies
    //wait for z seconds
    //end loop

}
