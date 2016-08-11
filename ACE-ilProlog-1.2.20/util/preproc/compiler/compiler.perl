#!/usr/bin/perl

package comp;

$cpreproc_use_socket = 0;

use Socket;
use FileHandle;
use IPC::Open2;
use Cwd;

sub new {
    my $type = shift;
    my $self = {};
    bless $self;
}

sub open_output_prolog {
	my ($self, $name, $fname) = @_;
	print PC "source \"$name\"\n";
	print PC "prolog \"$fname\"\n";
}

sub open_output_c {
	my ($self, $name, $fname) = @_;
	print PC "source \"$name\"\n";
	print PC "noprolog \"$fname\"\n";
}

sub cant_open {
	my ($self, $name) = @_;
	print PC "deleted \"$name\"\n";
}

sub close_output {
	print PC "#PRE#EOF\n";
	print PC "\0\n\n";
}

sub prolog_header {
}

sub second_pass {
}

sub process_specific {
}

sub write_line {
    #print "$main::line\n";
    print PC $main::line;
    print PC "\n";
}

sub write_nl {
    print PC "\n";
}

sub get_gapic_suffix {
    return ".pro";
}

sub open_gapic_glue {
    my ($self) = @_;
    $suffix = $self->get_gapic_suffix();
    $file = "$main::specific_dir/$main::choppedname.glue$suffix";
    open(GGLUE, ">$file") || die "Can't create glue code: $file";
}

sub close_gapic_glue {
    close(GGLUE);
}

sub gapic_print_header {
}

sub gapic_footer {
}

sub gapic_begin {
}

sub gapic_commit {
}

sub create_socket {
	$port = 5555;
	$them = 'localhost';

	print "Connecting to CPreproc on port $port\n\n";

	$sockaddr = 'S n a4 x8';
	chop($hostname = `hostname`);

	($name, $aliases, $proto) = getprotobyname('tcp');
	($name, $aliases, $type, $len, $thisaddr) = gethostbyname($hostname);
	($name, $aliases, $type, $len, $thataddr) = gethostbyname($them);

	$this = pack($sockaddr, &AF_INET, 0, $thisaddr);
	$that = pack($sockaddr, &AF_INET, $port, $thataddr);

	socket(PC, &PF_INET, &SOCK_STREAM, $proto) || die "Socket error: $!";
	bind(PC, $this) || die "Bind error: $!";
	connect(PC, $that) || die "Connect error: $!";

	autoflush PC 1;
}

sub create_cpreproc {
	my ($version, $proc);
	my ($self, $procdir, $fname) = @_;
	$proc = "$procdir/bin/$main::aceos/PPreproc";
	$version = "$procdir/PVersion.h";
	# Check if PVersion.h is newer than PProc
	@dummy1 = stat "$version";
	@dummy2 = stat "$proc";
	# Yes?
	if ($dummy1[9] >= $dummy2[9]) {
	        # Make PPreproc binary again
		$crdir = cwd();
		$dir = $proc;
		print "Making $dir\n";
		$dir =~ s/PPreproc//;
		chdir($dir);
		$result = system("make");
		system("make clean");
		chdir($crdir);
		# Check for make error
		if ($result != 0) {
			print "PPreproc make error :-(\n";
			exit(1);
		}
		# Remove .pch file - recompile all
		unlink($fname);
		$main::PREPROC_ALL = 1;
	}
	if ($cpreproc_use_socket == 1) {
		create_socket();
	} else {
		$cp_pid = open2(*PPIN, *PC, $proc);
	}
	print PC "load \"$fname\"\n";
}

sub get_second_pass_files {
	$done = 0;
	print PC "pass2\n";
	if ($cpreproc_use_socket == 1) {
		flush PC;
		while (($done == 0) && ($line = <STDIN>)) {
			chop($line);
			if ($line eq "*DONE*") {
				$done = 1;
			} else {
				main::set_second_pass_file($line);
			}
		}
	} else {
		while (($done == 0) && ($line = <PPIN>)) {
			chop($line);
			if ($line eq "*DONE*") {
				$done = 1;
			} else {
				main::set_second_pass_file($line);
			}
		}
	}
}

sub has_error_cpreproc {
	print PC "haserrors\n";
	$line = <PPIN>;
	chop($line);
	if ($line eq "*ERROR*") {
		return 1;
	} else {
		return 0;
	}
}

sub save_cpreproc {
	my ($self, $fname) = @_;
	print PC "save \"$fname\"\n";
}

sub remove_cpreproc {
	print PC "exit\n";
	$line = <PPIN>;
}

return 1;
