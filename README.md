## Shortcut for ssh, scp and telnet commands

No more need remember and type long host names, just configure short aliases for them.

Script written in Perl. Configuration is stored in YAML format.

### USAGE

```sh
# optional arguments for ssh or telnet command
c [arguments] server
# copy files with scp
c [arguments] server:/from/path1 to/local/path
c [arguments] from/local/path1 server:/to/path
# show full command
c -h server
# show only host (for usage in scripts)
c server^
```

### EXAMPLES

With config from `c.example.yaml`

#### SSH

```sh
# connect via ssh
c radius
# both connect to same server:
c db
c mysql
```

#### SCP

```sh
# copy files from server
c radius:/etc/hosts .
c radius:/etc/hosts radius-hosts
c radius:/path/to/{file1,file2}
c -r radius:/file1 radius:/file2 voip:/some/dir /tmp/

# copy local directory and file to server
c -r some/dir misc.txt radius:/tmp/
```

#### Telnet

```sh
# connect via telnet
c router
```

#### Other

```sh
c gw -h
# output: ssh -R52698:127.0.0.1:52698 -A -p 22000 iam@gw.example.com
c voip^
# output: voip.example.com
```


### INSTALL

Copy `c` script somewhere in `$PATH` and create `.c.yaml` in `$HOME` directory.

Then install required perl module with command `cpan YAML::Tiny`

### Changes

* 2014-08-14 - v0.005

    syntax for printing origin host

