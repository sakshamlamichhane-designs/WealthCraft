#pragma once

#include <memory>
#include "Asset.h"
#include "Player.h"
#include "Economy.h"
#include "EventManager.h"

class Game
{
private:
    Player player_;
    Economy economy_;
    EventManager eventManager_;

    int currentMonth_;
    bool running_;
    std::unique_ptr<Asset> createAsset(int choice);
    
public:
    Game();

    void initialize();

    void run();

    void update();

    void render();

    void shutdown();

    void nextMonth();    
    
    void showMenu();

    void handleChoice(int choice);

    void buyAssetMenu();

    void sellAssetMenu();

    void loanMenu();

    void financeMenu();

    void checkGameStatus();

    bool buyAsset(int choice);
    bool sellAsset(std::size_t index);

    Player& getPlayer();
    const Player& getPlayer() const;

    const Economy& getEconomy() const;

    const Event& getCurrentEvent() const;

    int getCurrentMonth() const;

    bool isRunning() const;
};