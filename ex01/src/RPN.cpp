/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:55:47 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/18 21:55:47 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "RPN.hpp"


int evaluateRPN( const std::string& rpnExpression, int* result )
{
  std::stack<int> rpnStack;
  std::string     operators = OPERATORS;
  std::string     validNumbers = "0123456789";

  for ( unsigned int i = 0; i < ( rpnExpression.length() - 1 ); i++ )
    if ( rpnExpression[i] != ' ' && rpnExpression[i + 1] != ' ' )
      return ( std::cerr << "Expression error: Tokens must be space-separated.\n", 1 );

  for ( unsigned int i = 0; i < rpnExpression.length(); i++ ) {
    if ( validNumbers.find( rpnExpression[i] ) != std::string::npos )
      rpnStack.push( rpnExpression[i] - '0' );

    else if ( operators.find( rpnExpression[i] ) != std::string::npos ) {
      if ( rpnStack.size() < 2 )
        return ( std::cerr << "Expression error: Insufficient operands for operator.\n", 1 );

      int value2 = rpnStack.top();
      rpnStack.pop();

      int value1 = rpnStack.top();
      rpnStack.pop();

      // DEBUG
      // std::cout << "value1: " << value1 << std::endl;
      // std::cout << "value2: " << value2 << std::endl;
      // std::cout << "rpnExpression[i]: " << rpnExpression[i] << std::endl;

      if ( rpnExpression[i] == '/' && value2 == 0 )
        return ( std::cerr << "Expression error: Division by 0.\n", 1 );

      rpnStack.push( operation( rpnExpression[i], value1, value2 ) );
    }

    else if ( rpnExpression[i] == ' ' )
      continue;

    else
      return ( std::cerr << "Expression error: Non-numeric or non-operator token.\n", 1 );
  }

  if ( rpnStack.size() != 1 )
    return ( std::cerr << "Expression error: Too many numbers or not enough operators.\n", 1 );

  *result = rpnStack.top();

  return 0 ;
}


int operation( const char op, const int value1, const int value2 )
{
  switch ( op ) {
    case '+':
      return value1 + value2;
    case '-':
      return value1 - value2;
    case '*':
      return value1 * value2;
    case '/':
      return value1 / value2;
    default:
      throw std::runtime_error( "Error: Invalid operator." );
  }
}
