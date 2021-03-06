//
// Created by PolarAir on 2019-04-16.
//

#ifndef KUNGFU_POSITION_MANAGER_H
#define KUNGFU_POSITION_MANAGER_H

#include "pnl_def.h"
#include <functional>
#include <memory>

namespace kungfu
{
    class PositionManager final : public IPnLDataHandler
    {
    public:
        explicit PositionManager(const char* account_id, const char* db);
        ~PositionManager();

        Position get_long_pos(const char* instrument_id, const char* exchange_id) const;
        Position get_short_pos(const char* instrument_id, const char* exchange_id) const;
        double get_last_price(const char* instrument_id, const char* exchange_id) const;
        std::vector<Instrument> get_all_pos_instruments() const;
        double get_market_value() const; // stock only

        // IPnLDataHandler
        void on_quote(const Quote* quote) override;
        void on_order(const Order* order) override;
        void on_trade(const Trade* trade) override;
        void on_positions(const std::vector<Position>& positions) override;
        void on_position_details(const std::vector<Position>& details) override;
        void on_account(const AccountInfo& account) override;
        void insert_order(const OrderInput* input) override;
        bool freeze_algo_order(uint64_t algo_id, const AssetsFrozen& frozen) override;
        void release_algo_order(uint64_t algo_id) override;
        void switch_day(const std::string& trading_day) override;
        int64_t get_last_update() const override;
        std::string get_current_trading_day() const override;
        void set_current_trading_day(const std::string& trading_day) override;
        void register_pos_callback(PositionCallback cb) override;
        void register_acc_callback(AccountCallback cb) override;
        void register_pnl_callback(PnLCallback cb) override;
        void set_initial_equity(double equity) override;
        void set_static_equity(double equity) override;
        // IPnLDataHandler

    private:
        class impl;
        impl* impl_;
    };
    typedef std::shared_ptr<PositionManager> PositionManagerPtr;
}

#endif //KUNGFU_POSITION_MANAGER_H
