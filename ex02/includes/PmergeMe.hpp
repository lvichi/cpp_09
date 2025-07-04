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

#include <cstdlib>      // std::strtol
#include <iostream>     // std::cerr
#include <climits>      // INT_MAX
#include <iterator>     // std::advance
#include <algorithm>    // std::swap, std::find
#include <sstream>      // stringstream
#include <vector>
#include <list>


// Parsing
template <typename T> int parseNumbers( T& container, char** numbers );

// Sorting
template <typename container> void PmergeMe( container& originalContainer );
template <typename container> void linearInsert( container& sorted, container& pendents );
template <typename container> void jacobsthalInsert( container& sorted, container& pendents );
template <typename T, typename Alloc> void jacobsthalInsert( std::list<T, Alloc>& sorted, std::list<T, Alloc>& pendents );
template <typename T> struct pairNode;

// Rebinding Container to hold std::pair<T*, void*>
template <typename container> struct pairNodeContainer;
template <typename T, typename Alloc> struct pairNodeContainer< std::vector<T, Alloc> >;
template <typename T, typename Alloc> struct pairNodeContainer< std::list<T, Alloc> >;

// LOG
template <typename T> std::string printContainer( T& container );
template <typename T> std::string printPairs( T& container );


template <typename container>
void PmergeMe( container& originalContainer )
{
  if (originalContainer.size() < 2)
    return;

  typedef typename container::value_type containerType;
  typedef typename pairNodeContainer<container>::type pNodeContainer;
  pNodeContainer* pairs = new pNodeContainer[ originalContainer.size() / 2 + 3 ];

  for ( typename container::iterator it = originalContainer.begin(); it != originalContainer.end(); ++it ) {
    containerType* big = &(*it);
    void* smallPair = 0;
    void* bigPair = 0;

    ::pairNode<containerType> pair( big, smallPair, bigPair );
    pairs[0].push_back(pair);
  }

//  // debug
//  std::cout << std::endl << printPairs(pairs[0]) << std::endl << std::endl;

  unsigned int iLevel = 0;
  while ( true ) {
    unsigned int iLevelNext = iLevel + 1;

    typename pNodeContainer::iterator itPairs = pairs[iLevel].begin();
    while ( itPairs != pairs[iLevel].end() ) {
      ::pairNode<containerType>* small = &(*itPairs);
      ++itPairs;

      ::pairNode<containerType>* big = &(*itPairs);
      if ( itPairs == pairs[iLevel].end() )
        big = small;

      if ( *big->big < *small->big )
        std::swap( *big, *small );

      ::pairNode<containerType> pair( &( *big->big ), small, big  );
      pairs[iLevelNext].push_back( pair );

      if ( itPairs == pairs[iLevel].end() ) {
        ( pairs[iLevelNext].back() ).smallPair = 0;
        break;
      }

      ++itPairs;
    }

//    // debug
//    std::cout << std::endl << printPairs(pairs[iLevelNext]) << std::endl << std::endl;

    ++iLevel;

    if ( pairs[iLevel].size() == 1 )
      break;
  }

  pNodeContainer sorted;

  sorted.push_back( pairs[iLevel].back() );

//  // DEBUG
//  std::cout << std::endl << "sorted: " << printPairs( sorted ) << std::endl << std::endl;

  while ( iLevel > 0 ) {
    pNodeContainer pendents;

    typename pNodeContainer::iterator itPairs = sorted.begin();
    while ( itPairs != sorted.end() ) {
      pairNode<containerType>* bigPair;
      if ( ( *itPairs ).bigPair )
        bigPair = static_cast<pairNode<containerType>*>( ( *itPairs ).bigPair );
      else
        bigPair = &(*itPairs);

      if ( !( *itPairs ).smallPair ) {
        std::swap( *itPairs, *bigPair );
        ++itPairs;
        continue;
      }
      ::pairNode<containerType>* smallPair = static_cast<pairNode<containerType>*>( ( *itPairs ).smallPair );
      pendents.push_back( *smallPair );

      std::swap( *itPairs, *bigPair );

      ++itPairs;
    }

//    ::linearInsert( sorted, pendents );
    ::jacobsthalInsert( sorted, pendents );

    --iLevel;

//    //debug
//    std::cout << std::endl << "pendent: " << printPairs( pendents ) << std::endl << std::endl;
//    std::cout << std::endl << "sorted: " << printPairs( sorted ) << std::endl << std::endl;
  }

  container sortedContainer;
  for ( typename pNodeContainer::iterator it = sorted.begin(); it != sorted.end(); ++it ) {
    sortedContainer.push_back( *(*it).big );
  }

  delete[] pairs;

  originalContainer = sortedContainer;

//  //debug
//  std::cout << std::endl << "sorted: " << printPairs( sorted ) << std::endl << std::endl;

//  //debug
//  std::cout << std::endl << printContainer( originalContainer ); std::cout << std::endl << std::endl;

  return;
}


template <typename T> struct pairNode
{
  T* big;
  void* smallPair;
  void* bigPair;

  pairNode() : big( 0 ), smallPair( 0 ), bigPair( 0 ) {}
  pairNode( T* big, void* smallPair, void* bigPair ) : big( big ), smallPair( smallPair ), bigPair( bigPair ) {}
  pairNode( const pairNode& rhs ) : big( rhs.big ), smallPair( rhs.smallPair ), bigPair( rhs.bigPair ) {}
  pairNode& operator=( const pairNode& rhs ) {
    if ( this != &rhs ) {
      this->big = rhs.big;
      this->smallPair = rhs.smallPair;
      this->bigPair = rhs.bigPair;
    }
    return *this;
  }
  ~pairNode() {}
};


template <typename container> void linearInsert( container& sorted, container& pendents )
{
  typename container::iterator itPendents = pendents.begin();
  unsigned int insertedIndex = 0;
  unsigned int stragglerCount = sorted.size() - pendents.size();

  while ( itPendents != pendents.end() ) {
    typename container::iterator itSorted = sorted.begin();
    std::advance( itSorted, ( insertedIndex * 2 + stragglerCount ) );

    while ( itSorted != sorted.begin() ) {
      --itSorted;
      if ( *( *itSorted ).big < *( *itPendents ).big ) {
        ++itSorted;
        break;
      }
    }

    sorted.insert( itSorted, *itPendents );

    ++itPendents;
    ++insertedIndex;
  }
}


template <typename container> void jacobsthalInsert( container& sorted, container& pendents )
{
  std::vector<unsigned int> jacobsthalIndex;

  std::vector<unsigned int> jacobsthalNumbers( 2, 1 );
  while (true) {
    unsigned int next = jacobsthalNumbers.back() + 2 * jacobsthalNumbers[ jacobsthalNumbers.size() - 2 ];
    if (next > pendents.size())
      break;
    jacobsthalNumbers.push_back(next);
  }

  for ( unsigned int k = jacobsthalNumbers.size() - 1; k >= 2; --k )
    jacobsthalIndex.push_back( jacobsthalNumbers[k] - 1);

  for ( unsigned int i = 0; i < pendents.size(); ++i )
    if ( std::find(jacobsthalIndex.begin(), jacobsthalIndex.end(), i) == jacobsthalIndex.end() )
      jacobsthalIndex.push_back(i);

//  // DEBUG
//  std::cout << std::endl << "jacobsthalIndex: " << printContainer( jacobsthalIndex ) << std::endl << std::endl;

  unsigned int insertedCount = sorted.size() - pendents.size();
  int lastJIndex = 0;
  typename container::iterator itPendents = pendents.begin();

  for ( unsigned int i = 0; i < jacobsthalIndex.size(); ++i ) {
    int jIndex = jacobsthalIndex[i];

    std::advance( itPendents, jIndex - lastJIndex );
    lastJIndex = jIndex;

    typename container::iterator itSorted = sorted.begin();
    std::advance( itSorted, ( insertedCount + jIndex )  );

    while ( itSorted != sorted.begin() ) {
      --itSorted;
      if ( *( *itSorted ).big < *( *itPendents ).big ) {
        ++itSorted;
        break;
      }
    }

    sorted.insert( itSorted, *itPendents );

    ++insertedCount;
  }
}


template <typename T, typename Alloc> void jacobsthalInsert( std::list<T, Alloc>& sorted, std::list<T, Alloc>& pendents )
{
  std::vector<unsigned int> jacobsthalIndex;

  std::vector<unsigned int> jacobsthalNumbers( 2, 1 );
  while (true) {
    unsigned int next = jacobsthalNumbers.back() + 2 * jacobsthalNumbers[ jacobsthalNumbers.size() - 2 ];
    if (next > pendents.size())
      break;
    jacobsthalNumbers.push_back(next);
  }

  for ( unsigned int k = jacobsthalNumbers.size() - 1; k >= 2; --k )
    jacobsthalIndex.push_back( jacobsthalNumbers[k] - 1);

  for ( unsigned int i = 0; i < pendents.size(); ++i )
    if ( std::find(jacobsthalIndex.begin(), jacobsthalIndex.end(), i) == jacobsthalIndex.end() )
      jacobsthalIndex.push_back( i );

//  // DEBUG
//  std::cout << std::endl << "jacobsthalIndex: " << printContainer( jacobsthalIndex ) << std::endl << std::endl;

  int stragglersCount = sorted.size() - pendents.size();
  int lastJIndex = 0;
  int backIndex = 0;
  typename std::list<T, Alloc>::iterator itPendents = pendents.begin();
  typename std::list<T, Alloc>::iterator itSorted = sorted.begin();

  std::advance( itSorted, stragglersCount );

  for ( unsigned int i = 0; i < jacobsthalIndex.size(); ++i ) {
    int jIndex = jacobsthalIndex[i];

    std::advance( itPendents, ( jIndex - lastJIndex ) );
    std::advance( itSorted, ( jIndex - lastJIndex ) + backIndex );

    lastJIndex = jIndex;

    backIndex = 0;
    while ( itSorted != sorted.begin() ) {
      --itSorted;
      if ( *( *itSorted ).big < *( *itPendents ).big ) {
        ++itSorted;
        break;
      }
      ++backIndex;
    }

    sorted.insert( itSorted, *itPendents );

//    ++insertedCount;
  }
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


template <typename T> std::string printContainer( T& container )
{
  std::stringstream sStream;

  typename T::iterator it = container.begin();

  while ( it != container.end() ) {
    sStream << *it << " ";
    it++;
  }

  return sStream.str();
}


template <typename T> std::string printPairs( T& container ) {
  std::stringstream sStream;

  sStream << "pair.big(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    sStream << *pair.big << " ";
  }

  sStream << std::endl;
  sStream << "pairs addresses: ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    sStream << &(*it) << " ";
  }

  sStream << std::endl;
  sStream << "pair.smallPair(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    sStream << pair.smallPair << " ";
  }

  sStream << std::endl;
  sStream << "pair.bigPair(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    sStream << pair.bigPair << " ";
  }

  return sStream.str();
}


template <typename T, typename Alloc>
struct pairNodeContainer< std::vector<T, Alloc> >
{
  typedef ::pairNode<T> pairNodeValue;
  typedef typename Alloc::template rebind<pairNodeValue>::other pairNodeAlloc;
  typedef std::vector<pairNodeValue, pairNodeAlloc> type;
};

template <typename T, typename Alloc>
struct pairNodeContainer< std::list<T, Alloc> >
{
  typedef ::pairNode<T> pairNodeValue;
  typedef typename Alloc::template rebind<pairNodeValue>::other pairNodeAlloc;
  typedef std::list<pairNodeValue, pairNodeAlloc> type;
};
