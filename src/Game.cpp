#include "Game.h"

#include "Stock.h"
#include "Bond.h"
#include "Crypto.h"
#include "RealEstate.h"
#include "FixedDeposit.h"
#include "MutualFund.h"

#include <memory>
#include <iostream>


Game::Game()
    : player_("Player", 50000.0, 50000.0),
      currentMonth_(0),
      running_(true),
      gameOver_(false),
      playerWon_(false)
{
}

void Game::initialize()
{
}

void Game::run()
{
    while (running_ && !gameOver_)
    {
        showMenu();

        int choice;
        std::cin >> choice;

        handleChoice(choice);
    }
}

void Game::update()
{
}

void Game::render()
{
}

void Game::shutdown()
{
}


// ============================================================
// MONTHLY SIMULATION
// ============================================================

void Game::nextMonth()
{
    if (gameOver_)
    {
        return;
    }

    if (currentMonth_ >= 12)
    {
        return;
    }

    currentMonth_++;

    std::cout << "\n=========================\n";
    std::cout << "MONTH " << currentMonth_ << "\n";
    std::cout << "=========================\n";

    // 1. Generate this month's economic event.
    Event event = eventManager_.generateEvent();

    std::cout << "\nECONOMIC EVENT\n";
    std::cout << "-------------------------\n";
    std::cout << event.title << "\n";
    std::cout << event.description << "\n";

    // 2. Handle salary-related events.
    if (event.type == EventType::SalaryRaise)
    {
        player_.applySalaryRaise();
    }
    else if (event.type == EventType::SalaryCut)
    {
        player_.applySalaryCut();
    }

    // 3. Apply the economic event ONCE.
    economy_.update(event);

    // 4. Update living expenses using inflation.
    player_.updateExpenses(economy_);

    // 5. Receive salary.
    player_.receiveSalary();

    // 6. Pay living expenses.
    player_.payMonthlyExpenses();

    // 7. Pay active loan installments.
    player_.payLoanInstallments();

    // 8. Update investment values.
    player_.updatePortfolio(economy_);

    // --------------------------------------------------------
    // FINANCIAL REPORT
    // --------------------------------------------------------

    std::cout << "\nFINANCIAL UPDATE\n";
    std::cout << "-------------------------\n";

    std::cout << "Salary: $"
              << player_.getSalary()
              << "\n";

    std::cout << "Living Expenses: $"
              << player_.getMonthlyExpenses()
              << "\n";

    std::cout << "Remaining Monthly Loan Payments: $"
              << player_.getMonthlyLoanPayments()
              << "\n";

    std::cout << "Inflation: "
              << economy_.getInflationRate() * 100.0
              << "%\n";

    std::cout << "Total Debt: $"
              << player_.getTotalDebt()
              << "\n";

    std::cout << "\nUpdated Portfolio:\n";
    player_.showPortfolio();

    std::cout << "\nCash: $"
              << player_.getCash()
              << "\n";

    std::cout << "Net Worth: $"
              << player_.calculateNetWorth()
              << "\n";

    // 9. Check bankruptcy or final result.
    checkGameStatus();
}


// ============================================================
// CONSOLE MENU
// ============================================================

void Game::showMenu()
{
    std::cout << "\n=============================\n";
    std::cout << "        WEALTHCRAFT\n";
    std::cout << "=============================\n";

    std::cout << "Cash: $"
              << player_.getCash()
              << "\n";

    std::cout << "Net Worth: $"
              << player_.calculateNetWorth()
              << "\n\n";

    std::cout << "1. Buy Asset\n";
    std::cout << "2. Sell Asset\n";
    std::cout << "3. View Portfolio\n";
    std::cout << "4. Take Loan\n";
    std::cout << "5. Next Month\n";
    std::cout << "6. Manage Finances\n";
    std::cout << "7. Exit\n\n";

    std::cout << "Choice: ";
}

void Game::handleChoice(int choice)
{
    switch (choice)
    {
        case 1:
            buyAssetMenu();
            break;

        case 2:
            sellAssetMenu();
            break;

        case 3:
            player_.showPortfolio();
            break;

        case 4:
            loanMenu();
            break;

        case 5:
            nextMonth();
            break;

        case 6:
            financeMenu();
            break;

        case 7:
            running_ = false;
            break;

        default:
            std::cout << "Invalid choice.\n";
    }
}


// ============================================================
// LOAN MENU
// ============================================================

void Game::loanMenu()
{
    std::cout << "\n========== LOAN CENTER ==========\n";

    std::cout << "1. Personal Loan\n";
    std::cout << "2. Mortgage\n";
    std::cout << "3. View Debt\n";
    std::cout << "4. Back\n";

    std::cout << "\nChoice: ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
        case 1:
        {
            double amount;

            std::cout << "\nPersonal Loan Amount: $";
            std::cin >> amount;

            if (player_.takeLoan(amount, 0.10, 24))
            {
                std::cout << "\nLoan approved!\n";
                std::cout << "Interest Rate: 10%\n";
                std::cout << "Duration: 24 months\n";
                std::cout << "Monthly Payment: $"
                          << player_.getMonthlyLoanPayments()
                          << "\n";
            }
            else
            {
                std::cout << "\nInvalid loan request.\n";
            }

            break;
        }

        case 2:
        {
            double amount;

            std::cout << "\nMortgage Amount: $";
            std::cin >> amount;

            if (player_.takeLoan(amount, 0.08, 120))
            {
                std::cout << "\nMortgage approved!\n";
                std::cout << "Interest Rate: 8%\n";
                std::cout << "Duration: 120 months\n";
                std::cout << "Monthly Payment: $"
                          << player_.getMonthlyLoanPayments()
                          << "\n";
            }
            else
            {
                std::cout << "\nInvalid mortgage request.\n";
            }

            break;
        }

        case 3:
        {
            std::cout << "\n========== DEBT ==========\n";

            std::cout << "Total Debt: $"
                      << player_.getTotalDebt()
                      << "\n";

            std::cout << "Monthly Payments: $"
                      << player_.getMonthlyLoanPayments()
                      << "\n";

            break;
        }

        case 4:
            return;

        default:
            std::cout << "Invalid choice.\n";
    }
}


// ============================================================
// FINANCIAL MANAGEMENT
// ============================================================

void Game::financeMenu()
{
    std::cout
        << "\n========== FINANCIAL MANAGEMENT ==========\n";

    std::cout << "Cash: $"
              << player_.getCash()
              << "\n";

    std::cout << "Monthly Salary: $"
              << player_.getSalary()
              << "\n";

    std::cout << "Monthly Expenses: $"
              << player_.getMonthlyExpenses()
              << "\n\n";

    std::cout << "1. Invest in Skills ($15000)\n";
    std::cout << "2. Cut Discretionary Expenses (-$3000/month)\n";
    std::cout << "3. Back\n";

    std::cout << "\nChoice: ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
        case 1:
        {
            if (player_.upgradeSkills())
            {
                std::cout << "\nSkill upgrade successful!\n";
                std::cout << "New Salary: $"
                          << player_.getSalary()
                          << "\n";
            }
            else
            {
                std::cout
                    << "\nNot enough cash for the upgrade.\n";
            }

            break;
        }

        case 2:
        {
            if (player_.cutDiscretionaryExpenses())
            {
                std::cout
                    << "\nExpenses reduced successfully!\n";

                std::cout
                    << "New Monthly Expenses: $"
                    << player_.getMonthlyExpenses()
                    << "\n";
            }
            else
            {
                std::cout
                    << "\nExpenses cannot be reduced further.\n";
            }

            break;
        }

        case 3:
            return;

        default:
            std::cout << "Invalid choice.\n";
    }
}


// ============================================================
// CONSOLE BUY MENU
// ============================================================

void Game::buyAssetMenu()
{
    std::cout << "\n========== SHOP ==========\n";

    std::cout << "1. Apple Stock\n";
    std::cout << "2. Government Bond\n";
    std::cout << "3. Bitcoin\n";
    std::cout << "4. House\n";
    std::cout << "5. Fixed Deposit\n";
    std::cout << "6. Index Fund\n";

    std::cout << "\nCURRENT MARKET PRICES\n";
    std::cout << "-------------------------\n";

    for (int i = 1; i <= 6; ++i)
    {
        std::cout << i
                  << ". $"
                  << getAssetPurchasePrice(i)
                  << "\n";
    }

    std::cout << "\nChoice: ";

    int choice;
    std::cin >> choice;

    std::unique_ptr<Asset> asset =
        createAsset(choice);

    if (!asset)
    {
        std::cout << "Invalid choice.\n";
        return;
    }

    std::string assetName =
        asset->getName();

    if (player_.buyAsset(std::move(asset)))
    {
        std::cout
            << "\nPurchased "
            << assetName
            << " successfully!\n";

        std::cout
            << "Remaining Cash: $"
            << player_.getCash()
            << "\n";
    }
    else
    {
        std::cout
            << "\nNot enough cash!\n";
    }
}


// ============================================================
// CONSOLE SELL MENU
// ============================================================

void Game::sellAssetMenu()
{
    std::cout
        << "\n========== SELL ASSET ==========\n";

    player_.showPortfolio();

    std::cout
        << "\nEnter asset number to sell: ";

    std::size_t choice;
    std::cin >> choice;

    if (choice == 0)
    {
        std::cout
            << "Invalid asset number.\n";

        return;
    }

    std::size_t index =
        choice - 1;

    if (player_.sellAsset(index))
    {
        std::cout
            << "\nAsset sold successfully!\n";

        std::cout
            << "Cash: $"
            << player_.getCash()
            << "\n";
    }
    else
    {
        std::cout
            << "\nUnable to sell that asset.\n";
    }
}


// ============================================================
// CURRENT MARKET PRICE
// ============================================================

double Game::getAssetPurchasePrice(int choice) const
{
    switch (choice)
    {
        case 1:
            return 200.0 *
                   economy_.getStockMultiplier();

        case 2:
            return 1000.0 *
                   economy_.getBondMultiplier();

        case 3:
            return 500.0 *
                   economy_.getCryptoMultiplier();

        case 4:
            return 10000.0 *
                   economy_.getRealEstateMultiplier();

        case 5:
            return 5000.0;

        case 6:
            return 2000.0 *
                   economy_.getStockMultiplier();

        default:
            return 0.0;
    }
}


// ============================================================
// ASSET FACTORY
// ============================================================

std::unique_ptr<Asset>
Game::createAsset(int choice)
{
    const double purchasePrice =
        getAssetPurchasePrice(choice);

    switch (choice)
    {
        case 1:
            return std::make_unique<Stock>(
                "Apple Stock",
                purchasePrice,
                0.05
            );

        case 2:
            return std::make_unique<Bond>(
                "Government Bond",
                purchasePrice,
                0.08
            );

        case 3:
            return std::make_unique<Crypto>(
                "Bitcoin",
                purchasePrice,
                0.15
            );

        case 4:
            return std::make_unique<RealEstate>(
                "House",
                purchasePrice
            );

        case 5:
            return std::make_unique<FixedDeposit>(
                "Fixed Deposit",
                purchasePrice,
                0.07
            );

        case 6:
            return std::make_unique<MutualFund>(
                "Index Fund",
                purchasePrice,
                0.08
            );

        default:
            return nullptr;
    }
}


// ============================================================
// GETTERS
// ============================================================

Player& Game::getPlayer()
{
    return player_;
}

const Player& Game::getPlayer() const
{
    return player_;
}

const Economy& Game::getEconomy() const
{
    return economy_;
}

const Event& Game::getCurrentEvent() const
{
    return eventManager_.getCurrentEvent();
}

int Game::getCurrentMonth() const
{
    return currentMonth_;
}

bool Game::isRunning() const
{
    return running_;
}

bool Game::isGameOver() const
{
    return gameOver_;
}

bool Game::playerWon() const
{
    return playerWon_;
}


// ============================================================
// GUI BUY / SELL
// ============================================================

bool Game::buyAsset(int choice)
{
    std::unique_ptr<Asset> asset =
        createAsset(choice);

    if (!asset)
    {
        return false;
    }

    return player_.buyAsset(
        std::move(asset));
}

bool Game::sellAsset(std::size_t index)
{
    return player_.sellAsset(index);
}


// ============================================================
// GAME STATUS
// ============================================================

void Game::checkGameStatus()
{
    // --------------------------------------------------------
    // BANKRUPTCY
    // --------------------------------------------------------

    if (player_.isBankrupt())
    {
        gameOver_ = true;
        playerWon_ = false;

        std::cout
            << "\n=============================\n"
            << "        BANKRUPTCY\n"
            << "=============================\n";

        std::cout
            << "You ran out of money and "
               "your net worth became negative.\n";

        std::cout
            << "You have lost the WealthCraft challenge.\n\n";

        std::cout
            << "Month Reached: "
            << currentMonth_
            << "\n";

        std::cout
            << "Final Cash: $"
            << player_.getCash()
            << "\n";

        std::cout
            << "Final Portfolio Value: $"
            << player_.getPortfolio()
                  .calculateTotalValue()
            << "\n";

        std::cout
            << "Final Debt: $"
            << player_.getTotalDebt()
            << "\n";

        std::cout
            << "Final Net Worth: $"
            << player_.calculateNetWorth()
            << "\n";

        return;
    }

    // --------------------------------------------------------
    // TWELVE-MONTH RESULT
    // --------------------------------------------------------

    if (currentMonth_ >= 12)
    {
        const double winningNetWorth =
            100000.0;

        gameOver_ = true;

        if (player_.calculateNetWorth() >=
            winningNetWorth)
        {
            playerWon_ = true;

            std::cout
                << "\n=============================\n"
                << "        YOU WIN!\n"
                << "=============================\n";

            std::cout
                << "Congratulations!\n"
                << "You successfully survived "
                   "all 12 months.\n\n";

            std::cout
                << "Target Net Worth: $"
                << winningNetWorth
                << "\n";

            std::cout
                << "Final Net Worth: $"
                << player_.calculateNetWorth()
                << "\n";
        }
        else
        {
            playerWon_ = false;

            std::cout
                << "\n=============================\n"
                << "       CHALLENGE FAILED\n"
                << "=============================\n";

            std::cout
                << "You survived all 12 months, "
                   "but did not reach the financial target.\n\n";

            std::cout
                << "Target Net Worth: $"
                << winningNetWorth
                << "\n";

            std::cout
                << "Final Net Worth: $"
                << player_.calculateNetWorth()
                << "\n";
        }

        std::cout
            << "\nFinal Cash: $"
            << player_.getCash()
            << "\n";

        std::cout
            << "Final Portfolio Value: $"
            << player_.getPortfolio()
                  .calculateTotalValue()
            << "\n";

        std::cout
            << "Final Debt: $"
            << player_.getTotalDebt()
            << "\n";

        // Keep the application open.
        // The GUI will display the final result screen.
    }
}