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
#include <string>
#include <vector>
#include <list>
#include <sstream>       // stringstream
#include <sys/time.h>    // timeval gettimeofday
#include <exception>


template <typename T> std::string printContainerElements( T& container );
double timer();


int main( int argc, char** argv )
{
  if ( argc == 1 )
    return ( std::cerr << "Use as ./PmergeMe 3 5 1 2 3" << std::endl, 1 );


  // VECTOR
  timer();

  std::vector<unsigned int> containerVector;
  containerVector.reserve( argc - 1 );

  if ( ::parseNumbers( containerVector, &argv[1] ) )
    return ( std::cerr << "Use with positive integers numbers." << std::endl, 1 );

  std::cout << "Before: " << printContainerElements( containerVector ) << std::endl;

  ::PmergeMe( containerVector );

  std::cout << "After: " << printContainerElements( containerVector ) << std::endl;

  double vectorTimer = timer();


//  // LIST
//  timer();
//
//  std::list<unsigned int> containerList;
//
//  if ( ::parseNumbers( containerList, &argv[1] ) )
//    return ( std::cerr << "Use with positive integers numbers." << std::endl, 1 );
//
//  // std::cout << "Before: " << printContainerElements( containerList ) << std::endl;
//
//  ::PmergeMe( containerList );
//
//  std::cout << "After: " << printContainerElements( containerList ) << std::endl;
//
//  double listTimer = timer();


  std::cout << "Time to process a range of " << containerVector.size()
            << " elements with std::vector : " << vectorTimer
            << " us" << std::endl;

//  std::cout << "Time to process a range of " << containerList.size()
//            << " elements with std::list : " << listTimer
//            << " us" << std::endl;

  return 0;
}


template <typename T>
std::string printContainerElements( T& container )
{
  std::stringstream ssNumbers;

  typename T::iterator it = container.begin();

  while ( it != container.end() ) {
    ssNumbers << *it << " ";
    it++;
  }

  return ssNumbers.str();
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
