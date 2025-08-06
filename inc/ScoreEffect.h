#pragma once
#include "SmokeEffect.h"
#include <string>

// Forward declaration
class Character;

class ScoreEffect : public SmokeEffect {
private:
    int scoreValue;
    Vector2 velocity;
    std::string scoreText;
    
    static Character* currentPlayer; // Static pointer để truy cập player
    
public:
    ScoreEffect(Vector2 pos, int score);
    ~ScoreEffect() = default;
    
    // Static method để set current player
    static void SetCurrentPlayer(Character* player) { currentPlayer = player; }
    
    void Update() override;
    void Draw() override;
    void UpdateStateAndPhysic() override;
    
    void SetScore(int score) { scoreValue = score; }
    int GetScore() const { return scoreValue; }
};
