/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:31:06 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/18 21:34:21 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "RPN.hpp"
#include <iostream>
#include <string>


int main( int argc, char** argv )
{
  std::string rpnExpression = ( argv[1] ) ? argv[1] : "";
  int         result;

  if ( rpnExpression.empty() || argc != 2 ) {
    std::cerr << "Incorrect args.\nUse ./RPN \"8 8 + 2 *\"" << std::endl;
    return 1;
  }

  if ( ::evaluateRPN( rpnExpression, &result ) )
    return 1;

  std::cout << result << std::endl;

  return 0;
}
