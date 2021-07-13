
//Prints out the gcd of the two given numbers
sub gcd ($$) {
   my ($x, $y) = @_;
   while ($x != $y) {
      if ($x > $y) 
      {
        $x -= $y
      } 
        else 
        {
            $y -= $x
        }
   }
   return $x
}


//Reads in numbers from the files specified on cmd line or from std in if no files
//scan each line for numbers, then at end of file, print sum of all numbers 
while ($line = <>) 
{
   $count += $& while $line =~ s/\d+//;
}
print $count, "\n";


//linear search findpred, giv ena predicate and a sequence of pairs, return the value associated with the first
//key that the predicate matches. Determine the not-found value from the examples. Do not use any higher-order
//functions
sub findpred ($@) {
   my ($pred, @array) = @_;
   for my $pair (@array) {
      return $pair->[1] if $pred->($pair->[0]);
   }
   return undef;
}


//Similar to above, but finds lines, words and char count
while ($line = <>) {
   ++$lines;
   $chars += length $line;
   @words = $line =~ m/(\S+)/g;
   $words += @words;
}
print "$lines $words $chars\n";

## Alternate way of incrementing $words:
## $words++ while $line =~ s/\S+//;


//Take files in argv and print out their stats - file size, mod time, filename
for $file (@ARGV) {
   @stat = stat $file;
   $size = $stat[7];
   $time = strftime "%b %e %H:%S", localtime $stat[9];
   printf "%6d %s %s\n", $size, $time, $file;
}


//Read in a sequence of graph links, print out corresponding graph
while ($line = <>) {
   next unless $line =~ m/(\S+)\s*->\s*(\S+)/;
   my ($key, $val) = ($1, $2);
   push @{$graph{$key}}, $val;
}


//Loop to print out the contents of the graph 
//(keys in lexographic order, then values from same key also in order)
for $key (sort keys %graph) {
   print "$key ->";
   print " $_" for sort @{$graph{$key}};
   print "\n";
}


//Same as above, goes through file and counts words, chars, lines
//just different syntax
use strict;
use warnings;

my $lines = 0;
my $words = 0;
my $chars = 0;

while (defined (my $line = <>)) {
   ++$lines;
   $chars += length $line;
   while ($line =~ s/\S+//) { ++$words }
}

print "$lines $words $chars\n";


//Finds all unique words in the file, and then prints all the words, 
//and number of times that word is found in the file
use strict;
use warnings;

my %hash;
while (my $line = <>) {
   while ($line =~ s/\w+//) {
      ++$hash{$&}
   }
}

for my $key (sort keys %hash) {
   print "$key $hash{$key}\n";
}
