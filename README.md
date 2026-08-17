WealthCraft

An Interactive Personal Finance Simulation Game

Project Type: C++20 OOP Semester Project
Institution: Tribhuvan University, Institute of Engineering, Pulchowk Campus
Department: Electronics and Computer Engineering
Platform: Windows (primary target)

Developers

Saksham Lamichhane — 082BCT065

Sameep Paudel — 082BCT068

1. Overview

WealthCraft is a single-player desktop personal-finance simulation game written in C++20. The player starts with a fixed amount of cash and monthly income, then makes financial decisions involving investments, living expenses, loans, and financial management while the simulated economy changes over time.

The project is designed around object-oriented programming. Investment types share a common abstract Asset interface, while Portfolio, Player, Economy, EventManager, Loan, and the GUI layer each have separate responsibilities.

The game advances in simulated monthly time steps. During each month, the simulation processes income, expenses, loan payments, an economic event, changes in economic conditions, and asset-price updates.

2. Current Features

Financial Simulation

Monthly salary and living expenses

Inflation-sensitive living expenses

Financial management through:

skill upgrades that increase salary

discretionary expense reduction

Net-worth calculation

Cash and liquidity tracking

Investment System

The game currently supports six investment classes:

Stocks — moderate volatility and market-sensitive price movement

Bonds — yield, inflation, and interest-rate effects

Cryptocurrency — high volatility and crypto-market effects

Real Estate — gradual appreciation with housing-market events

Fixed Deposits — periodic interest-based growth

Mutual Funds — moderate growth with stock-market influence

The six investment types inherit from the abstract Asset base class and implement their own simulatePrice() behavior.

Portfolio System

Buy assets

Sell assets

Hold multiple asset types simultaneously

Calculate total portfolio value

Update all assets polymorphically every simulated month

Uses std::unique_ptr for exclusive asset ownership

Loan System

Personal loans

Mortgages

EMI calculation using the standard amortization formula

Monthly installment processing

Remaining balance tracking

Total debt calculation

Monthly loan-payment tracking

Economic Simulation

The economy maintains simulated:

Inflation rate

Interest rate

Stock-market conditions

Cryptocurrency conditions

Real-estate conditions

Bond-market conditions

Economic events can include:

Bull Market

Market Crash

Crypto Boom

Crypto Crash

Housing Boom

Housing Crash

Inflation Spike

Interest Rate Hike

Recession

Quiet Month / no major event

Economic conditions recover gradually toward baseline values rather than resetting instantly every month.

Game Progression

12 simulated months

Monthly economic events

End-of-game financial summary

Bankruptcy-oriented debt/liquidity architecture prepared for extension

Graphical Interface

The current GUI uses:

SFML 3.0.2 for the application window and rendering foundation

Dear ImGui 1.91.9 for GUI widgets and panels

ImGui-SFML for Dear ImGui integration with SFML

The GUI currently provides a financial dashboard containing:

Cash

Net worth

Debt

Portfolio table

Current economic event

Inflation

Interest rate

Monthly expenses

Monthly loan payments

Buy Asset interface

Sell Asset interface

Loan Center interface

Financial Management interface

Next Month control

Exit control

3. OOP Design

The project intentionally demonstrates the major object-oriented programming concepts required by the project.

Abstraction

Asset is an abstract base class with the pure virtual function:

virtual void simulatePrice(const Economy& economy) = 0;

The base class defines the common contract for all investment types without defining one universal pricing algorithm.

Inheritance

The following classes inherit from Asset:

Asset
├── Stock
├── Bond
├── Crypto
├── RealEstate
├── FixedDeposit
└── MutualFund

Polymorphism

Portfolio stores different investment types through:

std::vector<std::unique_ptr<Asset>>

During a monthly update, the portfolio calls:

asset->simulatePrice(economy);

The correct derived-class implementation is selected through virtual dispatch.

Encapsulation

Important state such as cash, salary, expenses, portfolio contents, asset values, loan balances, and economic variables are kept private and modified through public member functions.

Composition

The main relationships are:

Game
├── Player
│   ├── Portfolio
│   │   └── Assets
│   └── Loans
├── Economy
└── EventManager

The GUI layer references the Game object and displays or commands the underlying simulation instead of duplicating the financial logic.

RAII / Smart Pointers

std::unique_ptr<Asset> is used because the portfolio exclusively owns its asset objects. This provides automatic lifetime management and avoids manual new/delete handling.

4. Project Structure

WealthCraft/
│
├── assets/
│   └── fonts/
│       └── DejaVuSans.ttf
│
├── include/
│   ├── Asset.h
│   ├── Stock.h
│   ├── Bond.h
│   ├── Crypto.h
│   ├── RealEstate.h
│   ├── FixedDeposit.h
│   ├── MutualFund.h
│   ├── Portfolio.h
│   ├── Player.h
│   ├── Loan.h
│   ├── Economy.h
│   ├── Event.h
│   ├── EventManager.h
│   ├── Random.h
│   ├── Game.h
│   └── GameWindow.h
│
├── src/
│   ├── main.cpp
│   ├── Asset.cpp
│   ├── Stock.cpp
│   ├── Bond.cpp
│   ├── Crypto.cpp
│   ├── RealEstate.cpp
│   ├── FixedDeposit.cpp
│   ├── MutualFund.cpp
│   ├── Portfolio.cpp
│   ├── Player.cpp
│   ├── Loan.cpp
│   ├── Economy.cpp
│   ├── EventManager.cpp
│   ├── Random.cpp
│   ├── Game.cpp
│   └── GameWindow.cpp
│
├── imgui/              # Dear ImGui source
├── imgui-sfml/         # ImGui-SFML integration source
├── save/               # reserved for persistence data
├── docs/               # project documentation
├── build/              # generated by CMake; do not version-control
│
├── CMakeLists.txt
├── README.md
└── .gitignore

5. Requirements

Compiler / Build Environment

Windows

MSYS2 UCRT64 environment

GCC / G++ with C++20 support

CMake 3.16 or newer

Libraries

SFML 3.0.2

Dear ImGui 1.91.9

ImGui-SFML compatible with the selected SFML 3 setup

The project currently compiles Dear ImGui and ImGui-SFML directly from the imgui/ and imgui-sfml/ directories.

6. Building the Project

Open an MSYS2 UCRT64 terminal and move into the project directory:

cd /c/Users/<username>/Desktop/WealthCraft

Configure the project:

cmake -S . -B build -G "MinGW Makefiles"

Build:

cmake --build build

Run:

./build/WealthCraft.exe

If the build/ directory already exists and the project configuration has not changed, normally only this is required:

cmake --build build

7. Running the Simulation

The main gameplay loop is:

Start Game
   ↓
Receive Salary
   ↓
Economic Event
   ↓
Economy Updates
   ↓
Inflation Updates Expenses
   ↓
Pay Living Expenses
   ↓
Pay Loan Installments
   ↓
Update Investment Values
   ↓
Recalculate Net Worth
   ↓
Player Makes Decisions
   ↓
Next Month

The player can use available cash to:

purchase investments

sell investments

take loans

invest in skills

reduce discretionary expenses

hold cash for liquidity

8. Financial Model

Net Worth

The game uses:

Net Worth = Cash + Portfolio Value - Total Debt

Taking a loan therefore increases cash and debt by equal amounts and does not immediately increase net worth.

Loan EMI

For a non-zero monthly interest rate, the standard amortization formula is used:

EMI = P × r × (1 + r)^n / ((1 + r)^n - 1)

where:

P = principal

r = monthly interest rate

n = remaining number of payments

Each payment is divided conceptually into interest and principal portions, reducing the outstanding balance over time.

Stock / Crypto Simulation

Stock and cryptocurrency values use percentage-based random movements combined with economic conditions. Crypto uses a larger volatility range than stocks.

Real Estate

Real estate uses slow baseline appreciation, small random variation, and stronger response to real-estate economic events.

Fixed Deposits / Bonds / Mutual Funds

These assets use simplified educational models rather than attempting to reproduce a real financial institution's complete pricing or accounting system. The intention is to demonstrate the effect of yield, inflation, interest rates, market conditions, and diversification in a game environment.

9. Design Philosophy

The project intentionally avoids external real-time financial APIs. Market behavior is simulated locally so the game remains deterministic in architecture, offline-capable, and easy to demonstrate in a classroom environment.

The project also avoids introducing artificial manager classes where they do not add meaningful responsibility. For example, loans remain owned by Player because Player is the object responsible for the player's financial obligations.

The GUI is kept separate from the financial engine. The GUI should request actions from Game rather than performing calculations independently. This avoids duplicated business logic and keeps the financial rules in one place.

10. Current Scope and Known Limitations

The current implementation focuses on the core simulation engine, six investment classes, loans, economic events, monthly progression, and a graphical interface.

The following proposal items are not currently part of the finished implementation and should not be presented as completed unless they are added later:

JSON save/load persistence

Audio system

Animated video backgrounds

20+ fully implemented economic events

Credit-score penalties for missed loan payments

Full bankruptcy rules

In-application net-worth history chart

Multiplayer / online features

Real-time market-data APIs

These are intentionally separated from the core simulation so that the project remains understandable and maintainable under the available development time.

11. Third-Party Libraries

SFML

Used for window creation, graphics infrastructure, input handling, and the graphical application foundation.

Official project: https://www.sfml-dev.org/

Dear ImGui

Used for immediate-mode graphical user interface components such as buttons, tables, dialogs, and information panels.

Official project: https://github.com/ocornut/imgui

ImGui-SFML

Provides Dear ImGui integration with SFML.

Official project: https://github.com/SFML/imgui-sfml

Third-party licenses are retained in their respective directories.

12. Development and Testing Approach

The project was developed incrementally. Individual classes were compiled and tested before integrating larger systems.

Examples of validation performed during development include:

successful CMake configuration

successful compilation and linking of the full project

buying different asset types

selling assets and verifying cash reconciliation

rejecting purchases when cash is insufficient

invalid sell-index handling

taking personal loans and mortgages

monthly loan amortization

economic-event-driven asset changes

month-by-month net-worth updates

12-month game completion

graphical window and GUI startup

A build should always be checked after significant code changes to catch interface and implementation mismatches early.

13. Team

Saksham Lamichhane (082BCT065)
Sameep Paudel (082BCT068)

The project is a collaborative semester project. 

14. Academic Note

WealthCraft is an educational simulation. Its financial models are simplified game models intended to demonstrate concepts such as cash flow, diversification, compounding, debt, inflation, volatility, and financial decision-making. It is not intended to provide real-world investment advice or reproduce live financial markets.

## Download

### Windows Portable Build

The latest portable Windows build is available from the GitHub Releases page:

https://github.com/sakshamlamichhane-designs/WealthCraft/releases/latest

Download `WealthCraft_Portable.zip`, extract it, open the `dist` folder, and run `WealthCraft.exe`.

No development environment or library installation is required for the portable build.
