#pragma once

class Loan
{
private:
    double principal_;
    double remainingBalance_;
    double annualInterestRate_;
    int remainingMonths_;
    double monthlyPayment_;

public:
    Loan(double principal,
         double annualInterestRate,
         int durationMonths);

    double calculateEMI() const;

    bool makePayment();

    bool isActive() const;

    double getRemainingBalance() const;
    double getMonthlyPayment() const;
    int getRemainingMonths() const;
    double getInterestRate() const;
};