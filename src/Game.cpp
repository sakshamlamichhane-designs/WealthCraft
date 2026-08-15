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
      running_(true)
{
}

void Game::initialize()
{
}

void Game::run()
{
    while (running_)
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

void Game::nextMonth()
{
    currentMonth_++;

    std::cout << "\n=========================\n";
    std::cout << "MONTH " << currentMonth_ << "\n";
    std::cout << "=========================\n";

    // 1. Determine the economic conditions for this month.
    Event event = eventManager_.generateEvent();

    std::cout << "\nECONOMIC EVENT\n";
    std::cout << "-------------------------\n";
    std::cout << event.title << "\n";
    std::cout << event.description << "\n";

    // 2. Apply the event to the economy.
    if (event.type == EventType::SalaryRaise)
    {
        player_.applySalaryRaise();
    }
    else if (event.type == EventType::SalaryCut)
   {
    player_.applySalaryCut();
   }

economy_.update(event);
    economy_.update(event);

    // 3. Update living costs using the current inflation rate.
    player_.updateExpenses(economy_);

    // 4. Receive salary.
    player_.receiveSalary();

    // 5. Pay living expenses.
    player_.payMonthlyExpenses();

    // 6. Pay active loan installments.
    player_.payLoanInstallments();

    // 7. Update investment prices.
    player_.updatePortfolio(economy_);

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

    checkGameStatus();
}

void Game::showMenu()
{
    std::cout << "\n=============================\n";
    std::cout << "        WEALTHCRAFT\n";
    std::cout << "=============================\n";

    std::cout << "Cash: $" << player_.getCash() << "\n";
    std::cout << "Net Worth: $" << player_.calculateNetWorth() << "\n\n";

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

void Game::financeMenu()
{
    std::cout << "\n========== FINANCIAL MANAGEMENT ==========\n";

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
            if (player_.upgradeSkills())
            {
                std::cout << "\nSkill upgrade successful!\n";
                std::cout << "New Salary: $"
                          << player_.getSalary()
                          << "\n";
            }
            else
            {
                std::cout << "\nNot enough cash for the upgrade.\n";
            }
            break;

        case 2:
            if (player_.cutDiscretionaryExpenses())
            {
                std::cout << "\nExpenses reduced successfully!\n";
                std::cout << "New Monthly Expenses: $"
                          << player_.getMonthlyExpenses()
                          << "\n";
            }
            else
            {
                std::cout << "\nExpenses cannot be reduced further.\n";
            }
            break;

        case 3:
            return;

        default:
            std::cout << "Invalid choice.\n";
    }
}

void Game::buyAssetMenu()
{
    std::cout << "\n========== SHOP ==========\n";

    std::cout << "1. Apple Stock        $200\n";
    std::cout << "2. Government Bond    $1000\n";
    std::cout << "3. Bitcoin            $500\n";
    std::cout << "4. House              $10000\n";
    std::cout << "5. Fixed Deposit      $5000\n";
    std::cout << "6. Index Fund         $2000\n";

    std::cout << "\nChoice: ";

    int choice;
    std::cin >> choice;

    std::unique_ptr<Asset> asset = createAsset(choice);

   if (!asset)
{
    std::cout << "Invalid choice.\n";
    return;
}
    std::string assetName = asset->getName();

if (player_.buyAsset(std::move(asset)))
{
std::cout << "\nPurchased "
          << assetName
          << " successfully!\n";

std::cout << "Remaining Cash: $"
          << player_.getCash()
          << "\n";
}
else
{
    std::cout << "\nNot enough cash!\n";
}   

    
}

void Game::sellAssetMenu()
{
    std::cout << "\n========== SELL ASSET ==========\n";

    player_.showPortfolio();

    std::cout << "\nEnter asset number to sell: ";

    std::size_t choice;
    std::cin >> choice;

    if (choice == 0)
    {
        std::cout << "Invalid asset number.\n";
        return;
    }

    // Portfolio display starts from 1,
    // while vector indexing starts from 0.
    std::size_t index = choice - 1;

    if (player_.sellAsset(index))
    {
        std::cout << "\nAsset sold successfully!\n";
        std::cout << "Cash: $" << player_.getCash() << "\n";
    }
    else
    {
        std::cout << "\nUnable to sell that asset.\n";
    }
}

std::unique_ptr<Asset> Game::createAsset(int choice)
{
    switch (choice)
    {
        case 1:
            return std::make_unique<Stock>(
                "Apple Stock",
                200.0,
                0.05
            );

        case 2:
            return std::make_unique<Bond>(
                "Government Bond",
                1000.0,
                0.08
            );

        case 3:
            return std::make_unique<Crypto>(
                "Bitcoin",
                500.0,
                0.15
            );

        case 4:
            return std::make_unique<RealEstate>(
                "House",
                10000.0
            );

        case 5:
            return std::make_unique<FixedDeposit>(
                "Fixed Deposit",
                5000.0,
                0.07
            );

        case 6:
             return std::make_unique<MutualFund>(
                "Index Fund",
                2000.0,
                0.08
    );

        default:
            return nullptr;
    }
}

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

bool Game::buyAsset(int choice)
{
    std::unique_ptr<Asset> asset = createAsset(choice);

    if (!asset)
    {
        return false;
    }

    return player_.buyAsset(std::move(asset));
}

bool Game::sellAsset(std::size_t index)
{
    
    return player_.sellAsset(index);

}

void Game::checkGameStatus()
{
    // Bankruptcy condition
    if (player_.isBankrupt())
    {
        std::cout << "\n=============================\n";
        std::cout << "        BANKRUPTCY\n";
        std::cout << "=============================\n";

        std::cout << "You ran out of money and your net worth became negative.\n";
        std::cout << "You have been declared bankrupt.\n\n";

        std::cout << "Final Cash: $"
                  << player_.getCash()
                  << "\n";

        std::cout << "Final Portfolio Value: $"
                  << player_.getPortfolio().calculateTotalValue()
                  << "\n";

        std::cout << "Final Debt: $"
                  << player_.getTotalDebt()
                  << "\n";

        std::cout << "Final Net Worth: $"
                  << player_.calculateNetWorth()
                  << "\n";

        running_ = false;
        return;
    }

    // Victory condition
    if (currentMonth_ >= 12)
    {
        const double winningNetWorth = 100000.0;

        std::cout << "\n=============================\n";

        if (player_.calculateNetWorth() >= winningNetWorth)
        {
            std::cout << "        YOU WIN!\n";
            std::cout << "=============================\n";

            std::cout << "You successfully completed the 12-month challenge!\n";
            std::cout << "Target Net Worth: $" << winningNetWorth << "\n";
            std::cout << "Final Net Worth: $"
                      << player_.calculateNetWorth()
                      << "\n";
        }
        else
        {
            std::cout << "        GAME OVER\n";
            std::cout << "=============================\n";

            std::cout << "You survived 12 months, but did not reach the financial target.\n";
            std::cout << "Target Net Worth: $" << winningNetWorth << "\n";
            std::cout << "Final Net Worth: $"
                      << player_.calculateNetWorth()
                      << "\n";
        }

        std::cout << "\nFinal Cash: $"
                  << player_.getCash()
                  << "\n";

        std::cout << "Final Portfolio Value: $"
                  << player_.getPortfolio().calculateTotalValue()
                  << "\n";

        std::cout << "Final Debt: $"
                  << player_.getTotalDebt()
                  << "\n";

        running_ = false;
    }
}