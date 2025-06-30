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


template <typename T> struct pairsNode;
template <typename container> void PmergeMe( container& originalContainer );
template <typename T> int parseNumbers( T& container, char** numbers );

// DEBUG
template <typename T> void printContainer( T& container );
template <typename T> void printPairs( const void* untyped_pairs );


template <typename container>
void PmergeMe( container& originalContainer )
{
  if ( originalContainer.size() < 2 )
    return;

  typedef typename container::value_type containerType;
  typedef pairsNode<containerType> pNode;

  const unsigned int maxLevel = ( originalContainer.size() / 2 + 3 );
  const unsigned int maxPair = ( originalContainer.size() / 2 + 3 );

  pNode** pairs = new pNode*[maxLevel];
  for (unsigned int i = 0; i < maxLevel; ++i)
    pairs[i] = new pNode[maxPair]();

  unsigned int iPair = 0;
  unsigned int iLevel = 0;

  typename container::iterator it = originalContainer.begin();
  while ( it != originalContainer.end() ) {
    pairs[0][iPair].small = &(*it++);

    if ( it == originalContainer.end() ) {
      pairs[0][iPair].big = pairs[0][iPair].small;
      pairs[0][iPair].straggler = true;
      break;
    }

    pairs[0][iPair].big = &(*it++);

    if ( *pairs[0][iPair].small > *pairs[0][iPair].big )
      std::swap( pairs[0][iPair].small, pairs[0][iPair].big );
    ++iPair;
  }

  // DEBUG
  ::printPairs<containerType>( pairs );

  iLevel = 0;
  while ( pairs[iLevel][1].big ) {
    iPair = 0;
    unsigned int iPairNextLevel = 0;
    unsigned int nextLevel = iLevel + 1;

    while ( pairs[iLevel][iPair].big ) {
      pairs[nextLevel][iPairNextLevel].small = &(*pairs[iLevel][iPair].big);
      pairs[nextLevel][iPairNextLevel].smallPair = &pairs[iLevel][iPair];

      ++iPair;

      if ( !pairs[iLevel][iPair].big ) {
        pairs[nextLevel][iPairNextLevel].big = pairs[nextLevel][iPairNextLevel].small;
        pairs[nextLevel][iPairNextLevel].bigPair = pairs[nextLevel][iPairNextLevel].smallPair;
        pairs[nextLevel][iPairNextLevel].straggler = true;
        break;
      }

      pairs[nextLevel][iPairNextLevel].big = &(*pairs[iLevel][iPair].big);
      pairs[nextLevel][iPairNextLevel].bigPair = &pairs[iLevel][iPair];

      ++iPair;

      if ( *pairs[nextLevel][iPairNextLevel].small > *pairs[nextLevel][iPairNextLevel].big )
        pairs[nextLevel][iPairNextLevel].swapElements();

      ++iPairNextLevel;
    }
    ++iLevel;

    // DEBUG
    //::printPairs<containerType>( pairs );
  }

  unsigned int lastLevel = iLevel;

  // DEBUG
//  ::printPairs<containerType>( pairs );

  unsigned iLevelNext = iLevel + 1;
  while ( pairs[iLevel][0].big ) {
    unsigned int iPairNext = 0;

    for ( iPair = 0; pairs[iLevel][iPair].big; ++iPair ) {
      pairsNode<containerType> *smallPair = static_cast< pairsNode<containerType> * >( pairs[iLevel][iPair].smallPair );

      if ( smallPair )
        pairs[iLevelNext][iPairNext++] = *smallPair;

      if ( iPairNext > 1 && *pairs[iLevelNext][iPairNext - 1].big < *pairs[iLevelNext][iPairNext - 2].big)
        std::swap( pairs[iLevelNext][iPairNext - 1], pairs[iLevelNext][iPairNext - 2] );
    }

    for ( iPair = 0; pairs[iLevel][iPair].big; ++iPair ) {
      if ( pairs[iLevel][iPair].bigPair == pairs[iLevel][iPair].smallPair )
        continue;

      pairsNode<containerType>* bigPair = static_cast< pairsNode<containerType>* >( pairs[iLevel][iPair].bigPair );

      if ( bigPair ) {
        pairs[iLevelNext][iPairNext++] = *bigPair;
      }
    }



    ++iLevelNext;
    ++iLevel;
  }
  std::cout << std::endl;

  // DEBUG
  ::printPairs<containerType>( pairs );

  // Linear insert
  container sortedContainer;
  sortedContainer.push_back( *pairs[lastLevel][0].small );
  sortedContainer.push_back( *pairs[lastLevel][0].big );

  ::printContainer( sortedContainer );
  std::cout << std::endl << std::endl;

  while ( pairs[++lastLevel][0].big ) {
    iPair = 0;

//    if (!pairs[lastLevel][iPair].straggler) {
//      sortedContainer.insert(sortedContainer.begin(), *pairs[lastLevel][iPair].small);
//      ::printContainer( sortedContainer );
//      std::cout << std::endl << std::endl;
//    }



    while ( pairs[lastLevel][iPair].big ) {
      if ( pairs[lastLevel][iPair].straggler ) {
        ++iPair;
        continue;
      }

      typename container::iterator pos = sortedContainer.begin() + ( iPair * 2 ) ;

      std::cout << "Inserting: " << *pairs[lastLevel][iPair].small << "  -  searching from: " << *pos << std::endl;


      while ( *pos > *pairs[lastLevel][iPair].small ) {
        if ( pos == sortedContainer.begin() )
          break;


        if ( *pos > *pairs[lastLevel][iPair].small ) {
          ++pos;
          break;
        }
      }

      sortedContainer.insert( pos, *pairs[lastLevel][iPair].small );

      ++iPair;

      ::printContainer( sortedContainer );
      std::cout << std::endl << std::endl;
    }


  }

  originalContainer = sortedContainer;

//    unsigned int inserted = 0;
//
//    // DEBUG
//    iPair = 0;
//    while ( pairs[iLevel][iPair].big ) {
//      std::cout << "pairs[" << iLevel << "][" << iPair << "]:  "
//                << "address: " << &pairs[iLevel][iPair]
//                << "  -  small: " << *pairs[iLevel][iPair].small
//                << "  -  big: " << *pairs[iLevel][iPair].big
//                << "  -  smallPair: " << pairs[iLevel][iPair].smallPair
//                << "  -  bigPair: " << pairs[iLevel][iPair].bigPair
//                << "  -  swapped: " << pairs[iLevel][iPair].swapped
//                << std::endl;
//      ++iPair;
//    }
//    std::cout << std::endl;
//
//    iPair = 0;

//    while ( pairs[iLevel][iPair].big ) {
//
////      if ( pairs[iLevel][iPair].swapped && pairs[iLevel][iPair].smallPair ) {
////        pairsNode<containerType>* smallPairBefore;
////        pairsNode<containerType>* bigPairBefore;
////        smallPairBefore = reinterpret_cast< pairsNode<containerType>* >( pairs[iLevel][iPair].smallPair );
////        bigPairBefore = reinterpret_cast< pairsNode<containerType>* >( pairs[iLevel][iPair].bigPair );
////
////        std::swap( *smallPairBefore, *bigPairBefore );
////        smallPairBefore->swapped = !smallPairBefore->swapped;
////        bigPairBefore->swapped = !bigPairBefore->swapped;
////        std::cout << "here" << std::endl;
////      }
//
//      if ( !pairs[iLevel][1].big ) {
//        sortedContainer.push_back(*pairs[iLevel][iPair].small);
//        sortedContainer.push_back(*pairs[iLevel][iPair].big);
//        ++iPair;
//        printContainer(sortedContainer);
//        std::cout << std::endl << std::endl;
//        continue;
//      }
//
//      it = sortedContainer.begin();
//
//      if ( iPair == 0 ) {
//        sortedContainer.insert(it, *pairs[iLevel][iPair].small);
//        ++iPair;
//        printContainer(sortedContainer);
//        std::cout << std::endl << std::endl;
//        continue;
//      }
//
//      std::advance( it, ( iPair + inserted ) );
//
//      std::cout << "Comparing *it: " << *it << " > small: " << *pairs[iLevel][iPair].small << std::endl;
//      while ( it != sortedContainer.begin() && *it > *pairs[iLevel][iPair].small ) {
//        std::cout << "Comparing *it: " << *it << " > small: " << *pairs[iLevel][iPair].small << std::endl;
//        --it;
//      }
//
//      sortedContainer.insert( ++it , *pairs[iLevel][iPair].small );
//
//      printContainer(sortedContainer);
//      std::cout << std::endl << std::endl;
//
//      ++iPair;
//      ++inserted;
//    }
//
//  }
//
//  originalContainer = sortedContainer;


	//if ( ++it == originalContainer.end() ) {
	//  pendElements.push_back( small );
	//  break;
	//}

	//containerType big = *it;
  //
  //  if ( small > big )
  //    std::swap( small, big );

//    pendElements.push_back( small );
//	mainChainElements.push_back( big );
//  }

  //if ( mainChainElements.size() >= 2 )
  //  ::PmergeMe( nContainer );

//  ::linearInsert( mainChainElements, pendElements );
  //::jacobsthalInsert( nContainer );

}


//template <typename container>
//void jacobsthalInsert( container& nContainer )
//{
//  typename container::iterator     it = nContainer.begin();
//  typename container::iterator     ite = nContainer.end();
//  typename container::iterator     initial = nContainer.begin();
//
//  while ( it != ite ) {
//	typename container::iterator pos = it;
//	while ( pos != initial && *pos > (*pos).second )
//		--pos;
//	nContainer.insert( pos, (*pos).second );
//    ++it;
//  }
//}


//template <typename container>
//void linearInsert( container& nContainer )
//{
//  typename container::iterator     it = nContainer.begin();
//  typename container::iterator     ite = nContainer.end();
//  //typename container::iterator     initial = nContainer.begin();
//
//  while ( it != ite ) {
//	//typename container::iterator pos = it;
//
//	//while ( pos != initial && pos->first > it->second.first )
//		//--pos;
//
//	//pairedElements.insert( pos, it->second );
//    //++it;)
//	++it;
//	continue;
//  }
//
//  return;
//}


template <typename T>
class pairsNode {
  public:
    T* big;
    T* small;

    void* bigPair;
    void* smallPair;

    bool straggler;

    pairsNode() : big( NULL ), small( NULL ), bigPair( NULL ), smallPair( NULL ), straggler( false ) {}

    pairsNode( const pairsNode& original ) {
      big = original.big;
      small = original.small;
      bigPair = original.bigPair;
      smallPair = original.smallPair;
      straggler = original.straggler;
    }

    pairsNode& operator=( const pairsNode& original ) {
      if ( this != &original ) {
        this->big = original.big;
        this->small = original.small;
        this->bigPair = original.bigPair;
        this->smallPair = original.smallPair;
        this->straggler = original.straggler;
      }
      return *this;
    }
    ~pairsNode() {}

    void swapElements() {
      std::swap( big, small );
      std::swap( bigPair, smallPair );
    }
};


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


template <typename T>
void printPairs( const void* untyped_pairs ) {
  const pairsNode<T>* const* pairs = static_cast<const pairsNode<T>* const*>( untyped_pairs );

  unsigned int testLevel = 0;
  while ( pairs[testLevel][0].big ) {
    unsigned int testPair = 0;
    while ( pairs[testLevel][testPair].big ) {
      std::cout << "pairs[" << testLevel << "][" << testPair << "]:  "
                << "address: " << &pairs[testLevel][testPair]
                << "  -  small: " << *pairs[testLevel][testPair].small
                << "  -  big: " << *pairs[testLevel][testPair].big
                << "  -  smallPair: " << pairs[testLevel][testPair].smallPair
                << "  -  bigPair: " << pairs[testLevel][testPair].bigPair;
      if ( pairs[testLevel][testPair].straggler )
        std::cout << "  -  straggler";
      std::cout << std::endl;
      ++testPair;
    }
    std::cout << std::endl;
    ++testLevel;
  }
  std::cout << std::endl << std::endl;
}
