#include "Item.h"
#include "ResrcManager.h"
#include <vector>
using namespace std;

class Coin : public Item {
private:
    vector<Texture2D*> coinFrames; 
public:
    Coin(Vector2 pos);
    ~Coin() override = default;

    void updateMario(Mario& mario) override;     
    void playCollisionSound() override;          

    void draw() override;                        
    void Update() override;                      
};