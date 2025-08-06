#include "../inc/Character/Toad.h"
#include "ResrcManager.h"
#include "../inc/World/GameClock.h"

void Toad::Update() {
    const float deltaTime = GetFrameTime();
    if (state == OBJECT_STATE_TO_BE_REMOVED) {
        sprite = nullptr;
        return;
    }
    if (state == OBJECT_STATE_DYING) {
        sprite = &ResrcManager::GetInstance().getTexture("TOAD_DIE");
        return;
    }

    switch(CharState) { // Corrected from TOADState to TOADState
        case SMALL:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0 && !isDucking) {
                    if ( direction == DIRECTION_RIGHT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_0_RIGHT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_1_RIGHT");
                        }
                    }
                    else  {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_0_LEFT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_1_LEFT");
                        }
                    }
                }
                if(vel.x == 0 && !isDucking) {
                    if ( direction == DIRECTION_RIGHT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_0_RIGHT");
                    } else if ( direction == DIRECTION_LEFT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_0_LEFT");
                    }
                }
            }

            if (state == OBJECT_STATE_JUMPING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_JUMPING_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_JUMPING_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_FALLING_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_FALLING_LEFT");
                }
            }
            if (state == OBJECT_STATE_DYING) {
                sprite = &ResrcManager::GetInstance().getTexture("TOAD_DIE");
            }
            if (state == OBJECT_STATE_VICTORY) {
                sprite = &ResrcManager::GetInstance().getTexture("SMALLTOAD_VICTORY");
            }
            break;
        }
        case BIG:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0){
                    if ( direction == DIRECTION_RIGHT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_0_RIGHT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_1_RIGHT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_2_RIGHT");
                        }
                    }
                    else if ( direction == DIRECTION_LEFT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_0_LEFT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_1_LEFT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_2_LEFT");
                        }
                    }
                }
                if(vel.x == 0) {
                    if ( direction == DIRECTION_RIGHT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_0_RIGHT");
                    } else if ( direction == DIRECTION_LEFT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_0_LEFT");
                    }
                }
            }

            if (state == OBJECT_STATE_JUMPING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_JUMPING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_JUMPING_0_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_FALLING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_FALLING_0_LEFT");
                }
            }
            if (state == OBJECT_STATE_DYING) {
                sprite = &ResrcManager::GetInstance().getTexture("TOAD_DIE");
            }
            if (state == OBJECT_STATE_VICTORY) {
                sprite = &ResrcManager::GetInstance().getTexture("SUPER_TOAD_VICTORY");
            }
            break;
        }
        case FIRE:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0){
                    if ( direction == DIRECTION_RIGHT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_0_RIGHT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_1_RIGHT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_2_RIGHT");
                        }
                    }
                    else if ( direction == DIRECTION_LEFT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_0_LEFT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_1_LEFT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_2_LEFT");
                        }
                    }
                }
                if(vel.x == 0) {
                    if ( direction == DIRECTION_RIGHT) {
                        sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_0_RIGHT");
                    } else if ( direction == DIRECTION_LEFT) {
                        sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_0_LEFT");
                    }
                }
            }

            if (state == OBJECT_STATE_JUMPING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_JUMPING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_JUMPING_0_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_FALLING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_FALLING_0_LEFT");
                }
            }

            if (state == OBJECT_STATE_DYING) {
                sprite = &ResrcManager::GetInstance().getTexture("TOAD_DIE");
            }
            if (state == OBJECT_STATE_VICTORY) {
                sprite = &ResrcManager::GetInstance().getTexture("FIRE_TOAD_VICTORY");
            }
            break;
        }
    }
}



