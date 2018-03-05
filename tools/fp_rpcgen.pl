#!/usr/bin/perl -w
#*********************************************************************
#
# (C) Copyright Broadcom Corporation 2010-2014
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
#*********************************************************************
#
# Common RPCGEN script for all supported API group types
#

use warnings;
use strict;

use File::Basename;
use File::Path qw(make_path remove_tree);
use Getopt::Long;

my @arglist = @ARGV;
my $numargs = $#arglist + 1;

# list of makefile packages supported by this script
#
# Note: It was easier to support both packages in the
#       same script file than it was to split out the
#       common subroutines into a separate file or
#       Perl module.
#
my $pkgid_openapi   = 'rpcgen/openapi';
my $pkgid_usmdb     = 'rpcgen/usmdb';
my $pkgid_luaconn   = 'luaconn';
my $pkgid_ofdpa     = 'rpcgen/ofdpa';

# list of all group API names supported by this script
my $api_usmdb     = 'usmdb';
my $api_openapi   = 'openapi';
my $api_ofdpa     = 'ofdpa';

my $pkgid = $pkgid_usmdb;                       # makefile package name (default: rpcgen/usmdb)

my $proto_api = '';                             # Prototyping API component is present
my $verbose = '';                               # controls script output
my $debug = '';                                 # controls script debug output

# process command line parameters
#
GetOptions('pkgid=s'  => \$pkgid,
           'proto_api'  => \$proto_api,
           'verbose'  => \$verbose,
           'debug'    => \$debug);

# force verbose mode if debugging is specified
$verbose = 1 if $debug;

# Set this to 1 for standalone testing of script outside of make process
my $debugmode = 0;                              # set to 0 for normal usage

# NORMAL Settings
# base path to view name must be defined or this script will not work.
my $basepath = ($pkgid eq $pkgid_ofdpa) ? $ENV{'OFDPA_BASE'} : $ENV{'FP_BASE'} or die;
my $outpath = "$basepath/$ENV{'OUTPATH'}";         # relative path to view name
my $outdir = "$outpath/objects/$pkgid";            # generated file output directory
my $fpre = '';                                     # file name prefix

# init customization parameter values (defaults to usmdb)
my $group = 'USMDB';                            # group name in upper-case (default: USMDB)
my $compid = 'luaweb';                          # generated file header component id (default: luaweb)
my $apiname = 'usmdb';                          # unique API name identifier string
my $apiname2 = 'usm[Dd]b';                      # more precise API name identifier string
my $apiname3 = 'Usmdb';                         # API name token used in function names, etc.
my $buffdesc = 'L7_buffdesc';                   # buffer descriptor type name (default: L7_buffdesc)
my $fidrc = 'L7_int32';                         # generic return code type for FID functions
my $dummyrc = 'L7_int';                         # return code type for dummy rc (i.e. void) functions
my $clientapiprefix = 'rpcclt_';                # prefix used on client APIs
my $uintvartype = "L7_uint32";                  # type of 32 bit unsigned integers
my $ucharvartype = "L7_uchar8";                 # type of 8 bit unsigned characters
my $rpcprefix = "L7_";                          # prefix before various APIs and types
my $pkgname = "FASTPATH";                       # name used in descriptions

if ($pkgid eq $pkgid_openapi) {
   # override customization parameter values for OPEN API
   $group = 'OPENAPI';                          # group name in upper-case
   $compid = 'openclt';                         # generated file header component id
   $apiname = 'openapi';                        # unique API name identifier string
   $apiname2 = 'openapi[^_ ]';                  # more precise API name identifier string
   $apiname3 = 'Openapi';                       # API name token used in function names, etc.
   $buffdesc = 'open_buffdesc';                 # buffer descriptor type name
   $fidrc = 'int';                              # generic return code type for FID functions
   $dummyrc = 'int';                            # return code type for dummy rc (i.e. void) functions
} elsif ($pkgid eq $pkgid_ofdpa) {
   # override customization parameter values for OFDPA
   $group = 'OFDPA';                            # group name in upper-case
   $compid = 'openclt';                         # generated file header component id
   $apiname = 'ofdpa';                          # unique API name identifier string
   $apiname2 = 'ofdpa[^_ ]';                    # more precise API name identifier string
   $apiname3 = 'Ofdpa';                         # API name token used in function names, etc.
   $buffdesc = 'ofdpa_buffdesc';                # buffer descriptor type name
   $fidrc = 'int';                              # generic return code type for FID functions
   $dummyrc = 'int';                            # return code type for dummy rc (i.e. void) functions
   if ($proto_api) {
       $clientapiprefix = '';                   # Null prefix to make the client API names the same as the server.
   }
   $uintvartype = "uint32_t";                   # type of 32 bit unsigned integers
   $ucharvartype = "uint8_t";                   # type of 8 bit unsigned characters
   $rpcprefix = '';                             # prefix before various APIs and types
   $pkgname = '';                               # name used in descriptions
   # override path settings
   $outpath = $ENV{'OUTPATH'};                  # relative path to view name
   $outdir = "$ENV{'OUTPATH'}/rpc";              # generated file output directory
}

# globals
my @protos = ();                                # list of function prototypes found
my @rejects = ();                               # list of function prototypes rejected
my @fsigsrev = ();                              # list of unique API function signatures -- index-to-key reverse lookup
                                                #
my %syshdrs = ();                               # hash of system #include headers
my %lochdrs = ();                               # hash of local non-API #include headers
my %apihdrs = ();                               # hash of local API #include headers
my %fsigsct = ();                               # hash of unique API function signatures -- key occurrence counts
my %fsigsidx = ();                              # hash of unique API function signatures -- key-to-index mapping

my $fprefix = $apiname."Wrapper";               # API wrapper function name prefix
my $fidx_offset = 1;                            # offset to convert from from array index to wrapper function name index

if ($debugmode != 0) {
   # DEBUG Settings
   $basepath = $ENV{'TMP_FP_BASE'};             # relative path to view name
   $outpath = "$basepath/$ENV{'TMP_OUTPATH'}";  # relative path to view name
   $outdir = "tempgen/$pkgid";                  # generated file output directory
   $fpre = 'tmp_';                              # file name prefix
   $verbose = 1;                                # force verbose output
   $debug = 1;                                  # force debug output
}

# external file names
my $outfile_good = "$outdir/_protos_accepted";
my $outfile_bad  = "$outdir/_protos_rejected";
my $outfile_dtyp = "$outdir/_protos_datatypes";
my $outfile_dbg  = "$outdir/_protos_dumpout1";
my $outfile_pkg  = "$outdir/_protos_pkg_";
my $outfile_sigs = "$outdir/_protos_signatures";

# external dump files (for debugging)
my $outfile_clt_cfile   = "$outdir/_dump_rpcclt_cfile";
my $outfile_clt_map_hdr = "$outdir/_dump_rpcclt_map_hdr";
my $outfile_clt_usr_hdr = "$outdir/_dump_rpcclt_usr_hdr";

# external build messages (formatted same as makefile msgs)
my $fmtmsg_fmtstr = "%-25s:  %s\n";
my $fmtmsg_generating  = "GENERATING...[$pkgid]";
my $fmtmsg_processing = "PROCESSING...[$pkgid]";

my $maxptrparms = 0;                            # max number of function pointer parms seen
my @maxptrparms_fn = ();                        # functions needing max pointer parms
my $maxparms = 0;                               # max number of function parms seen
my @maxparms_fn = ();                           # functions needing max parms

# core package name for generated .pkg files used by tolua++
my $core_pkg = 'luafp';

# common return code values
my $RC_SUCCESS = 1;
my $RC_FAILURE = 0;

# common regular expression for parsing API function parameter data types
my $fproto = qr/(.*?)($apiname\w+)\((.*?)\)/i;
my $datatype_re = qr/(\w+\s*\**\s*)(.*)/;
my $qual_datatype_re = qr/(const\s+)?(\w+\s*\**\s*)(.*)/;


#### HELPER SUBROUTINES ####

# custom sort subroutine -- $a and $b always provided upon entry
sub by_api_name {
   my ($ua, $ub) = ($a, $b);
   my $pat = qr/.*?($apiname\w*)\s*\(/i;
   my $tmp = '';
   # only try to compare function names if both
   # comparators successfully match the pattern
   # string (otherwise compare the original
   # values)
   if ($a =~ $pat) {
      $tmp = $1;
      if ($b =~ $pat) {
         $ub = $1;
         $ua = $tmp;
      }
   }
   "\L$ua" cmp "\L$ub";
}

# get the default initialization value for the specified return type
sub get_default_rc_val {
   my ($retc) = @_;
   my $default = '0';

   if ($apiname eq $api_usmdb) {
      if ($retc eq 'L7_RC_t') {
         $default = 'L7_FAILURE';
      } elsif ($retc eq 'L7_BOOL') {
         $default = 'L7_FALSE';
      }
   }
   elsif ($apiname eq $api_openapi) {
      $default = 'OPEN_E_RPC';
   }
   elsif ($apiname eq $api_ofdpa) {
      $default = 'OFDPA_E_RPC';
   }

   return $default;
}

# get the prototype hash key from the function signature
sub get_proto_hash_key {
   my ($proto) = @_;

   # pick apart the function prototype (all have same general form)
   my ($retc, $func, $allparms) = ($_ =~ ($fproto));
   my @parms = split /,/, $allparms;
   my $numparms = $#parms + 1;
   my $nument = $numparms;
   my $datatype;
   my $varname;
   my $i;
   my $key;

   # record each arg data type in the hash
   my @dtypes = ();
   $retc =~ s/\s+$//;
   for ($i = 0; $i < $numparms; $i++) {
      ($datatype, $varname) = $parms[$i] =~ $datatype_re;
      $datatype =~ s/\s+//g;                # eliminate all spaces
      $dtypes[$i] = $datatype;
   }
   $key = join ",", $retc, @dtypes;

   return $key;
}

# clean up back-to-back conditional statements
# with no intervening lines
sub clean_cond_stmts {
   my @list = ();
   my $numpop = 0;

   for (@_) {
      if (/^\s*#(if|elif|else)/) {
         $numpop++;
      } elsif (! /^\s*#endif/) {
         $numpop = 0;
      } elsif ($numpop > 0) {
         # clean up special case of #endif with nothing but conditional
         # statement(s) preceding it
         while ($numpop-- > 0) {
            pop(@list);
         }
         next;
      }
      push(@list, $_);
   }
   return @list;
}

# count actual number of function prototype entries
# in a list (ignoring any conditional statements)
sub count_protos {
   my $count = 0;

   for (@_) {
      $count++ if (! /^\s*#/);
   }
   return $count;
}

# track the maximum number of parms used by a single function
sub update_max_parms_stats {
   my ($num, $func, $sref_maxp, $aref_maxp_fn) = @_;
   if ($num >= $$sref_maxp) {
      if ($num > $$sref_maxp) {
         $$sref_maxp = $num;
         @$aref_maxp_fn = ();
      }
      push(@$aref_maxp_fn, $func);
   }
}

# print the contents of a list to a file
sub print_list_to_file {
   my ($file) = shift @_;
   open TMP, ">$file";
   print TMP @_;
   close TMP;
}

sub output_preproc_stmt {
   my ($line, $newl) = @_;
   my $str;

   # treat #filename as a special token used to identify
   # the source .c file being processed
   if ($line =~ /#filename:\s+(.*)/) {
      ($str = $1) =~ s/\.\.\///g;
      print "$newl/* $str */\n";
   } elsif ($line =~ /^\s*#endif/) {
      print $line;
   } else {
      print "$newl$line";
      $newl = "";
   }
   return $newl;
}

#
# Create _protos_datatypes file.
#
sub print_datatypes_to_file {
   my ($file, $aref_protos) = @_;
   my %dtypes;
   my %rtypes;
   my $key;

   open TMP, ">$file";
   select TMP;

   foreach (@$aref_protos) {
      # ignore preprocessor conditional statements
      next if (/^\s*#/);
      # pick apart the function prototype (all have same general form)
      my ($retc, $func, $allparms) = ($_ =~ ($fproto));
      my @parms = split /,/, $allparms;
      my $numparms = $#parms + 1;
      my $nument = $numparms;
      my $qual;
      my $datatype;
      my $varname;
      my $i;

      $retc =~ s/\s+$//;
      if ($numparms > 0) {
         ($datatype, $varname) = $parms[0] =~ $datatype_re;
         if ($datatype =~ /^\s*void\s*$/) {
            $nument = 0;
         }
      }

      # record each arg data type in the hash (including RC)
      $rtypes{$retc}++;
      $dtypes{$retc}++;
      for ($i = 0; $i < $numparms; $i++) {
         ($datatype, $varname) = $parms[$i] =~ $datatype_re;
         $datatype =~ s/\s+/ /g;                # reduce multiple spaces to single space
         $datatype =~ s/\s+$//g;                # remove trailing spaces
         $dtypes{$datatype}++;
      }
   }

   # print the overall data types
   printf "List of $group arg data types (count of occurrences):\n";
   foreach $key (sort keys %dtypes) {
      print "  $key ($dtypes{$key})\n";
   }
   print "\n";

   # print just the return types
   printf "\nList of $group return types (count of occurrences):\n";
   foreach $key (sort keys %rtypes) {
      print "  $key ($rtypes{$key})\n";
   }
   print "\n";

   select STDOUT;
   close TMP;

   return $RC_SUCCESS;
} #endsub print_datatypes_to_file

#
# Create _protos_signatures file.
#
sub print_protosigs_to_file {
   my ($file, $aref_protos, $href_fsigsidx, $href_fsigsct, $aref_fsigsrev) = @_;
   my @dtypes;
   my $key;
   my $fidx;
   my $idx;

   open TMP, ">$file";
   select TMP;

   %$href_fsigsidx = ();
   %$href_fsigsct = ();
   @$aref_fsigsrev = ();

   foreach (@$aref_protos) {
      # ignore preprocessor conditional statements
      next if (/^\s*#/);
      $key = get_proto_hash_key($_);
      $$href_fsigsct{$key}++;
   }

   # create reverse lookup and hash key index lookup tables
   #
   # Note: Begin with normal array index 0, but wrapper
   #       function names are defined as index+1.
   $idx = 0;
   foreach $key (keys %$href_fsigsct) {
      $$aref_fsigsrev[$idx] = $key;
      $$href_fsigsidx{$key} = $idx++;
   }
   $idx--;

   # print the function signature lookup array
   printf "List (0-based) of all unique $group function signatures (count of occurrences):\n";
   foreach (0..$idx) {
      $key = $$aref_fsigsrev[$_];
      $fidx = $_ + $fidx_offset;
      print "  [$_] $fprefix$fidx:  $key ($$href_fsigsct{$key})\n";
   }
   print "\n";

   select STDOUT;
   close TMP;

   return $RC_SUCCESS;
} #endsub print_protosigs_to_file


#### FILE PARSING SUBROUTINES ####

#
# Parse source file to extract all function prototypes
#
sub processSrcFile {
   my ($fname, $fdbg, $aref_protos, $aref_rejects) = @_;  # get input file name, arrays to update
   my $filesep = "#filename: $fname\n";         # separator line between files
   my $lines = '';
   my $prev = '';
   my $curr = '';
   my $disc = 0;
   my $match = '';
   my $keywds = '';
   my $preproc = '';
   my $delim = '';
   my $sepr = '@@@';
   my $is_paren;
   my @items;
   my @notsupp;
   my @temp;

   if ($fname =~ /.*\.h$/) {
      $delim = ';';
   } elsif ($fname =~ /.*\.c$/) {
      $delim = '\{';
   } else {
      warn "Must be a $group .h or .c file!";
      return ();
   }

   if ($fdbg) {
      open DUMP, ">$fdbg" if $debug;
   }

   if (! open SRC, "<$fname") {
      warn "Cannot open $fname for processing\n";
      return ();
   }
   $lines = join $sepr, <SRC>;
   close SRC;

   # conditioning: post-process the file string in multiple steps (update as needed):
   $lines =~ s/\r\n/\n/g;                       # replace CRLF with LF
   $lines =~ s/\/\*(.|\n)*?\*\///g;             # remove C comments
   $lines =~ s/\/\/.*\n/\n/g;                   # remove C++ comments
   $lines =~ s/(extern|EXT_API) //g;            # remove leading 'extern' or 'EXT_API'

   # remove code blocks within outermost { }
   while ($lines =~ s/{[^{]*?}//gs) { }

   @temp = split /$sepr/, $lines;
   # join lines between function parameter parentheses
   $is_paren = 0;
   foreach (@temp) {
      s/^\s+//;
      $is_paren++ if (/\(/);
      $is_paren-- if (/\)/);
      s/\n$// if $is_paren;

      # also use this loop to record #include statements
      # in its appropriate hash
      if (/^\s*#include (.*)/) {
         my $tok = $1;
         if ($tok =~ /<.*?>/) {
            $syshdrs{$tok} += 1;
         } elsif ($tok =~ /"$apiname.*?"/) {
            $apihdrs{$tok} += 1;
         } else {
            $lochdrs{$tok} += 1;
         }
      }
   }

   # more conditioning:
   $lines = join ' ', @temp;
   $lines =~ s/^[ ]+//g;                        # remove leading whitespace at beginning of file
   $lines =~ s/\n[ ]+/\n/g;                     # remove leading whitespace between lines
   $lines =~ s/\n\s*($apiname2)/ $1/g;          # remove newline that precedes API function name (to join with its return type)
   $lines =~ s/[ ]+/ /g;                        # replace multiple whitespace with single
   $lines =~ s/\(\s+/\(/g;                      # remove spaces after '('
   $lines =~ s/\s+\)/\)/g;                      # remove spaces before ')'
   $lines =~ s/\n/\n@@@/g;                      # add separator for upcoming split

   # pre-compile match regex string
   $match = qr/\s*(\S+.*?$apiname\w*\s*\([^\(\)]*\))/i;  # API function signature
   $keywds = 'static';                          # specific C keywords to filter
   $preproc = 'define |include |pragma ';       # pre-processor keywords to filter

   @items = $filesep;
   @notsupp = $filesep;
   @temp = split /@@@/,$lines;
   for (@temp) {
      # filtering: discard residual lines in multiple steps (update as needed):
      next if /^\s*#($preproc)/;                # remove specified pre-processor statements
      next if (/^\s*[^#]?($keywds) /);          # remove entries beginning with a specified keyword

      # copy the conditional statements as they occur
      if (/^\s*#/) {
         push(@items, $_);
         next;
      }

      s/\s*(\()\s*/$1/g;                        # remove space(s) before and after '('
      s/\s*(\))\s*/$1/g;                        # remove space(s) before and after ')'

      # matching: look specifically for API function signature
      # (ending delimeter differs for .h versus .c files)
       if (/$match/) {
         ($curr = $1) =~ s/\s+/ /g;
         # reject any entry containing a char or void pointer (rc or parm)
         # (this also covers the L7_char8 xxx[] form)
         if ($curr =~ /(u?char8?|u?int8|void)(\s*\*|\s+\w+\[\s*\])/) {
            push(@notsupp, $curr.";\n");
         # reject any entry containing a [u]int8 pointer (rc or parm)
         # (this also covers the L7_uint8 xxx[] form)
         #
         # Note: L7_[u]int8 is intended to point to a single byte, but has
         #       been misused as an unbounded string pointer instead of
         #       using L7_[u]char8.  This match will cause some legitimate
         #       APIs to be rejected until the offending APIs are re-classified
         #       to avoid using the L7_[u]int8 data type, after which this
         #       rule MUST be removed.
         } elsif ($curr =~ /(u?int8)(\s*\*|\s+\w+\[\s*\])/) {
            push(@notsupp, $curr.";\n");
         # reject any entry containing an array xxx[] parameter (rc or parm)
         # (there is no way to bound the data size for RPC)
         } elsif ($curr =~ /(\w+\s*\[\s*\])/) {
            push(@notsupp, $curr.";\n");
         # reject any entry containing a float or double value or pointer (rc or parm)
         # (this also covers the L7_double64 data type)
         } elsif ($curr =~ /(float|double|double64)(\s*\*|\s+\w+)/) {
            push(@notsupp, $curr.";\n");
         # reject any entry containing a pointer return type of any kind
         } elsif ($curr =~ /(\w+\s*\*+\s*$apiname2\w+)/) {
            push(@notsupp, $curr.";\n");
         } else {
            push(@items, $curr.";\n");
         }
      }
   }
   printf DUMP "@items" if $debug;

   # make another pass to clean up back-to-back #if*/#else/#elif/#endif
   # containing no other intervening lines
   @items = &clean_cond_stmts(@items);

   # update the accumulation lists
   push @$aref_protos, @items if $#items >= 0;
   push @$aref_rejects, @notsupp if $#notsupp >= 0;

   close DUMP if $debug;

   return (@items);
} #endsub processSrcFile

#
# Write file header using specified info elements
#
sub writeFileHdr {
   my ($fname, $purpose, $component, $comments) = @_;     # collect input args
   my ($wkdy, $mon, $day, $time, $year) = split /\s+/, localtime;
   #my $date = join "-", $day, $mon, $year;
   my $blk = '';

if ($pkgid eq $pkgid_openapi) {
   $blk = <<EOL;
/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-$year. All rights reserved.
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************
*
* \@filename  $fname
*
* \@purpose   $purpose
*
* \@component $component
*
* \@comments  $comments
*
* \@create    (see file timestamp)
*
* \@author    auto-gen
*
* \@end
*
**********************************************************************
*******  THIS FILE IS AUTO-GENERATED DURING THE BUILD PROCESS  *******
*******            PLEASE DO NOT MODIFY ITS CONTENTS           *******
**********************************************************************/

EOL
}else{
  $blk = <<EOL;
/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-$year. All rights reserved.
*
**********************************************************************
*
* \@filename  $fname
*
* \@purpose   $purpose
*
* \@component $component
*
* \@comments  $comments
*
* \@create    (see file timestamp)
*
* \@author    auto-gen
*
* \@end
*
**********************************************************************
*******  THIS FILE IS AUTO-GENERATED DURING THE BUILD PROCESS  *******
*******            PLEASE DO NOT MODIFY ITS CONTENTS           *******
**********************************************************************/

EOL
}


   # NOTE: Caller is responsible for establishing the desired output stream (file, etc.)
   printf $blk;

} #endsub writeFileHdr


#### FILE GENERATION SUBROUTINES ####

#
# Generate common file:  rpccom_xxxxx_all_incl.h
#
sub genRpccomApiAllInclHdr {
   my $fn = $fpre."rpccom_$apiname\_all_incl.h";
   my $ofn = "$outdir/$fn";
   my $purpose = "${pkgname} RPC common $group header includes.";
   my $component = "rpcgen";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = "";
   my $htag = $fn;

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
#ifndef $htag
#define $htag

EOL
   print $blk;

   foreach my $name (sort keys %apihdrs)  {
      printf "$newl#include $name\n";
      $newl = $newl_reinit;
   }

   $blk = <<EOL;

#endif /* $htag */
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpccomApiAllInclHdr

#
# Generate common file:  rpccom_xxxxx_includes.h
#
# NOTE: This file is currently not needed to build the
#       RPC client or server code.  This function is
#       retained in case all of the header includes
#       are needed in the future.
#
sub genRpccomApiIncludes {
   my $fn = $fpre."rpccom_$apiname\_includes.h";
   my $ofn = "$outdir/$fn";
   my $purpose = "${pkgname} RPC common header includes.";
   my $component = "rpcgen";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = $newl_reinit;
   my $htag = $fn;
   my $name;

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
#ifndef $htag
#define $htag

EOL
   print $blk;

   printf "$newl/* System Headers */\n";
   foreach $name (sort keys %syshdrs)  {
      printf "$newl#include $name\n";
      $newl = $newl_reinit;
   }

   printf "$newl\n/* Local Headers */\n";
   foreach $name (sort keys %lochdrs)  {
      printf "$newl#include $name\n";
      $newl = $newl_reinit;
   }

   printf "$newl\n/* $group Headers */\n";
   foreach $name (sort keys %apihdrs)  {
      printf "$newl#include $name\n";
      $newl = $newl_reinit;
   }

   $blk = <<EOL;

#endif /* $htag */
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpccomApiIncludes

#
# Generate common file:  rpccom_xxxxx.h
#
sub genRpccomApiHdr {
   my $fn = $fpre."rpccom_$apiname\.h";
   my $ofn = "$outdir/$fn";
   my $purpose = "${pkgname} $group function RPC common wrappers header file.";
   my $component = "rpcgen";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = $newl_reinit;
   my $htag = $fn;
   my $i = 0;
   my $fmt1 = "  %-67s = %5d%s  %s\n";

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
#ifndef $htag
#define $htag

EOL
   print $blk;

   $blk = <<EOL;
#include "flex.h"

EOL
   print $blk if $apiname ne $api_ofdpa;

   $blk = <<EOL;
/* list of $group function type IDs */
typedef enum
{
EOL
   print $blk;

   printf $fmt1, "RPCCOM_$group\_FID_NONE", $i++, ",", "/* for bounds-checking */";
   foreach (@protos) {
      # output preprocessor conditional statements as they occur
      if (/^\s*#/) {
         $newl = output_preproc_stmt($_, $newl);
      } elsif (/\s+[*]*($apiname\w+)\(/i) {
         my $fun = $1."_FID";
         printf "$newl$fmt1", $fun, $i++, ",", "";
         $newl = $newl_reinit;                  # set this for next entry
      }
   }
   printf "$newl$fmt1", "RPCCOM_$group\_FID_TOTAL", $i++, " ", "/* for bounds-checking */";

   $blk = <<EOL;

} RPCCOM_$group\_FID_t;

#endif /* $htag */
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpccomApiHdr

#
# Generate client file:  rpcclt_xxxxx_map.h or rpcclt_xxxxx_usr.h
#
sub genRpccltApiMapHdr {
   my ($kind) = @_;
   my $is_usr = ($kind eq 'usr');
   my $fn = $fpre."rpcclt_$apiname\_map.h";
   my $ofn = "$outdir/$fn";
   my $dbgfile = $outfile_clt_map_hdr;
   my $purpose = "${pkgname} $group function RPC client mapping header file.";
   my $component = $compid;
   my $comments = "";
   my $blk = '';
   my $blk_str1 = '#include "rpccom_open.h"';
   my $newl_reinit = "";
   my $newl = "";
   my $htag;
   my $fmt1  = "#define %-65s ${clientapiprefix}%s\n";
   my $fmt1s = "#define %-71s rpcclt_$fprefix%d(%s%s_FID%s)\n";
   my $fmt2s = "#undef %s\n";
   my $fidtype = "$fidrc ";
   my $fname;

   if ($is_usr) {
      # override settings needed to generate wrapper header file
      $fn = $fpre."rpcclt_$apiname\_usr.h";
      $dbgfile = $outfile_clt_usr_hdr;
      $purpose = "${pkgname} $group function signature RPC client user mapping header file.";
      $fidtype = '';
   }

   $htag = $fn;
   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   open DUMP, ">$dbgfile" if $debug;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
#ifndef $htag
#define $htag

#include "rpccom_$apiname.h"

EOL
   print $blk;

   foreach (@protos) {
      # output preprocessor conditional statements as they occur
      if (/^\s*#/) {
         $newl = output_preproc_stmt($_, $newl);
      } elsif (/\s+[*]*($apiname\w+)\(/i) {
         $fname = $1;
         if ($proto_api) {
            # uses simple function renaming
            printf "$newl$fmt1", $fname, $fname;
            printf DUMP "$fname: $fname\n" if $debug;
         } else {
            # uses function macro redefinition
            my $fn1 = "$fname(";
            my $key = get_proto_hash_key($_);
            my $idx = $fsigsidx{$key} + $fidx_offset;
            my @parms = split /,/, $key;
            my $parmlist = "";
            my $s1 = "";
            foreach (1..$#parms) {
               $parmlist .= ",p$_";
            }
            $s1 = $parmlist;
            $s1 =~ s/^,//;                         # remove leading ","
            $s1 =~ s/[\(\)]//g;                    # remove parentheses around each parameter
            # clear out mapping function parms list if first parm is missing or 'void'
            if ((! $parms[1]) || ($parms[1] =~ /^\s*void\s*$/)) {
               $parmlist = "";
               $s1 = "" if $is_usr;                # right-hand side parms also cleared for usr mapping file
            }
            $fn1 .= "$s1)";
            printf "$newl$fmt2s", $fname if $is_usr;
            printf "$newl$fmt1s", $fn1, $idx, $fidtype, $fname, $parmlist;
            printf DUMP "$fname: fn1=$fn1 key=$key idx=$idx parmlist=$parmlist s1=$s1\n" if $debug;
         }
         $newl = $newl_reinit;                  # set this for next entry
      }
   }

   $blk = <<EOL;

#endif /* $htag */
EOL
   print $blk;

   close DUMP if $debug;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpccltApiMapHdr

#
# Generate client file:  rpcclt_xxxxx_wrapper.h
#
# (only needed for wrapper function signatures)
#
sub genRpccltApiWrapperHdr {
   my $fn = $fpre."rpcclt_$apiname\_wrapper.h";
   my $ofn = "$outdir/$fn";
   my $purpose = "${pkgname} $group function signature RPC client wrappers header file.";
   my $component = $compid;
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = "";
   my $htag = $fn;
   my $fmt1 = "extern %-7s rpcclt_$fprefix%d($fidrc fid%s);";
   my @parms;
   my $datatype;
   my $key;
   my $fidx;
   my $retc;
   my $parmlist;

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
#ifndef $htag
#define $htag

/* $group wrapper function prototypes */

EOL
   print $blk;

   # list the API wrapper function prototypes
   foreach (0..$#fsigsrev) {
      $fidx = $_ + $fidx_offset;
      $key = $fsigsrev[$_];
      # split the parts of the key into an array
      @parms = split /,/, $key;
      $retc = $parms[0];
      $parmlist = "";
      # ignore parms list if first parm is missing or 'void'
      if ($parms[1] && !($parms[1] =~ /^\s*void\s*$/)) {
            foreach (1..$#parms) {
               $datatype = $parms[$_];
               $parmlist .= ", $datatype p$_";
            }
      }
      $parmlist =~ s/(\w)(\*+)\s*p/$1 $2p/g;    # insert space before first data type ptr '*'
      printf "$newl$fmt1\n", $retc, $fidx, $parmlist;
      $newl = $newl_reinit;                     # set this for next entry
   }

   $blk = <<EOL;

#endif /* $htag */
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpccltApiWrapperHdr

#
# Generate client file:  rpcclt_xxxxx.h
#
sub genRpccltApiHdr {
   my $fn = $fpre."rpcclt_$apiname\.h";
   my $ofn = "$outdir/$fn";
   my $purpose = "${pkgname} $group function RPC client header file.";
   my $component = $compid;
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = "";
   my $htag = $fn;
   my $fmt1 = "#define %-65s rpcclt_%s\n";

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   # Note: the order of the following #include statements is important
   #       (do not change arbitrarily!)
   #
   $blk = <<EOL;
#ifndef $htag
#define $htag

EOL
   print $blk;

   $blk = <<EOL;
#include "rpcclt_$apiname\_map.h"
EOL
   print $blk unless ($clientapiprefix eq '');

   $blk = <<EOL;
#include "rpccom_$apiname\_all_incl.h"
EOL
   print $blk;

   $blk = <<EOL;
#include "rpcclt_$apiname\_wrapper.h"
#include "rpcclt_$apiname\_usr.h"
EOL
   print $blk unless $proto_api;

   $blk = <<EOL;

#endif /* $htag */
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpccltApiHdr

#
# Generate client file:  rpcclt_xxxxx.c
#
sub genRpccltApiCfile {
   my $fn = $fpre."rpcclt_$apiname\.c";
   my $ofn = "$outdir/$fn";
   my $dbgfile = $outfile_clt_cfile;
   my $purpose = "${pkgname} $group function RPC client wrappers.";
   my $component = $compid;
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "\n";
   my $newl = $newl_reinit;
   my $fmt1 = "%s ${clientapiprefix}%s(";
   my $fmt2 = "  %s rc = %s, *prc = &rc;                    /* dummy variable */";
   my $fmt3 = "  %s rc = %s, *prc = &rc;";
   my $fmt4 = "  rpccltSuppWrapper$apiname3(%s_FID,";
   my $fmt5_usmdb = "                         (L7_rpccltVaParm_t)&%s";
   my $fmt5_openapi = "                          (L7_rpccltVaParm_t)&%s";
   my $fmt5_ofdpa = "                         (rpccltVaParm_t)&%s";
   my $fmt5 = ($apiname eq $api_usmdb) ? $fmt5_usmdb : ($apiname eq $api_openapi) ? $fmt5_openapi : $fmt5_ofdpa;
   my $fmt6 = "  return rc;";
   my $fmt21 = "%s rpcclt_%s(";
   my $fmt22 = "{\n  %srpcclt_$fprefix%d(%s);\n}";
   my @scanlist;
   my ($retc, $func, $allparms);
   my $idx;


   if ($proto_api) {
      @scanlist = @protos;
   } else {
      $fmt1 = "%s rpcclt_%s($fidrc fid, ";
      $fmt4 = "  rpccltSuppWrapper$apiname3(fid,";
      @scanlist = @fsigsrev;
   }

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   open DUMP, ">$dbgfile" if $debug;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   # header includes
   $blk = <<EOL;
#include <stdio.h>
#include <sys/types.h>
#include "datatypes.h"
EOL
   print $blk if $apiname eq $api_usmdb;

   $blk = <<EOL;
#include <stdio.h>
#include <stddef.h>
#include <sys/un.h>
#include "openapi_common.h"
EOL
   print $blk if $apiname eq $api_openapi;

   $blk = <<EOL;
#include "rpccom_$apiname.h"
#include "rpcclt_support.h"
#include "rpcclt_$apiname.h"

/* NOTE: The wrapper function updates the rc variable with the
 *       return code from the ${group} API function.
 */

/**********************************
 * Client-side ${group} API Functions
 **********************************
 */

EOL
   print $blk;

   $idx = 0;
   foreach (@scanlist) {
      my @parms;
      my $numparms;
      my $nument;
      my $qual;
      my $datatype;
      my $varname;
      my $i;
      my $imax;
      my $spacer = '';
      my $s1 = '';
      my $s2 = '';

      if ($proto_api) {
         # output preprocessor conditional statements as they occur
         if (/^\s*#/) {
            $newl = output_preproc_stmt($_, $newl);
            next;
         }
         # pick apart the function prototype (all have same general form)
         ($retc, $func, $allparms) = ($_ =~ ($fproto));
         @parms = split /,/, $allparms;
         $numparms = $#parms + 1;
         if ($numparms > 0) {
            ($datatype, $varname) = $parms[0] =~ $datatype_re;
         }
         printf DUMP "func=$func: numparms=$numparms retc=$retc allparms=$allparms\n" if $debug;
      } else {
         my $key = $_;
         my $fidx = $fsigsidx{$key} + $fidx_offset;
         # split the parts of the key into an array
         @parms = split /,/, $key;
         $retc = shift @parms;
         $func = "$fprefix$fidx";
         $allparms = join ",", @parms;
         $numparms = $#parms + 1;
         if ($numparms > 0) {
            ($datatype, $varname) = ($parms[0], '');
         }
         printf DUMP "$idx: fidx=$fidx key=$key: numparms=$numparms retc=$retc allparms=$allparms\n" if $debug;
         $idx++;
      }

      $retc =~ s/\s+$//;                        # remove trailing spaces from return value
      $nument = $numparms;                      # starts out the same...

      if ($numparms > 0) {
         if ($datatype =~ /^\s*void\s*$/) {
            $nument = 0;                        # ...force to 0 if first parm is void
         }
      }

      # build the function signature line
      $s1 = sprintf "$newl$fmt1", $retc, $func;
      $imax = $proto_api ? $numparms : $nument;
      for ($i = 1; $i <= $imax; $i++) {
         if ($proto_api) {
            ($datatype, $varname) = $parms[$i-1] =~ $datatype_re;
         } else {
            ($datatype, $varname) = ($parms[$i-1], "p$i");
         }
         $datatype =~ s/\s+/ /g;                # reduce multiple spaces to single space
         $datatype =~ s/\s+$//g;                # remove trailing spaces
         $datatype =~ s/\*/ \*/;                # insert space before first data type ptr '*'
         $spacer = ($datatype =~ /\*$/) ? '' : ' ';  # if ptr var or not...
         $spacer = '' if $nument == 0;               # ...override if no parms at all
         $s1 .= sprintf "$datatype$spacer$varname, ";
      }
      $s1 =~ s/, $//;                           # remove trailing ', ' sequence before closing the string
      printf "$s1)\n";

      # function body
      #
      # use return type for rc variable
      # (if it's 'void', use a dummy integer return type)
      printf "{\n";
      if ($retc eq 'void') {
         printf "$fmt2\n", $dummyrc, get_default_rc_val($dummyrc);
      } else {
         printf "$fmt3\n", $retc, get_default_rc_val($retc);
      }
      printf "$fmt4\n", $func;

      # always specify rc variable as first parameter
      $s1 = ($nument > 0) ? ",\n" : "";
      printf "$fmt5$s1", 'prc';

      # build the set of parameters for the function wrapper call
      $s1 = '';
      for ($i = 1; $i <= $nument; $i++) {
         if ($proto_api) {
            ($qual, $datatype, $varname) = $parms[$i-1] =~ $qual_datatype_re;
            $varname =~ s/[\[\]]//g;               # remove square brackets from name
         } else {
            ($qual, $datatype, $varname) = ('', $parms[$i-1], "p$i");
         }
         $s1 .= sprintf $fmt5, $varname;
         $s1 .= sprintf ",\n" if $i < $nument;  # i.e., except for last parm
      }
      $s1 .= sprintf ");";
      printf "$s1\n";
      printf "$fmt6\n" if $retc ne 'void';
      printf "}\n";
      $newl = $newl_reinit;                     # set this for next entry
   }

   close DUMP if $debug;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpccltApiCfile

#
# Generate server file:  rpcsrv_xxxxx.h
#
sub genRpcsrvApiHdr {
   my $fn = $fpre."rpcsrv_$apiname\.h";
   my $ofn = "$outdir/$fn";
   my $purpose = "${pkgname} $group function RPC server wrappers header file.";
   my $component = "rpcsrv";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = $newl_reinit;
   my $htag = $fn;
   my $fmt1  = "extern void rpcsrv_%s(${uintvartype} numParms, ${buffdesc} *pparm[], ${ucharvartype} *rcbuf);";

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
#ifndef $htag
#define $htag

EOL
   print $blk;

   foreach (@protos) {
      # output preprocessor conditional statements as they occur
      if (/^\s*#/) {
         $newl = output_preproc_stmt($_, $newl);
         next;
      }
      # pick apart the function prototype (all have same general form)
      my ($retc, $func, $allparms) = ($_ =~ ($fproto));

      printf "$newl$fmt1\n", $func;
      $newl = $newl_reinit;                     # set this for next entry
   }

   $blk = <<EOL;

#endif /* $htag */
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpcsrvApiHdr

#
# Generate server file:  rpcsrv_xxxxx.c
#
sub genRpcsrvApiCfile {
   my $fn = $fpre."rpcsrv_$apiname\.c";
   my $ofn = "$outdir/$fn";
   my $purpose = "${pkgname} $group function RPC server wrappers.";
   my $component = "rpcsrv";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "\n";
   my $newl = $newl_reinit;
   my $spaces;
   my $fmt1  = "void rpcsrv_%s(${uintvartype} numParms, ${buffdesc} *pparm[], ${ucharvartype} *rcbuf)";
   my $fmt2  = "  %s%s(";
   my $fmt3a = "pparm[%d]";
   my $fmt3b = "*pparm[%d]";
   my $fmt4  = "(%s)pparm[%d]->pstart";
   my $fmt5  = "*(%s*)pparm[%d]->pstart";
   my $fmt6  = "  memcpy(rcbuf, &%s, sizeof(%s));";

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   # header includes
   $blk = <<EOL;
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
EOL
   print $blk if $apiname eq $api_openapi;

   $blk = <<EOL;
#include <stdint.h>
EOL
   print $blk if $apiname eq $api_ofdpa;

   $blk = <<EOL;
#include "rpcsrv_support.h"
#include "rpcsrv_$apiname\.h"
#include "rpccom_$apiname\_all_incl.h"
EOL
   print $blk;

   my $d1 = sprintf $fmt1, "$apiname\_dummyFn";
   $blk = <<EOL;

static $dummyrc dummyRc = 0;

/* this is a fake function to ensure the dummyRc variable is referenced at least once by this file (DO NOT USE) */
$d1
{
  memcpy(rcbuf, &dummyRc, sizeof(dummyRc));
}


/**********************************
 * Server-side $group Functions
 **********************************
 */

EOL
   print $blk;
   foreach (@protos) {
      # output preprocessor conditional statements as they occur
      if (/^\s*#/) {
         $newl = output_preproc_stmt($_, $newl);
         next;
      }
      # pick apart the function prototype (all have same general form)
      my ($retc, $func, $allparms) = ($_ =~ ($fproto));
      my @parms = split /,/, $allparms;
      my $numparms = $#parms + 1;
      my $nument = $numparms;
      my $datatype;
      my $varname;
      my $i;
      my $fmt;
      my $s1 = '';
      my $s2 = '';

      $retc =~ s/\s+$//;
      if ($numparms > 0) {
         ($datatype, $varname) = $parms[0] =~ $datatype_re;
         if ($datatype =~ /^\s*void\s*$/) {
            $nument = 0;
         }
      }

      printf "$newl$fmt1\n", $func;
      printf "{\n";

      # always put return code info as first element
      # (if it's 'void', use a dummy return type as a msg placeholder)
      $s1 = ($retc eq 'void') ? "" : "$retc rc = ";
      $s2 = ($nument == 0) ? "" : "\n";
      printf "$fmt2$s2", $s1, $func;

      for ($i = 0; $i < $nument; $i++) {
         ($datatype, $varname) = $parms[$i] =~ $datatype_re;
         $datatype =~ s/\s+/ /g;                # reduce multiple spaces to single space
         $datatype =~ s/\s+$//g;                # remove trailing spaces
         $datatype .= " *" if ($varname =~ /\w+\s*\[\]/);   # treat 'xxx[]' as 'xxx *'
         if ($datatype =~ /$buffdesc\s*(\*?)/) {
            # if not a pointer parm, need to dereference pparm[] (i.e. use $fmt3b)
            $fmt = ($1 ne '') ? $fmt3a : $fmt3b;
            $s1 = sprintf $fmt, $i+1;
         } elsif ($datatype =~ /\*$/) {
            $s1 = sprintf $fmt4, $datatype, $i+1;
         } else {
            $s1 = sprintf $fmt5, $datatype, $i+1;
         }
         $spaces = ($retc eq 'void') ? 10 : 20;
         $s1 .= ",\n" if $i < ($nument - 1);
         $s2 = " " x $spaces;
         printf "$s2$s1";
      }
      printf ");\n";
      $s1 = ($retc eq 'void') ? "dummyRc" : "rc";
      printf "$fmt6\n", $s1, $s1;
      printf "}\n";
      $newl = $newl_reinit;                     # set this for next entry
   }

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpcsrvApiCfile

#
# Generate client and server function table files
#
sub genRpcxxxApiFtableCfile {
   my ($kind) = @_;
   my $is_cltopen = ($kind eq 'client-open');
   my $is_cltsig = ($kind eq 'client-sig');
   my $is_server = ($kind eq 'server');
   my ($fn, $purpose, $component, $comments);
   my ($name, $lbl, $dtype, $finit);
   my ($fmt1, $fmt2, $fmt3, $fmt4, $fmt5);
   my @scanlist;
   my $fmt_spec = "static %s %s_%s_SPEC =";
   my $fmt_tbl = "  %-67s = &%s_%s_SPEC;";
   my $ofn;
   my $dummy_type = $dummyrc;
   my $nullp = "(void *)NULL";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = $newl_reinit;
   my $idx = 0;
   my $str = '';

   if ($is_server) {
      $fn = $fpre."rpcsrv_$apiname\_ftable.c";
      $purpose = "${pkgname} $group RPC server function table.";
      $component = "rpcsrv";
      $comments = "";
      $name = "Server";
      $lbl = "rpcsrv";
      $dtype = "${rpcprefix}rpcsrvFtableEnt_t";
      $finit = sprintf "rpcsrv%sFtableInit", $apiname3;
      $fmt1 = "  /*%05s*/";
      $fmt2 = "  /*%05s*/   { (void *)rpcsrv_%s, %d,";
      $fmt3 = "                %1s %s";
      $fmt4 = "                }";
      $fmt5 = "              };";
      @scanlist = @protos;
   } else {
      $fn = $fpre."rpcclt_$apiname\_ftable.c";
      $purpose = "${pkgname} $group RPC client function table.";
      $component = $compid;
      $comments = "";
      $name = "Client";
      $lbl = "rpcclt";
      $dtype = "${rpcprefix}rpccltFtableEnt_t";
      $finit = sprintf "rpcclt%sFtableInit", $apiname3;
      $fmt1 = "  /*%05s*/";
      $fmt2 = "  /*%05s*/   { ${clientapiprefix}%s, 0x%x, 0x%x, %d,";
      $fmt3 = "                %1s {%s}";
      $fmt4 = "                }";
      $fmt5 = "              };";
      @scanlist = @protos;
      if ($is_cltsig) {
         # special values for condensed signature functions
         $fmt_spec = "%s %s_%s_SPEC =";
         @scanlist = @fsigsrev;
      }
   }
   $ofn = "$outdir/$fn";

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   # system header includes
   unless ($is_server) {
      $blk = <<EOL;
#include <stdio.h>
#include <sys/types.h>
EOL
      print $blk if $apiname eq $api_usmdb;

      $blk = <<EOL;
#include <stdio.h>
#include <stddef.h>
#include <sys/un.h>
EOL
      print $blk if $apiname eq $api_openapi;
   }

   # RPC header includes
   $blk = <<EOL;
#include "rpccom_$apiname\.h"
#include "$lbl\_support.h"
#include "$lbl\_$apiname\.h"
EOL
   print $blk;

   # API header includes
   $blk = <<EOL;
#include "rpccom_$apiname\_all_incl.h"
EOL
   print $blk if $is_server;

   # header includes
   $blk = <<EOL;

/* $name-side $group function table
 *
 * NOTE: This table is initialized in full during run-time.
 *       See the $finit() function below.
 */
$dtype *$lbl\_$apiname\_ftable_g[RPCCOM_$group\_FID_TOTAL+1] = { $nullp };


/******************************************************
 * $name-side $group Function Specification Structures
 ******************************************************
 */

/* NOTE: The first parm entry listed is always the $group function
 *       return type.  For functions that return 'void', a dummy
 *       type of $dummy_type is used as a placeholder in the message.
 */

EOL
   print $blk;

   $str = sprintf "$fmt1   /* index 0 not used */", $idx;
   printf "$str\n";
   $idx++;

   # first pass: create the individual function specification structures
   #
   foreach (@scanlist) {
      # output preprocessor conditional statements as they occur
      if (/^\s*#/) {
         $newl = output_preproc_stmt($_, $newl);
         next;
      }

      my $entry = $_;
      my ($retc, $func, $allparms);
      my @parms;

      if ($is_cltsig) {
         my $key = $entry;
         my $fidx = $fsigsidx{$key} + $fidx_offset;
         # split the parts of the key into an array
         @parms = split /,/, $key;
         $retc = shift @parms;
         $func = "$fprefix$fidx";
         $allparms = join ",", @parms;
      } else {
         # pick apart the function prototype (all have same general form)
         ($retc, $func, $allparms) = ($_ =~ ($fproto));
         @parms = split /,/, $allparms;
      }
      my $numparms = $#parms + 1;
      my $nument = $numparms;
      my $flags = 0;
      my $outparm_mask = 0;
      my ($qual, $datatype, $varname);
      my $numptrparms = 0;
      my $i;
      my $str_ent = '';
      my $brc = '';
      my $s1 = '';
      my $s2 = '';
      my $s3 = '';

      $retc =~ s/\s+$//;
      if ($numparms > 0) {
         if ($is_cltsig) {
            ($datatype, $varname) = ($parms[0], "p0");
         } else {
            ($datatype, $varname) = $parms[0] =~ $datatype_re;
         }
         if ($datatype =~ /^\s*void\s*$/) {
            $nument = 0;
         }
      }

      # determine which parms are output values

      # always put return code info as first element
      # (if it's 'void', use a dummy return type as a msg placeholder)
      $s1 = ($retc eq 'void') ? "sizeof($dummy_type)" : "sizeof($retc)";
      $s2 = ($nument > 0) ? ",\n" : "";
      $str_ent .= sprintf "$fmt3$s2", "{", $s1;
      $outparm_mask = 0x80000000;               # ret code is always treated as output parm

      # accumulate parm array init list in a string before printing it,
      # so the output parm mask can be built prior to writing it.
      #
      # nument is 0 if parm list is empty or contains just a 'void' entry
      for ($i = 0; $i < $nument; $i++) {
         if ($is_cltsig) {
            ($qual, $datatype, $varname) = ('', $parms[$i], "p$i");
         } else {
            ($qual, $datatype, $varname) = $parms[$i] =~ $qual_datatype_re;
         }
         $datatype =~ s/\s+/ /g;                # reduce multiple spaces to single space
         $datatype =~ s/\s+$//g;                # remove trailing spaces
         $datatype =~ s/\*/ \*/;                # insert space before first data type ptr '*'
         $varname =~ s/[\[\]]//g;               # remove square brackets from name
         if ($datatype =~ /$buffdesc\s*\*?/) {
            $s1 = sprintf "${rpcprefix}RPCCOM_DEVMSG_PARM_BUFFER_ARG";
         } else {
            ($s1 = $datatype) =~ s/[\s\*]//g;   # extract the type of pointer
            $s1 = sprintf "sizeof(%s)", $s1;
         }
         if ($is_server) {
            $str = sprintf $fmt3, " ", $s1;
         } else {
            # assume any pointer parm requires server to provide output data
            $outparm_mask |= (0x40000000 >> $i) if ($datatype =~ /\*$/);
            $numptrparms++ if ($datatype =~ /u?char8?\s*\*$/);
            $str = sprintf $fmt3, " ", $s1;
         }
         $str .= sprintf ",\n" if $i < ($nument - 1);
         $str_ent .= $str;
      }
      $str_ent .= sprintf "\n$fmt4\n";          # end of array init list


      printf "$newl$fmt_spec\n", $dtype, $lbl, $func;
      if ($is_server) {
         $s1 = sprintf "$newl$fmt2", $idx, $func, $nument+1;
      } else {
         $s1 = sprintf "$newl$fmt2", $idx, $func, $flags, $outparm_mask, $nument+1;
      }
      printf "$s1\n";
      $idx++;
      printf "$str_ent";

      print "$fmt5\n";                          # end of function entry
      $newl = $newl_reinit;                     # set this for next entry

      # update some script housekeeping stats
      update_max_parms_stats($numparms, $func, \$maxparms, \@maxparms_fn);
      update_max_parms_stats($numptrparms, $func, \$maxptrparms, \@maxptrparms_fn);
   }
   printf "$newl$fmt1\n", "-END-";
   print "\n\n";

   # second pass: create array of function specification structure pointers
   #
   $blk = <<EOL;
/****************************************
 * $name-side $group Function Table Init
 ****************************************
 */
void $finit(void)
{
  $dtype **ftable = $lbl\_$apiname\_ftable_g;

EOL
   print $blk;
   $newl = "";
   foreach (@protos) {
      my $entry = $_;

      # output preprocessor conditional statements as they occur
      if (/^\s*#/) {
         $newl = output_preproc_stmt($_, $newl);
         next;
      }
      # pick apart the function prototype (all have same general form)
      my ($junk, $func) = ($_ =~ ($fproto));

      my $s1 = sprintf "ftable[%s_FID]", $func;
      if ($is_cltsig) {
         # map the original API function signature to its corresponding
         # wrapper function
         my $fidx = $fsigsidx{get_proto_hash_key($entry)} + $fidx_offset;
         $func = "$fprefix$fidx";
      }
      printf "$newl$fmt_tbl\n", $s1, $lbl, $func;
      $newl = $newl_reinit;                     # set this for next entry
   }
   print "}\n\n";

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genRpcxxxApiFtableCfile

#
# Generate tolua++ file:  tolua_usmdb_protos_enable.h
#
sub genToluaUsmdbProtosEnableHdr {
   my ($href_modules) = @_;
   my $mod;
   my $fn = $fpre."tolua_usmdb_protos_enable.h";
   my $ofn = "$outdir/$fn";
   my $purpose = "Lua Web tolua++ USMDB prototypes enablement header file";
   my $component = "luaconn";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = $newl_reinit;
   my $htag = $fn;
   my $i = 0;
   my $fmt1 = "#define %s_%s\n";
   my $prefix = "TOLUA_ENABLE_tolua_fpdev";

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
#ifndef $htag
#define $htag
EOL
   print $blk;


   foreach $mod (sort keys %$href_modules) {

      foreach (@{$$href_modules{$mod}}) {
         print "\n" if (/^s*#filename/);   # put blank line before each file section
         # output preprocessor conditional statements as they occur
         if (/^\s*#/) {
            $newl = output_preproc_stmt($_, $newl);
         } elsif (/\s+[*]*(usmdb\w+)\(/i) {
            my $fun = $1."00";
            my $pre = $prefix."_".$mod;
            $pre = $prefix if ($mod eq $core_pkg);
            printf "$newl$fmt1", $pre, $fun;
            $newl = $newl_reinit;                  # set this for next entry
         }
      }
   }

   $blk = <<EOL;

#endif /* $htag */
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genToluaUsmdbProtosEnableHdr

#
# Generate tolua++ file:  tolua_usmdb_cleaned_api_xxx.pkg
#
sub genToluaUsmdbCleanedApiPkg {
   my ($pkg_name, @list) = @_;

   my $fn = "tolua_usmdb_cleaned_api";
   my $purpose = "Lua Web tolua++ cleaned USMDB prototypes package file";
   if ($pkg_name ne $core_pkg) {
      $fn = $fn."_$pkg_name";
      $purpose = $purpose.": $pkg_name";
   }
   $fn = $fpre.$fn.".pkg";

   my $ofn = "$outdir/$fn";
   my $component = "luaconn";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = $newl_reinit;
   my $htag = $fn;
   my $i = 0;
   my $fmt1 = "%s %s(";

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
\$pfile "tolua_luafp_common_defs.pkg"

/* include these header files in the generated wrapper code */
\$#include "l7_common.h"
\$#include "tolua_usertypes_enable.h"
\$#include "tolua_usmdb_protos_enable.h"
\$#include "luaconn_connector.h"
\$#include "rpcclt_usmdb.h"
EOL
   print $blk;

   if ($#list >= 0) {
      $blk = <<EOL;

/* parse these prototypes to generate the wrapper functions */
EOL
      print $blk;

      foreach (@list) {
         print "\n" if (/^s*#filename/);   # put blank line before each file section
         # output preprocessor conditional statements as they occur
         if (/^\s*#/) {
            $newl = output_preproc_stmt($_, $newl);
            next;
         }
         # pick apart the function prototype (all have same general form)
         my ($retc, $func, $allparms) = ($_ =~ ($fproto));
         my @parms = split /,/, $allparms;
         my $numparms = $#parms + 1;
         my $nument = $numparms;
         my $datatype;
         my $varname;
         my $i;
         my $spacer = '';
         my $s1 = '';

         $retc =~ s/\s+$//;
         if ($numparms > 0) {
            ($datatype, $varname) = $parms[0] =~ $datatype_re;
            if ($datatype =~ /^\s*void\s*$/) {
               $nument = 0;
            }
         }

         # build the function signature line
         $s1 = sprintf "$newl$fmt1", $retc, $func;
         for ($i = 0; $i < $numparms; $i++) {
            ($datatype, $varname) = $parms[$i] =~ $datatype_re;
            $datatype =~ s/\s+/ /g;                # reduce multiple spaces to single space
            $datatype =~ s/\s+$//g;                # remove trailing spaces
            $datatype .= " *" if ($varname =~ /\w+\s*\[\]/);   # treat 'xxx[]' as 'xxx *'
            $spacer = ($datatype =~ /\*$/) ? '' : ' ';  # if ptr var or not...
            $spacer = '' if $nument == 0;               # ...override if no parms at all
            $s1 .= sprintf "$datatype$spacer$varname";
            if ($i < ($numparms - 1)) {
               $s1 .= sprintf ", ";
            }
         }
         printf "$s1);\n";
      }
   }

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genToluaUsmdbCleanedApiPkg

#
# Generate tolua++ file:  tolua_wrapper_launcher.pkg
#
sub genToluaWrapperLauncher {
   my $fn = $fpre."tolua_wrapper_launcher.c";
   my $ofn = "$outdir/$fn";
   my $purpose = "Lua Web tolua++ USMDB wrapper launcher file";
   my $component = "luaconn";
   my $comments = "";
   my $blk = '';
   my $newl_reinit = "";
   my $newl = $newl_reinit;
   my $htag = $fn;
   my $i = 0;
   my $mod;
   my $fmt1 = "%s %s(";
   my $extern_fmt = "extern int luaopen_fpdev%s(lua_State* tolua_S);";
   my $invoke_fmt = "luaopen_fpdev%s(L);";
   my @modlist = qw/ base bgp ip_mcast metro mgmt_support qos routing routingv6
                     security switching system_support wireless /;
   my %modlist = (
      'base'            => "",
      'bgp'             => "L7_BGP_PACKAGE",
      'ip_mcast'        => "L7_IP_MCAST_PACKAGE",
      'metro'           => "L7_METRO_PACKAGE",
      'mgmt_support'    => "",
      'qos'             => "L7_QOS_PACKAGE",
      'routing'         => "L7_ROUTING_PACKAGE",
      'routingv6'       => "L7_IPV6_PACKAGE",
      'security'        => "",
      'switching'       => "",
      'system_support'  => "",
      'wireless'        => "L7_WIRELESS_PACKAGE",
   );

   $htag =~ s/(.*)/\U_$1/gi;
   $htag =~ s/\./_/gi;

   $ofn =~ s/\.?\.\///g;
   printf $fmtmsg_fmtstr, $fmtmsg_generating, $ofn;

   open OFILE, ">$outdir/$fn";
   select OFILE;

   # start with generic file header
   &writeFileHdr($fn, $purpose, $component, $comments);

   $blk = <<EOL;
/* include these header files in the generated wrapper code */
#include "tolua++.h"
#include "luafp_lib.h"
#include "flex.h"

/* define the complete set of externs for all tolua++ wrapper opener functions */
EOL
   print $blk;

   # output each of the module prototypes on a separate line
   printf "$extern_fmt\n", '';                  # core wrapper file
   foreach $mod (sort keys %modlist)  {
      printf "$extern_fmt\n", "_$mod";          # module wrapper file
   }

   $blk = <<EOL;

/*
 * Open each of the defined tolua USMDB wrappers
 */
int luaconnToluaWrapperOpen(lua_State *L)
{
  /* invoke each of the applicable luaopen functions for
   * the tolua wrappers
   *
   * all possible luaopen functions are listed, with conditional
   * ifdefs used for flex packages
   *
   * Note: This is necessary because the code generation script
   *       does not have access to the same build package info
   *       as the make files.
   */

EOL
   print $blk;

   print "  luaopen_fpdev(L);\n\n";

   foreach $mod (sort keys %modlist) {
      my $cond = $modlist{$mod};
      my $is_cond = ($cond ne "");

      if ($is_cond) {
         print "#ifdef $cond\n";
         print "  luaopen_fpdev_$mod(L);\n";
         print "#endif\n\n";
      } else {
         print "  luaopen_fpdev_$mod(L);\n\n";
      }
   }

   $blk = <<EOL;
  return 1;
}
EOL
   print $blk;

   select STDOUT;
   close OFILE;

   return $RC_SUCCESS;
} #endsub genToluaWrapperLauncher


#### BUILD PACKAGE MAIN SUBROUTINES ####

#
# Generate files for rpcgen build package
#
sub rpcgenMain {
   # create each generated file needed for RPC
   &genRpccomApiAllInclHdr or warn "Unable to create rpccom_$apiname\_all_incl.h file!";
   &genRpccomApiHdr or warn "Unable to create rpccom_$apiname\.h file!";
   if ($clientapiprefix ne '') {
      &genRpccltApiMapHdr('map') or warn "Unable to create rpcclt_$apiname\_map.h file!";
   }
   if (! $proto_api) {
      &genRpccltApiMapHdr('usr') or warn "Unable to create rpcclt_$apiname\_usr.h file!";
      &genRpccltApiWrapperHdr or warn "Unable to create rpcclt_$apiname\_wrapper.h file!";
   }
   &genRpccltApiHdr or warn "Unable to create rpcclt_$apiname\.h file!";
   &genRpccltApiCfile or warn "Unable to create rpcclt_$apiname\.c file!";
   &genRpcxxxApiFtableCfile($proto_api ? 'client-open' : 'client-sig') or warn "Unable to create rpcclt_$apiname\_ftable.c file!";
   &genRpcsrvApiHdr or warn "Unable to create rpcsrv_$apiname\.h file!";
   &genRpcsrvApiCfile or warn "Unable to create rpcsrv_$apiname\.c file!";
   &genRpcxxxApiFtableCfile('server') or warn "Unable to create rpcsrv_$apiname\_ftable.c file!";

   if ($debug) {
      if ($maxparms > 0) {
         print "Maximum number of parms ($maxparms) used by the following functions:\n";
         foreach (@maxparms_fn) {
            print "  $_\n";
         }
      }
      if ($maxptrparms > 0) {
         print "Maximum number of pointer parms ($maxptrparms) used by the following functions:\n";
         foreach (@maxptrparms_fn) {
            print "  $_\n";
         }
      }
   }
} #endsub rpcgenOpenapi

#
# Generate files for luaconn build package
#
sub luaconnMain {
   my ($href_modules) = @_;
   my $mod;

   # create USMDB protocol enabler file used by each tolua++ wrapper file
   &genToluaUsmdbProtosEnableHdr($href_modules) or warn "Unable to create tolua_usmdb_protos_enable.h file!";

   # create individual USMDB module .pkg files used as input to the tolua++ wrapper utility
   foreach $mod (sort keys %$href_modules) {
      &genToluaUsmdbCleanedApiPkg($mod, @{$$href_modules{$mod}}) or warn "Unable to create tolua_usmdb_cleaned_api_$mod.pkg file!";
      # Save resulting package list file for debugging.
      &print_list_to_file($outfile_pkg.$mod, @{$$href_modules{$mod}}) if $debug;
   }

   # create tolua++ wrapper launcher file
   &genToluaWrapperLauncher or warn "Unable to create tolua_wrapper_launcher.c file!";

} #endsub luaconnMain


#### MAIN SECTION ####

my @files = <STDIN>;
my @newitems = ();
my @baditems = ();
my %modules;

# Create a dummy module entry that is used to
# generate the core tolua++ wrapper file (the array
# remains empty for this entry)
#
$modules{$core_pkg} = [];

if ($debug) {
   printf "arg list is @arglist (count=%d)\n", $numargs;
   printf "file list is \n @files (count=%d)\n", $#files+1;
   print "basepath=$basepath\noutpath=$outpath\noutdir=$outdir\n";
}

foreach (@files) {
   chomp;
   my $fn = $_;
   $fn =~ s/\.?\.\///g;
   my $mod = 'base';                    # default to base module
   $mod = $1 if ($fn =~ /src\/usmdb\/(\w+)\//);
   printf $fmtmsg_fmtstr, $fmtmsg_processing, $fn if $verbose;
   @newitems = &processSrcFile($_, $outfile_dbg, \@protos, \@rejects);
   # add new items to package-specific array
   push @{$modules{$mod}}, @newitems;
   printf "  (%d items)\n", $#newitems+1 if $verbose;
}

# Note: Do not sort the @protos or @rejects lists since that will
#       mix up all of the preprocessor conditional statements.
#@protos = sort by_api_name @protos;
#@rejects = sort by_api_name @rejects;

printf "$group Totals: Accepted = %d, Rejected = %d\n",
       &count_protos(@protos), &count_protos(@rejects) if $verbose;

# save resulting list contents to file for debugging.
&print_list_to_file($outfile_good, @protos);
&print_list_to_file($outfile_bad, @rejects);
&print_datatypes_to_file($outfile_dtyp, \@protos);
&print_protosigs_to_file($outfile_sigs, \@protos, \%fsigsidx, \%fsigsct, \@fsigsrev) unless $proto_api;

# create output directory for storing generated files
if (! -e $outdir) {
   make_path($outdir, {mode => 0775}) or die "Cannot create directory $outdir ($!)";
}

if ($pkgid eq $pkgid_luaconn) {
    &luaconnMain(\%modules);
} else {
    &rpcgenMain;
}

exit(0);
