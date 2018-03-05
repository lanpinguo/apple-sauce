#!/usr/local/bin/perl
#
# (C) Copyright Broadcom Corporation 2012-2013
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

use File::Path;
use Shell;
use Getopt::Std;

my %args;
getopts('o:', \%args);
my $output_file = $args{o};
my @ofdpaapi_include_files = @ARGV;

my $swig_related_content = q'
%feature("autodoc", "1");
%inline %{

#include <arpa/inet.h>

typedef uint32_t in_addr_t;

/* set MAC address from its string representation */
char * MACAddress_set(ofdpaMacAddr_t *address, char *a) {
   union
   {
     unsigned char  bytes[6];
     unsigned short shorts[3];
   } mac;

   if (6 != sscanf(a, " %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx ",
                   &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
   {
     if (6 != sscanf(a, " %2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx ",
                     &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
     {
       if (3 != sscanf(a, " %4hx.%4hx.%4hx ", &mac.shorts[0], &mac.shorts[1], &mac.shorts[2]))
       {
          return "ERROR: MAC Address Format";
       }
       else
       {
         /* successfully converted address string to short int values, need to handle endian issues here */
         mac.shorts[0] = htons(mac.shorts[0]);
         mac.shorts[1] = htons(mac.shorts[1]);
         mac.shorts[2] = htons(mac.shorts[2]);
       }
     }
   }
   memcpy(address->addr, mac.bytes, sizeof(address->addr));

   return a;
}

/* convert a MAC address into a string */
char * MACAddress_get(ofdpaMacAddr_t *address) {
   int ad[6];
   static char a[50];
   ad[0] = address->addr[0];
   ad[1] = address->addr[1];
   ad[2] = address->addr[2];
   ad[3] = address->addr[3];
   ad[4] = address->addr[4];
   ad[5] = address->addr[5];
   sprintf(a, "%02x:%02x:%02x:%02x:%02x:%02x", ad[0], ad[1], ad[2], ad[3], ad[4], ad[5]);
   return a;
}

/* set in6_addr address from its string representation */
char * in6Address_set(struct in6_addr *address, char *a) {

  if (inet_pton(AF_INET6, a, address) == 1)
     return a;
  else
     return NULL;
}

/* convert an IPv6 address into a string */
char * in6Address_get(struct in6_addr *address) {
   static char a[INET6_ADDRSTRLEN];

   inet_ntop(AF_INET6, address, a, sizeof(a));
   return a;
}

int BuffDesc_pstart_setbytes(ofdpa_buffdesc *buffdesc, const char *value) {
   if (!buffdesc->size) {
      printf("ERROR: Set size first\n");
      return -1;
   }
   else {
      if (buffdesc->pstart) free((char*)buffdesc->pstart);
      buffdesc->pstart = (char *) malloc(buffdesc->size+1);
      if (value) {
         memcpy(buffdesc->pstart, value, buffdesc->size+1);
      }
      return 0;
   }
}

int BuffDesc_getbyte(ofdpa_buffdesc *buffdesc, int offset) {
   if (!buffdesc->size) {
      printf("ERROR: Set size first\n");
      return -1;
   }

   if (!buffdesc->pstart) {
      printf("ERROR: pstart is NULL\n");
      return -2;
   }

   if (buffdesc->size < offset) {
      printf("ERROR: offset is beyond end of buffer\n");
      return -3;
   }

   return(buffdesc->pstart[offset]);
}

int BuffDesc_setbyte(ofdpa_buffdesc *buffdesc, int offset, const char data) {
   if (!buffdesc->size) {
      printf("ERROR: Set size first\n");
      return -1;
   }

   if (!buffdesc->pstart) {
      printf("ERROR: pstart is NULL\n");
      return -2;
   }

   if (buffdesc->size < offset) {
      printf("ERROR: offset is beyond end of buffer\n");
      return -3;
   }

   buffdesc->pstart[offset] = data;
   return 0;
}

%}

struct in6_addr
{
   uint8_t s6_addr[16];
};

struct timeval {
   long tv_sec;
   long tv_usec;
};

%include "stdint.i"
%include "cpointer.i"
%pointer_functions(uint8_t, uint8_tp);
%pointer_functions(uint16_t, uint16_tp);
%pointer_functions(uint32_t, uint32_tp);
%pointer_functions(uint64_t, uint64_tp);
';

my $ofdpawrap_content = qq|%module OFDPA_python
%{
#define SWIG_FILE_WITH_INIT 1
@{[&print_includes("#", @ofdpaapi_include_files)]}
%}
@{[$swig_related_content]}
@{[&print_includes("%", @ofdpaapi_include_files)]}
@{[&print_enums]}
|;

open(OUTFILE, ">", $output_file) || die("Can't open output file $output_file");
print OUTFILE $ofdpawrap_content;
close(OUTFILE);

sub print_enums
{
  my @enums = &get_enums;
  my $str = "";
  foreach my $enum (@enums)
  {
    $str .= "%pointer_functions(". $enum . ", " . $enum . "p" . ");\n";
  }
  return $str;
}

sub get_enums
{
  my @enums = ();

  foreach my $header_file (@ofdpaapi_include_files)
  {
    #read contents of a file into a variable
    my $content = do {
        local $/ = undef;
        open my $fh, "<", $header_file
            or die "could not open $header_file: $!";
        <$fh>;
    };
    #remove comments in the file
    $content =~ s#/\*[^*]*\*+([^/*][^*]*\*+)*/
             |//[^\n]*
             |q("(\\.|[^"\\])*" | '(\\.|[^'\\])*' | .[^/"'\\]*)
             #defined $2 ? $2 : ""#gsex;

    while ($content =~ /(typedef\s+enum\s*{[^}]*}[^;]+;)/g)
    {
      my ($tmp1, $enumName) = split("}", $1);
      $enumName =~ s/^\s+//; #remove leading spaces
      $enumName =~ s/\s+$//; #remove trailing spaces
      chop($enumName); #remove last character ';'
      $enumName =~ s/\s+$//; #remove trailing spaces
      push(@enums, $enumName);
    }
  }
  return @enums;
}

sub print_includes
{
  my ($suffix, @file_list) = @_;
  my $str = "";
  foreach my $this_file (@file_list)
  {
    $str .= $suffix . "include \"" . $this_file . "\"\n";
  }
  chomp($str);
  return $str;
}
