/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:22:24 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/20 13:22:24 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h>    // timeval gettimeofday


double timer();


int main( int argc, char** argv )
{
  if ( argc == 1 )
    return ( std::cerr << "Use as ./PmergeMe 3 5 1 2 3" << std::endl, 1 );


  // VECTOR
  std::vector<unsigned int> containerVector;
  containerVector.reserve( argc - 1 );

  if ( ::parseNumbers( containerVector, &argv[1] ) )
    return ( std::cerr << "Use with positive integers numbers." << std::endl, 1 );

  std::cout << "Before: " << printContainer( containerVector ) << std::endl;

  timer();

  ::PmergeMe( containerVector );

  double vectorTimer = timer();

//  std::cout << "After:  " << printContainer( containerVector ) << std::endl;



  // LIST
  std::list<unsigned int> containerList;

  if ( ::parseNumbers( containerList, &argv[1] ) )
    return ( std::cerr << "Use with positive integers numbers." << std::endl, 1 );

//  std::cout << "Before: " << printContainer( containerList ) << std::endl;

  timer();

  ::PmergeMe( containerList );

  double listTimer = timer();

  std::cout << "After:  " << ::printContainer( containerList ) << std::endl;


  std::cout << "Time to process a range of " << containerVector.size()
            << " elements with std::vector : " << vectorTimer
            << " us" << std::endl;

  std::cout << "Time to process a range of " << containerList.size()
            << " elements with std::list : " << listTimer
            << " us" << std::endl;


//  /* ---------- verify the std::vector ---------- */
//  bool vectorOK = true;
//  for ( unsigned int i = 0; i < containerVector.size() && vectorOK; ++i )
//    for ( unsigned int j = i + 1; j < containerVector.size(); ++j )
//      if ( containerVector[i] > containerVector[j])
//        vectorOK = false;
//
//  std::cout << "vector is " << (vectorOK ? "sorted\n" : "NOT sorted\n");
//
//  /* ---------- verify the std::list ---------- */
//  bool listOK = true;
//  for ( std::list<unsigned int>::const_iterator outer = containerList.begin(); outer != containerList.end() && listOK; ++outer) {
//    std::list<unsigned int>::const_iterator inner = outer;
//    ++inner;
//    for ( ; inner != containerList.end(); ++inner)
//      if (*outer > *inner)
//        listOK = false;
//  }
//
//  std::cout << "list is " << (listOK ? "sorted\n" : "NOT sorted\n");


  return 0;
}


double timer()
{
  static timeval timerSet = {};

  if ( !timerSet.tv_usec ) {
    gettimeofday( &timerSet, NULL );
    return 0;
  }

  timeval now;
  gettimeofday( &now, NULL );

  long seconds = now.tv_sec - timerSet.tv_sec;
  long microseconds = now.tv_usec - timerSet.tv_usec;
  double elapsed = (seconds * 1000000) + microseconds;

  timerSet.tv_usec = 0;

  return elapsed;
}
