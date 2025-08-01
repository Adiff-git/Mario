#include "../inc/Character/Mario.h"
#include "ResrcManager.h"


void Mario::Update() {
    const float deltaTime = GetFrameTime();
    if (state == OBJECT_STATE_TO_BE_REMOVED) {
        sprite = nullptr;
        return;
    }
    if (state == OBJECT_STATE_DYING) {
        sprite = &ResrcManager::GetInstance().getTexture("MARIO_DIE");
        return;
    }

    switch(CharState) { // Corrected from MarioState to marioState
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
                    else  {
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
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_JUMPING_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_JUMPING_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_FALLING_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_FALLING_LEFT");
                }
            }
            if (state == OBJECT_STATE_DYING) {
                sprite = &ResrcManager::GetInstance().getTexture("MARIO_DIE");
            }
            if (state == OBJECT_STATE_VICTORY) {
                sprite = &ResrcManager::GetInstance().getTexture("SMALLMARIO_VICTORY");
            }
            break;
        }
        case BIG:
        {
            if(state == OBJECT_STATE_ON_GROUND) {
                if (vel.x != 0){
                    if ( direction == DIRECTION_RIGHT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_0_RIGHT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_1_RIGHT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_2_RIGHT");
                        }
                    }
                    else if ( direction == DIRECTION_LEFT) {
                        if(currentFrame == 0) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_0_LEFT");
                        } else if(currentFrame == 1) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_1_LEFT");
                        } else if(currentFrame == 2) {
                            sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_2_LEFT");
                        }
                    }
                }
                if(vel.x == 0) {
                    if ( direction == DIRECTION_RIGHT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_0_RIGHT");
                    } else if ( direction == DIRECTION_LEFT) {
                        sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_0_LEFT");
                    }
                }
            }

            if (state == OBJECT_STATE_JUMPING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_JUMPING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_JUMPING_0_LEFT");
                }
            }

            if (state == OBJECT_STATE_FALLING) {
                if (direction == DIRECTION_RIGHT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_FALLING_0_RIGHT");
                } else if (direction == DIRECTION_LEFT) {
                    sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_FALLING_0_LEFT");
                }
            }
            if (state == OBJECT_STATE_DYING) {
                sprite = &ResrcManager::GetInstance().getTexture("MARIO_DIE");
            }
            if (state == OBJECT_STATE_VICTORY) {
                sprite = &ResrcManager::GetInstance().getTexture("SUPER_MARIO_VICTORY");
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

            if (state == OBJECT_STATE_DYING) {
                sprite = &ResrcManager::GetInstance().getTexture("MARIO_DIE");
            }
            if (state == OBJECT_STATE_VICTORY) {
                sprite = &ResrcManager::GetInstance().getTexture("FIRE_MARIO_VICTORY");
            }
            break;
        }
    }
}
