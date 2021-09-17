/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 23:51:29 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/17 19:34:38 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>

bool compare_end(std::string const &s1, std::string const &s2);
bool line_equal(std::string const &line, std::string const &str);
int check_details(std::string line);
int nbr_lines(std::string source);
bool is_whitespace(const std::string &s);
char **ft_charSplit(char const *s, char *set);
std::string trim(const std::string &s);



#endif