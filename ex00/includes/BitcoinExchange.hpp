/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:20:20 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/18 12:20:20 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>        // cout, cerr, endl
#include <map>             // map
#include <string>          // string
#include <cstring>         // strerror
#include <fstream>         // ifstream
#include <ctime>           // tm, strptime
#include <cstdlib>         // strtod

#define MAX_VALUE 1000
#define DATABASE_SEPARATOR ','
#define INPUT_SEPARATOR '|'


class BitcoinExchange
{
  private:
    std::map<std::string,  double> _bitcoinPrices;
    // HELPERS
    static int stringSplit( const std::string& line, std::string* date, std::string* value, char separator );
    static std::string stringTrim( const std::string& str);
    static int dateCheck( const std::string& date );
    static int valueCheck( const std::string& valueString, double* value );

  public:
    BitcoinExchange();
    BitcoinExchange( const std::map<std::string,  double> bitcoinPrices );
    BitcoinExchange( const BitcoinExchange& original );
    BitcoinExchange& operator=( const BitcoinExchange& original );
    ~BitcoinExchange();

    void setBitcoinPrices( const std::map<std::string,  double> bitcoinPrices );

    const std::map<std::string,  double> getBitcoinPrices() const;

    static int getCSVData( std::map<std::string,  double>* data, const std::string dataPath );

    void getExchangeRate( const std::string inputPath );
};


