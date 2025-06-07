#include "Mario.h"

// full constructor
Mario::Mario(Vector2 pos, int lives, MarioState form)
    : Object(pos, Vector2{50, 50}, Vector2{0, 0}, WHITE, 0.1f, 4, DIRECTION_RIGHT),
      lives(lives), 
      accelerationX(660.5f), 
      maxSpeedX(500.0f), 
      SpeedY(10.0f),
      marioState(form), 
      AdditionalState(SMALL),
      isDucking(false) 
      { // Removed direction initialization
    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT");
    if ( form == SMALL)
        SetSize(Vector2{32, 40});
    else
        SetSize(Vector2{32, 56});
    state = OBJECT_STATE_ON_GROUND;
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

Mario::Mario() : lives(3), accelerationX(0), maxSpeedX(0), SpeedY(0), isDucking(false), marioState(SMALL), AdditionalState(SMALL) {
    // Initialize default Mario state
}

Mario::~Mario() {
    // Destructor
}

void Mario::SetLives(int lives) {
    this->lives = lives;
}

void Mario::SetSprite(Texture2D sprite) {
    this->sprite = &sprite; // Assuming sprite is a Texture2D reference
}
void Mario::SetState(ObjectState state) {
    this->state = state;
}

int Mario::GetLives() const
{
    return lives;
}

bool Mario::GetIsDucking() const
{
    return isDucking;
}

std::list<Fireball *> *Mario::GetFireballs()
{
    return &fireballs;
}

void Mario::jump() {
    vel.y = -SpeedY; // 
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

void Mario::Duck()
{
    this->isDucking = true;
}

void Mario::HandleInput()
{
    const float deltaTime = GameClock::getInstance().FIXED_TIME_STEP;

    if (IsKeyDown(KEY_RIGHT)) moveRight();
    else if(IsKeyDown(KEY_LEFT)) moveLeft();
    else stopMoving();
    
    if(state == OBJECT_STATE_ON_GROUND) {
        if( IsKeyPressed(KEY_UP)) {
            jump();
        }
        if (IsKeyPressed(KEY_DOWN)) {
            Duck();
        } else isDucking = false; // Reset ducking state if not pressing down
    }
    if(IsKeyPressed(KEY_SPACE)){
        changeToBig();
    }

    if(IsKeyPressed(KEY_F)){
        changetoFire();
    }
    if (marioState == FIRE) {
        if ( IsKeyPressed(KEY_Z)) {
            // fire();
        }
    }
}

void Mario::Update() {
    // UpdateCollisionProbes();
    // UpdateStateAndPhysic();
    // HandleInput();
    const float deltaTime = GetFrameTime();
    switch(marioState) { // Corrected from MarioState to marioState
        case SMALL:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0 && !isDucking) {
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
                if(vel.x == 0 && !isDucking) {
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
        case FIRE:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0){
                    if ( direction == DIRECTION_RIGHT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_0_RIGHT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_1_RIGHT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_2_RIGHT");
                        }
                    }
                    else if ( direction == DIRECTION_LEFT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_0_LEFT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_1_LEFT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_2_LEFT");
                        }
                    }
                }
                if(vel.x == 0) {
                    if ( direction == DIRECTION_RIGHT) {
                        sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_0_RIGHT");
                    } else if ( direction == DIRECTION_LEFT) {
                        sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_0_LEFT");
                    }
                }
            }

            if (state == OBJECT_STATE_JUMPING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_JUMPING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_JUMPING_0_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_FALLING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_FALLING_0_LEFT");
                }
            }
            break;
        }
    }
}

void Mario::UpdateStateAndPhysic() {
    HandleInput();
    const float deltaTime = GetFrameTime();
    switch(marioState) { // Corrected from MarioState to marioState
        case SMALL:
        {
            if (state == OBJECT_STATE_ON_GROUND) {
                if(vel.x != 0 && !isDucking) {
                    frameTime = 0.1;
                    frameAcumulator += deltaTime;
                    maxFrames = 1;
                    if (frameAcumulator > frameTime) {
                        currentFrame++;
                        if (currentFrame > maxFrames) {
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
                if(vel.x != 0 && !isDucking) {
                    frameTime = 0.1;
                    frameAcumulator += deltaTime;
                    maxFrames = 2;
                    if (frameAcumulator > frameTime) {
                        currentFrame++;
                        if (currentFrame > maxFrames) {
                            currentFrame = 0;
                        }
                        frameAcumulator -= frameTime;
                    }
                }
            }
            break;
        }
        case FIRE:
        {
            if (state == OBJECT_STATE_ON_GROUND) {
                if(vel.x != 0 && !isDucking) {
                    frameTime = 0.1;
                    frameAcumulator += deltaTime;
                    maxFrames = 3;
                    if (frameAcumulator > frameTime) {
                        currentFrame++;
                        if (currentFrame > maxFrames) {
                            currentFrame = 0;
                        }
                        frameAcumulator -= frameTime;
                    }
                }
            }
            break;
        }
    }

    if (vel.y > 0) { // falling then y > 0 because y axis is inverted
        state = OBJECT_STATE_FALLING;
    }
    if ( state == OBJECT_STATE_ON_GROUND) {
        vel.y = 0;
    }

    vel.y += GameWorld::GetGravity() * deltaTime; // Apply gravity

    Object::UpdateStateAndPhysic(); // Call the base class method to handle position updates and collision checks
    // because Object::UpdateStateAndPhysic() will update the position based on velocity

    // Update fireballs
    for (auto i = fireballs.begin(); i != fireballs.end();)
    {
        Fireball* fireball = *i;
        if(fireball->isOutOfDistance()){
            delete fireball;
            fireball = nullptr;
            i = fireballs.erase(i);
        }
        else{
            fireball->UpdateStateAndPhysic();
            ++i;
        }
    }
    UpdateCollisionProbes(); // Update the position of collision probes
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

void Mario::changeToSmall()
{
    if (marioState == BIG) {
        marioState = SMALL;
        SetSize(Vector2{32, 40}); // Update size for SMALL Mario
    }
}

void Mario::changetoFire()
{
    if (marioState == SMALL || marioState == BIG) {
        marioState = FIRE;
        SetSize(Vector2{32, 56}); // Update size for FIRE Mario
        sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_0_RIGHT");
    }
}

void Mario::UpdateCollisionProbes() {
    // Update the position of collision probes based on Mario's position and size
    cpN.setPos(Vector2{ pos.x + size.x / 2 - 5, pos.y });
    cpS.setPos(Vector2{ pos.x + size.x / 2 - 5, pos.y + size.y - 5 });
    cpE.setPos(Vector2{ pos.x + size.x - 5, pos.y + size.y / 2 - 5 });
    cpW.setPos(Vector2{ pos.x, pos.y + size.y / 2 - 5 });
}
