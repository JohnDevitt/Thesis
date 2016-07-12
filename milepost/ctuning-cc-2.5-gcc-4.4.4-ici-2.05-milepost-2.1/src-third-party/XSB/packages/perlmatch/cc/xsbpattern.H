%% This file is generated on-the-fly by plmchconfig.P
%% Do not edit--any changes will be written over.
%% XSB config:    i386-apple-darwin9.6.0-btc
%% XSB CC:        gcc
%% Perl CC:       cc
%% Perl CC Opts:  -arch i386 -arch ppc -g -pipe -fno-common -DPERL_DARWIN -no-cpp-precomp -fno-strict-aliasing -Wdeclaration-after-statement -I/usr/local/include -I/System/Library/Perl/5.8.8/darwin-thread-multi-2level/CORE 
:- import slash/1 from machine.
:- export try_match__/2, next_match__/0, do_bulk_match__/3, load_perl__/0, unload_perl__/0, get_match_resultC__/2, perl_substitute__/3, get_bulk_match_result__/3.
:- ldoption(' -rpath /System/Library/Perl/5.8.8/darwin-thread-multi-2level/CORE -L/System/Library/Perl/5.8.8/darwin-thread-multi-2level/CORE -lperl -ldbm -ldl -lm -lutil -lc').
