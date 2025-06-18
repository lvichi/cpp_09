/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:39:03 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/18 12:39:03 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "BitcoinExchange.hpp"


BitcoinExchange::BitcoinExchange() {}


BitcoinExchange::BitcoinExchange( const std::map<std::string,  double> bitcoinPrices )
{
  _bitcoinPrices = bitcoinPrices;
}


BitcoinExchange::BitcoinExchange( const BitcoinExchange& original ) :
  _bitcoinPrices(original._bitcoinPrices) {}


BitcoinExchange& BitcoinExchange::operator=( const BitcoinExchange& original )
{
  _bitcoinPrices = original._bitcoinPrices;
  return *this;
}


BitcoinExchange::~BitcoinExchange() {}


void BitcoinExchange::setBitcoinPrices( const std::map<std::string,  double> bitcoinPrices )
{
  _bitcoinPrices = bitcoinPrices;
}


const std::map<std::string,  double> BitcoinExchange::getBitcoinPrices() const
{
  return _bitcoinPrices;
}


int BitcoinExchange::getCSVData( std::map<std::string,  double>* data, const std::string dataPath )
{
  unsigned int  i = 1;
  std::string   date;
  std::string   valueStr;
  double        value;
  std::string   line;

  std::ifstream CSVFile( dataPath.c_str() );

  if ( !CSVFile.is_open() )
    return ( std::cerr << "CSV Error: could not open file '" << dataPath << "'. " << std::endl
                       << std::strerror(errno) << std::endl, 1 );

  if ( !std::getline( CSVFile, line ) || CSVFile.peek() == std::ifstream::traits_type::eof() )
    return ( std::cerr << "CSV Error: empty file '" << dataPath << "'. " << std::endl, 1 );

  while ( std::getline( CSVFile, line ) && ++i ) {
    if ( line.empty() )
      continue;

    if (
      BitcoinExchange::stringSplit( line, &date, &valueStr, DATABASE_SEPARATOR ) ||
      BitcoinExchange::dateCheck( date ) ||
      BitcoinExchange::valueCheck( valueStr, &value )
    )
      return ( std::cerr << "in line " << i << " of CSV file." << std::endl, 1 );

    // DEBUG
    // std::cout << "date: " << date;
    // std::cout << " value: " << value << std::endl;

    if ( data->find(date) != data->end() )
      return ( std::cerr << "Error: Duplicated key entry in line " << i << " of CSV file." << std::endl, 1 );

    (*data)[date] = value;
  }
  return 0;
}


void BitcoinExchange::getExchangeRate( const std::string inputPath )
{
  unsigned int  i = 1;
  std::string   date;
  std::string   valueStr;
  double        value;
  std::string   line;

  std::ifstream inputFile( inputPath.c_str() );

  if ( !inputFile.is_open() ) {
    std::cerr << "Error: could not open file '" << inputPath << "'. " << std::endl
              << std::strerror(errno) << std::endl;
    return;
  }

  if ( !std::getline( inputFile, line ) || inputFile.peek() == std::ifstream::traits_type::eof() ) {
    std::cerr << "Error: empty file '" << inputPath << "'. " << std::endl;
    return;
  }

  while ( std::getline( inputFile, line ) && ++i ) {
    if ( line.empty() )
      continue;

    else if ( BitcoinExchange::stringSplit( line, &date, &valueStr, INPUT_SEPARATOR ) )
      std::cerr << "in line " << i << "." << std::endl;

    else if ( BitcoinExchange::dateCheck( date ) )
      std::cerr << "=> " << date << " in line " << i << "." << std::endl;

    else if ( BitcoinExchange::valueCheck( valueStr, &value ) )
      std::cerr << "=> " << valueStr << " in line " << i << "." << std::endl;

    else if ( value < 0 )
      std::cerr << "Error: not a positive number in line " << i << "." << std::endl;

    else if ( value > MAX_VALUE )
      std::cerr << "Error: too large a number ( " << value << " ) in line " << i << "." << std::endl;

    else {
      // DEBUG
      // std::cout << "date: " << date;
      // std::cout << " value: " << value << std::endl;

      std::map<std::string, double>::iterator it = _bitcoinPrices.upper_bound(date);
      if ( it != _bitcoinPrices.begin() )
        --it;

      std::cout << date << " => " << value << " = " << ( value * it->second ) << std::endl;
    }
  }
}


// HELPERS
int BitcoinExchange::stringSplit( const std::string& line, std::string* date, std::string* value, char separator )
{
  unsigned int separatorPos = line.find(separator);

  if ( separatorPos > line.length() )
    return ( std::cerr << "Error: missing separator ", 1 );

  *date = BitcoinExchange::stringTrim( line.substr(0, separatorPos) );
  *value = BitcoinExchange::stringTrim( line.substr(separatorPos + 1) );

  return 0;
}


std::string BitcoinExchange::stringTrim( const std::string& str )
{
  int start = 0;
  int end   = str.length() - 1;

  if ( str.empty() )
    return str;

  while ( start < end && str[start] == ' ' )
    start++;

  while ( end >= start && str[end] == ' ' )
    end--;

  std::string strClean = str.substr(start, end - start + 1);

  return strClean;
}


int BitcoinExchange::dateCheck( const std::string& date )
{
  struct tm timeStruct;

  char* endPtr = strptime( date.c_str(), "%Y-%m-%d", &timeStruct );

  if ( endPtr == NULL || *endPtr != '\0' )
    return ( std::cerr << "Error: invalid date ", 1 );

  return 0;
}


int BitcoinExchange::valueCheck( const std::string& valueString, double* value )
{
  char* end  = 0;

  *value = std::strtod( valueString.c_str(), &end );

  if ( *end != 0 || valueString.empty() )
    return ( std::cerr << "Error: invalid value ", 1 );

  return 0;
}

