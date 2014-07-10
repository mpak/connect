#!/usr/bin/env perl
use strict;
use warnings;
use YAML::Tiny qw(LoadFile);
use List::Util qw(first);

our $VERSION = '0.004';


my $config = load_config("$ENV{HOME}/.c.yaml");
my @cmd = parse_command_line(@ARGV);
exec { $cmd[0] } @cmd or die "Unable execute: $!\n";



sub load_config {
	my $rc_file = shift;
	die "Config file '$rc_file' not found or not readable\n" unless -f -r $rc_file;
	return LoadFile($rc_file);
}


sub find_server {
	my $name = shift;
	for my $type ('ssh', 'telnet') {
		keys %{$config->{$type}}; # reset iterator
		while (my ($keys, $conf) = each %{$config->{$type}}) {
			next unless has_key($keys, $name);
			my ($host, @args) = split ' ', $conf;
			set_user($host) if $type eq 'ssh';
			my @cmd = parse_args(@args);
			return {
				host => $host,
				type => $type,
				args => \@cmd,
			};
		}
	}
	die "Server not found: $name\n";
}


sub has_key {
	my ($keys, $name) = @_;
	return defined first { $_ eq $name } split ' ', $keys;
}


sub set_user {
	my ($host) = @_;
	return if $host =~ /\@/;
	my $user = $config->{user} || $ENV{USER} or return;
	$_[0] = join '@', $user, $host;
}


sub parse_args {
	my @cmd;
	for my $arg (@_) {
		if (my ($flag) = $arg =~ /^:(\w+)/) {
			$arg = $config->{flags}{$flag};
			warn "Unknown flag :$flag\n" unless defined $arg;
		}
		push @cmd, $arg if defined $arg;
	}
	return @cmd;
}


sub parse_command_line {
	my ($flags_ref, $paths_ref, %opt) = separate_flags_and_paths(@_);

	my ($bin, @paths);
	if (!@$paths_ref) {
		usage();
	} elsif (@$paths_ref > 1) {
		($bin, @paths) = bin_and_paths_for_copy(@$paths_ref);
	} else {
		($bin, @paths) = bin_and_server_for_connection(@$paths_ref);
	}
	my @cmd = ($bin, @$flags_ref, @paths);

	if ($opt{show_cmd}) {
		print "@cmd\n";
		exit 0;
	}

	return @cmd;
}


sub bin_and_paths_for_copy {
	my $remote_copy;
	my @files;
	for my $path (@_) {
		my ($name, $file) = split ':', $path, 2;
		if (defined $file) {
			my $server = find_server($name);
			die "Attempt copy file with Telnet: $path\n" if $server->{type} eq 'telnet';
			$remote_copy = 1;
			push @files, join ':', $server->{host}, $file;
		} else {
			push @files, $name;
		}
	}
	die "No remote servers specified (local copy not supported)\n" unless $remote_copy;
	return ('scp', @files);
}


sub bin_and_server_for_connection {
	my $server = find_server(@_);
	return $server->{type}, @{$server->{args}}, $server->{host};
}


sub separate_flags_and_paths {
	my (@flags, @paths, %opt);
	for my $arg (@_) {
		if ($arg eq '-h') {
			$opt{show_cmd} = 1;
		} elsif ($arg =~ /^-/) {
			push @flags, $arg;
		} else {
			push @paths, $arg;
		}
	}
	return (\@flags, \@paths, %opt);
}


sub usage {
	die sprintf <<"END_USAGE";
Connection shortcuts. v$VERSION

$0 [args] server
	connect to server

$0 [args] src_path1 [src_path2...] dst_path
	copy files with `scp'

$0 -h ...
	show full command and exit

END_USAGE
}
