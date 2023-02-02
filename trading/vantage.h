#ifndef ALPHA_VANTAGE_CLIENT_H
#define ALPHA_VANTAGE_CLIENT_H

#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>
#include <sstream>

class AlphaVantageClient {
public:
    AlphaVantageClient(const std::string& api_key) : api_key_(api_key) {}

    std::vector<double> get_daily_prices(const std::string& symbol) {
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY_ADJUSTED&symbol=" + symbol + "&apikey=" + api_key_ + "&outputsize=full";

        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize cURL." << std::endl;
            exit(1);
        }

        std::ostringstream buffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
            exit(1);
        }

        curl_easy_cleanup(curl);
        std::string response = buffer.str();
        Json::Value root;
        Json::Reader reader;
        bool parsing_success = reader.parse(response, root);
        if (!parsing_success) {
            std::cerr << "Failed to parse API response." << std::endl;
            exit(1);
        }

        std::vector<double> prices;
        Json::Value daily_data = root["Time Series (Daily)"];

        // Print the contents of the daily_data object
        std::cout << daily_data.toStyledString() << std::endl;

        for (auto& date : daily_data.getMemberNames()) {
            double close = daily_data[date]["4. close"].asDouble();
            std::cout << close << std::endl;
            prices.push_back(close);
        }

        for (int i = 0; i < prices.size(); i++)
            std::cout << prices[i] << ' ';

        return prices;
    }

    void buy_stock(const std::string& symbol) {
        // ...
    }

    void sell_stock(const std::string& symbol) {
        // ...
    }

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::ostringstream*)userp)->write((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::string api_key_;
};

#endif  // ALPHA_
