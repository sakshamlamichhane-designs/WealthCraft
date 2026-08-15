#include "GameWindow.h"

#include <imgui.h>

#pragma push_macro("IMGUI_API")
#undef IMGUI_API
#include <imgui-SFML.h>
#pragma pop_macro("IMGUI_API")

#include <stdexcept>
#include <string>


GameWindow::GameWindow(Game& game)
    : game_(game),
      window_(
          sf::VideoMode({1200, 700}),
          "WealthCraft"),
      deltaClock_(),
      showBuyWindow_(false),
      showSellWindow_(false),
      showLoanWindow_(false),
      showFinanceWindow_(false),
      loanAmount_(20000.0),
      statusMessage_("")
{
    window_.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window_))
    {
        throw std::runtime_error(
            "Failed to initialize ImGui-SFML.");
    }

    ImGui::GetIO().FontGlobalScale = 1.10f;

    ImGuiStyle& style =
        ImGui::GetStyle();

    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
}


// ============================================================
// MAIN LOOP
// ============================================================

void GameWindow::run()
{
    while (window_.isOpen())
    {
        processEvents();

        ImGui::SFML::Update(
            window_,
            deltaClock_.restart());

        render();

        ImGui::SFML::Render(window_);

        window_.display();
    }

    ImGui::SFML::Shutdown(window_);
}


// ============================================================
// EVENT PROCESSING
// ============================================================

void GameWindow::processEvents()
{
    while (const std::optional event =
               window_.pollEvent())
    {
        ImGui::SFML::ProcessEvent(
            window_,
            *event);

        if (event->is<sf::Event::Closed>())
        {
            window_.close();
        }
    }
}


// ============================================================
// RENDER
// ============================================================

void GameWindow::render()
{
    window_.clear(
        sf::Color(24, 27, 32));

    if (game_.isGameOver())
    {
        drawGameOverWindow();
    }
    else
    {
        drawDashboard();

        if (showBuyWindow_)
        {
            drawBuyWindow();
        }

        if (showSellWindow_)
        {
            drawSellWindow();
        }

        if (showLoanWindow_)
        {
            drawLoanWindow();
        }

        if (showFinanceWindow_)
        {
            drawFinanceWindow();
        }
    }
}


// ============================================================
// WINDOW STATE MANAGEMENT
// ============================================================

void GameWindow::closeOtherWindows(
    bool keepBuy,
    bool keepSell,
    bool keepLoan,
    bool keepFinance)
{
    if (!keepBuy)
    {
        showBuyWindow_ = false;
    }

    if (!keepSell)
    {
        showSellWindow_ = false;
    }

    if (!keepLoan)
    {
        showLoanWindow_ = false;
    }

    if (!keepFinance)
    {
        showFinanceWindow_ = false;
    }
}


// ============================================================
// DASHBOARD
// ============================================================

void GameWindow::drawDashboard()
{
    ImGui::SetNextWindowPos(
        ImVec2(20.0f, 20.0f),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(1160.0f, 650.0f),
        ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;

    ImGui::Begin(
        "WEALTHCRAFT",
        nullptr,
        flags);

    ImGui::Text(
        "WEALTHCRAFT  |  Month %d / 12",
        game_.getCurrentMonth());

    ImGui::Separator();

    // --------------------------------------------------------
    // FINANCIAL SUMMARY
    // --------------------------------------------------------

    ImGui::Columns(
        3,
        nullptr,
        false);

    ImGui::Text("CASH");

    ImGui::Text(
        "$%.2f",
        game_.getPlayer().getCash());

    ImGui::NextColumn();

    ImGui::Text("NET WORTH");

    ImGui::Text(
        "$%.2f",
        game_.getPlayer().calculateNetWorth());

    ImGui::NextColumn();

    ImGui::Text("DEBT");

    ImGui::Text(
        "$%.2f",
        game_.getPlayer().getTotalDebt());

    ImGui::Columns(1);

    ImGui::Separator();

    // --------------------------------------------------------
    // MAIN CONTENT
    // --------------------------------------------------------

    ImGui::Columns(
        2,
        "MainColumns",
        true);

    // --------------------------------------------------------
    // PORTFOLIO
    // --------------------------------------------------------

    ImGui::Text("PORTFOLIO");

    const Portfolio& portfolio =
        game_.getPlayer().getPortfolio();

    if (portfolio.assetCount() == 0)
    {
        ImGui::TextDisabled(
            "No assets owned.");
    }
    else
    {
        if (ImGui::BeginTable(
                "PortfolioTable",
                2,
                ImGuiTableFlags_Borders |
                ImGuiTableFlags_RowBg |
                ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn("Asset");
            ImGui::TableSetupColumn("Current Value");

            ImGui::TableHeadersRow();

            for (std::size_t i = 0;
                 i < portfolio.assetCount();
                 ++i)
            {
                const Asset& asset =
                    portfolio.getAsset(i);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);

                ImGui::Text(
                    "%s",
                    asset.getName().c_str());

                ImGui::TableSetColumnIndex(1);

                ImGui::Text(
                    "$%.2f",
                    asset.getValue());
            }

            ImGui::EndTable();
        }
    }

    ImGui::Spacing();

    ImGui::Text(
        "Portfolio Value: $%.2f",
        portfolio.calculateTotalValue());

    ImGui::NextColumn();

    // --------------------------------------------------------
    // ECONOMIC INFORMATION
    // --------------------------------------------------------

    ImGui::Text(
        "ECONOMIC EVENT");

    ImGui::Separator();

    const Event& event =
        game_.getCurrentEvent();

    ImGui::Text(
        "%s",
        event.title.c_str());

    ImGui::Spacing();

    ImGui::TextWrapped(
        "%s",
        event.description.c_str());

    ImGui::Spacing();

    ImGui::Text(
        "Inflation: %.2f%%",
        game_.getEconomy()
            .getInflationRate() * 100.0);

    ImGui::Text(
        "Interest Rate: %.2f%%",
        game_.getEconomy()
            .getInterestRate() * 100.0);

    ImGui::Text(
        "Monthly Expenses: $%.2f",
        game_.getPlayer()
            .getMonthlyExpenses());

    ImGui::Text(
        "Loan Payments: $%.2f",
        game_.getPlayer()
            .getMonthlyLoanPayments());

    ImGui::Columns(1);

    ImGui::Separator();

    // --------------------------------------------------------
    // STATUS MESSAGE
    // --------------------------------------------------------

    if (!statusMessage_.empty())
    {
        ImGui::TextWrapped(
            "%s",
            statusMessage_.c_str());

        ImGui::Spacing();
    }

    // --------------------------------------------------------
    // MAIN BUTTONS
    // --------------------------------------------------------

    if (ImGui::Button(
            "BUY ASSET",
            ImVec2(175.0f, 50.0f)))
    {
        closeOtherWindows(
            true,
            false,
            false,
            false);

        showBuyWindow_ = true;
    }

    ImGui::SameLine();

    if (ImGui::Button(
            "SELL ASSET",
            ImVec2(175.0f, 50.0f)))
    {
        closeOtherWindows(
            false,
            true,
            false,
            false);

        showSellWindow_ = true;
    }

    ImGui::SameLine();

    if (ImGui::Button(
            "LOAN CENTER",
            ImVec2(175.0f, 50.0f)))
    {
        closeOtherWindows(
            false,
            false,
            true,
            false);

        showLoanWindow_ = true;
    }

    ImGui::SameLine();

    if (ImGui::Button(
            "FINANCE",
            ImVec2(150.0f, 50.0f)))
    {
        closeOtherWindows(
            false,
            false,
            false,
            true);

        showFinanceWindow_ = true;
    }

    ImGui::SameLine();

    if (ImGui::Button(
            "NEXT MONTH",
            ImVec2(175.0f, 50.0f)))
    {
        game_.nextMonth();

        statusMessage_ =
            "A new month has begun.";

        if (game_.isGameOver())
        {
            closeOtherWindows(
                false,
                false,
                false,
                false);
        }
    }

    ImGui::SameLine();

    if (ImGui::Button(
            "EXIT",
            ImVec2(100.0f, 50.0f)))
    {
        window_.close();
    }

    ImGui::End();
}


// ============================================================
// BUY WINDOW
// ============================================================

void GameWindow::drawBuyWindow()
{
    ImGui::SetNextWindowSize(
        ImVec2(540.0f, 440.0f),
        ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowPos(
        ImVec2(330.0f, 110.0f),
        ImGuiCond_FirstUseEver);

    ImGui::Begin(
        "Buy Assets",
        &showBuyWindow_);

    ImGui::Text(
        "Available Cash: $%.2f",
        game_.getPlayer().getCash());

    ImGui::TextDisabled(
        "Prices reflect current economic conditions.");

    ImGui::Separator();

    const char* names[] =
    {
        "Apple Stock",
        "Government Bond",
        "Bitcoin",
        "House",
        "Fixed Deposit",
        "Index Fund"
    };

    for (int i = 0; i < 6; ++i)
    {
        const double currentPrice =
            game_.getAssetPurchasePrice(i + 1);

        ImGui::PushID(i);

        ImGui::Text(
            "%s",
            names[i]);

        ImGui::SameLine(240.0f);

        ImGui::Text(
            "$%.2f",
            currentPrice);

        ImGui::SameLine(390.0f);

        if (ImGui::Button(
                "BUY",
                ImVec2(80.0f, 30.0f)))
        {
            if (game_.buyAsset(i + 1))
            {
                statusMessage_ =
                    std::string("Purchased ") +
                    names[i] +
                    ".";

                showBuyWindow_ = false;
            }
            else
            {
                statusMessage_ =
                    "Not enough cash.";
            }
        }

        ImGui::PopID();
    }

    ImGui::Separator();

    if (ImGui::Button(
            "BACK",
            ImVec2(100.0f, 35.0f)))
    {
        showBuyWindow_ = false;
    }

    ImGui::End();
}


// ============================================================
// SELL WINDOW
// ============================================================

void GameWindow::drawSellWindow()
{
    ImGui::SetNextWindowSize(
        ImVec2(560.0f, 430.0f),
        ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowPos(
        ImVec2(320.0f, 110.0f),
        ImGuiCond_FirstUseEver);

    ImGui::Begin(
        "Sell Assets",
        &showSellWindow_);

    const Portfolio& portfolio =
        game_.getPlayer().getPortfolio();

    if (portfolio.assetCount() == 0)
    {
        ImGui::TextDisabled(
            "No assets available to sell.");
    }
    else
    {
        for (std::size_t i = 0;
             i < portfolio.assetCount();
             ++i)
        {
            const Asset& asset =
                portfolio.getAsset(i);

            ImGui::PushID(
                static_cast<int>(i));

            ImGui::Text(
                "%s",
                asset.getName().c_str());

            ImGui::SameLine(280.0f);

            ImGui::Text(
                "$%.2f",
                asset.getValue());

            ImGui::SameLine(430.0f);

            if (ImGui::Button(
                    "SELL",
                    ImVec2(80.0f, 30.0f)))
            {
                const std::string soldName =
                    asset.getName();

                if (game_.sellAsset(i))
                {
                    statusMessage_ =
                        std::string("Sold ") +
                        soldName +
                        ".";

                    showSellWindow_ = false;

                    ImGui::PopID();
                    ImGui::End();

                    return;
                }

                statusMessage_ =
                    "Unable to sell that asset.";
            }

            ImGui::PopID();
        }
    }

    ImGui::Separator();

    if (ImGui::Button(
            "BACK",
            ImVec2(100.0f, 35.0f)))
    {
        showSellWindow_ = false;
    }

    ImGui::End();
}


// ============================================================
// LOAN WINDOW
// ============================================================

void GameWindow::drawLoanWindow()
{
    ImGui::SetNextWindowSize(
        ImVec2(530.0f, 430.0f),
        ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowPos(
        ImVec2(335.0f, 110.0f),
        ImGuiCond_FirstUseEver);

    ImGui::Begin(
        "Loan Center",
        &showLoanWindow_);

    ImGui::Text(
        "Current Cash: $%.2f",
        game_.getPlayer().getCash());

    ImGui::Text(
        "Current Debt: $%.2f",
        game_.getPlayer().getTotalDebt());

    ImGui::Text(
        "Monthly Payments: $%.2f",
        game_.getPlayer()
            .getMonthlyLoanPayments());

    ImGui::Separator();

    ImGui::Text("Loan Amount");

    ImGui::InputDouble(
        "##LoanAmount",
        &loanAmount_,
        1000.0,
        5000.0,
        "%.2f");

    if (loanAmount_ < 1000.0)
    {
        loanAmount_ = 1000.0;
    }

    ImGui::Spacing();

    if (ImGui::Button(
            "TAKE PERSONAL LOAN",
            ImVec2(240.0f, 45.0f)))
    {
        if (game_.getPlayer().takeLoan(
                loanAmount_,
                0.10,
                24))
        {
            statusMessage_ =
                "Personal loan approved.";

            showLoanWindow_ = false;
        }
        else
        {
            statusMessage_ =
                "Loan request rejected.";
        }
    }

    ImGui::Spacing();

    if (ImGui::Button(
            "TAKE MORTGAGE",
            ImVec2(240.0f, 45.0f)))
    {
        if (game_.getPlayer().takeLoan(
                loanAmount_,
                0.08,
                120))
        {
            statusMessage_ =
                "Mortgage approved.";

            showLoanWindow_ = false;
        }
        else
        {
            statusMessage_ =
                "Mortgage request rejected.";
        }
    }

    ImGui::Separator();

    ImGui::Text(
        "Total Debt: $%.2f",
        game_.getPlayer().getTotalDebt());

    ImGui::Text(
        "Monthly Debt Service: $%.2f",
        game_.getPlayer()
            .getMonthlyLoanPayments());

    ImGui::Spacing();

    if (ImGui::Button(
            "BACK",
            ImVec2(100.0f, 35.0f)))
    {
        showLoanWindow_ = false;
    }

    ImGui::End();
}


// ============================================================
// FINANCE WINDOW
// ============================================================

void GameWindow::drawFinanceWindow()
{
    ImGui::SetNextWindowSize(
        ImVec2(530.0f, 410.0f),
        ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowPos(
        ImVec2(335.0f, 120.0f),
        ImGuiCond_FirstUseEver);

    ImGui::Begin(
        "Financial Management",
        &showFinanceWindow_);

    ImGui::Text(
        "Monthly Salary: $%.2f",
        game_.getPlayer().getSalary());

    ImGui::Text(
        "Monthly Expenses: $%.2f",
        game_.getPlayer()
            .getMonthlyExpenses());

    ImGui::Separator();

    if (ImGui::Button(
            "INVEST IN SKILLS - $15000",
            ImVec2(280.0f, 45.0f)))
    {
        if (game_.getPlayer().upgradeSkills())
        {
            statusMessage_ =
                "Skill upgrade successful.";
        }
        else
        {
            statusMessage_ =
                "Not enough cash "
                "for the skill upgrade.";
        }
    }

    ImGui::Spacing();

    if (ImGui::Button(
            "REDUCE EXPENSES - $3000/MONTH",
            ImVec2(280.0f, 45.0f)))
    {
        if (game_.getPlayer()
                .cutDiscretionaryExpenses())
        {
            statusMessage_ =
                "Monthly expenses reduced.";
        }
        else
        {
            statusMessage_ =
                "Monthly expenses cannot "
                "be reduced further.";
        }
    }

    ImGui::Separator();

    ImGui::Text(
        "Salary: $%.2f",
        game_.getPlayer().getSalary());

    ImGui::Text(
        "Expenses: $%.2f",
        game_.getPlayer()
            .getMonthlyExpenses());

    ImGui::Spacing();

    if (ImGui::Button(
            "BACK",
            ImVec2(100.0f, 35.0f)))
    {
        showFinanceWindow_ = false;
    }

    ImGui::End();
}


// ============================================================
// FINAL RESULT WINDOW
// ============================================================

void GameWindow::drawGameOverWindow()
{
    ImGui::SetNextWindowPos(
        ImVec2(250.0f, 90.0f),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(700.0f, 520.0f),
        ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;

    const char* title =
        game_.playerWon()
            ? "WEALTHCRAFT - YOU WIN!"
            : "WEALTHCRAFT - GAME OVER";

    ImGui::Begin(
        title,
        nullptr,
        flags);

    ImGui::Spacing();
    ImGui::Spacing();

    if (game_.playerWon())
    {
        ImGui::Text(
            "CONGRATULATIONS!");

        ImGui::Spacing();

        ImGui::TextWrapped(
            "You successfully survived all 12 months "
            "and reached the required financial target.");
    }
    else
    {
        if (game_.getPlayer().isBankrupt())
        {
            ImGui::Text(
                "BANKRUPTCY");

            ImGui::Spacing();

            ImGui::TextWrapped(
                "Your financial position became "
                "unsustainable before the challenge ended.");
        }
        else
        {
            ImGui::Text(
                "CHALLENGE FAILED");

            ImGui::Spacing();

            ImGui::TextWrapped(
                "You survived all 12 months, but did "
                "not reach the required financial target.");
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text(
        "Final Month: %d",
        game_.getCurrentMonth());

    ImGui::Spacing();

    ImGui::Text(
        "Final Cash: $%.2f",
        game_.getPlayer().getCash());

    ImGui::Text(
        "Final Portfolio Value: $%.2f",
        game_.getPlayer()
            .getPortfolio()
            .calculateTotalValue());

    ImGui::Text(
        "Final Debt: $%.2f",
        game_.getPlayer().getTotalDebt());

    ImGui::Text(
        "Final Net Worth: $%.2f",
        game_.getPlayer()
            .calculateNetWorth());

    ImGui::Spacing();

    ImGui::Text(
        "Target Net Worth: $100000.00");

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button(
            "EXIT GAME",
            ImVec2(180.0f, 55.0f)))
    {
        window_.close();
    }

    ImGui::End();
}