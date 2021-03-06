#! /usr/local/bin/perl

# The contents of this file are subject to the Netscape Public License
# Version 1.0 (the "NPL"); you may not use this file except in
# compliance with the NPL.  You may obtain a copy of the NPL at
# http://www.mozilla.org/NPL/
#
# Software distributed under the NPL is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
# for the specific language governing rights and limitations under the
# NPL.
#
# The Initial Developer of this code under the NPL is Netscape
# Communications Corporation.  Portions created by Netscape are
# Copyright (C) 1998 Netscape Communications Corporation.  All Rights
# Reserved.

$klist = @ARGV[0];
$gperfopts = @ARGV[1];
$clazz = @ARGV[2];
$prefix = @ARGV[3];
$props = @ARGV[4];
$ids = @ARGV[5];


open(NAMES_FILE, "<$props");
open(HASH, "|/tools/ns/bin/gperf -T -t -l $gperfopts -NHashFunc -p -k$klist,\$ > gperf.out.$$");

# NOTE: the decl here needs to match the TAG_TABLE rules in genhash.inc!!!
print HASH 'struct StaticNameTable { char *name; int id; };
%%
';

open(ID_FILE, ">$ids");

# Load in tag names
$nextval = 0;
while (<NAMES_FILE>) {
  if (/(.*)/) {
    $prop = $1;
    $vals[$nextval] = $prop;
    $nextval = $nextval + 1;
  }
}

# Output tag id's to ID_FILE and hash info to HASH
print ID_FILE "/* Do not edit - generated by $0 */\n";
for ($i = 0; $i < $nextval; $i++) {
  $val = $vals[$i];
  $newval = $val;
  $newval =~ tr/a-z/A-Z/;
  $newval =~ s/-/_/g;
  print ID_FILE "#define $prefix$newval $i\n";
  print HASH $vals[$i] . ", $i\n";
}
print ID_FILE "#define $prefix" . "MAX $i\n";

close(NAMES_FILE);
close(HASH);
open(C, "<gperf.out.$$");
unlink("gperf.out.$$");
open(T, "<genhash.inc");

while (<T>) {
  if (/^@begin/) {
    ($name, $start, $end) =
      m#@begin[ 	]*([A-Za-z0-9_]+)[ 	]*/([^/]*)/[ 	]*/([^/]*)/#;
    $line = <C> until (eof(C) || $line =~ /$start/);
    if ($line =~ /$start/) {
      $template{$name} .= $line;
      do {
	$line = <C>;
	$template{$name} .= $line;
      } until ($line =~ /$end/ || eof(C));
    }
  } elsif (/^@include/) {
    ($name) = /@include[ 	]*(.*)$/;
    print $template{$name};
  } elsif (/^@sub/) {
    ($name, $old, $new) =
      m#@sub[ 	]*([A-Za-z0-9_]*)[ 	]/([^/]*)/([^/]*)/#;
    $template{$name} =~ s/$old/$new/g;
  } elsif (/^@classfunc/) {
    print "PRInt32 $clazz::LookupName(const char* str)\n";
  } elsif (/^@classincl/) {
    print "#include \"$clazz.h\"\n";
  } elsif (/^@/) {
    ;
  } else {
    print $_;
  }
}

# Print out tag name table at the end
print "\nconst nsStaticNameTableEntry $clazz::kNameTable[] = {\n";
for ($i = 0; $i < $nextval; $i++) {
  $val = $vals[$i];
  $newval = $val;
  $newval =~ tr/a-z/A-Z/;
  print "  { \"$vals[$i]\", $i }, \n";
}
print "};\n";
