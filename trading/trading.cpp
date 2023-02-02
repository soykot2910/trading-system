
#include <iostream>
#include "vantage.h"

int main()
{
    AlphaVantageClient client("XRJG1N3NFELAPZJR");
    client.get_daily_prices("MSFT");
}

