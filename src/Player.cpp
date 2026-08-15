#include "Player.h"
#include "Asset.h"
#include "Economy.h"

#include <iostream>
#include <utility>

Player::Player(std::string name, double cash, double salary)
    : name_(std::move(name)),
      cash_(cash),
      monthlySalary_(salary),
      monthlyExpenses_(40000.0),
      skillLevel_(0)
{
}

void Player::addCash(double amount) {
    cash_ += amount;
}

bool Player::spendCash(double amount) {
    if (amount > cash_) {
        return false;
    }
    cash_ -= amount;
    return true;
}

void Player::receiveSalary() {
    cash_ += monthlySalary_;
}

bool Player::buyAsset(std::unique_ptr<Asset> asset)
{
    if (!asset)
    {
        return false;
    }

    if (!spendCash(asset->getValue()))
    {
        return false;
    }

    portfolio_.addAsset(std::move(asset));

    return true;
}

void Player::updatePortfolio(const Economy& economy)
{
    portfolio_.updateAssets(economy);
}

double Player::getCash() const {
    return cash_;
}

double Player::getSalary() const {
    return monthlySalary_;
}

double Player::getMonthlyExpenses() const
{
    return monthlyExpenses_;
}

double Player::calculateNetWorth() const
{
    return cash_
         + portfolio_.calculateTotalValue()
         - getTotalDebt();
}

const std::string& Player::getName() const {
    return name_;
}

const Portfolio& Player::getPortfolio() const {
    return portfolio_;
}

bool Player::sellAsset(std::size_t index)
{
    if (index >= portfolio_.assetCount())
    {
        return false;
    }

    double value = portfolio_.getAsset(index).getValue();

    addCash(value);

    return portfolio_.removeAsset(index);
}

void Player::showPortfolio() const
{
    portfolio_.printAssets();
}

void Player::payMonthlyExpenses()
{
    cash_ -= monthlyExpenses_;

    if (cash_ < 0.0)
    {
        cash_ = 0.0;
    }
}

void Player::updateExpenses(const Economy& economy)
{
    double monthlyInflation =
        economy.getInflationRate() / 12.0;

    monthlyExpenses_ *= (1.0 + monthlyInflation);

    if (monthlyExpenses_ > 100000.0)
    {
        monthlyExpenses_ = 100000.0;
    }
}

bool Player::takeLoan(double amount,
                      double annualInterestRate,
                      int durationMonths)
{
    if (amount <= 0.0 ||
        annualInterestRate < 0.0 ||
        durationMonths <= 0)
    {
        return false;
    }

    Loan loan(amount, annualInterestRate, durationMonths);

    loans_.push_back(loan);

    cash_ += amount;

    return true;
}

void Player::payLoanInstallments()
{
    for (Loan& loan : loans_)
    {
        if (!loan.isActive())
        {
            continue;
        }

        double payment = loan.getMonthlyPayment();

        if (cash_ >= payment)
        {
            cash_ -= payment;
            loan.makePayment();
        }
    }
}

double Player::getTotalDebt() const
{
    double totalDebt = 0.0;

    for (const Loan& loan : loans_)
    {
        totalDebt += loan.getRemainingBalance();
    }

    return totalDebt;
}

double Player::getMonthlyLoanPayments() const
{
    double totalPayment = 0.0;

    for (const Loan& loan : loans_)
    {
        if (loan.isActive())
        {
            totalPayment += loan.getMonthlyPayment();
        }
    }

    return totalPayment;
}

bool Player::upgradeSkills()
{
    const double upgradeCost = 15000.0;

    if (cash_ < upgradeCost)
    {
        return false;
    }

    cash_ -= upgradeCost;

    skillLevel_++;

    monthlySalary_ *= 1.10;

    return true;
}

bool Player::cutDiscretionaryExpenses()
{
    const double reduction = 3000.0;

    if (monthlyExpenses_ <= 25000.0)
    {
        return false;
    }

    monthlyExpenses_ -= reduction;

    return true;
}

bool Player::isBankrupt() const
{

    // The player is bankrupt if they have no cash
    // and their total net worth is negative.
    return cash_ <= 0.0 && calculateNetWorth() < 0.0;

}

int Player::getSkillLevel() const
{
    return skillLevel_;
}

void Player::applySalaryRaise()
{
    monthlySalary_ *= 1.10;
}

void Player::applySalaryCut()
{
    monthlySalary_ *= 0.90;
}