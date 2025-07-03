class Map1State : public GameState {
    private:
        // Game world components for map1
        bool isPaused;
        
    public:
        Map1State();
        void Enter() override;
        void Update() override;
        void Draw() override;
        void Exit() override;
        void HandleInput() override;
    };