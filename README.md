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
```

### EXAMPLES

With config from `c.example.yaml`

#### SSH

```sh
# connect via ssh
c radius
# with additional arguments
c radius -l user
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

### INSTALL

Copy `c` script somewhere in `$PATH` and create `.c.yaml` in `$HOME` directory.

Then install required perl module with command `cpan YAML::Tiny`
