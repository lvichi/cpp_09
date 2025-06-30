/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:24:42 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/20 13:24:42 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <cstdlib>      // strtol
#include <iostream>     // cerr
#include <climits>      // INT_MAX
#include <algorithm>    // find insert
#include <iterator>     // distance advance
#include <vector>
#include <list>


template <typename container> void PmergeMe( container& originalContainer );
template <typename T> int parseNumbers( T& container, char** numbers );


// LOG
template <typename T> void printContainer( T& container );
template <typename T> void printPairs( T& container );


// Rebinding Container to hold std::pair<T*, void*>
template <typename container> struct make_pair_container;
template <typename T, typename Alloc>
struct make_pair_container< std::vector<T, Alloc> >
{
  typedef std::pair<T*, void*> pair_value;
  typedef typename Alloc::template rebind<pair_value>::other pair_alloc;
  typedef std::vector<pair_value, pair_alloc> type;
};

template <typename T, typename Alloc>
struct make_pair_container< std::list<T, Alloc> >
{
  typedef std::pair<T*, void*> pair_value;
  typedef typename Alloc::template rebind<pair_value>::other pair_alloc;
  typedef std::list<pair_value, pair_alloc> type;
};




template <typename container>
void PmergeMe( container& originalContainer )
{
  if (originalContainer.size() < 2)
    return;

  typedef typename container::value_type containerType;
  typedef typename make_pair_container<container>::type PairContainer;
  PairContainer pairs[ originalContainer.size() / 2 ] = {};

  for ( typename container::iterator it = originalContainer.begin(); it != originalContainer.end(); ++it ) {
    containerType *big = &(*it);
    void* small = 0;

    std::pair < containerType * , void * > pair = std::make_pair(big, small);
    pairs[0].push_back(pair);
  }

  // debug
  std::cout << std::endl;
  printPairs(pairs[0]);
  std::cout << std::endl << std::endl;

  unsigned int iLevel = 0;
  while ( true ) {
    unsigned int iLevelNext = iLevel + 1;

    typename PairContainer::iterator itPairs = pairs[iLevel].begin();
    while ( itPairs != pairs[iLevel].end() ) {
      std::pair<containerType* , void*>* small = &(*itPairs);
      ++itPairs;

      std::pair<containerType* , void*>* big;
      if ( itPairs == pairs[iLevel].end() )
        big = small;
      else
        big = &(*itPairs);

      if ( *big->first < *small->first )
        std::swap( big, small );

      std::pair<containerType* , void*> pair = std::make_pair( &(*big->first), small );
      pairs[iLevelNext].push_back(pair);

      if ( itPairs == pairs[iLevel].end() ) {
        ( pairs[iLevelNext].back() ).second = 0;
        break;
      }

      ++itPairs;

    }

    // debug
    std::cout << std::endl;
    printPairs(pairs[iLevelNext]);
    std::cout << std::endl << std::endl;

    ++iLevel;

    if ( pairs[iLevel].size() == 1 )
      break;
  }

  PairContainer sorted;

  sorted.push_back();

//  while ( iLevel > 0 ) {
//    typename PairContainer::iterator itPairs = pairs[iLevel].begin();
//    while ( itPairs != pairs[iLevel].end() ) {
//
//      if ( (*itPairs).second ) {
//        mainChain.push_back( *((*itPairs).first) )
//        typename PairContainer second =
//        pendents.push_back( *((*itPairs).second) )
//      }
//
//
//      ++itPairs;
//    }
//
//    --iLevel;
//  }


//  //debug
//  std::cout << std::endl;
//  printContainer( originalContainer );
//  std::cout << std::endl << std::endl;

  return;
}


template <typename T>
int parseNumbers( T& container, char** numbers )
{
  char* end = 0;
  long number;

  for ( int i = 0; numbers[i]; i++ ) {
    number = std::strtol( numbers[i], &end, 10 );

    if ( *end != 0 || number <= 0 || number > INT_MAX )
      return ( std::cerr << "Error: Invalid number \"" << numbers[i] << "\" .\n", 1 );

    container.push_back( number );
  }

  return 0;
}




template <typename T> void printContainer( T& container )
{
  typename T::iterator it = container.begin();

  while ( it != container.end() ) {
    std::cout << *it << " ";
    it++;
  }
}


template <typename T> void printPairs( T& container ) {

  std::cout << "pair.first(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    std::cout << *pair.first << " ";
  }

  std::cout << std::endl;
  std::cout << "pairs addresses: ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    std::cout << &(*it) << " ";
  }

  std::cout << std::endl;
  std::cout << "pair.seconds(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    std::cout << pair.second << " ";
  }
}


