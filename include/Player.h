#pragma once

#include "Asset.h"
#include "Portfolio.h"
#include "Loan.h"

#include <string>
#include <vector>
#include <memory>

class Economy;

class Player
{
public:

    Player(std::string name, double cash, double salary);

    void addCash(double amount);
    bool spendCash(double amount);

    void receiveSalary();
    void payMonthlyExpenses();

    bool buyAsset(std::unique_ptr<Asset> asset);
    bool sellAsset(std::size_t index);

    void updatePortfolio(const Economy& economy);

    void showPortfolio() const;

    // Loan system
    bool takeLoan(double amount,
                  double annualInterestRate,
                  int durationMonths);

    void payLoanInstallments();
    void updateExpenses(const Economy& economy);

    double getTotalDebt() const;
    double getMonthlyLoanPayments() const;

    // Getters
    double getCash() const;
    double getSalary() const;
    double getMonthlyExpenses() const;

    double calculateNetWorth() const;

    const std::string& getName() const;

    const Portfolio& getPortfolio() const;

    bool upgradeSkills();

    int getSkillLevel() const;

    bool cutDiscretionaryExpenses();

    bool isBankrupt() const;

    void applySalaryRaise();

    void applySalaryCut();
private:

    std::string name_;

    double cash_;

    double monthlyExpenses_;

    double monthlySalary_;

    Portfolio portfolio_;

    std::vector<Loan> loans_;

    int skillLevel_;
};