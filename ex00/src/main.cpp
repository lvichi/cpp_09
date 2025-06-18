/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:39:22 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/15 14:39:22 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>            // cout, cerr, endl
#include <string>              // string
#include <cstdlib>             // getenv
#include "BitcoinExchange.hpp"

#define DATAPATH "data.csv"


int main( int argc, char** argv )
{
  (void)argc;

  // Retrieving database and input
  std::string inputPath = ( argv[1] ) ? argv[1] : "";
  char* datapathEnv = std::getenv("DATAPATH");
  std::string databasePath = ( datapathEnv ) ? datapathEnv : DATAPATH;

  // DEBUG
  // std::cout << "inputPath: " << inputPath << std::endl;
  // std::cout << "databasePath: " << databasePath << std::endl;

  std::map<std::string, double> btcData;

  if ( BitcoinExchange::getCSVData( &btcData, databasePath ) )
    return 1;

  BitcoinExchange btcExchange( btcData );

  btcExchange.getExchangeRate( inputPath );

  return 0;
}
