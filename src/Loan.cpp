#include "Loan.h"
#include <cmath>

Loan::Loan(double principal,
           double annualInterestRate,
           int durationMonths)
    : principal_(principal),
      remainingBalance_(principal),
      annualInterestRate_(annualInterestRate),
      remainingMonths_(durationMonths),
      monthlyPayment_(calculateEMI())
{
}

double Loan::calculateEMI() const
{
    double monthlyRate = annualInterestRate_ / 12.0;

    if (monthlyRate == 0.0)
    {
        return principal_ / remainingMonths_;
    }

    double numerator =
        principal_ *
        monthlyRate *
        std::pow(1.0 + monthlyRate, remainingMonths_);

    double denominator =
        std::pow(1.0 + monthlyRate, remainingMonths_) - 1.0;

    return numerator / denominator;
}

bool Loan::makePayment()
{
    if (!isActive())
    {
        return false;
    }

    double monthlyRate = annualInterestRate_ / 12.0;

    double interest = remainingBalance_ * monthlyRate;

    double principalPayment = monthlyPayment_ - interest;

    remainingBalance_ -= principalPayment;

    remainingMonths_--;

    if (remainingBalance_ < 0.01)
    {
        remainingBalance_ = 0.0;
        remainingMonths_ = 0;
    }

    return true;
}

bool Loan::isActive() const
{
    return remainingMonths_ > 0 &&
           remainingBalance_ > 0.0;
}

double Loan::getRemainingBalance() const
{
    return remainingBalance_;
}

double Loan::getMonthlyPayment() const
{
    return monthlyPayment_;
}

int Loan::getRemainingMonths() const
{
    return remainingMonths_;
}

double Loan::getInterestRate() const
{
    return annualInterestRate_;
}