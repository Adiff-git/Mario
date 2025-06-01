#include "Mario.h"

// full constructor
Mario::Mario(Vector2 pos, int lives, MarioState form)
    : Object(pos, Vector2{50, 50}, Vector2{0, 0}, WHITE, 0.1f, 4, DIRECTION_RIGHT),
      lives(lives), accelerationX(660.5f), maxSpeedX(500.0f), SpeedY(10.0f),
      marioState(form), AdditionalState(SMALL) { // Removed direction initialization
    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT");
    if ( form == SMALL)
        SetSize(Vector2{32, 40});
    else
        SetSize(Vector2{32, 56});

    cpN.setSize(Vector2{size.x/2, 1});
    cpS.setSize(Vector2{size.x/2, 1});
    cpE.setSize(Vector2{1, size.y/2-5});
    cpW.setSize(Vector2{1, size.y/2-5});
    UpdateCollisionProbes();
    cpN.setColor(RED);
    cpS.setColor(RED);
    cpE.setColor(RED);
    cpW.setColor(RED);

}

Mario::Mario() : Mario(Vector2{0, 0}, 3, SMALL) {
    // Default constructor initializes with default values
}

Mario::~Mario() {
    // Destructor
}

void Mario::jump() {
    vel.y = -SpeedY; // Set a negative speed for jumping
    state = OBJECT_STATE_JUMPING;
}

void Mario::moveLeft() {
    if ( direction == DIRECTION_RIGHT) {
        direction = DIRECTION_LEFT;
        vel.x = 0;
        frameAcumulator = 0;
    }

    if( vel.x + accelerationX * GameClock::getInstance().FIXED_TIME_STEP <= -maxSpeedX) {
        vel.x = -maxSpeedX;
    } else {
        vel.x -= accelerationX* GameClock::getInstance().FIXED_TIME_STEP;
    }
}

void Mario::moveRight() {
    if ( direction == DIRECTION_LEFT) {
        direction = DIRECTION_RIGHT;
        vel.x = 0;
        frameAcumulator = 0;
    }

    if( vel.x + accelerationX * GameClock::getInstance().FIXED_TIME_STEP >= maxSpeedX) {
        vel.x = maxSpeedX;
    } else {
        vel.x += accelerationX* GameClock::getInstance().FIXED_TIME_STEP;
    }
}

void Mario::stopMoving() {
    vel.x = 0;
    frameAcumulator = 0;
}

void Mario::HandleInput()
{
    const float deltaTime = GameClock::getInstance().FIXED_TIME_STEP;

    if (IsKeyDown(KEY_RIGHT)) moveRight();
    else if(IsKeyDown(KEY_LEFT)) moveLeft();
    else stopMoving();
    
    if(state == OBJECT_STATE_ON_GROUND) {
        if( IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            jump();
        }
    }
    if(IsKeyPressed(KEY_SPACE)){
        changeToBig();
    }
}

void Mario::Update() {
    UpdateCollisionProbes();
    UpdateStateAndPhysic();
    HandleInput();
    const float deltaTime = GetFrameTime();
    switch(marioState) { // Corrected from MarioState to marioState
        case SMALL:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0){
                    if ( direction == DIRECTION_RIGHT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_1_RIGHT");
                        }
                    }
                    else if ( direction == DIRECTION_LEFT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_0_LEFT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_1_LEFT");
                        }
                    }
                }
                if(vel.x == 0) {
                    if ( direction == DIRECTION_RIGHT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT");
                    } else if ( direction == DIRECTION_LEFT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_0_LEFT");
                    }
                }
            }

            if (state == OBJECT_STATE_JUMPING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_JUMPING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_JUMPING_0_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_FALLING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_FALLING_0_LEFT");
                }
            }
            break;
        }
        case BIG:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0){
                    if ( direction == DIRECTION_RIGHT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_0_RIGHT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_1_RIGHT");
                        }
                    }
                    else if ( direction == DIRECTION_LEFT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_0_LEFT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_1_LEFT");
                        }
                    }
                }
                if(vel.x == 0) {
                    if ( direction == DIRECTION_RIGHT) {
                        sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_0_RIGHT");
                    } else if ( direction == DIRECTION_LEFT) {
                        sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_0_LEFT");
                    }
                }
            }

            if (state == OBJECT_STATE_JUMPING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_JUMPING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_JUMPING_0_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_FALLING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("BIG_MARIO_FALLING_0_LEFT");
                }
            }
            break;
        }
    }
}

void Mario::UpdateStateAndPhysic() {
    HandleInput();
    const float deltaTime = GameClock::getInstance().FIXED_TIME_STEP;
    switch(marioState) { // Corrected from MarioState to marioState
        case SMALL:
        {
            if (state == OBJECT_STATE_ON_GROUND) {
                if(vel.x != 0) {
                    frameTime = 0.1;
                    frameAcumulator += deltaTime;
                    maxFrames = 1;
                    if (frameAcumulator >= frameTime) {
                        currentFrame++;
                        if (currentFrame >= maxFrames) {
                            currentFrame = 0;
                        }
                        frameAcumulator -= frameTime;
                    }
                }
            }
            break; // Added missing break
        }
        case BIG:
        {
            if (state == OBJECT_STATE_ON_GROUND) {
                if(vel.x != 0) {
                    frameTime = 0.1;
                    frameAcumulator += deltaTime;
                    maxFrames = 2;
                    if (frameAcumulator >= frameTime) {
                        currentFrame++;
                        if (currentFrame >= maxFrames) {
                            currentFrame = 0;
                        }
                        frameAcumulator -= frameTime;
                    }
                }
            }
            break;
        }
    }
}

void Mario::draw() {
    Update();
    DrawTexture(*sprite, pos.x, pos.y, WHITE);
    cpN.draw();
    cpS.draw();
    cpE.draw();
    cpW.draw();
}

void Mario::changeToBig() {
    if (marioState == SMALL) {
        marioState = BIG;
        SetSize(Vector2{32, 56}); // Update size for BIG Mario
    }
}

void Mario::UpdateCollisionProbes() {
    // Update the position of collision probes based on Mario's position and size
    cpN.setPos(Vector2{ pos.x + size.x / 2 - 5, pos.y });
    cpS.setPos(Vector2{ pos.x + size.x / 2 - 5, pos.y + size.y - 5 });
    cpE.setPos(Vector2{ pos.x + size.x - 5, pos.y + size.y / 2 - 5 });
    cpW.setPos(Vector2{ pos.x, pos.y + size.y / 2 - 5 });
}
