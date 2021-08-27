#pragma once
//======================================================================================================================
// 2021 Copyright Mystic Lake Software
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//		 File: TcpClient.h
//
//     Author: rmerriam
//
//    Created: Jun 27, 2021
//
//======================================================================================================================
#include <arpa/inet.h>  //inet_addr
#include <netdb.h>      //hostent
#include <sys/socket.h> //socket

#include "Stream.h"

namespace rvr {

class TcpClient : public Stream {
   public:
   TcpClient() {
      sock = -1;
      port = 0;
      address = "";
   }
   virtual ~TcpClient() override;

   TcpClient(TcpClient const& other) = delete;
   TcpClient(TcpClient&& other) = delete;
   TcpClient& operator=(TcpClient const& other) = delete;
   TcpClient& operator=(TcpClient&& other) = delete;

   bool conn(std::string, int);
   virtual auto read() -> uint8_t const override;
   virtual auto write(uint8_t const& ch) -> int64_t const override;

   private:
   int sock;
   std::string address;
   std::string response_data = "";
   int port;
   struct sockaddr_in server;
};

/*
 Connect to a host on a certain port number
 */
bool TcpClient::conn(std::string address, int port) {
   // create socket if it is not already created
   if(sock == -1) {
      // Create socket
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if(sock == -1) {
         perror("Could not create socket");
      }

      std::cout << "Socket created\n";
   } else { /* OK , nothing */
   }

   // setup address structure
   if(inet_addr(address.c_str()) == -1u) {
      struct hostent* he;
      struct in_addr** addr_list;

      // resolve the hostname, its not an ip address
      if((he = gethostbyname(address.c_str())) == NULL) {
         // gethostbyname failed
         herror("gethostbyname");
         std::cout << "Failed to resolve hostname\n";

         return false;
      }

      // Cast the h_addr_list to in_addr , since h_addr_list also has the ip
      // address in long format only
      addr_list = (struct in_addr**)he->h_addr_list;

      for(int i = 0; addr_list[i] != NULL; i++) {
         server.sin_addr = *addr_list[i];
         break;
      }
   }

   // plain ip address
   else {
      server.sin_addr.s_addr = inet_addr(address.c_str());
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(port);

   // Connect to remote server
   if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
      perror("connect failed. Error");
      return false;
   }

   return true;
}

/*
 Send data to the connected host
 */
auto TcpClient::write(uint8_t const& ch) -> int64_t const {
   return ::send(sock, &ch, 1, 0);
}

/*
 Receive data from the connected host
 */
auto TcpClient::read() -> uint8_t const {
   char ch;

   // Receive a reply from the server
   if(::recv(sock, &ch, 1, 0) < 0) {
      puts("recv failed");
      return -1;
   }

   return ch;
}

inline TcpClient::~TcpClient() {
}
} /* namespace rvr */
