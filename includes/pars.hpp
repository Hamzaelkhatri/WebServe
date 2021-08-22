/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 19:31:17 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/22 19:06:37 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARS_HPP
#define PARS_HPP

#include "ErrorHandling.hpp"

class LocationManage
{
    private:


    public:
    
};

class  Server_element
{
    private:

    public:
    
};

class Parsing
{
    private:
    std::map<int,Server_element>parsing;
        

    public:
        Parsing(std::map<int, std::string>);
        ~Parsing();
};
#endif