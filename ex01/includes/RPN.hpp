/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:36:30 by lvichi            #+#    #+#             */
/*   Updated: 2025/06/18 21:36:30 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <string>
#include <stack>

#define OPERATORS "+-/*"


int evaluateRPN( const std::string& rpnExpression, int* result );
int operation( const char op, const int value1, const int value2 );
