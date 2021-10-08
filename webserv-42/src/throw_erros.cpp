/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw_erros.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 11:16:58 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/16 12:14:07 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include "../includes/socket.hpp"
#include "../includes/ErrorHandling.hpp"

const char *Socket::CreatSocketFailed::what() const throw()
{
    return "socket failed";
}

const char *Socket::SetSocketFailed::what() const throw()
{
    return "setsockets failed";
}

const char *Socket::BindFailed::what() const throw()
{
    return "Binding failed";
}
const char *Socket::ListenFailed::what() const throw()
{
    return "Listning failed";
}

const char *Server::AcceptFailed::what() const throw()
{
    return "Acceptance failed";
}

const char *Server::SelectFailed::what() const throw()
{
    return "Selected failed";
}

const char *ErrorHandling::FileNotFound::what() const throw()
{
    return "Error file not found";
}

const char *ErrorHandling::NoComment::what() const throw()
{
    return "Error: no comment allowed";
}

const char *ErrorHandling::StartWithServer::what() const throw()
{
    return "Error: You should start with Server";
}

const char *ErrorHandling::OpenBracket::what() const throw()
{
    return "Error : after server should be only  open bracket";
}

const char *ErrorHandling::CloseBracket::what() const throw()
{
    return "Error : close  bracket in new line alone";
}

const char *ErrorHandling::NewlineBracket::what() const throw()
{
    return "Error : open bracket in new line alone";
}

const char *ErrorHandling::Oneserver::what() const throw()
{
    return "Error : No 2 server inside another one";
}

const char *ErrorHandling::NumberBrackets::what() const throw()
{
    return "Error : check The number of brackets";
}