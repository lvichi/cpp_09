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
template <typename container> void linearInsert( container& sorted, container& pendents );
template <typename T> struct triple
{
  T* big;
  void* smallPair;
  void* bigPair;

  triple() : big( 0 ), smallPair( 0 ), bigPair( 0 ) {}
  triple( T* big, void* smallPair, void* bigPair ) : big( big ), smallPair( smallPair ), bigPair( bigPair ) {}
  triple( const triple& rhs ) : big( rhs.big ), smallPair( rhs.smallPair ), bigPair( rhs.bigPair ) {}
  triple& operator=( const triple& rhs ) {
    if ( this != &rhs ) {
      this->big = rhs.big;
      this->smallPair = rhs.smallPair;
      this->bigPair = rhs.bigPair;
    }
    return *this;
  }
  ~triple() {}

  void swap() {
    if ( this->smallPair && this->bigPair ) {
      std::swap( *static_cast<triple*>( this->smallPair ),
                 *static_cast<triple*>( this->bigPair ) );
      std::swap( this->smallPair, this->bigPair );
      static_cast<triple*>(this->smallPair)->swap();
      static_cast<triple*>(this->bigPair)->swap();
    }
  }

  // Comparison operators
  bool operator<( const triple& rhs ) const { return *this->big < *rhs.big; }
  bool operator>( const triple& rhs ) const { return *this->big > *rhs.big; }
  bool operator==( const triple& rhs ) const { return *this->big == *rhs.big; }
  bool operator!=( const triple& rhs ) const { return *this->big != *rhs.big; }
  bool operator<=( const triple& rhs ) const { return *this->big <= *rhs.big; }
  bool operator>=( const triple& rhs ) const { return *this->big >= *rhs.big; }
};


template <typename T>
triple<T> makeTriple( T* big, void* smallPair = 0, void* bigPair  = 0)
{
  return triple<T>(big, smallPair, bigPair);
};



// LOG
template <typename T> void printContainer( T& container );
template <typename T> void printTriples( T& container );


// Rebinding Container to hold std::pair<T*, void*>
template <typename container> struct tripleContainer;
template <typename T, typename Alloc>
struct tripleContainer< std::vector<T, Alloc> >
{
  typedef ::triple<T> tripleValue;
  typedef typename Alloc::template rebind<tripleValue>::other tripleAlloc;
  typedef std::vector<tripleValue, tripleAlloc> type;
};

template <typename T, typename Alloc>
struct tripleContainer< std::list<T, Alloc> >
{
  typedef ::triple<T> tripleValue;
  typedef typename Alloc::template rebind<tripleValue>::other tripleAlloc;
  typedef std::list<tripleValue, tripleAlloc> type;
};




template <typename container>
void PmergeMe( container& originalContainer )
{
  if (originalContainer.size() < 2)
    return;

  typedef typename container::value_type containerType;
  typedef typename tripleContainer<container>::type tContainer;
  tContainer pairs[ originalContainer.size() / 2 + 3 ] = {};

  for ( typename container::iterator it = originalContainer.begin(); it != originalContainer.end(); ++it ) {
    containerType* big = &(*it);
    void* smallPair = 0;
    void* bigPair = 0;

    ::triple<containerType> pair = ::makeTriple( big, smallPair, bigPair );
    pairs[0].push_back(pair);
  }

//  // debug
//  std::cout << std::endl;
//  printTriples(pairs[0]);
//  std::cout << std::endl << std::endl;

  unsigned int iLevel = 0;
  while ( true ) {
    unsigned int iLevelNext = iLevel + 1;

    typename tContainer::iterator itPairs = pairs[iLevel].begin();
    while ( itPairs != pairs[iLevel].end() ) {
      ::triple<containerType>* small = &(*itPairs);
      ++itPairs;

      ::triple<containerType>* big;
      if ( itPairs == pairs[iLevel].end() )
        big = small;
      else
        big = &(*itPairs);

      if ( *big < *small ) {
//        std::cout << "Swapping big and small pairs: " << *big->big << " < " << *small->big << std::endl;
        std::swap( *big, *small );
//        static_cast<triple<containerType>*>(big)->swap();
//        static_cast<triple<containerType>*>(small)->swap();

      }

      ::triple<containerType> pair = ::makeTriple( &(*big->big), small, big  );
      pairs[iLevelNext].push_back(pair);

      if ( itPairs == pairs[iLevel].end() ) {
        ( pairs[iLevelNext].back() ).smallPair = 0;
        ( pairs[iLevelNext].back() ).bigPair = 0;
        break;
      }

      ++itPairs;

    }

//    // debug
//    std::cout << std::endl;
//    printTriples(pairs[iLevelNext]);
//    std::cout << std::endl << std::endl;

    ++iLevel;

    if ( pairs[iLevel].size() == 1 )
      break;
  }


//  // DEBUG
//  iLevel = 0;
//  while ( pairs[iLevel].size() > 0 ) {
//    std::cout << std::endl;
//    printTriples(pairs[iLevel]);
//    std::cout << std::endl << std::endl;
//    ++iLevel;
//  }
//  --iLevel;


//  std::cout << std::endl;
//  std::cout << std::endl;


  tContainer sorted;

  sorted.push_back( pairs[iLevel].back() );

//  // DEBUG
//  std::cout << std::endl;
//  std::cout << "sorted: ";
//  printTriples( sorted );
//  std::cout << std::endl << std::endl;

  while ( iLevel > 0 ) {
    tContainer pendents;

    typename tContainer::iterator itPairs = sorted.begin();
    while ( itPairs != sorted.end() ) {
      triple<containerType>* bigPair;
      if ( ( *itPairs ).bigPair )
        bigPair = static_cast<triple<containerType>*>( ( *itPairs ).bigPair );
      else
        bigPair = &(*itPairs);

      if ( !( *itPairs ).smallPair ) {
        std::swap( *itPairs, *bigPair );
        ++itPairs;
        continue;
      }
      ::triple<containerType>* smallPair = static_cast<triple<containerType>*>( ( *itPairs ).smallPair );
      pendents.push_back( *smallPair );

      std::swap( *itPairs, *bigPair );

      ++itPairs;
    }

//    //debug
//    std::cout << std::endl;
//    std::cout << "pendent: ";
//    printTriples( pendents );
//    std::cout << std::endl << std::endl;

    ::linearInsert( sorted, pendents );

//    //debug
//    std::cout << std::endl;
//    std::cout << "sorted: ";
//    printTriples( sorted );
//    std::cout << std::endl << std::endl;

    --iLevel;

  }

  container sortedContainer;
//  sortedContainer.resize( originalContainer.size() );
  for ( typename tContainer::iterator it = sorted.begin(); it != sorted.end(); ++it ) {
    sortedContainer.push_back( *(*it).big );
  }

  originalContainer = sortedContainer;

//    //debug
//    std::cout << std::endl;
//    std::cout << "sorted: ";
//    printTriples( sorted );
//    std::cout << std::endl << std::endl;


//  //debug
//  std::cout << std::endl;
//  printContainer( originalContainer );
//  std::cout << std::endl << std::endl;

  return;
}


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


template <typename T> void printTriples( T& container ) {

  std::cout << "pair.big(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    std::cout << *pair.big << " ";
  }

  std::cout << std::endl;
  std::cout << "pairs addresses: ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    std::cout << &(*it) << " ";
  }

  std::cout << std::endl;
  std::cout << "pair.smallPair(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    std::cout << pair.smallPair << " ";
  }

  std::cout << std::endl;
  std::cout << "pair.bigPair(s): ";
  for ( typename T::iterator it = container.begin(); it != container.end(); ++it ) {
    typename T::value_type pair = *it;
    std::cout << pair.bigPair << " ";
  }
}


